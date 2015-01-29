#include "phoneutria.h"

char *get_ip_addr(char *_host_name, char *_ip_addr)
{
  struct hostent *host_info;
    
  if((host_info = gethostbyname(_host_name)) == NULL)
    return NULL;
  
  if(inet_ntop(AF_INET, (void *)host_info->h_addr_list[0], _ip_addr, IPV4_ADDR_LEN) == NULL)
	return NULL;
  
  return _ip_addr;
}

int create_socket(char *_host_name)
{
	char *server_ip;
	int sock;
	int retcode;
	struct sockaddr_in server_addr;
	struct timeval timeout;
	
	if((sock = socket(AF_INET,SOCK_STREAM, 0)) == -1)
	{
		printf("Error while creating the socket	\n");
		exit(1);
	}
	server_addr.sin_family = AF_INET;
	server_ip = malloc(sizeof(char) * (IPV4_ADDR_LEN + 1));
	if(get_ip_addr(_host_name, server_ip) == NULL)
	{
		free(server_ip);
		return -1;
	}
	inet_aton(server_ip, &server_addr.sin_addr);
	server_addr.sin_port = htons(80);
	retcode = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(retcode == -1)
	{
		printf("Error while connecting to %s\n", server_ip);
		free(server_ip);
		return -1;
	}
      
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
        printf("setsockopt failed\n");
	
	free(server_ip);
	
	return sock;
}

int get_page(char **_seeds, int _num_seeds, char **_query, int _num_query)
{
	int sock;
	char request[1024];
	char *url;
	url_info_t *url_info;
	site_node_t *site_queue;
	int i;
	int page_depth;
	
	url_info = malloc(sizeof(url_info_t));

	site_queue = NULL;
	seed_host = malloc(sizeof(char *) * (_num_seeds + 1));
	seed_host[_num_seeds] = NULL;
	
	for(i = 0; i < _num_seeds; i++)
	{
		get_url_info(_seeds[i], url_info);
		is_known_page(_seeds[i]);
		add_url(&site_queue, _seeds[i], url_info->host_name, 0);
		get_url_info(_seeds[i], url_info);
		//strcpy(seed_host, url_info->host_name);
		seed_host[i] = strdup(url_info->host_name);
	}
	
	while(site_queue != NULL)
	{
		url = get_url(&site_queue, &page_depth);
		if(url == NULL)
		{
			free(url_info);
			return 0;
		}
		if(!get_url_info(url, url_info))
			continue;
		free(url);
		
		i = 0;
		while(url_info->subdomain[i] != NULL && (sock = create_socket(url_info->subdomain[i])) < 0)
			i++;
		
		/*if(sock < 0)
			continue;*/
		memset(request, '\0', 1024);
		sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", url_info->path, url_info->host_name);
		write(sock, request, 1024);
		parse_page(sock, &site_queue, url_info->host_name, _query, _num_query, page_depth, _seeds, _num_seeds);
		//print_queue(site_queue);
		close(sock);
	}
		
	free(url_info);
	free_queue(site_queue);
	
	return 0;
}

int main(int argc, char **argv)
{
	int i;
	char **seeds, **keys;
	int num_seeds, num_keys;
	
	init_hash_table();

	num_seeds = num_keys = 0;
	
	seeds = &argv[1];
	i = 1;
	while(strcmp(argv[i++], "-d"));
	
	num_seeds = i - 2;
	
	/*extern defined in link_parser.h*/
	depth = strtol(argv[i], NULL, 10);
	
	keys = &argv[i + 1];
	num_keys = argc - i - 1;
	
	get_page(seeds, num_seeds, keys, num_keys);
	
	
	
	exit(0);
}