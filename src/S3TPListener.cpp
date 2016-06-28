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

#include "S3TPListener.h"

S3TPListener::S3TPListener(S3TPHandler& s3tpHandler, CommandHandler& commandHandler) :
    bufferSize(1024),
    s3tpHandler(s3tpHandler),
    commandHandler(commandHandler) {
}

void S3TPListener::waitForCommands() {
    char buffer[bufferSize];
    while(true) {
        s3tpHandler.receive(buffer, bufferSize);
        Command command(buffer);
        commandHandler.handleCommand(command);
        //TODO: React in case an error happens
    }
}
