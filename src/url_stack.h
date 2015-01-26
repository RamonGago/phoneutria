#ifndef URL_H_
#define URL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "link_parser.h"

#define URL_MAX_LEN 4096

typedef struct url_node_t
{
	char *url;
	struct url_node_t *next_url;
} url_node_t;

/*typedef struct url_queue_t
{
	url_node_t *head;
	url_node_t *tail;
} url_queue_t;*/

typedef struct site_node_t
{
	char *site_name;
	struct url_node_t *url_stack;
	struct site_node_t *next_site;
} site_node_t;

int add_url(site_node_t **_site_queue, char *_url, char *_site_name);					/*insert url in queue*/
char *get_url(site_node_t **_site_queue);												/*get url from queue*/
void free_queue(site_node_t *_site_queue);												/*delete url from queue*/
int print_queue(site_node_t *_site_queue);												

#endif