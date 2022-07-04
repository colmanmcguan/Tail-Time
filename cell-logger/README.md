# Tor Cell Logger
The cell logger captures the cells emitted from the modified instance of Tor and stores them as a trace in a corresponding .csv file.

## About
Individual cells are listed for from tor in a UDP packet on port 7000. For each new trace, the controller starts the cell logger by passing the name of the csv file to log to. Once the trace is done, the controller sends a special packet to the cell logger telling it to shutdown.

For convenience of passing data around, variables are stored in a struct clpacket (cell logger packet) which contains the following:

```C
struct clpacket {
	unsigned char header;
	unsigned short length;
	clock_t time;
	unsigned char payload[MAX_TRMSN_SIZE];
};
```

The header determines the command associated with the packet:
* Incoming cell - 0x01
* Outgoing cell - 0x02
* Shutdown      - 0x03
The length field indicates the length of the time and payload fields only.

## Building
The cell logger can be built using a simple make command from the cell-logger directory. The binary will be placed in the build/ directory.

## Usage
```bash
./cell-logger <filename.csv>
```
