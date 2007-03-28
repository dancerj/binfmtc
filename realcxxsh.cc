/*BINFMTCXX: -lreadline -I/usr/include/readline
exit 1

 *  binfmt_misc C++ Interpreter
 *  Copyright (C) 2005-2007 Junichi Uekawa
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
 * test program: Real C++ shell implemented in binfmtc.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <readline.h>
#include <history.h>
#include <iostream>
#include <sstream>


using namespace std;

int main(int argc, char** argv)
{
  char * str = NULL;
  stringstream defs;
  stringstream header;
  
  defs << "#include <iostream>\n" 
       << "using namespace std;\n" ;

  for (int i=1; i < argc; ++i)
    {
      header << argv[i] << " ";
    }

  while (NULL!=(str = readline("REAL c++sh: ")))
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
	      cout << "defs:" << endl 
		   << defs.str() << endl
		   << "header:" << endl 
		   << header.str() << endl;
	    }
	  else 
	    defs << str << endl;
	  free(str);
	  continue;
	}

      stringstream temporarysource;

      temporarysource << 
	"/*BINFMTCXX: " << 
	header.str() << 
	"\n" <<
	"*/\n" <<
	defs.str() <<
	"\n"  <<
	"int main(int argc, char ** argv)\n"
	"{\n" <<
	str << ";\n" 
	"return 0; \n"
	"}\n";
	
      char * tempfilename = NULL;
      asprintf(&tempfilename, "%s/realcshXXXXXX",
	       getenv("BINFMTCTMPDIR")?:
	       getenv("TMPDIR")?:
	       getenv("TEMPDIR")?:
	       "/tmp"
	       );

      int fd;
      fd=mkstemp(tempfilename);
      fchmod(fd, 0700);
      write(fd, temporarysource.str().c_str(),
	    temporarysource.str().size());
      close(fd);
      system (tempfilename);
      unlink (tempfilename);
      free (str);
    }
  cout << ("\n");
  return 0;
}
