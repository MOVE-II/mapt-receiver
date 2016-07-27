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

#include <string.h>
#include <iostream>
#include <sys/stat.h>
#include "DataHandler.h"

DataHandler::DataHandler(S3TPHandler& s3tpHandler, const char* dataFilePath) :
    dataFilePath(dataFilePath),
    fileMutex(),
    dataFileStream(),
    s3tpHandler(s3tpHandler){
    if(!doesFileExist(dataFilePath)) {
        cerr << "File " << dataFilePath << " doesn't exist. Creating..." << endl;
        fstream createStream(dataFilePath, ios::out);
        if(createStream.fail()) {
            string error = string("File ") + dataFilePath + string(" doesn't exist and unable to create it!");
            throw error;
        }
        createStream.close();
    }
    dataFileStream.open(dataFilePath, ios::in | ios::out | ios::binary | ios::ate);
    if(dataFileStream.fail()) {
        string error = "DataFileStream for mapt data storage could not be created.";
        throw error;
    }
    streampos fileSize = dataFileStream.tellp();
    if(fileSize != 0) {
        //Ensures that the data is still aligned to the mapt package size
        int packageOffset = (int) (fileSize % MAPT_PACKAGE_SIZE);
        if(packageOffset != 0) {
            cerr << "Data in file not aligned! ADDED aligning bytes" << endl;
            int numZeroesToAdd = MAPT_PACKAGE_SIZE - packageOffset;
            char* zeroes = (char*) malloc(numZeroesToAdd * sizeof(char));
            memset(zeroes, 0xFE, numZeroesToAdd);
            dataFileStream.write(zeroes, numZeroesToAdd);
        }
        dataFileStream.seekg(0, dataFileStream.end);
    }
}

inline bool DataHandler::doesFileExist(const char* filePath) {
    struct stat buffer;
    return stat(filePath, &buffer) == 0;
}

/**
 * Writes received data to disk
 */
void DataHandler::addData(char* data) {
    fileMutex.lock();
    dataFileStream.write(data, MAPT_PACKAGE_SIZE);
    fileMutex.unlock();
}

/**
 * Returns the next data package. Blocks until a package is available.
 */
void DataHandler::popData(char* data) {
    //TODO: wait if no data available
    fileMutex.lock();
    dataFileStream.read(data, MAPT_PACKAGE_SIZE);
    fileMutex.unlock();
}

/**
 * Wait for the next data package to be ready and send it down to earth
 */
void DataHandler::sendData() {
    char* binaryData = (char*) malloc(MAPT_PACKAGE_SIZE * sizeof(char));
    popData(binaryData);
    s3tpHandler.send(binaryData, MAPT_PACKAGE_SIZE);
    delete binaryData;
}
