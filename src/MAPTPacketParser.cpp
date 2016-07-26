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

#include <tuple>
#include "MAPTPacketParser.h"

MAPTPacketParser::MAPTPacketParser(DataHandler &dataHandler) :
    dataHandler(dataHandler) {
}

void MAPTPacketParser::parseData(char *data, int len) {
    dataHandler.addData(make_tuple(data, len));
}

MAPTPacketParser::~MAPTPacketParser() {
}
