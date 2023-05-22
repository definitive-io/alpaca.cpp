#include "socket.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

ClientConnection::ClientConnection(const int listenSockfd) : listenSockfd(listenSockfd) {
    sockaddr_in clientAddress{};
    socklen_t clientAddressLength = sizeof(clientAddress);
    if ((clientSockfd = accept(listenSockfd, (struct sockaddr*)&clientAddress, &clientAddressLength)) < 0) {
        fprintf(stderr, "Failed to create client socket connection.");
        exit(EXIT_FAILURE);
    }

    // Parse client IP address
    struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&clientAddress;
	struct in_addr ipAddr = pV4Addr->sin_addr;
	char clientIp[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &ipAddr, clientIp, INET_ADDRSTRLEN);
	printf("Client connection from IP: %s\n", clientIp);
    this->clientIp = std::string(clientIp);
}

ClientConnection::~ClientConnection() {
    close(clientSockfd);
}

std::string ClientConnection::getClientIp() {
    return clientIp;
}

ssize_t ClientConnection::getRequest(char * buffer, size_t bufferSize) {
    ssize_t bytesRead = ::read(clientSockfd, buffer, bufferSize - 1);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        std::cout << "Received prompt request: " << buffer << std::endl;
    } else {
        std::cerr << "Failed to read request from client" << std::endl;
    }
    return bytesRead;
}

ssize_t ClientConnection::sendResponse(char * buffer, size_t bufferSize) {
    ssize_t bytesWritten = ::write(clientSockfd, buffer, bufferSize);
    if (bytesWritten > 0) {
        buffer[bytesWritten] = '\0';
        std::cout << "Wrote prompt response: " << buffer << std::endl;
    } else {
        std::cerr << "Failed to write response to client" << std::endl;
    }
    return bytesWritten;
}


Socket::Socket(const std::string& ipAddress, int port)
    : ipAddress(ipAddress), port(port), sockfd(-1)
{
    openSocket();
}

Socket::~Socket() {
    closeSocket();
}

ClientConnection * Socket::getNextRequest() {
        return new ClientConnection(sockfd);
}

void Socket::openSocket() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    if (inet_pton(AF_INET, ipAddress.c_str(), &(serverAddress.sin_addr)) <= 0) {
        std::cerr << "Invalid address or address not supported" << std::endl;
        closeSocket();
        return;
    }

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to bind server socket" << std::endl;
        close(sockfd);
        return;
    }

    if (listen(sockfd, 5) < 0) {
        std::cerr << "Failed to listen on server socket" << std::endl;
        close(sockfd);
        return;
    }

    std::cout << "Server listening on port " << port << std::endl;
}

void Socket::closeSocket() {
    if (sockfd != -1) {
        close(sockfd);
        sockfd = -1;
    }
}