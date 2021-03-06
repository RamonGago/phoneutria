#ifndef URL_H_
#define URL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HASH_SIZE 1727604

#define URL_MAX_LEN 4096

typedef struct url_node_t
{
	char *url;
	struct url_node_t *next_url;
} url_node_t;

typedef struct site_node_t
{
	char *site_name;
	int depth;
	struct url_node_t *url_stack;
	struct site_node_t *next_site;
} site_node_t;

int add_url(site_node_t **_site_queue, char *_url, char *_site_name, int _depth);
char *get_url(site_node_t **_site_queue, int *_depth);
void free_queue(site_node_t *_site_queue);
int print_queue(site_node_t *_site_queue);

#endif