#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "network.h"

/* setup socket */
int setup()
{
	int sockfd, status;
	struct addrinfo hints, *iplist, *ip_ptr;

	printf("+------------------------+\n");
	printf("setting up cell logger\n");
	printf("+------------------------+\n");

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	/* bind us to the socket */
	if ((status = getaddrinfo(IP, RECV_PORT, &hints, &iplist)) != 0) {
		fprintf(stderr, "[!] getaddrinfo() failed with: %s\n", 
				gai_strerror(status));
		exit(1);
	}

	for (ip_ptr = iplist; ip_ptr != NULL; ip_ptr = ip_ptr->ai_next) {
		sockfd = socket(ip_ptr->ai_family, 
				ip_ptr->ai_socktype, ip_ptr->ai_protocol);
		if (sockfd == -1) continue;

		if (bind(sockfd, ip_ptr->ai_addr, ip_ptr->ai_addrlen) == -1) {
			close(sockfd);
			continue;
		}

		break;
	}

	/* done with this */
	freeaddrinfo(iplist);

	/* couldn't bind to socket */
	if (ip_ptr == NULL) {
		fprintf(stderr, "[!] bind() failed\n");
		exit(1);
	}

	printf("[*] Bind successful. Listening on port %s\n", RECV_PORT);

	return (sockfd);
}

/* malloc a packet and set to 0 */
struct packet * create_packet() {
	struct packet *pkt; 

	pkt = malloc(sizeof(struct packet));

	if (pkt == NULL) {
		fprintf(stderr, "[!] failed to allocate memory for packet\n");
		return (pkt);
	}

	memset(pkt, 0, sizeof(*pkt));
	return (pkt);
}

void destroy_packet(struct packet *pkt)
{
	free(pkt);
}

/* read the next incoming packet and load it into a packet struct */
struct packet * get_packet(int sockfd) {
	int numbytes;
	char buf[MAX_PACKET_SIZE + 1];
	socklen_t addrlen;
	struct sockaddr sendraddr;
	struct packet *pkt;

	memset(&buf, 0, sizeof(buf));
	pkt = create_packet();

	if (pkt == NULL)
		return (pkt);

	numbytes = recvfrom(sockfd, buf, MAX_PACKET_SIZE, 0,
				(struct sockaddr *)&sendraddr, &addrlen);
	if (numbytes == -1) {
		fprintf(stderr, "[!] recvfrom() failed\n");
		exit(1);
	}

	pkt->header = buf[0];
	pkt->length = (unsigned short)buf[1];
	strncpy(pkt->payload, buf + 2, (int)pkt->length);

	return (pkt);
}

/* send a packet to control.
 * only used for acknowledging a new trace */
int send_packet(struct packet *pkt)
{
	char buf[MAX_PACKET_SIZE + 1];
	int ret, sockfd, status;
	struct addrinfo hints, *iplist, *ip_ptr;

	memset(&buf, 0, sizeof(buf));

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((status = getaddrinfo(IP, SEND_PORT, &hints, &iplist)) != 0) {
		fprintf(stderr, "[!] getaddrinfo() failed with: %s\n", 
				gai_strerror(status));
		exit(1);
	}

	for (ip_ptr = iplist; ip_ptr != NULL; ip_ptr = ip_ptr->ai_next) {
		sockfd = socket(ip_ptr->ai_family, 
				ip_ptr->ai_socktype, ip_ptr->ai_protocol);
		if (sockfd != -1)
			break;
	}

	/* done with this */
	freeaddrinfo(iplist);

	if (sockfd == -1) {
		fprintf(stderr, "[!] socket() failed\n");
		return (-1);
	}

	/* read packet struct into buffer */
	buf[0] = pkt->header;
	buf[1] = (unsigned char)pkt->length;
	strncpy(buf + 2, pkt->payload, pkt->length + 1);

	ret = sendto(sockfd, buf, strlen(buf), 0, 
			ip_ptr->ai_addr, ip_ptr->ai_addrlen);
	if (ret == -1)
		fprintf(stderr, "[!] error sending ack\n");

	return (0);
}
