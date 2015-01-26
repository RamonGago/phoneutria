#ifndef PHONEUTRIA_H_
#define PHONEUTRIA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <netinet/in.h>
#include "url_stack.h"
#include <netdb.h>
#include "link_parser.h"
#include "url_stack.h"

#define IPV4_ADDR_LEN 15

extern int h_errno; 											/*needed by gethostbyname*/

char *get_ip_addr(char *_host_name, char *_ip_addr);			/*get IP address from hostname*/
int create_socket(char *_host_name);							/*create socket for connection*/
int get_page(char *_seed, char *_query);						/*download page from url*/

#endif
