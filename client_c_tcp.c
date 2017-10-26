//Inspiration from https://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/tcpclient.c
//I took the intro part
#include <stdio.h> 
    #include <stdlib.h> 
    #include <errno.h> 
    #include <string.h> 
    #include <netdb.h> 
    #include <sys/types.h> 
    #include <netinet/in.h> 
    #include <sys/socket.h> 
    #include <unistd.h>

     

    #define MAXDATASIZE 100 /* max number of bytes we can get at once */

    int main(int argc, char *argv[])
    {
        int sockfd, numbytes;  
        char buf[MAXDATASIZE];
        struct hostent *he;
        struct sockaddr_in their_addr; /* connector's address information */

        if (argc != 3) {
            fprintf(stderr,"usage: client hostname portnumber\n");
            exit(1);
        }




        if ((he=gethostbyname(argv[1])) == NULL) {  /* get the host info */
            herror("gethostbyname");
            exit(1);
        }

        int portnumber = atoi(argv[2]);
	  

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("socket");
            exit(1);
        }

        their_addr.sin_family = AF_INET;      /* host byte order */
        their_addr.sin_port = htons(portnumber);    /* short, network byte order */
        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
        bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */

        if (connect(sockfd, (struct sockaddr *)&their_addr, \
                                              sizeof(struct sockaddr)) == -1) {
            perror("connect");
            exit(1);
        }
	
	char message[128];
	printf("Enter string: ");
	scanf("%s", message);
	//	printf("you entered: '%s'\n", message);
		
	if (send(sockfd, message, strlen(message), 0) == -1){
	  perror("send");
	  exit (1);
	}
	//	printf("After the send function \n");
	//	int i = 1;
	while(1){
	  //	  if (i >5)
	  // break;
	  //i++;
	  if ((numbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
            		perror("recv");
            		exit(1);
	  }
	  buf[numbytes] = '\0';
	  //   	  printf("!!!! BUF IS %s to server\n", buf);
	      
          if (strcmp(buf, "a")==0){
		break;
	  }
		
	  printf("From server: %s\n",buf);
	  // sprintf(buf, "a");
		//		printf("sssss\n");
	  if (send(sockfd, "a", strlen("a"), 0) == -1){
	      perror("send");
	      exit (1);
	    }
	}

        close(sockfd);

        return 0;
    }

