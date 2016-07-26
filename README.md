# mapt-receiver
This program is responsible for receiving the data from the MAPT payload and sending it to the ground station via S3TP. For this purpose, the MAPT payload establishes a TCP connection to this program, which acts as a TCP server.

The program will run with three threads:

 * One thread is waiting for the TCP packets from the Payload and stores them in a binary file in non-volatile memory.

 * Another thread waits for commands from the groundstation, sent via S3TP, and handles them accordingly. These commands have yet to be defined, and could for example give the current progress.

 * A third thread is continuously sending all the stored data down to the ground station via S3TP.

##### MAPT data package format
The data packages sent from the MAPT payload have the following structure:

> [TCP Header] [Timestamp (64 bits)] [Event Number (64 bits)] [Detector Data (900 x 32 bits])

This leads to a total package size of 3616 Bytes (28928 bits).
