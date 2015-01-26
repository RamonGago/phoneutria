/*
This file is part of Phoneutria package.
Writen by (alphabetic order)
- Danilo Cantarella (https://github.com/Flyer-90);
- Roberta Maccarrone (https://github.com/diarbuse);
- Cristina Parasiliti Parracello (https://github.com/CryPara);
- Filippo Randazzo (https://github.com/filirnd);
- Dario Safarally (https://github.com/stormspeed);
- Sebastiano Siragusa (https://github.com/sebysira);
- Federico Vindigni (https://github.com/federicovindigni);
Full Phoenutria is released by GPL3 licence.
*/

#ifndef LINK_PARSER_H_
#define LINK_PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "url_stack.h"

#define URL_MAX_LEN 4096

#define HOST_MAX_LEN 300
#define FILE_PATH_MAX_LEN 3786
#define FILE_EXT_MAX_LEN 10
#define DOMAIN_MAX_LEVEL 10

#define HASH_SIZE 11881376 					/*26^5*/

extern char seed_host[URL_MAX_LEN];

typedef struct url_info_t					/*struct for url's information*/
{
	char host_name[HOST_MAX_LEN];
	char path[FILE_PATH_MAX_LEN];
	char file_ext[FILE_EXT_MAX_LEN];
	char *subdomain[DOMAIN_MAX_LEVEL];
} url_info_t;

typedef struct page_node_t					/*struct for node*/
{
	char *url;
	struct page_node_t *next;
} page_node_t;

void init_hash_table();																					/*initialize hash table with NULL*/
int is_known_page(char *_url);																			/*check if url exist in hash table*/
page_node_t *create_node(char *_url, page_node_t *_next);												/*create node to insert in hash table*/
int parse_page(int _sock, site_node_t **_site_queue, char *_host_name, char *_query, int _page_depth);	/*parsing page to search query keyword and new urls*/
int file_ext_is_good(char *_file_ext);																	/*check if file's extension*/
int get_hash(char *_url);																				/*calculate hash from url*/
url_info_t *get_url_info(char *_url, url_info_t *_url_info);											/*get url's information*/
int spot_url(int _sock, site_node_t **_site_queue, FILE *page, char *_host_name, int _page_depth);		/*search new urls to add in queue*/


#endif

/*
This file is part of Phoneutria package.
Writen by (alphabetic order)
- Danilo Cantarella (https://github.com/Flyer-90);
- Roberta Maccarrone (https://github.com/diarbuse);
- Cristina Parasiliti Parracello (https://github.com/CryPara);
- Filippo Randazzo (https://github.com/filirnd);
- Dario Safarally (https://github.com/stormspeed);
- Sebastiano Siragusa (https://github.com/sebysira);
- Federico Vindigni (https://github.com/federicovindigni);
Full Phoenutria is released by GPL3 licence.
*/