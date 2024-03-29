#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

struct server_socket {
    int sockfd;
    struct sockaddr_in sockaddr;
};

struct server_socket get_socket();

int Send(int sockfd, const void *buf, unsigned int len, int flags);

int Accept(int sockfd, struct sockaddr *addr, unsigned int* addrlen);

int Recv(int sockfd, void *buf, unsigned int len, int flags);
