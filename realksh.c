/*BINFMTC: -lreadline -I/usr/include/readline
exit 1

 *  binfmt_misc Kernel Module C Interpreter
 *  Copyright (C) 2005 Junichi Uekawa
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
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <readline.h>
#include <history.h>

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

int main(int argc, char** argv)
{
  char * str = NULL;
  char * tempdirname = NULL;
  char * tempfilename = NULL;
  char * commandline = NULL;
  char * kerneldirname = NULL;
  char * kbuildfilename = NULL;
  
  const char* module_author = "dancerj";
  const char* module_description = "...." ;
  const char* module_license = "GPL" ;
  const char* modulename = "realkshmod2";

  FILE * f;
  defsp t, defs=NULL;
  
  defs=add_string(defs, "#include <linux/init.h>");
  defs=add_string(defs, "#include <linux/module.h>");

  /* get it from /lib/modules/KVER/build/ */
  {
    struct utsname u;
    uname (&u);
    asprintf (&kerneldirname, "/lib/modules/%s/build", 
	      u.release);
  }
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
  
  while (NULL!=(str = readline("REAL ksh: ")))
    {
      if (*str=='\0')		/* ignore blanks. */
	continue;
      add_history(str);

      if (*str=='#')
	{
	  /* ## debug symbol to dump header file list. */
	  if (*(str+1)=='#')
	    {
	      for (t=defs; t; t=t->next)
		{
		  printf("%s\n", t->s);
		}
	      continue;
	    }
	  defs=add_string(defs, str);
	  continue;
	}

      f = fopen(tempfilename, "w");
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

      asprintf(&commandline, 
	       "cd %s && make -C \"%s\" M=\"%s\" && sudo insmod %s.ko",
	       tempdirname, kerneldirname, 
	       tempdirname, modulename);
      system (commandline);
      free(commandline);

      asprintf(&commandline, 
	       "sudo rmmod %s.ko",
	       modulename
	       );
      system (commandline);
      free(commandline);
      
      free(str);
    }
  printf ("\n");
  unlink (tempfilename);
  unlink (kbuildfilename);
  rmdir (tempdirname);
  return 0;
}

