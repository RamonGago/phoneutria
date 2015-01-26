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
	
	/*cerco in quale sito (stack) inserire il nuovo url (nodo)*/
	aux = *_top;
	prev = NULL;
	while(aux != NULL && strcmp(_site_name, aux->site_name) != 0)
	{
		prev = aux;
		aux = aux->next_site;
	}
	
	/*se aux non è nullo allora devo aggiornare url_stack del site_node 
	  a cui l'url appartiene.*/
	if(aux)
	{
		new_url->next_url = aux->url_stack;
		aux->url_stack = new_url;
	}

	/*se aux è nullo (ossia se il sito a cui l'url appartiene non ha un 
	  suo stack) allora devo creare il site node (ricordo che site node è una coda).*/
	else {
		new_site = malloc(sizeof(site_node_t));
		new_site->site_name = strdup(_site_name);
		new_site->next_site = NULL;
		new_site->url_stack = new_url;
		new_site->depth = _depth;
		/*il primo inserimento è ovviamente a parte (poiché aggiorna il 
		  puntatore all'intera struttura)*/
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
	
	/*un nodo della site queue non viene eliminato quando l'ultimo url del suo 
	  stack viene estratto. L'eliminazione avverà alla get successiva. Questo 
	  perché quell'ultima pagina estratta potrebbe contenere altri link allo 
	  stesso sito.*/
	
	/*se il primo site_node della coda ha lo stack degli url vuoto, allora 
	  seleziono il successivo (se non esiste ho finito tutti i link da visitare)*/
	if((*_site_queue)->url_stack == NULL)
	{
		aux1 = *_site_queue;
		*_site_queue = (*_site_queue)->next_site;
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

/*int main()
{
	site_node_t *site_queue = NULL;
	char *c;

	//printf("main: %p\n", site_queue);
	add_url(&site_queue, "www.unict.it/pag1", "www.unict.it");
	//printf("main: %p\n", site_queue);
	add_url(&site_queue, "www.google.it/gog1", "www.google.it");
	add_url(&site_queue, "www.google.it/gog2", "www.google.it");
	add_url(&site_queue, "facebook.com/fb1", "facebook.com");
	add_url(&site_queue, "www.unict.it/pag2", "www.unict.it");
	add_url(&site_queue, "facebook.com/fb2", "facebook.com");
	add_url(&site_queue, "facebook.com/fb3", "facebook.com");

	print_queue(site_queue);
	free(get_url(&site_queue));
	print_queue(site_queue);
	free(get_url(&site_queue));
	print_queue(site_queue);
	free(get_url(&site_queue));
	print_queue(site_queue);
	
	free_queue(site_queue);
	
	exit(0);
}*/