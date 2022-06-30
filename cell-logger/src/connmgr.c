#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"
#include "connmgr.h"

/* let control know that we're ready for the next trace */
void ack_new_trace(char *payload)
{
	struct packet *pkt;

	pkt = create_packet();
	pkt->header = ACKNOWLEDGE;
	pkt->length = strlen(payload);
	strncpy(pkt->payload, payload, pkt->length);

	send_packet(pkt);
	destroy_packet(pkt);
}

/* pick apart function code and log accordingly */
int process_packet(struct packet *pkt, FILE *data) {

	/* length does not match payload length */
	if (pkt->length != strlen(pkt->payload))
		goto fail;

	switch (pkt->header) {
		case INCOMING:
		case OUTGOING:
			/* packet malformatted */
			if (pkt->length != 0x02)
				goto fail;

			/* log incoming cell */
			fprintf(data, "%s, ", pkt->payload);
			break;
		case NEW_TRACE:
			if (ftell(data) != 0) {
				/* edit out the preceeding ", " */
				fseek(data, -2, SEEK_CUR);
				fprintf(data, "\n");
			}

			/* log new website name */
			fprintf(data, "%s, ", pkt->payload);

			/* let control know we're ready for the next trace */
			goto new_trace;
		case SHUTDOWN:
			/* shutting down */
			goto shutdown;
		default:
			/* packet malformed */
			goto fail;
	}

	return (GOOD_PACK);

	new_trace:
		return (SEND_ACK);

	shutdown:
		return (CLOSE_CONN);

	/* some sort of bad packet */
	fail:
		fprintf(stderr, "[!] bad packet");
		return (BAD_PACK);
}

/* get the next packet and determine what to do */
int manage_connection(int sockfd, FILE *data)
{
	int ret;
	struct packet *pkt;

	pkt = get_packet(sockfd);
	if (pkt != NULL)
		ret = process_packet(pkt, data);

	switch (ret) {
		case SEND_ACK:
			ack_new_trace(pkt->payload);
			break;
		case CLOSE_CONN:
			destroy_packet(pkt);
			return (CLOSE_CONN);
		case BAD_PACK:
			destroy_packet(pkt);
			return (BAD_PACK);
	}

	destroy_packet(pkt);
	return (GOOD_PACK);
}

