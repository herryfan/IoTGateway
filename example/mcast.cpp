/*
 * This file defines mcast app tp test multicast
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>  
#include <limits.h>  
#include <memory.h>  
#include <signal.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <arpa/inet.h>  
#include <netdb.h>  
#include <netinet/in.h>
#include "DgramMulticast.h"

static unsigned short gport;
static char ggroup_ip[0xff];

void usage()
{
    printf("Please input parameter\n"
            "\t-t\t\t app type, c as client, s as server\n"
            "\t-p\t\t port, default value is xxxx\n"
            "\t-g\t\t group ip, default value is xxxx\n");
}


void client()
{
    int socket_fd;
    struct sockaddr_in local_address;
    Dgram_Multicast mcast(gport, ggroup_ip);
    char buffer[0xff];
    sockaddr_in peer_addr;
    socklen_t socklen;
    socklen = sizeof(struct sockaddr_in);


    socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_fd == -1)
    {
        printf("Failed to create cleint socket fd\n");
        return;
    }

    bzero(&local_address, sizeof(local_address));

    local_address.sin_family = AF_INET;
    local_address.sin_port = htons(2345);
    local_address.sin_addr.s_addr = htons(INADDR_ANY);

    if (bind(socket_fd, (struct sockaddr*)&local_address, sizeof(local_address))
            < 0)
    {
        printf("Failed to bind client socket\n");
        return;
    }

    while(1)
    {
        int n = sendto(socket_fd, "hello,world", 11, 0, (struct
                    sockaddr*)&mcast.GetMulticastAddress().ipv4_addr(),sizeof(struct
                        sockaddr_in));
        if (n < 0)
        {
            printf("Failed to send message to group\n");
            return;
        }
            
       int revLen = recvfrom(socket_fd,buffer,sizeof(buffer),0,(struct
                   sockaddr*)&peer_addr, &socklen);
    
       printf("recv %s from [%s:%d]\n", buffer,
               inet_ntoa(peer_addr.sin_addr), peer_addr.sin_port);
        
        sleep(5);
    }
}

void server()
{
    int socket_fd;
    Dgram_Multicast mcast(gport, ggroup_ip);
    sockaddr_in peer_addr;
    char buffer[0xff];
    int counter = 0;
    socklen_t socklen;
    socklen = sizeof(struct sockaddr_in);

    socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (socket_fd == -1)
    {
        printf("Failed to create server socket\n");
        return;
    }

    if (mcast.join(socket_fd) < 0 )
    {
        printf("Failed to join group\n");
        return;
    }

    while(1)  
    {     
       int revLen = recvfrom(socket_fd,buffer,sizeof(buffer),0,(struct
                   sockaddr*)&peer_addr, &socklen);

        if(revLen < 0)  
        {  
            printf("failed to recvfrom()\n");  
            return;
        }  

        else  
        {  
            printf("revLen \
                    :%d \
                    brocast_srouce:%s\t \
                    brocast_port:%d\t \
                    counter:%d\n",revLen,inet_ntoa(peer_addr.sin_addr),ntohs(peer_addr.sin_port),counter++);             
        }  

        usleep(100000);  
    }  
}

int main(int argc, char** argv)
{
    int opt;
    char app_type;


    // Set defualt value
    gport = 5683;
    strncpy(ggroup_ip, "ff05::fd", 0xff);

    while ((opt = getopt(argc, argv, "t:pg")) != -1 )
    {
        switch(opt)
        {
            case 't':
                app_type = *optarg;
                break;

            case 'p':
                gport = atoi(optarg);
                break;

            case 'g':
                strncpy(ggroup_ip, optarg, 0xff);
                break;

            default:
                usage();
                exit(0);
        }
    }

    printf("input parameter:\n"
            "\t-t\t\t%c\n"
            "\t-p\t\t%u\n"
            "\t-g\t\t%s\n",
            app_type, gport, ggroup_ip);

    switch(app_type)
    {
        case 'c':
            client();
            break;
        case 's':
            server();
            break;
        default:
            usage();
            exit(0);
    }

    return 0;
}

