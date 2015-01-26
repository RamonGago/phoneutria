#include "url_stack.h"

int add_url(site_node_t **_top, char *_url, char *_site_name)
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
		if(prev)														/*if stack is empty*/
			prev->next_site = new_site;
		else
			*_top = new_site;
	}

	
	return 0;
}

char *get_url(site_node_t **_site_queue)
{
	site_node_t *aux1;
	url_node_t *aux2;
	char *url;

	/*a node can be delete from his stack only if there aren't other urla in this stack*/
	if(!(*_site_queue)->url_stack)										/*check if url_stack is not null*/								
	{
		aux1 = *_site_queue;
		*_site_queue = (*_site_queue)->next_site;						/*go to next node*/
		free(aux1->site_name);
		free(aux1);
		if(!_site_queue)												
			return NULL;
	}
	
	aux2 = (*_site_queue)->url_stack;
	(*_site_queue)->url_stack = (*_site_queue)->url_stack->next_url;
	
	url = aux2->url;
	free(aux2);
	
	printf("Parsing: %s\n", url);
	
	return url;
}

/*int add_url_dfs(url_node_t **_top, char *_url)
{
	url_node_t *new_node;
	
	new_node = malloc(sizeof(url_node_t));
	new_node->url = strdup(_url);
	new_node->next_url = *_top;
	*_top = new_node;
	
	return 0;
}

char *get_url_dfs(url_node_t **_top)
{
	url_node_t *aux;
	char *url;
	
	aux = *_top;
	*_top = (*_top)->next_url;
	url = aux->url;
	free(aux);
	
	return url;
}

int add_url_sdfs(url_queue_t **_top, char *_url, char *_seed)
{
	return 0;
}*/

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
		printf("%s\n\n", _site_queue->site_name);
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

