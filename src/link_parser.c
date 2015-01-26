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

#include "link_parser.h"

int max_depth = 2;
int file_index = 0;
char seed_host[URL_MAX_LEN];

page_node_t *known_pages[HASH_SIZE];

url_info_t *get_url_info(char *_url, url_info_t *_url_info)
{
	int url_len;
	int i, j, k;
	int has_domain;
	
	url_len = strlen(_url);

	i = j = has_domain = 0;						
	if(strncasecmp(_url, "http://", 7) == 0)			/*obtain url domain from http:// to first '/' character */
		i = 7;
	while(i < url_len && _url[i] != '/')				/*check url's characters until next '/' */
	{
		if(_url[i] == '.')								/*count number of dots in url*/
			has_domain++;
		_url_info->host_name[j++] = _url[i++];			/*cut http:// part*/
	}
	_url_info->host_name[j] = '\0';
	
	if(!has_domain)										/*if there is almost one domain*/
		return NULL;
	
	j = 0;
	while(i < url_len)									/*obtain path from host_name*/
		_url_info->path[j++] = _url[i++];
	if(j == 0)
		_url_info->path[j++] = '/';
	_url_info->path[j] = '\0';
	
	_url_info->file_ext[0] = '\0';						/*obtain page's extension*/
	i = j = 0;
	while(i < url_len)									/*search last '/' in url*/
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
			while(i < url_len && _url[i] != '?')		/*extract extension from url*/
				_url_info->file_ext[j++] = _url[i++];
		}
		_url_info->file_ext[j] = '\0';
	}
	
	i = strlen(_url_info->host_name) - 1;				/*obtain host_name's levels for dns*/
	
	while(_url_info->host_name[i] != '.')				/*waste url's first level to obtain minimum url, like string.com*/
		i--;
	
	j = k = 0;
	while(j < i)
	{
		_url_info->subdomain[k++] = &(_url_info->host_name[j]); /*extract subdomain from url*/
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
	
	for(i = 0; i < HASH_SIZE; i++)						/*init with NULL all table's cell*/
		known_pages[i] = NULL;
}

int get_hash(char *_url)
{
	int hash;
	char hash_str[URL_MAX_LEN];
	int lp_index;
	int i, j;
	int url_len;
	
	if(strncasecmp(_url, "www", 3) == 0)				/*check if there is 'www' in url and cut it*/
		_url = &(_url[3]);
	
	url_len = strlen(_url);
	
	i = j = lp_index = 0;
	while(i < url_len)									/*create hash string considering only characters between 'a' and 'z'*/
	{
		if(_url[i] >= 'a' && _url[i] <= 'z')
			hash_str[j++] = _url[i];
		i++;
	}
	hash_str[j] = '\0';						
	
	hash = hash_str[0] - 'a' + 1;						/*perform hash on hash string*/
	hash *= hash_str[strlen(hash_str) - 1] - 'a' + 1;
	hash *= hash_str[(strlen(hash_str) - 1) / 2] - 'a' + 1;
	hash *= hash_str[(strlen(hash_str) - 1) / 4] - 'a' + 1;
	hash *= hash_str[((strlen(hash_str) - 1) * 3) / 4] - 'a' + 1;
	
	return hash;
}

page_node_t *create_node(char *_url, page_node_t *_next)
{
	page_node_t *node;

	node = malloc(sizeof(page_node_t));					/*allocate memory to create a node*/
	node->url = strdup(_url);							/*set url string*/
	node->next = _next;									/*set next node*/

	return node;
}

int is_known_page(char *_url)
{
	int hash;
	int res;
	page_node_t *aux;
	page_node_t *prev;
	page_node_t *new_node;
	
	hash = get_hash(_url);								/*obtain hash from url*/
	
	if(known_pages[hash] == NULL)						/*if hash table is empty, add firt node*/
	{
		known_pages[hash] = create_node(_url, NULL);
		return 0;
	}
	
	/*if hash table is not empty, search node in collision list*/
	aux = known_pages[hash]; 							/*current node*/
	prev = NULL; 										/*support node to middle insert*/
	
	while( aux != NULL && (res = strcmp(_url, aux->url)) > 0)	/*search insert's point in list*/
	{	
		prev = aux;
		aux = aux->next;
	}

	if(res == 0)										/*if url exist*/
		return 1;
	
	/*if url not exist, insert it in list*/
	if(aux == NULL || res < 0)
	{
		new_node = create_node(_url, aux);
		if(prev != NULL)								/*insert in middle*/
			prev->next = new_node;
		else
			known_pages[hash] = new_node;				/*insert in head*/
		return 0;
	}
	
	return 1;
}

