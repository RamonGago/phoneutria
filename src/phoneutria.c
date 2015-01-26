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

#include "phoneutria.h"

char *get_ip_addr(char *_host_name, char *_ip_addr)				/*obtain IP address from domain*/
{
  struct hostent *host_info;
    
  if((host_info = gethostbyname(_host_name)) == NULL)
    return NULL;
  
  if(inet_ntop(AF_INET, (void *)host_info->h_addr_list[0], _ip_addr, IPV4_ADDR_LEN) == NULL)
	return NULL;
  
  return _ip_addr;
}

int create_socket(char *_host_name)								/*create socket for connection*/
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
	server_addr.sin_family = AF_INET;							/*set IP's parameters*/
	server_ip = malloc(sizeof(char) * (IPV4_ADDR_LEN + 1));
	if(get_ip_addr(_host_name, server_ip) == NULL)
	{
		free(server_ip);
		return -1;
	}
	inet_aton(server_ip, &server_addr.sin_addr);
	server_addr.sin_port = htons(80);							/*set port*/
	retcode = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));		/*connect to address*/
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
	
	url_info = malloc(sizeof(url_info_t));						/*allocate memory for struct url_info*/

	site_queue = NULL;											/*init queue to store urls*/
	
	get_url_info(_seed, url_info);								/*call function to get url's information*/
	is_known_page(_seed);
	add_url(&site_queue, _seed, url_info->host_name, 0);		/*call function to add url into queue*/
	get_url_info(_seed, url_info);
	strcpy(seed_host, url_info->host_name);
	
	while(site_queue != NULL)									/*while there are urls in queue*/
	{
		url = get_url(&site_queue, &page_depth);				/*get url from queue*/
		if(url == NULL)
		{
			free(url_info);
			return 0;
		}

		if(!get_url_info(url, url_info))
			continue;
		free(url);
		
		i = 0;													/*create socket for subdomain*/
		while(url_info->subdomain[i] != NULL && (sock = create_socket(url_info->subdomain[i])) < 0)
			i++;
		
		if(sock < 0)
			continue;

		memset(request, '\0', 1024);							/*init request's memory*/
		sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", url_info->path, url_info->host_name);	/*prepare request string*/
		write(sock, request, 1024);								/*write request in socket*/
		parse_page(sock, &site_queue, url_info->host_name, _query, page_depth);							/*parse result from socket*/
		close(sock);											/*close socket*/
	}
	
	free(url_info);
	free_queue(site_queue);
	
	return 0;
}

int main(int argc, char **argv)
{
	init_hash_table();											/*init hash table for collision avoidance*/
 	get_page(argv[1], argv[2]);									/*call function to start download and search process*/
		
	exit(0);
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