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


enum state_codes{
    INIT,
    off,
    cc_at_plus1,
    on,
    cc_from_on,
    cc_at_minus1,
    lock_cmd,
    lock_sw,
    lock_hw,
    lock_emergency,
    lock_overheat,
    reset_oc,
    lock_controlTimeout,
    on_static,
    on_static_stress,
    NUM_STATES
};

struct udp_data{
    int i_crete;
    enum state_codes state;
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
    //int i_crete, state, temperature, to_plus1, to_minus1, i_plus1, from_plus1, i_minus1, from_minus1;
    char buf[BUFLEN];
    char ip_addr_c[3];
    int ip_addr1, ip_addr2, ip_addr3;
    int ip_addr;


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
        printf("=============================================\n");
        printf("Waiting for data...");
        fflush(stdout);        
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }

        ip_addr_c[0] = (char) inet_ntoa(si_other.sin_addr)[10];
        ip_addr_c[1] = (char) inet_ntoa(si_other.sin_addr)[11];
        ip_addr_c[2] = (char) inet_ntoa(si_other.sin_addr)[12];
        ip_addr1 = ip_addr_c[0] - '0';
        ip_addr2 = ip_addr_c[1] - '0';
        ip_addr3 = ip_addr_c[2] - '0';
        ip_addr = ip_addr1*100 + ip_addr2*10 + ip_addr3;
        printf("\nIP struct: %d\n", ip_addr);

        data_udp[ip_addr].i_crete     = (((data_udp[ip_addr].i_crete && 0) | buf[1]) << 8) | buf[0];
        data_udp[ip_addr].state       = (((data_udp[ip_addr].state && 0) | buf[3]) << 8) | buf[2];
        data_udp[ip_addr].temperature = (((data_udp[ip_addr].temperature && 0) | buf[5]) << 8) | buf[4];
        data_udp[ip_addr].to_plus1    = (((data_udp[ip_addr].to_plus1 && 0) | buf[7]) << 8) | buf[6];
        data_udp[ip_addr].to_minus1   = (((data_udp[ip_addr].to_minus1 && 0) | buf[9]) << 8) | buf[8];
        data_udp[ip_addr].i_plus1     = (((data_udp[ip_addr].i_plus1 && 0) | buf[11]) << 8) | buf[10];
        data_udp[ip_addr].from_plus1  = (((data_udp[ip_addr].from_plus1 && 0) | buf[13]) << 8) | buf[12];
        data_udp[ip_addr].i_minus1    = (((data_udp[ip_addr].i_minus1 && 0) | buf[15]) << 8) | buf[14];
        data_udp[ip_addr].from_minus1 = (((data_udp[ip_addr].from_minus1 && 0) | buf[17]) << 8) | buf[16];
        
        printf("I_crete: %d\n" ,    data_udp[ip_addr].i_crete);
        if(data_udp[ip_addr].state == INIT)
            printf("state: INIT\n");
        if(data_udp[ip_addr].state == off)
            printf("state: off\n");
        if(data_udp[ip_addr].state == on)
            printf("state: on\n");
        if(data_udp[ip_addr].state == cc_from_on)
            printf("state: cc_from_on\n");
        if(data_udp[ip_addr].state == cc_at_minus1)
            printf("state: cc_at_minus1\n");
        if(data_udp[ip_addr].state == lock_cmd)
            printf("state: lock_cmd\n");
        if(data_udp[ip_addr].state == lock_sw)
            printf("state: lock_sw\n");
        if(data_udp[ip_addr].state == lock_hw)
            printf("state: lock_hw\n");
        if(data_udp[ip_addr].state == lock_emergency)
            printf("state: lock_emergency\n");
        if(data_udp[ip_addr].state == lock_overheat)
            printf("state: lock_overheat\n");
        if(data_udp[ip_addr].state == reset_oc)
            printf("state: reset_oc\n");
        if(data_udp[ip_addr].state == lock_controlTimeout)
            printf("state: lock_controlTimeout\n");
        if(data_udp[ip_addr].state == on_static)
            printf("state: on_static\n");
        if(data_udp[ip_addr].state == on_static_stress)
            printf("state: on_static_stress\n");
        if(data_udp[ip_addr].state == cc_at_plus1)
            printf("state: cc_at_plus1\n");


       printf("temperature: %d\n" ,data_udp[ip_addr].temperature);
       printf("to_plus1: %d\n" ,   data_udp[ip_addr].to_plus1 );
       printf("to_minus1: %d\n" ,  data_udp[ip_addr].to_minus1);
       printf("i_plus1: %d\n" ,    data_udp[ip_addr].i_plus1);
       printf("from_plus1: %d\n" , data_udp[ip_addr].from_plus1);
       printf("i_minus1: %d\n" ,   data_udp[ip_addr].i_minus1);
       printf("from_minus1: %d\n", data_udp[ip_addr].from_minus1);        


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
