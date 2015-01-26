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

#include "url_stack.h"

int add_url(site_node_t **_top, char *_url, char *_site_name, int _depth)
{
	url_node_t *new_url;
	site_node_t *new_site;
	site_node_t *aux;
	site_node_t *prev;
	
	new_url = malloc(sizeof(url_node_t));
	new_url->url = strdup(_url);
	new_url->next_url = NULL;
	
	aux = *_top;
	prev = NULL;
	while(aux != NULL && strcmp(_site_name, aux->site_name) != 0)		/*search site's stack to insert new url*/
	{
		prev = aux;
		aux = aux->next_site;
	}
	
	if(aux)																/*if aux is not null, insert new url in stack*/
	{
		new_url->next_url = aux->url_stack;
		aux->url_stack = new_url;
	}

	else {																/*if aux is null, create new stack to insert new url*/
		new_site = malloc(sizeof(site_node_t));
		new_site->site_name = strdup(_site_name);
		new_site->next_site = NULL;
		new_site->url_stack = new_url;

		new_site->depth = _depth;

		if(prev)
			prev->next_site = new_site;
		else
			*_top = new_site;
	}

	printf("spotted url: %s at depth: %d\n", _url, _depth);
	
	return 0;
}

char *get_url(site_node_t **_site_queue, int *_depth)
{
	site_node_t *aux1;
	url_node_t *aux2;
	char *url;
	
	/*a node can be delete from his stack only if there aren't other urla in this stack*/
	if((*_site_queue)->url_stack == NULL)
	{
		aux1 = *_site_queue;
		*_site_queue = (*_site_queue)->next_site;						/*go to next node*/
		free(aux1->site_name);
		free(aux1);
		if(!(*_site_queue))
			return NULL;
	}

	aux2 = (*_site_queue)->url_stack;
	(*_site_queue)->url_stack = (*_site_queue)->url_stack->next_url;
	
	url = aux2->url;
	*_depth = (*_site_queue)->depth;
	free(aux2);
	
	printf("Parsing: %s\n", url);

	
	return url;
}

void free_queue(site_node_t *_top)
{
	site_node_t *aux1;
	url_node_t *aux2;
	
	while(_top)
	{
		while(_top->url_stack)
		{
			aux2 = _top->url_stack;
			_top->url_stack = _top->url_stack->next_url;
			free(aux2->url);
			free(aux2);
		}
		free(_top->site_name);
		aux1 = _top;
		_top = _top->next_site;
		free(aux1);
	}
}

int print_queue(site_node_t *_site_queue)
{
	url_node_t *aux;
	
	while(_site_queue)
	{
		aux = _site_queue->url_stack;
		printf("\n\n*****************************\n");
		printf("%s %d\n\n", _site_queue->site_name, _site_queue->depth);
		while(aux)
		{
			printf("\t%s\n", aux->url);
			aux = aux->next_url;
		}
		printf("*****************************\n");
		_site_queue = _site_queue->next_site;
	}
	printf("\n\n");

	return 0;
}

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
