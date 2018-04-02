#include "User.h"
#include "Channel.h"
#include "Server.h"
#include "commands.h"
#include "test.h"
#include "network.h"
#include <signal.h>
#include <iostream>

using std::cout;

int new_connection(int servfd, Server& serv) {
    struct sockaddr_in client;
    unsigned int cliaddr_len = sizeof(client);
    int err;
    int bytes_recv;
    int client_sock;
    char msg[] = "Hi!\n";

    client_sock = accept(servfd, (struct sockaddr*)&client, &cliaddr_len);
    if (client_sock == -1) {
        perror("accept failed");
        return 1;
        }

    cout << "accept passed";

    err = send(client_sock, msg, sizeof(msg), 0);
    if(err == -1) {
        perror("send failed");
    }

    return 0;
}

int main (int argc, const char * argv[]) {
    int nfds;
    int result;
    Server serv;
    struct timeval tv;
    struct server_socket serv_sock;
    fd_set readfds;

    //prevent process from stopping when a send() fails
    signal(SIGPIPE, SIG_IGN);
    
    serv_sock = get_socket(); 
    nfds = serv_sock.sockfd + 1;

    cout << ntohs(serv_sock.sockaddr.sin_port) << '\n';

    while(1) {
        FD_ZERO(&readfds);
        FD_SET(serv_sock.sockfd, &readfds);
        tv.tv_sec = 1000000;
        tv.tv_usec = 0;

        listen(serv_sock.sockfd, 20);

        result = select(nfds, &readfds, (fd_set*)NULL, (fd_set*)NULL, &tv);
        if (result > 0) {
            if (FD_ISSET(serv_sock.sockfd, &readfds))
                cout << "calling new_connection";
                new_connection(serv_sock.sockfd, serv);
            }
        else 
            perror("select failed");
    }

	return 0;
}
