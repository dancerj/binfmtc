/*BINFMTC: -lreadline -I/usr/include/readline
exit 1

 *  binfmt_misc Kernel Module C Interpreter
 *  Copyright (C) 2005,2006 Junichi Uekawa
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * test program: Real C shell implemented in binfmtc.
 *
 * M-x compile:
 * gcc -S -c realksh.c -I /usr/include/readline -Wall -O2 
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <readline.h>
#include <history.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

typedef struct defs_list
{
  struct defs_list* next;
  char* s;
} * defsp;


defsp add_string(defsp orig, const char* newstr)
{
  defsp t = malloc(sizeof(struct defs_list));
  t->next=orig;
  t->s=strdup(newstr);
  return t;
}

pid_t kmsgloop(void)
{
  /* do a loop looking at kmsg
     This only works if you're running as root.
   */
  int f;
  int cnt;
  pid_t p;
  

  if (!(p=fork()))
    {
      char log_buffer[1024];
      if (-1==(f=open("/proc/kmsg", O_RDONLY)))
	{
	  perror("open: /proc/kmsg cannot be opened, you won't see dmesg");
	  exit (1); 
	}
      
      while (( cnt = read (f, log_buffer, sizeof(log_buffer) - 1 )) >= 0 )
	{
	  write(1, "KMSG: ", 6);
	  write(1, log_buffer, cnt);
	  write(1, "\n:", 1);
	}
      perror("read: /proc/kmsg handling failed");
      exit (1);
    }
  return p;
}

int main(int argc, char** argv)
{
  int ret;
  char * str = NULL;
  char * tempdirname = NULL;
  char * tempfilename = NULL;
  char * commandline = NULL;
  char * kerneldirname = NULL;
  char * kbuildfilename = NULL;

  /* the descriptions can be anything, since we aren't really using it for anything important */
  const char* module_author = "dancerj";
  const char* module_description = "...." ;
  const char* module_license = "GPL" ;

  /* the name should not
     duplicate what's already existing within kernel. */
  const char* modulename = "realkshmod2";

  FILE * f;
  defsp t, defs=NULL;
  pid_t p;
  
  /* initialize the header file list.
     This is the minimal list that would enable 
     some useful module loading to be done.
     
     It also allows most kernel operations, like
     printk, and mfspr (ppc) etc.
     
     Add lines here to improve default for your liking.
   */
  defs=add_string(defs, "#include <linux/init.h>");
  defs=add_string(defs, "#include <linux/module.h>");
  
  /* fork a process to output dmesg.
   */
  p=kmsgloop();

  /* 
     get the path to kernel build directory from /lib/modules/KVER/build/
   */
  {
    struct utsname u;
    uname (&u);
    asprintf (&kerneldirname, "/lib/modules/%s/build", 
	      u.release);
  }

  /* prepare module build directory */
  asprintf(&tempdirname, "%s/realkshXXXXXX",
	   getenv("BINFMTCTMPDIR")?:
	   getenv("TMPDIR")?:
	   getenv("TEMPDIR")?:
	   "/tmp"
	   );
  mkdtemp(tempdirname);
  asprintf(&kbuildfilename, "%s/Kbuild",
	   tempdirname);
  asprintf(&tempfilename, "%s/%s-main.c",
	   tempdirname,
	   modulename);
  f=fopen (kbuildfilename, "w");
  fprintf(f, "obj-m:=%s.o\n"
	  "%s-y:=%s-main.o\n",
	  modulename, 
	  modulename, modulename);
  fclose(f);
  
  if (getuid()!=0)
    {
      fprintf(stderr, "Warning: root privilege is probably required for most operation\n");
    }

  while (NULL!=(str = readline("REAL ksh: ")))
    {
      if (*str=='\0')		/* ignore blanks lines, they clutter history. */
	continue;
      add_history(str);

      /* 
	 ## = dump list 
	 # ... = add this line. e.g. #include <.....h>
      */
      if (*str=='#')
	{
	  /* ## debug symbol to dump header file list. */
	  if (*(str+1)=='#')
	    {
	      printf("List of items in the # list:\n");
	      for (t=defs; t; t=t->next)
		{
		  printf("%s\n", t->s);
		}
	      continue;
	    }
	  defs=add_string(defs, str);
	  continue;
	}

      /* 
	 creation of module source-code to be processed
       */
      f=fopen(tempfilename, "w");
      chmod(tempfilename, 0700);
      for (t=defs; t; t=t->next)
	{
	  fprintf(f, "%s\n", t->s);
	}
      fprintf(f, 
	      "MODULE_AUTHOR(\"%s\");\n"
	      "MODULE_DESCRIPTION(\"%s\");\n"
	      "MODULE_LICENSE(\"%s\");\n"
	      "static int __init %s_init(void)\n"
	      "{\n"
	      "%s;\n"
	      "return 0; \n"
	      "}\n"
	      "static void __exit %s_cleanup(void)\n"
	      "{\n"
	      "}\n"
	      "module_init(%s_init);\n"
	      "module_exit(%s_cleanup);\n"
	      ,
	      module_author,
	      module_description,
	      module_license, 
	      modulename,
	      str,
	      modulename,
	      modulename,
	      modulename);
      fclose (f);

      /* build and execute */
      asprintf(&commandline, 
	       "cd %s && make -s -C \"%s\" M=\"%s\" && insmod %s.ko",
	       tempdirname, kerneldirname, 
	       tempdirname, modulename);
      if ((ret=system (commandline)))
	{
	  fprintf(stderr, "non-zero return code from make/exec command: %i\n", ret);
	}
      free(commandline);

      /* clean-up module regardless of failure or success */
      asprintf(&commandline, 
	       "rmmod %s.ko",
	       modulename
	       );
      system (commandline);
      free(commandline);
      free(str);
    }

  /* After ctrl-D, you will come here, do a clean-up
   */
  printf ("\n");

  asprintf(&commandline, 
	   "make -s -C \"%s\" M=\"%s\" clean ",
	   kerneldirname, tempdirname);
  system(commandline);
  unlink (tempfilename);
  unlink (kbuildfilename);
  rmdir (tempdirname);

  kill(p, SIGTERM);
  if (-1==waitpid(p, NULL, 0))
    perror("waitpid");

  return 0;
}

