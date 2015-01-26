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

#ifndef PHONEUTRIA_H_
#define PHONEUTRIA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <netinet/in.h>
#include "url_stack.h"
#include <netdb.h>
#include "link_parser.h"
#include "url_stack.h"

#define IPV4_ADDR_LEN 15

extern int h_errno; 											/*needed by gethostbyname*/

char *get_ip_addr(char *_host_name, char *_ip_addr);			/*get IP address from hostname*/
int create_socket(char *_host_name);							/*create socket for connection*/
int get_page(char *_seed, char **_query, int _num_query);		/*download page from url*/

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
