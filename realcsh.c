/*BINFMTC: -lreadline -I/usr/include/readline
exit 1

 *  binfmt_misc C Interpreter
 *  Copyright (C) 2005-2006 Junichi Uekawa
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
  char * tempfilename = NULL;
  FILE * f;
  int fd;
  defsp t, defs=NULL;
  defsp header=NULL;
  int i;

  for (i=1; i < argc; ++i)
    header=add_string(header, argv[i]);

  defs=add_string(defs, "#include <stdlib.h>");
  defs=add_string(defs, "#include <unistd.h>");
  defs=add_string(defs, "#include <stdio.h>");

  while (NULL!=(str = readline("REAL csh: ")))
    {
      if (*str=='\0')		/* ignore blanks. */
	{
	  free(str);
	  continue;
	}

      add_history(str);

      if (*str=='#')
	{
	  /* ## debug symbol to dump header file list. */
	  if (*(str+1)=='#')
	    {
	      printf("defs:\n");
	      for (t=defs; t; t=t->next)
		{
		  printf("  %s\n", t->s);
		}
	      printf("header:\n");
	      for (t=header; t; t=t->next)
		{
		  printf("  %s\n", t->s);
		}
	      free(str);
	      continue;
	    }
	  defs=add_string(defs, str);
	  free(str);
	  continue;
	}
      asprintf(&tempfilename, "%s/realcshXXXXXX",
	       getenv("BINFMTCTMPDIR")?:
	       getenv("TMPDIR")?:
	       getenv("TEMPDIR")?:
	       "/tmp"
	       );
      f = fdopen(fd=mkstemp(tempfilename), "w");
      fchmod(fd, 0700);

      fprintf(f,
	      "/*BINFMTC:");
      for (t=header; t; t=t->next)
	{
	  fprintf(f," %s", t->s);
	}

      fprintf(f,
	      "\n*/\n");
      for (t=defs; t; t=t->next)
	{
	  fprintf(f, "%s\n", t->s);
	}
      fprintf(f,
	      "int main(int argc, char ** argv)\n"
	      "{\n"
	      "%s;\n"
	      "return 0; \n"
	      "}\n",
	      str);
      fclose (f);
      system (tempfilename);
      unlink (tempfilename);
      free (str);
    }
  printf ("\n");
  return 0;
}
