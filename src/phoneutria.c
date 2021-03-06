#include "phoneutria.h"

time_t start;

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
      
    timeout.tv_sec = 5;
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
	char **seed_host;
	time_t stop;
	
	url_info = malloc(sizeof(url_info_t));
	seed_host = malloc(sizeof(char *) * _num_seeds);
	site_queue = NULL;
	
	for(i = 0; i < _num_seeds; i++)
	{
		if(strncasecmp(_seeds[i], "http://", 7) == 0)
			_seeds[i] = &(_seeds[i][7]);

		if(strncasecmp(_seeds[i], "www.", 4) == 0)
			_seeds[i] = &(_seeds[i][4]);
		printf("seed: %s\n", _seeds[i]);
		get_url_info(_seeds[i], url_info);
		is_known_page(_seeds[i]);
		add_url(&site_queue, _seeds[i], url_info->host_name, 0);
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
		//printf("RICHIESTA %s\n", request);
		write(sock, request, 1024);
		parse_page(sock, &site_queue, url_info->host_name, url_info->path, _query, _num_query, page_depth, seed_host, _num_seeds);
		//print_queue(site_queue);
		close(sock);
		stop = time(NULL);
		if(stop-start > sim_time * 60) {
			printf("Simulation completed! Exiting...\n");
      		return 0;
		}
	}
		
	free(url_info);
	free_queue(site_queue);
	printf("Simulation completed! Exiting...\n");
	return 0;
}

int main(int argc, char **argv)
{
	int i;
	DIR *dir;
	char **seeds, **keys;
	int num_seeds, num_keys;
	
	if( argc < 5){
		printf("Error: too few arguments!\nUsage: ./phoneutria SEED1 [SEED2 SEED3 .. ] -t TIME_SIM KEYWORD1 [KEYWORD2 KEYWORD3 .. ]\n");
 		return 0;
	}
	
	init_hash_table();
	
	if(!(dir = opendir("output")))
		mkdir("output", S_IRWXU | S_IRWXG | S_IRWXO);
	else 
		closedir(dir);

	num_seeds = num_keys = 0;
	
	seeds = &argv[1];
	i = 1;
	while(strcmp(argv[i++], "-t"));
	
	num_seeds = i - 2;
	
	/*extern defined in link_parser.h*/
	sim_time = strtol(argv[i], NULL, 10);
	
	keys = &argv[i + 1];
	num_keys = argc - i - 1;

	start = time(NULL);
	
	get_page(seeds, num_seeds, keys, num_keys);
	
	
	
	exit(0);
}
