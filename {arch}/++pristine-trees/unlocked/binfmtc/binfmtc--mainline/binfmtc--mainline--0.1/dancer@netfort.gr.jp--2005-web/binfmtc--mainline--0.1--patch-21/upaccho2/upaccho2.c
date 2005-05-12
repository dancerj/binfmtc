/*BINFMTC: upaccho2-webservice.c
exit 1

A simple webserver in C.
7 May 2005 Junichi Uekawa

Based on:
A quick and dirty webserver that does almost nothing.
23 feb 2001 Junichi Uekawa

*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "upaccho2-webservice.h"

#define TMPBUFSIZE 2048		/* length of incoming command to accept */

void openanddump(int sock, const char * path, const char * filename)
{
  FILE* f=fopen(filename, "r");
  int readbytes;
  char buf[TMPBUFSIZE+1];

  if (f)
    {
      http_header(sock, "text/html");
      
      while(0!=(readbytes=fread(buf, 1, TMPBUFSIZE, f)))
	{
	  write(sock, buf, readbytes);
	}
      fclose(f);
    } 
  else
    {
      http_404(sock);
    }
}

int main(int ac, char** av)
{
  if (ac != 2)
    {
      fprintf(stderr, "Please specify the port as the command-line parameter\n");
      exit(1);
    }
  http_add_handler("/", openanddump);
  http_initiate_webserver(atoi(av[1]));
  return 0;
}
 
