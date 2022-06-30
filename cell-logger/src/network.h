#ifndef NETOWRK_H_
#define NETWORK_H_

#define IP "127.0.0.1"
#define SEND_PORT "7000"
#define RECV_PORT "7001"

#define MAX_PACKET_SIZE 512

/* headers */
#define INCOMING	0x01    /* incoming tor cell */
#define OUTGOING	0x02    /* outgoing tor cell */
#define ACKNOWLEDGE	0x03    /* acknowledge new webpage */
#define NEW_TRACE	0x04    /* indicate next cells belong to new trace */
#define SHUTDOWN	0x05    /* shutdown logger */

#define GOOD_PACK 	1
#define BAD_PACK	2
#define SEND_ACK	3
#define CLOSE_CONN	4

struct packet {
	char header;
	unsigned short length;
	char payload[MAX_PACKET_SIZE];
};

int setup(void);
struct packet * create_packet(void);
void destroy_packet(struct packet *pkt);
struct packet * get_packet(int sockfd);
int send_packet(struct packet *pkt);

#endif
