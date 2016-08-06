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
#include <future>
#include <iostream>
#include "TCPListener.h"
#include "S3TPListener.h"

#define DATAFILE_PATH "/tmp/maptData.bin"

using namespace std;

/**
 * Continuously receives TCP Packets and stores them to the disk
 */
void receiveTCPPackets(MAPTPacketParser *maptPacketParser) {
    try {
        TCPListener tcpListener(*maptPacketParser, 1337);
        tcpListener.receiveData();
    } catch (string error) {
        cerr << "ERROR in TCPReceiver thread: " << error << endl;
    }
    cerr << "ERROR: TCPReceiver thread stopped unexpectedly!" << endl;
    exit(1);
}

/**
 * Continuously wait for commands from the ground station and handle them accordingly
 */
void receiveGroundStationCommands(S3TPListener *s3tpListener) {
    try {
        s3tpListener->waitForCommands();
    } catch (string error) {
        cerr << "ERROR in S3TPListener thread: " << error << endl;
    }
    cerr << "ERROR: S3TPListener thread stopped unexpectedly!" << endl;
    exit(1);
}

/**
 * Continuously send the received data to the ground station
 */
void sendDataToGroundStation(DataHandler *dataHandler) {
    try {
        while(true) {
            dataHandler->sendData();
        }
    } catch (string error) {
        cerr << "ERROR in S3TP Sender thread: " << error << endl;
    }
    cerr << "ERROR: S3TP Sender thread stopped unexpectedly!" << endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    try {
        S3TPHandler s3tpHandler;
        DataHandler dataHandler(s3tpHandler, DATAFILE_PATH);
        MAPTPacketParser maptPacketParser(dataHandler);
        CommandHandler commandHandler(dataHandler);
        S3TPListener s3tpListener(s3tpHandler, commandHandler);
        future<void> tcpReceiverThread = async(launch::async, receiveTCPPackets, &maptPacketParser);
        s3tpHandler.initialize();
        future<void> s3tpSenderThread = async(launch::async, sendDataToGroundStation, &dataHandler);
        future<void> s3tpListenerThread = async(launch::async, receiveGroundStationCommands, &s3tpListener);
        tcpReceiverThread.get();
        s3tpSenderThread.get();
        s3tpListenerThread.get();
    } catch(string error) {
        cerr << "ERROR catched in main: " << error << endl;
    }
    return 0;
}