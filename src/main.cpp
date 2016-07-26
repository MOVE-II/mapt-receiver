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
#include <thread>
#include <iostream>
#include "TCPListener.h"
#include "S3TPHandler.h"
#include "S3TPListener.h"

using namespace std;

void runUDPReceiverThread(DataHandler* dataHandler) {
    try {
        TCPListener udpListener(*dataHandler, 1337);
        udpListener.receiveData();
    } catch (string error) {
        cerr << "ERROR in UDPReceiver thread: " << error << endl;
        exit(1);
    }
}

void runS3TPListenerThread(S3TPListener* s3tpListener) {
    try {
        s3tpListener->waitForCommands();
    } catch (string error) {
        cerr << "ERROR in S3TPListener thread: " << error << endl;
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    S3TPHandler s3tpHandler;
    DataHandler dataHandler(s3tpHandler);
    CommandHandler commandHandler(dataHandler);
    S3TPListener s3tpListener(s3tpHandler, commandHandler);
    thread udpReceiverThread(runUDPReceiverThread, &dataHandler);
    thread s3tpListenerThread(runS3TPListenerThread, &s3tpListener);
    udpReceiverThread.join();
    s3tpListenerThread.join();
    return 0;
}