#include "link_parser.h"

/*
 * Estraggo tutto le lettere dell'url.
 * Aggiungo entropia a questa stringa.
 * Calcolo l'hash usando questa stringa.
 * Do maggiore peso alle lettere del dominio.
 * Faccio un hashing a a dizionario.
 */

int sim_time = 0;
int file_index = 0;
int redir_cont = 0;

page_node_t *known_pages[HASH_SIZE];

url_info_t *get_url_info(char *_url, url_info_t *_url_info)
{
	int url_len;
	int i, j, k;
	int has_domain;
	
	url_len = strlen(_url);

	/*
	 *ottengo l'host_name di livello più basso ossia la stringa quello delimitata 
	 *da http:// a destra e dal carattare / (o dalla fine dell'url) a sinistra
	 */
	i = j = has_domain = 0;
	if(strncasecmp(_url, "http://", 7) == 0)
		i = 7;
	while(i < url_len && _url[i] != '/')
	{
		if(_url[i] == '.')
			has_domain++;
		_url_info->host_name[j++] = _url[i++];
	}
	_url_info->host_name[j] = '\0';
	
	/*l'host_name deve avere almeno un dominio (quello di primo livello)*/
	if(!has_domain)
		return NULL;
	
	/*ottengo il path ossia tutto quello che viene nell'url dopo l'host_name*/
	j = 0;
	while(i < url_len)
		_url_info->path[j++] = _url[i++];
	if(j == 0)
		_url_info->path[j++] = '/';
	_url_info->path[j] = '\0';
	
	
	/*ottengo l'estensione (se c'è) della pagina (per saltare css, js, etc.)*/
	_url_info->file_ext[0] = '\0';
	i = j = 0;
	while(i < url_len)
		if(_url[i++] == '/')
			j = i;
	
	if(j != 0)
	{
		while(j < url_len && _url[j] != '?')
		{
			if(_url[j++] == '.')
				i = j;
		}
		
		j = 0;
		if(i > 0)
		{
			while(i < url_len && _url[i] != '?')
				_url_info->file_ext[j++] = _url[i++];
		}
		_url_info->file_ext[j] = '\0';
	}
	
	/*ottengo i vari livelli dell'host_name per il dns*/
	i = strlen(_url_info->host_name) - 1;
	
	/*scarto il dominio di primo livello poichè l'url minimo è stringa.com*/
	while(_url_info->host_name[i] != '.')
		i--;
	
	j = k = 0;
	while(j < i)
	{
		_url_info->subdomain[k++] = &(_url_info->host_name[j]);
		while(_url_info->host_name[j] != '.')
			j++;
		j++;
	}
	_url_info->subdomain[k] = NULL;
	
	return _url_info;
}

void init_hash_table()
{
	int i;
	
	for(i = 0; i < HASH_SIZE; i++)
		known_pages[i] = NULL;
}

int get_val(char _c)
{
	if(_c >= 'a' && _c <= 'z')
		return (_c - 'a' + 11);
	else if(_c >= '0' && _c <= '9')
		return (_c - '0' + 1);
	return 0;
}

int get_hash(char *_url)
{
	int hash;
	char hash_str[URL_MAX_LEN];
	int url_len;
	int i, j;
	
	url_len = strlen(_url);
	
	i = j = 0;
	while(i < url_len)
	{
		if((_url[i] >= 'a' && _url[i] <= 'z') || (_url[i] >= '0' && _url[i] <= '9'))
			hash_str[j++] = _url[i];
		i++;
	}
	hash_str[j] = '\0';
	
	hash = 0;
	if(j > 3)
	{
		hash = get_val(hash_str[0]);
		hash *= get_val(hash_str[j - 1]);
		hash *= get_val(hash_str[(j - 1) / 4]);
		hash *= get_val(hash_str[((j - 1) * 3) / 4]);
	}
	
	return hash;
}

page_node_t *create_node(char *_url, page_node_t *_next)
{
	page_node_t *node;

	node = malloc(sizeof(page_node_t));
	node->url = strdup(_url);
	node->next = _next;
	
	return node;
}

