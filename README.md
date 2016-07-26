# mapt-receiver
This program is responsible for receiving the data from the MAPT payload and on request sending it to the ground station via S3TP.

The program will run with two threads:

 * One thread is waiting for the TCP packets from the Payload and stores them in a list. Alternatively, if the DataHandler was set into Passthrough mode by the groundstation, the data is directly forwarded to the Groundstation via S3TP.

 * Another thread waits for commands from the groundstation, sent via S3TP, and on request send all the received data back to the groundstation.

##### MAPT data package format
The data packages sent from the MAPT payload have the following structure:

> [TCP Header] [Timestamp (64 bits)] [Event Number (64 bits)] [Detector Data (900 x 32 bits])

This leads to a total package size of 3616 Bytes (28928 bits).
