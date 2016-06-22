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

#include "S3TPHandler.h"
#include <s3tp/s3tp.h>

S3TPHandler::S3TPHandler() :
    s3tpSocket(-1),
    maptDataPort(1337) { //TODO: Add correct port!
    s3tpSocket = s3tp_create();
    s3tp_connect(s3tpSocket, maptDataPort);
}

void S3TPHandler::send(char *data, int len) {
    s3tp_send(s3tpSocket, data, len);
}
