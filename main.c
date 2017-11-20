/*
    UDP Server
*/
#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <arpa/inet.h>
#include <sys/socket.h>
#include <assert.h>
 
#define BUFLEN 100   //Max length of buffer
#define PORT 29000   //The port on which to listen for incoming data
#define IP_REC "192.168.0.254"
 
void die(char *s)
{
    perror(s);
    exit(1);
}


struct udp_data{
	int i_crete;
    int state;
    int temperature;
    int to_plus1;
    int to_minus1;
    int i_plus1;
    int from_plus1;
    int i_minus1;
    int from_minus1;
};


int main(void)
{
    struct sockaddr_in si_me, si_other;
    struct udp_data data_udp[254];
    int s, i, slen = sizeof(si_other) , recv_len;
    int i_crete, state, temperature, to_plus1, to_minus1, i_plus1, from_plus1, i_minus1, from_minus1;
    char buf[BUFLEN];
    char ip_addr[8];

     
    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
     
    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = inet_addr("192.168.0.254"); //htonl(INADDR_ANY);
     

    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }
     
    //keep listening for data
    while(1)
    {
        printf("Waiting for data...");
        fflush(stdout);        
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }
         
        *ip_addr = (char) inet_ntoa(si_other.sin_addr);
        printf("%s\n", ip_addr);
	
        //print details of the client/peer and the data received
    	i_crete = 0;      
    	state = 0;      
    	temperature = 0;      
    	to_plus1 = 0;      
    	to_minus1 = 0;      
    	i_plus1 = 0;      
    	from_plus1 = 0;      
    	i_minus1 = 0;      
    	from_minus1 = 0;      

    	if(recv_len > 0)
        {
            printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));

    	    i_crete 	= ((i_crete | buf[1]) << 8) | buf[0];
    	    state 	= ((state | buf[3]) << 8) | buf[2];
    	    temperature = ((temperature | buf[5]) << 8) | buf[4];
    	    to_plus1 	= ((to_plus1 | buf[7]) << 8) | buf[6];
    	    to_minus1 	= ((to_minus1 | buf[9]) << 8) | buf[8];
    	    i_plus1 	= ((i_plus1 | buf[11]) << 8) | buf[10];
    	    from_plus1 	= ((from_plus1 | buf[13]) << 8) | buf[12];
    	    i_minus1 	= ((i_minus1 | buf[15]) << 8) | buf[14];
    	    from_minus1 = ((from_minus1 | buf[17]) << 8) | buf[16];

    	    printf("I_crete: %d\n" , i_crete);
            printf("state: %d\n" , state);
            printf("temperature: %d\n" ,temperature);
            printf("to_plus1: %d\n" ,to_plus1 );
            printf("to_minus1: %d\n" , to_minus1);
            printf("i_plus1: %d\n" , i_plus1);
            printf("from_plus1: %d\n" ,from_plus1);
            printf("i_minus1: %d\n" , i_minus1);
            printf("from_minus1: %d\n" , from_minus1);

        }

        /* 
        //now reply the client with the same data
        if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            die("sendto()");
        }
        */
    }
 
    shutdown(s,2);
    return 0;
}