int is_known_page(char *_url)
{
	int hash;
	int res;
	page_node_t *aux;
	page_node_t *prev;
	page_node_t *new_node;

	if(strncasecmp(_url, "http://", 7) == 0)
		_url = &(_url[7]);

	if(strncasecmp(_url, "https://", 8) == 0)
		_url = &(_url[8]);

	if(strncasecmp(_url, "www.", 4) == 0)
		_url = &(_url[4]);
	
	hash = get_hash(_url);
	//printf("prendo l'url %s  e trovo hash %d \n", _url, hash);
	
	/*se la entry è vuota inserisco il primo nodo*/
	if(known_pages[hash] == NULL)
	{
		known_pages[hash] = create_node(_url, NULL);
		return 0;
	}
	
	/*altrimenti devo cercare nella lista di collisione*/
	aux = known_pages[hash]; /*nodo corrente*/
	prev = NULL; /*nodo di supporto per l'inserimento in mezzo alla lista*/
	
	/*cerco il punto di inserimento dell'url nella lista*/
	while( aux != NULL && (res = strcmp(_url, aux->url)) > 0)
	{	
		prev = aux;
		aux = aux->next;
	}

	/*se l'url esiste già*/
	if(res == 0)
		return 1;

	/*altrimenti lo devo inserire nella lista.
	  ci sono tre casi di inserimento: in coda o nel mezzo (equivalenti) e in testa*/
	if(aux == NULL || res < 0)
	{
		new_node = create_node(_url, aux);
		if(prev != NULL)
			prev->next = new_node;
		else
			known_pages[hash] = new_node;
		return 0;
	}
	
	return 1;
}

int file_ext_is_good(char *_file_ext)
{
	if(strlen(_file_ext) == 0 ||
		strlen(_file_ext) > 4 ||
		strcmp(_file_ext, "html") == 0 ||
		strcmp(_file_ext, "htm") == 0 ||
		strcmp(_file_ext, "xhtml") == 0 ||
		strcmp(_file_ext, "xml") == 0 ||
		strcmp(_file_ext, "php") == 0 ||
		strcmp(_file_ext, "txt") == 0 ||
		strcmp(_file_ext, "asp") == 0 ||
		strcmp(_file_ext, "aspx") == 0 ||
		strcmp(_file_ext, "jsp") == 0 ||
		strcmp(_file_ext, "jspx") == 0 ||
		strcmp(_file_ext, "do") == 0)
			return 1;
	else
		return 0;
}

int read_line(int _sock, char *_buf, int _buf_len)
{
	char read_char, c;
	int line_len;
	
	line_len = 0;
	memset(_buf, '\0', _buf_len);
	
	while(read(_sock, &read_char, 1) > 0)
	{
		c = tolower(read_char);
		if(c == '\n')
		{
			_buf[line_len++] = '\0';
			return line_len;
		}
		
		_buf[line_len++] = c;
		
		if(line_len == _buf_len - 1)
		{
			_buf[line_len] = '\0';
			return 0;
		}
	}
	
	return 0;
}

