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

#ifndef MAPT_RECEIVER_DATAHANDLER_H
#define MAPT_RECEIVER_DATAHANDLER_H

#include <list>
#include <tuple>
#include <mutex>
#include <fstream>
#include <condition_variable>
#include "S3TPHandler.h"

#define MAPT_PACKAGE_SIZE 3616

using namespace std;

class DataHandler {
private:
    const char*dataFilePath;
    mutex fileMutex;
    streampos readpos;
    fstream dataFileStream;
    S3TPHandler& s3tpHandler;
    condition_variable conditionVariable;
    void popData(char* data);
    void alignDataFile();
    uint getBytesAvailableForRead();
    bool doesFileExist(const char* filePath);
    void initializeDataFile(const char* dataFilePath);
    void createFileIfNotExisting(const char* dataFilePath);
public:
    DataHandler(S3TPHandler& s3tpHandler, const char* dataFilePath);
    void sendData();
    void addData(char* data);

};


#endif //MAPT_RECEIVER_DATAHANDLER_H