int file_ext_is_good(char *_file_ext)
{
	if(strlen(_file_ext) == 0 ||						/*check if extension's file is valid*/
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

int parse_page(int _sock, site_node_t **_site_queue, char *_host_name, char *_query, int _page_depth)
{
	char read_char;
	char c;
	int is_http;
	int is_https;
	int is_href;
	int is_query;
	int query_len;
	int i;
	FILE *page;
	char path[1000];

	memset(path, '\0', 1000);							/*init path's memory*/
	sprintf(path, "dump/%d", file_index++);	/*set output's path*/
	page = fopen(path, "w");							/*open page's file in write mode*/
	
	query_len = strlen(_query);
	is_http = is_https = is_href = is_query = i = 0;
	while(read(_sock, &read_char, 1) > 0)				/*read from socket 1 character at time*/
	{
		c = tolower(read_char);					
		fputc(c, page);									/*put every character in page file*/
		
		if(c == _query[i])								/*check if query word is contained in page*/
			i++;
		else
			i = 0;
		if(i == query_len)								/*if there is match*/
			is_query++;
		
		if((c == 'h' && is_http == 0) ||				/*check if there is http:/ string*/
			(c == 't' && is_http == 1) ||
			(c == 't' && is_http == 2) ||
			(c == 'p' && is_http == 3) ||
			(c == ':' && is_http == 4) ||
			(c == '/' && is_http == 5))
			is_http++;
		else if(c == '/' && is_http == 6)
			spot_url(_sock, _site_queue, page, NULL, _page_depth);
		else
			is_http = 0;
		
		if((c == 'h' && is_https == 0) ||				/*check if there is https:/" string*/
			(c == 't' && is_https == 1) ||
			(c == 't' && is_https == 2) ||
			(c == 'p' && is_https == 3) ||
			(c == 's' && is_https == 4) ||
			(c == ':' && is_https == 5) ||
			(c == '/' && is_https == 6))
			is_https++;
		else if(c == '/' && is_https == 7)
			spot_url(_sock, _site_queue, page, NULL, _page_depth);
		else
			is_https = 0;
		
		if((c == 'h' && is_href == 0) ||				/*check if there is href=" string*/
			(c == 'r' && is_href == 1) ||
			(c == 'e' && is_href == 2) ||
			(c == 'f' && is_href == 3) ||
			(c == '=' && is_href == 4) ||
			(c == '"' && is_href == 5) )
			is_href++;
		else if(c == '/' && is_href == 6)
			spot_url(_sock, _site_queue, page, _host_name, _page_depth);
		else
			is_href = 0;
	}
	fclose(page);
	
	/*if(!is_query)
		remove(path);*/
	
	return 0;
}

int spot_url(int _sock, site_node_t **_site_queue, FILE *page, char *_host_name, int _page_depth)
{
	int i;
	char c;
	char read_char;
	char has_domain;
	char url[URL_MAX_LEN];
	char aux[URL_MAX_LEN];
	url_info_t url_info;

	
	url_info.host_name[0] = '\0';
	url_info.path[0] = '\0';
	url_info.file_ext[0] = '\0';
	
	has_domain = i = 0;
	while(read(_sock, &read_char, 1) > 0)				/*read from socket 1 character at time*/
	{
		c = tolower(read_char);
		fputc(c, page);
		
		/*check if c is a valid url's character*/
		if((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') ||
			c == '.' || c == '/' || c == '_' || c == '$' || c == ',' || c == '-' ||
			c == ';' || c == '+' || c == '&' || c == '%' || c == '?' || c == '=' || c == ':')
		{
			url[i++] = c;								/*create url string*/
			if(c == '.')								/*count number of domain*/
				has_domain++;
		}
		else											/*check if url is a good link*/
		{
			url[i] = '\0';					
			if(_host_name)
			{
				strcpy(aux, url);
				strcpy(url, _host_name);
				strcat(url, "/");
				strcat(url, aux);
			}
			get_url_info(url, &url_info);				/*obtain url's information*/
			if(i > 4 && (has_domain || _host_name) && file_ext_is_good(url_info.file_ext))
			{
				if(!is_known_page(url))					/*if url not exist in hash table*/
				{
					if(strcmp(seed_host, url_info.host_name) != 0 && (_page_depth + 1) <= max_depth)
						add_url(_site_queue, url, url_info.host_name, _page_depth + 1);
					else if(strcmp(seed_host, url_info.host_name) == 0)
						add_url(_site_queue, url, url_info.host_name, _page_depth);
				}
				has_domain = 0;
			}
			return 0;
		}
	}
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