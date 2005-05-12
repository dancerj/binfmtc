/*BINFMTC: -lreadline -I/usr/include/readline
exit 1

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
 * test program: Real C shell implemented in binfmtc.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <readline.h>
#include <history.h>

int main(int argc, char** argv)
{
  char * str = NULL;
  char * tempfilename = NULL;
  FILE * f;
  int fd;
  
  while (NULL!=(str = readline("REAL csh: ")))
    {
      add_history(str);
      asprintf(&tempfilename, "%s/realcshXXXXXX",
	       getenv("BINFMTCTMPDIR")?:
	       getenv("TMPDIR")?:
	       getenv("TEMPDIR")?:
	       "/tmp"
	       );
      f = fdopen(fd=mkstemp(tempfilename), "w");
      fchmod(fd, 0700);
      
      fprintf(f, 
	      "/*BINFMTC:\n"
	      "*/\n"
	      "#include <stdlib.h>\n"
	      "#include <unistd.h>\n"
	      "#include <stdio.h>\n"
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

