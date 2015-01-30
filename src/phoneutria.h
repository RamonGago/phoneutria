#ifndef PHONEUTRIA_H_
#define PHONEUTRIA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <dirent.h>
#include <netdb.h>
#include "link_parser.h"
#include "url_stack.h"
#include <time.h>

#define IPV4_ADDR_LEN 15

extern int h_errno; //needed by gethostbyname
extern time_t start;

char *get_ip_addr(char *_host_name, char *_ip_addr);
int create_socket(char *_host_name);
int get_page(char **_seeds, int _num_seeds, char **_query, int _num_query);

#endif
