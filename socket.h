#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <netinet/in.h>

class ClientConnection {
    public:
        ClientConnection(const int sockfd);
        ~ClientConnection();
        ssize_t getRequest(char * buffer, size_t bufferSize);
        ssize_t sendResponse(char * buffer, size_t bufferSize);
        std::string getClientIp();

    private:
        const int listenSockfd;
        std::string clientIp;
        int clientSockfd;
};

class Socket {
    public:
        Socket(const std::string& ipAddress, int port);
        ~Socket();
        ClientConnection * getNextRequest();

    private:
        std::string ipAddress;
        int port;
        int sockfd;
        struct sockaddr_in address;

        void openSocket();
        void closeSocket();
};

#endif  // SOCKET_H