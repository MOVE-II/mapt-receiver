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
#include <iostream>
#include "TCPListener.h"

using namespace std;

TCPListener::TCPListener(MAPTPacketParser& maptPacketParser, int port) :
    port(port),
    serverSocketFileDescriptor(-1),
    maptPacketParser(maptPacketParser) {
    memset(&sockaddrIn, 0, sizeof(sockaddrIn));
    initializeSocket();
}

/**
 * Receive data via UDP and store it by passing it to the DataHandler.
 */
void TCPListener::receiveData() {
    int dataReceivedLen;
    char buffer[MAPT_PACKAGE_SIZE];
    while(true) {
        socklen_t sockaddrInSize = sizeof(sockaddrIn);
        int clientSocketFileDescriptor = accept(serverSocketFileDescriptor, (struct sockaddr*) &sockaddrIn, &sockaddrInSize);
        if(clientSocketFileDescriptor == -1) {
            string error = string("Failed to accept client: ") + strerror(errno);
            throw error;
        }
        bool clientConnected = true;
        while(clientConnected) {
            try{
                dataReceivedLen = (int) recvfrom(clientSocketFileDescriptor, buffer, MAPT_PACKAGE_SIZE, 0, nullptr, nullptr);
                //TODO: Receive bytes until a complete package of the defined langth was received
                if(dataReceivedLen == MAPT_PACKAGE_SIZE) {
                    char* data = (char*) malloc(MAPT_PACKAGE_SIZE);
                    memcpy(data, buffer, MAPT_PACKAGE_SIZE);
                    maptPacketParser.parseData(data);
                } else if(dataReceivedLen <= 0) {
                    string error = string("Failed to receive data from socket: ") + strerror(errno);
                    throw error;
                }
            } catch (string error) {
                clientConnected = false;
                cerr << "Lost connection to client.... Waiting for new connection" << endl;
            }
        }
    }
}

/**
 * Initializes the socket. On error, a string exception is thrown.
 */
void TCPListener::initializeSocket() {
    serverSocketFileDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serverSocketFileDescriptor == -1) {
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