int parse_page(int _sock, site_node_t **_site_queue, char *_host_name, char * _path_name, char **_query, int _num_query, int _page_depth, char **_seeds, int _num_seeds)
{
	char read_char;
	char c;
	int is_http;
	int is_https;
	int is_href;
	int is_query;
	int index[_num_query];
	int found[_num_query];
	int i, j;
	FILE *page;
	char path[1000];
	//char response_line[URL_MAX_LEN];	
	
	memset(path, '\0', 1000);
	sprintf(path, "output/%d.txt", file_index++);
	page = fopen(path, "w");

	fprintf(page, "%s%s %d\n", _host_name, _path_name, _page_depth);
	
	//read_line(_sock, response_line, URL_MAX_LEN); 
	//printf(" ----- %s\n", response_line);
	
	/*if(!strstr(response_line, "200")){
		remove(path);
		return -1;
	}*/
	
	/*if(redir_cont == 2)
	{
		redir_cont = 0;
		return 0;
	}*/
	
	/*while(read(_sock, response_line, URL_MAX_LEN) > 0);
		return 0;*/
	
	for(i = 0; i < _num_query; i++){
		index[i] = 0;
		found[i] = 0;
	}

	is_http = is_https = is_href = is_query = i = 0;
	while(read(_sock, &read_char, 1) > 0)
	{
		c = tolower(read_char);
		fputc(c, page);
		
		/*mi sembra normale il fatto che la query non sia cercata negli url*/
		for(j = 0; j < _num_query; j++)
		{
			if(c == _query[j][index[j]]) 
				index[j]++; 
			else
				index[j] = 0;

			if(index[j] == strlen(_query[j])){
				found[j]=1;
			}
		}
		
		if((c == 'h' && is_http == 0) ||
			(c == 't' && is_http == 1) ||
			(c == 't' && is_http == 2) ||
			(c == 'p' && is_http == 3) ||
			(c == ':' && is_http == 4) ||
			(c == '/' && is_http == 5))
			is_http++;
		else if(c == '/' && is_http == 6)
			spot_url(_sock, _site_queue, page, NULL, _page_depth, _seeds, _num_seeds);
		else
			is_http = 0;
		
		if((c == 'h' && is_https == 0) ||
			(c == 't' && is_https == 1) ||
			(c == 't' && is_https == 2) ||
			(c == 'p' && is_https == 3) ||
			(c == 's' && is_https == 4) ||
			(c == ':' && is_https == 5) ||
			(c == '/' && is_https == 6))
			is_https++;
		else if(c == '/' && is_https == 7)
			spot_url(_sock, _site_queue, page, NULL, _page_depth, _seeds, _num_seeds);
		else
			is_https = 0;
		
		if((c == 'h' && is_href == 0) ||
			(c == 'r' && is_href == 1) ||
			(c == 'e' && is_href == 2) ||
			(c == 'f' && is_href == 3) ||
			(c == '=' && is_href == 4) ||
			(c == '"' && is_href == 5) )
			is_href++;
		else if(c == '/' && is_href == 6)
			spot_url(_sock, _site_queue, page, _host_name, _page_depth, _seeds, _num_seeds);
		else
			is_href = 0;
	}
	fclose(page);
	//printf("num parole= %d - found %d", _num_query, found[0]);
	if(_num_query == 1 && found[0] == 1)		//caso base con solo una parola come argomento
		is_query = 1;	

	for(j=1;j<_num_query;j++){
		if(found[0]==1 && found[j]==1){
			is_query=1;
			break;
		} 
	}
	if(!is_query)
		remove(path);
	return 0;
}

int spot_url(int _sock, site_node_t **_site_queue, FILE *page, char *_host_name, int _page_depth, char **_seeds, int _num_seeds)
{
	int i;
	char c;
	char read_char;
	char has_domain;
	char url[URL_MAX_LEN];
	char aux[URL_MAX_LEN];
	url_info_t url_info;
	int is_seed;

	
	url_info.host_name[0] = '\0';
	url_info.path[0] = '\0';
	url_info.file_ext[0] = '\0';
	
	has_domain = i = 0;
	while(read(_sock, &read_char, 1) > 0)
	{
		c = tolower(read_char);
		fputc(c, page);
		
		if((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') ||
			c == '.' || c == '/' || c == '_' || c == '$' || c == ',' || c == '-' ||
			c == ';' || c == '+' || c == '&' || c == '%' || c == '?' || c == '=' || c == ':')
		{
			url[i++] = c;
			if(c == '.')
				has_domain++;
		}
		else
		{
			if(url[i-1] == '/') 
				url[i-1] = '\0';
			else
				url[i] = '\0';
			if(_host_name)
			{
				strcpy(aux, url);
				strcpy(url, _host_name);
				strcat(url, "/");
				strcat(url, aux);
			}
			get_url_info(url, &url_info);
			if(i > 4 && (has_domain || _host_name) && file_ext_is_good(url_info.file_ext))
			{
				if(!is_known_page(url))
				{	
					i = is_seed = 0;
					while(i < _num_seeds && (is_seed = strcmp(_seeds[i], url_info.host_name)) != 0)
						i++;
						
					if(is_seed == 0 && strlen(url) < URL_VALID_MAX_LEN)
						add_url(_site_queue, url, url_info.host_name, _page_depth);
					else if(strlen(url) < URL_VALID_MAX_LEN)
						add_url(_site_queue, url, url_info.host_name, _page_depth + 1);
				}
				has_domain = 0;
			}
			return 0;
		}
	}
	return 0;
}
