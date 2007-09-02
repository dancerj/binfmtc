/*
 *  binfmt_misc C Interpreter
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
 * Interepreter for binfmt_misc C compilation.
 *
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include "binfmtc.h"

#define BINFMTC_MAGIC_LEN strlen(binfmtc_magic)
#define BINFMTC_DEBUG (getenv("BINFMTC_DEBUG"))

char* compile_source(const char *sourcename)
{
  char* origdir = getcwd(NULL, 0);
  char* path = strdup (sourcename);
  char* path_delimiter;
  char* tempfilename = NULL; 
  char* s = NULL;
  char* gcccommandline = NULL;
  size_t size;
  FILE* f;
  int i;
  
  asprintf(&tempfilename, "%s/binfmtcXXXXXX",
	   getenv("BINFMTCTMPDIR")?:
	   getenv("TMPDIR")?:
	   getenv("TEMPDIR")?:
	   "/tmp"
	   );
  
  if (NULL==(path_delimiter=strrchr(path, '/')))
    {
      fprintf(stderr, 
	      "binfmtc: Could not determine the directory name of source %s\n", 
	      sourcename);
      return NULL;
    }
  
  *path_delimiter=0;/* obtain the dirname */
  close(mkstemp(tempfilename)); /* get temporary filename */
  if (!(f=fopen(sourcename,"rt")))
    {
      fprintf(stderr, 
	      "binfmtc: Could not open file %s for read\n", 
	      sourcename);
      unlink(tempfilename);
      return NULL;
    }

  if (!getline (&s, &size, f))
    {
      fprintf(stderr, 
	      "binfmtc: Could not read file %s\n", 
	      sourcename);
      unlink(tempfilename);
      return NULL;
    }

  /* compare with magic */
  if (size < BINFMTC_MAGIC_LEN || 
      (memcmp(binfmtc_magic, s, BINFMTC_MAGIC_LEN)))
    {
      fprintf(stderr, 
	      "binfmtc: %s magic invalid \n", 
	      sourcename);
      unlink(tempfilename);
      return NULL;
    }
  fclose(f);

  /* make the newlines removed */
  for(i=strlen(s)-1; i; --i)
    {
      if (!isspace (s[i]))
        break;
      else
	s[i]=0;
    }

  asprintf (&gcccommandline,
	    "%s -o %s %s %s %s \"%s\"",
	    compiler_name(),
	    tempfilename,
	    gcc_x,
	    s+BINFMTC_MAGIC_LEN,
	    default_options(),
	    basename(sourcename));

  if (BINFMTC_DEBUG)
    fprintf(stderr, "binfmtc: Execute command-line: %s\n", gcccommandline);

  if (chdir(path))			/* go to the source's dir */
    {
      fprintf(stderr, 
	      "binfmtc: Cannot chdir to directory where source is: %s \n", 
	      path);
      unlink(tempfilename);
      return NULL;
    }
  
  if (system(gcccommandline))
    {
      fprintf(stderr, 
	      "binfmtc: Compilation failed for %s, see above messages for details\n", 
	      sourcename);
      unlink(tempfilename);
      return NULL;
    }
  
  if (chdir(origdir))
    {
      fprintf(stderr, 
	      "binfmtc: Cannot chdir to original working directory: %s \n", 
	      path);
      unlink(tempfilename);
      return NULL;
    }

  free(origdir);
  free(path);
  free(s);
  free(gcccommandline);
  return tempfilename;
}

/**
   Function to execute the compiled program.
   
   The reason the compiled program is execed as the parent program
   is to allow use of same PID for the compiled program, so that 
   it is possible to use "strace" "kill -HUP" and other conventional 
   methods of debugging.
 */
int exec_prog(const char * filename, int argc, char**argv)
{
  int pid;
  int parent_pid;

  switch(pid=fork())
    {
    case -1:
      /* fork failed */
      fprintf(stderr, 
	      "binfmtc: Failed to fork \n");
      return EXIT_FAILURE;
      break;
    case 0:
      /* when I am the child process, unlink the file after the parent
	 process has exited.
       */
      parent_pid=getppid();
      if (daemon(0,0) < 0)
	{
	  exit (-1);
	}
      while ((parent_pid!=-1)
	     && (kill(parent_pid,0) >= 0))
	{
	  sleep(1);
	}
      unlink(filename);
      exit(0);
    default:
      /* I am the parent process, exec into the built program */
      waitpid(pid,NULL,0);
      execvp(filename, argv);
      fprintf(stderr, 
	      "binfmtc: failed execvp of %s \n", filename);
      break;
    }
  return EXIT_FAILURE;
}

int main(int argc, char**argv)
{
  char* filename=NULL;
  int exit_code = EXIT_FAILURE;
  if (argc < 2)
    {
      /* Wrong number of command-line args; maybe someone tried to exec me directly. */
      fprintf(stderr,
	      "binfmtc: Wrong number of command-line options, use binfmt_misc\n%s\n",
	      language_type);
    }
  else if ((filename=compile_source(argv[1])))
    exit_code=exec_prog(filename, argc-1, &(argv[1]));
  return exit_code;
}
