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
	free(server_ip);
	
	return sock;
}

int get_page(char *_seed, char *_query)
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
	
	get_url_info(_seed, url_info);
	/***/is_known_page(_seed);
	add_url(&site_queue, _seed, url_info->host_name, 0);
	get_url_info(_seed, url_info);
	strcpy(seed_host, url_info->host_name);
	
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
		
		if(sock < 0)
			continue;
		memset(request, '\0', 1024);
		sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", url_info->path, url_info->host_name);
		write(sock, request, 1024);
		parse_page(sock, &site_queue, url_info->host_name, _query, page_depth);
		//print_queue(site_queue);
		close(sock);
	}
		
	free(url_info);
	free_queue(site_queue);
	
	return 0;
}

int main(int argc, char **argv)
{
	/*FILE *f;
	
	f = fopen("dump_test/dump", "w");
	fclose(f);*/
	
	init_hash_table();
 	get_page(argv[1], argv[2]);
	
	/*url_info_t *url_info = malloc(sizeof(url_info_t));
	char *subd;
	int i;
	
	get_url_info(argv[1], url_info);
	
	i = 0;
	while((subd = url_info->subdomain[i]))
	{
		printf("%s\n", subd);
		i++;
	}*/
	
	/*url_info_t *url_info = malloc(sizeof(url_info_t));

	if(!get_url_info(argv[1], url_info))
	{
		printf("%s is not an url!\n", argv[1]);
		exit(1);
	}

	
	printf("Host name: %s\n", url_info->host_name);
	
	if(strlen(url_info->path) == 0)
		printf("File path: none\n");
	else
		printf("File path: %s\n", url_info->path);
	
	if(strlen(url_info->file_ext) == 0)
		printf("File extension: none\n");
	else
		printf("File extension: %s\n", url_info->file_ext);

	if(file_ext_is_good(url_info->file_ext))
		printf("Valid extension\n");
	else
		printf("Invalid extension\n");*/
	
	exit(0);
}