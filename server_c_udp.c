// Inspiration from http://www.linuxhowtos.org/data/6/server_udp.c
//I copied the general outline

#include <stdbool.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <ctype.h>

#define BUFSIZE 1024

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
   int sock, length, n;
   socklen_t fromlen;
   struct sockaddr_in server;
   struct sockaddr_in from;
   char buf[1024];

   if (argc < 2) {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(0);
   }
   
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Opening socket");
   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(atoi(argv[1]));
   if (bind(sock,(struct sockaddr *)&server,length)<0) 
       error("binding");
   fromlen = sizeof(struct sockaddr_in);
   n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
   buf[n-1]= '\0'; 
   // printf("recieved from client: n is: %d", n);
   //  printf("Buf is '%s'\n", buf);
   
   if (n < 0) error("recvfrom");
   while (1) {
     int sum = 0;
     char buf2[1024];
     //     printf("Buf is : '%s'\n", buf);
     //  printf("length is %d\n", strlen(buf));
     bool string_is_ok = true;
     for (int i = 0; i < (strlen(buf)); i++){
       //printf("Buf[i] is %c\n", buf[i]);
	if (!isdigit(buf[i])){
	  n = sendto(sock,"Sorry, cannot compute!",23, 0,(struct sockaddr *)&from,fromlen);
	  if (n < 0) 
	    error("socket 2");
	  string_is_ok = false;
	  break;
	}
	sum+= (buf[i]-'0');
      }
     if (string_is_ok == false)
       break;
     sprintf(buf2, "%d", sum);
     //  printf("Sending to client:  '%s'\n", buf2);
     n = sendto(sock,buf2,17,
                  0,(struct sockaddr *)&from,fromlen);
     if (n <0)
       error ("ERROR writing to socket");
     bzero(buf, BUFSIZE);
     n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
     // printf("Recieved from client, n is: %d\n", n);
     //  printf("buf is '%s'\n", buf);
     if (n < 0) 
	  error("ERROR reading from socket");
     sprintf(buf, "%d", sum);
     if (strlen(buf2) ==1){
	//	printf("\n");
	//	sleep(1);
       //       printf("Server sending a\n");
       n = sendto(sock,"a", strlen("a"),
                  0,(struct sockaddr *)&from,fromlen);
       if (n < 0) 
	  error("socket 1");
      	
	//	printf("Sending Letter A and exit\n");
        break;
      }
   }
   close(sock);
   //   return 0;
 }

