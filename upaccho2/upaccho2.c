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
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include "upaccho2-webservice.h"

#define TMPBUFSIZE 2048		/* length of incoming command to accept */

/* hacked up content_type which approximates the type of file */
const char* content_type(const char* filename)
{
  const char* filename_extention = strrchr(filename, '.');
  if (filename_extention)
    {
      if (!strcmp(filename_extention, ".html"))
	{
	  return "text/html";
	}
      else if (!strcmp(filename_extention, ".txt"))
	{
	  return "text/plain";
	}
      else if (!strcmp(filename_extention, ".png"))
	{
	  return "image/png";
	}
      else if (!strcmp(filename_extention, ".mp3"))
	{
	  return "audio/mp3";
	}
    }

  /* fallback to text/html */
  return "text/html";
}

void openanddump(int sock, const char* path, const char* filename)
{
  FILE* f=fopen(filename, "r");
  int readbytes;
  char buf[TMPBUFSIZE+1];
  struct stat st;
  stat(filename, &st);

  signal(SIGPIPE, SIG_IGN);

  if (f)
    {
      http_header_with_size(sock, content_type(filename), st.st_size);

      while(0!=(readbytes=fread(buf, 1, TMPBUFSIZE, f)))
	{
	  if (-1 == write(sock, buf, readbytes))
	    {
	      perror("socket write fail");
	      break;
	    }
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
  int port;

  if (ac != 2 && ac != 1)
    {
      fprintf(stderr, "Please specify the port as the command-line parameter\n");
      exit(1);
    }
  http_add_handler("/", openanddump);
  if (ac == 2)
    {
      port = atoi(av[1]);
    }
  else
    {
      // use random free port the OS assigns.
      port = 0;
    }

  http_initiate_webserver(port);
  return 0;
}

