/*
	Port scanner code in c
*/
#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include <time.h>

int validateIP(const char *s)
{
    int len = strlen(s);

    if (len < 7 || len > 15)
        return 1;

    char tail[16];
    tail[0] = 0;

    unsigned int d[4];
    int c = sscanf(s, "%3u.%3u.%3u.%3u%s", &d[0], &d[1], &d[2], &d[3], tail);

    if (c != 4 || tail[0])
        return 1;

    for (int i = 0; i < 4; i++)
        if (d[i] > 255)
            return 1;

    return 0;
}

void *SocketMaker(start,end){

	struct hostent *host;
	struct sockaddr_in sa;
	int sock, err, i;

	//Initialise the sockaddr_in structure
	strncpy((char*)&sa , "" , sizeof sa);
	sa.sin_family = AF_INET;
	
	
	//Start the port scan loop
	printf("Starting the portscan loop: \n");
	for( i = start ; i <= end ; i++) 
	{
		//Fill in the port number
		sa.sin_port = htons(i);
		//Create a socket of type internet
		sock = socket(AF_INET , SOCK_STREAM , 0);
		
		//Check whether socket created fine or not
		if(sock < 0) 
		{
			perror("\nSocket");
			exit(1);
		}
		//Connect using that socket and sockaddr structure
		err = connect(sock , (struct sockaddr*)&sa , sizeof sa);
		
		//not connected
		if( err < 0 )
		{
			//printf("%s %-5d %s\r" , hostname , i, strerror(errno));
			fflush(stdout);
		}
		//connected
		else
		{
			printf("%-5d open\n",  i);
		}
		close(sock);
	}
	
	printf("\r");
	fflush(stdout);
}


int main(int argc , char **argv)
{
	
	int err, i , sock ,start , end;
	char hostname[100];
	pthread_t thread_id;

	printf("Enter valid IP: ");
	scanf("%s" , &hostname);
	
	//Get the hostname to scan
	while (validateIP(hostname) == 1){
		printf("Enter valid IP: ");
		scanf("%s" , &hostname);
	}
	
	//Get start port number
	printf("\nEnter START valid port number: ");
	scanf("%d" , &start);

	while (start < 0 || start > 65535){
		printf("Enter START valid port number: ");
		scanf("%d" , &start);
	}

	//Get end port number
	printf("\nEnter END valid port number (higher or equal than the starting port): ");
	scanf("%d" , &end);

	while ((end < 0) || (end > 65535) || (end < start) ){
		printf("Enter END valid port number (higher or equal than the starting port): ");
		scanf("%d" , &end);
	}

	SocketMaker(start,end);
	return(0);
}