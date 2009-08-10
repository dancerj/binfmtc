/*
header file for upaccho2 webserver.
7 May 2005 Junichi Uekawa
*/

#ifndef __UPACCHO2WEBSERVICE_H__
#define __UPACCHO2WEBSERVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

void http_404(int sock);
void http_header(int sock, const char* content_type);
void http_header_with_size(int sock, const char* content_type, size_t content_length);
void* http_add_handler(const char* path,
		      void (*func)(int, const char*, const char*));
int http_initiate_webserver(int portnumber);

#ifdef __cplusplus
} // extern "C"
#endif
#endif
