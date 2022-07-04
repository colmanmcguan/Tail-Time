#ifndef CELL_LOGGER_PACKET_H_
#define CELL_LOGGER_PACKET_H_

#include <sys/socket.h>
#include <time.h>

#define MAX_PACKET_SIZE 512

/* headers */
#define INCOMING	0x01    /* incoming tor cell */
#define OUTGOING	0x02    /* outgoing tor cell */
#define SHUTDOWN	0x03    /* shutdown logger */

#define GOOD_PACK 	1
#define BAD_PACK	2
#define CLOSE_CONN	3

struct addr {
	struct sockaddr_storage *addr;
	socklen_t addrlen;
};

struct clpacket {
	char header;
	unsigned short length;
	clock_t time;
	char payload[MAX_PACKET_SIZE];
	struct addr *senderaddr;
};

#endif