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

#include "DataHandler.h"

DataHandler::DataHandler(S3TPHandler& s3tpHandler) :
    listMutex(),
    passthroughMode(false),
    s3tpHandler(s3tpHandler),
    dataList() {
}

void DataHandler::addData(tuple<char*, int> data) {
    if(passthroughMode) {
        s3tpHandler.send(get<0>(data), get<1>(data));
    } else {
        listMutex.lock();
        dataList.push_back(data);
        listMutex.unlock();
    }
}

/**
 * Returns first element in the list. If the list is empty, returns a tuple containing a nullptr and a size value of 0.
 */
tuple<char *, int> DataHandler::popData() {
    if(dataList.empty())
        return make_tuple((char*) nullptr, 0);
    listMutex.lock();
    tuple<char*, int> returnData = dataList.front();
    dataList.pop_front();
    listMutex.unlock();
    return returnData;
}

/**
 * Returns the amount of Data packets currently stored.
 */
int DataHandler::getNumStoredData() {
    return dataList.size();
}

void DataHandler::setPassthroughMode(bool mode) {
    passthroughMode = mode;
}
