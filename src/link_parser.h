#ifndef LINK_PARSER_H_
#define LINK_PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include "url_stack.h"


#define URL_MAX_LEN 4096
#define URL_VALID_MAX_LEN 400

#define HOST_MAX_LEN 300
#define FILE_PATH_MAX_LEN 3786
#define FILE_EXT_MAX_LEN 10
#define DOMAIN_MAX_LEVEL 10

//#define HASH_SIZE 11881376 //26^5
#define HASH_SIZE 1727604 //36^4+36^3+36^2+36

extern int sim_time;

typedef struct url_info_t
{
	char host_name[HOST_MAX_LEN];
	char path[FILE_PATH_MAX_LEN];
	char file_ext[FILE_EXT_MAX_LEN];
	char *subdomain[DOMAIN_MAX_LEVEL];
} url_info_t;

typedef struct page_node_t
{
	char *url;
	struct page_node_t *next;
} page_node_t;

void init_hash_table();
int is_known_page(char *_url);
page_node_t *create_node(char *_url, page_node_t *_next);
int parse_page(int _sock, site_node_t **_site_queue, char *_host_name, char * _host_path, char **_query, int _num_query, int _page_depth, char **_seeds, int _num_seeds);
int file_ext_is_good(char *_file_ext);
int get_hash(char *_url);
url_info_t *get_url_info(char *_url, url_info_t *_url_info);
int spot_url(int _sock, site_node_t **_site_queue, FILE *page, char *_host_name, int _page_depth, char **_seeds, int _num_seeds);

#endif
