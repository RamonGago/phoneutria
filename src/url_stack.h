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

#ifndef URL_H_
#define URL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int add_url(site_node_t **_site_queue, char *_url, char *_site_name, int _depth);		/*insert url in queue*/
char *get_url(site_node_t **_site_queue, int *_depth);									/*get url from queue*/
void free_queue(site_node_t *_site_queue);												/*delete url from queue*/
int print_queue(site_node_t *_site_queue);												

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