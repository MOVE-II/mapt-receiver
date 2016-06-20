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
#include "UDPListener.h"

using namespace std;

UDPListener::UDPListener(int port) :
    port(port),
    socketFileDescriptor(-1),
    dataList() {
    memset(&sockaddrIn, 0, sizeof(sockaddrIn));
    initializeSocket();
}

/**
 * Receive data via UDP and store it by passing it to the DataHandler.
 */
void UDPListener::receiveData() {

}

/**
 * Initializes the socket. On error, a string exception is thrown.
 */
void UDPListener::initializeSocket() {
    socketFileDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(socketFileDescriptor == -1) {
        string error = string("Socket could not be initialized: ") + strerror(errno);
        throw error;
    }
    sockaddrIn.sin_family = AF_INET;
    sockaddrIn.sin_port = htons(port);
    sockaddrIn.sin_addr.s_addr = htonl(INADDR_ANY);
    int rc = bind(socketFileDescriptor, (struct sockaddr*) &sockaddrIn, sizeof(sockaddrIn));
    if(rc != 0) {
        string error = string("Could not bind socket to specified port: ") + strerror(errno);
        throw error;
    }
}
