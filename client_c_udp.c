// Inspiration from http://www.linuxhowtos.org/data/6/client_udp.c
// I copied the general outline

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void error(const char *);
int main(int argc, char *argv[])
{
   int sock, n;
   unsigned int length;
   struct sockaddr_in server, from;
   struct hostent *hp;
   char buffer[1024];
   
   if (argc != 3) { printf("Usage: server port\n");
                    exit(1);
   }
   sock= socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("socket");

   server.sin_family = AF_INET;
   hp = gethostbyname(argv[1]);
   if (hp==0) error("Unknown host");

   bcopy((char *)hp->h_addr, 
        (char *)&server.sin_addr,
         hp->h_length);
   server.sin_port = htons(atoi(argv[2]));
   length=sizeof(struct sockaddr_in);
   printf("Enter string: ");
   bzero(buffer,1024);
   fgets(buffer,1024,stdin);
   n=sendto(sock,buffer,
            strlen(buffer),0,(const struct sockaddr *)&server,length);

   if (n < 0) error("Sendto");
   int i = 0;
   while(1){
     i++;
     bzero(buffer,1024);
     n = recvfrom(sock,buffer,1024,0,(struct sockaddr *)&from, &length);
     if (n < 0) error("recvfrom");
     //     printf("Recive from server, N is %d\n", n);
     //  printf("Buf is  '%s'\n", buffer);
     buffer[n] = '\0';
     
     if (strcmp(buffer, "a")==0){
		break;
	  }
     if (strcmp(buffer, "Sorry, cannot compute!")==0){
	 printf("Sorry, cannot compute!\n");
	 //n=sendto(sock,temp,
	 // strlen(temp),0,(const struct sockaddr *)&server,length);
	 break;
       }
     printf("From server: %s\n",buffer);
     char temp[126];
     sprintf(temp, "%d", i);
     // printf("sending '%s' to server\n", temp);
     n=sendto(sock,temp,
            strlen(temp),0,(const struct sockaddr *)&server,length);
      if (n < 0) error("Sendto");


   }  
   close(sock);
   return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
