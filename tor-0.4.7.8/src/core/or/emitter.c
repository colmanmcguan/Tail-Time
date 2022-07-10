#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>

#include "core/or/circuit_st.h"
#include "core/or/cell_st.h"
#include "core/or/cell_queue_st.h"
#include "core/or/channel.h"
#include "core/or/clpacket.h"
#include "core/or/emitter.h"

int get_socket(void)
{
	int sockfd, status;
	struct addrinfo hints, *iplist, *ip_ptr;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(NULL, DSTPORT, &hints, &iplist)) != 0) {
		fprintf(stderr, "[!] getaddrinfo() failed with: %s\n", gai_strerror(status));
		exit(1);
	}

	/* get socket */
	for (ip_ptr = iplist; ip_ptr != NULL; ip_ptr = ip_ptr->ai_next) {
		sockfd = socket(ip_ptr->ai_family, ip_ptr->ai_socktype, ip_ptr->ai_protocol);

		if (sockfd == -1) continue;

		break;
	}

	/* done with this */
	freeaddrinfo(iplist);

	if (ip_ptr == NULL) {
		close(sockfd);
		return (-1);
	}

	if ((connect(sockfd, (struct sockaddr *)ip_ptr->ai_addr, ip_ptr->ai_addrlen)) == -1) {
		close(sockfd);
		return (-1);
	}

	return (sockfd);
}

void fill_icell_clpacket(struct clpacket *clpkt, cell_t *cell)
{
	struct timespec tspec;

	clock_gettime(CLOCK_REALTIME, &tspec);
	clpkt->header = INCOMING;
	clpkt->tspec.tv_sec = tspec.tv_sec;
	clpkt->tspec.tv_nsec = tspec.tv_nsec;
	clpkt->command = cell->command;
	clpkt->circ_id = cell->circ_id;
}

void fill_ocell_clpacket(struct clpacket *clpkt, channel_t *chan, packed_cell_t *cell)
{
	struct timespec tspec;

	clock_gettime(CLOCK_REALTIME, &tspec);
	clpkt->header = OUTGOING;
	clpkt->tspec.tv_sec = tspec.tv_sec;
	clpkt->tspec.tv_nsec = tspec.tv_nsec;

	if (chan->wide_circ_ids) {
		clpkt->command = *(uint8_t *)(cell->body + 4);
		clpkt->circ_id = *(uint32_t *)(cell->body);
	} else {
		clpkt->command = *(uint8_t *)(cell->body + 2);
		clpkt->circ_id = *(uint16_t *)(cell->body);
	}
}

void send_clpacket(struct clpacket *clpkt)
{
	int sockfd;
	char buf[CLPACKET_SIZE];

	if ((sockfd = get_socket()) == -1)
		return;

	memset(buf, 0, CLPACKET_SIZE);
	serialize_clpacket(buf, clpkt);
	send(sockfd, buf, CLPACKET_SIZE, 0);
	close(sockfd);
}
