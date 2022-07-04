#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "clpacket.h"

/* malloc a packet */
struct clpacket * create_clpacket() {
	struct addr *addr;
	struct clpacket *clpkt;

	clpkt = malloc(sizeof(struct clpacket));

	if (clpkt == NULL) {
		fprintf(stderr, "[!] failed to allocate memory for packet\n");
		return (clpkt);
	}

	return (clpkt);
}

void destroy_clpacket(struct clpacket *clpkt)
{
	free(clpkt);
}

/* pack packet contents into buffer */
void serialize_clpacket(char *buf, struct clpacket *clpkt)
{
	buf[0] = clpkt->header;
	buf[1] = clpkt->length >> 8;
	buf[2] = clpkt->length;
	buf[3] = clpkt->time >> 24;
	buf[4] = clpkt->time >> 16;
	buf[5] = clpkt->time >> 8;
	buf[6] = clpkt->time;
	memcpy(buf + 7, clpkt->payload, MAX_TRMSN_SIZE - 7);
}

/* unpack buffer into packet */
void unserialize_clpacket(char *buf, struct clpacket *clpkt)
{
	clpkt->header = buf[0];
	clpkt->length = (buf[1] << 8 | buf[2]);
	clpkt->time = (buf[3] << 24 | buf[4] << 16 | buf[5] << 8 | buf[6]);
	memcpy(clpkt->payload, buf + 7, MAX_TRMSN_SIZE - 7);
}

