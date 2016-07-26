/**
 * Copyright (C) 2016  Jonas Sticha (jonas.sticha@tum.de)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include "TCPListener.h"

using namespace std;

TCPListener::TCPListener(MAPTPacketParser& maptPacketParser, int port) :
    port(port),
    maxPacketSize(1500),
    serverSocketFileDescriptor(-1),
    maptPacketParser(maptPacketParser) {
    memset(&sockaddrIn, 0, sizeof(sockaddrIn));
    initializeSocket();
}

/**
 * Receive data via UDP and store it by passing it to the DataHandler.
 */
void TCPListener::receiveData() {
    int dataReceived;
    char buffer[maxPacketSize];
    while(true) {
        socklen_t sockaddrInSize = sizeof(sockaddrIn);
        int clientSocketFileDescriptor = accept(serverSocketFileDescriptor, (struct sockaddr*) &sockaddrIn, &sockaddrInSize);
        if(clientSocketFileDescriptor == -1) {
            string error = string("Failed to accept client: ") + strerror(errno);
            throw error;
        }
        dataReceived = recvfrom(serverSocketFileDescriptor, buffer, maxPacketSize, 0, nullptr, nullptr);
        if(dataReceived > 0) {
            char* data = (char*) malloc(dataReceived);
            memcpy(data, buffer, dataReceived);
            maptPacketParser.parseData(data, dataReceived);
        } else if(dataReceived == -1) {
            string error = string("Failed to receive data from socket: ") + strerror(errno);
            throw error;
        }
    }
}

/**
 * Initializes the socket. On error, a string exception is thrown.
 */
void TCPListener::initializeSocket() {
    if(serverSocketFileDescriptor == -1) {
    serverSocketFileDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        string error = string("Socket could not be initialized: ") + strerror(errno);
        throw error;
    }
    sockaddrIn.sin_family = AF_INET;
    sockaddrIn.sin_port = htons(port);
    sockaddrIn.sin_addr.s_addr = htonl(INADDR_ANY);
    int rc = bind(serverSocketFileDescriptor, (struct sockaddr*) &sockaddrIn, sizeof(sockaddrIn));
    if(rc != 0) {
        string error = string("Could not bind socket to specified port: ") + strerror(errno);
        throw error;
    }
    if(listen(serverSocketFileDescriptor, 1) != 0) {
        string error = string("Could not set socket in listening mode: ") + strerror(errno);
        throw error;
    }
}
