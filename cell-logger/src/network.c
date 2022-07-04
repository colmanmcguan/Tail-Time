#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#include "network.h"

void * get_addr_type(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in *)sa)->sin_addr);
	return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

unsigned short get_addr_port(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return (ntohs(((struct sockaddr_in *)sa)->sin_port));
	return (ntohs(((struct sockaddr_in6 *)sa)->sin6_port));
}

/* setup socket */
int setup()
{
	int sockfd, status;
	struct addrinfo hints, *iplist, *ip_ptr;
	char addr_string[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(NULL, RECV_PORT, &hints, &iplist)) != 0) {
		fprintf(stderr, "[!] getaddrinfo() failed with: %s\n", 
				gai_strerror(status));
		exit(1);
	}

	/* bind us to socket */
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

	printf("[*] Using address: %s\n", 
			inet_ntop(ip_ptr->ai_addr->sa_family,
					get_addr_type(ip_ptr->ai_addr),
					addr_string, sizeof(addr_string)));
	printf("[*] Bind successful. Port: %hu\n", 
		get_addr_port(ip_ptr->ai_addr));

	return (sockfd);
}

/* malloc a packet */
struct clpacket * create_clpacket() {
	struct addr *addr;
	struct clpacket *clpkt; 

	clpkt = malloc(sizeof(struct clpacket));

	if (clpkt == NULL) {
		fprintf(stderr, "[!] failed to allocate memory for packet\n");
		return (clpkt);
	}

	clpkt->senderaddr = malloc(sizeof(struct addr));
	if (clpkt->senderaddr == NULL) {
		fprintf(stderr, "[!] failed to allocate memory for packet\n");
		return (clpkt);
	}

	clpkt->senderaddr->addr = NULL;

	return (clpkt);
}

void destroy_clpacket(struct clpacket *clpkt)
{
	free(clpkt->senderaddr->addr);
	free(clpkt->senderaddr);
	free(clpkt);
}

/* pack packet contents into buffer */
void unserialize_clpacket(char *buf, struct clpacket *clpkt)
{
	clpkt->header = buf[0];
	clpkt->length = (buf[1] << 8 | buf[2]);
	clpkt->time = (buf[3] << 24 | buf[4] << 16 | buf[5] << 8 | buf[6]);
	memcpy(clpkt->payload, buf + 7, MAX_PACKET_SIZE - 7);
}

/* read the next incoming packet and load it into a packet struct */
struct clpacket * get_clpacket(int sockfd) {
	int numbytes;
	char buf[MAX_PACKET_SIZE];
	socklen_t addrlen;
	struct sockaddr_storage *senderaddr;
	struct clpacket *clpkt;

	clpkt = create_clpacket();
	memset(&buf, 0, sizeof(buf));
	senderaddr = malloc(sizeof(struct sockaddr_storage));
	addrlen = sizeof(struct sockaddr);

	if (clpkt == NULL)
		return (clpkt);

	if ((recvfrom(sockfd, buf, MAX_PACKET_SIZE, 0,
			(struct sockaddr *)senderaddr, &addrlen)) == -1) {
		fprintf(stderr, "[!] recvfrom() failed\n");
		exit(1);
	}

	unserialize_clpacket(buf, clpkt);
	clpkt->senderaddr->addr = senderaddr;
	clpkt->senderaddr->addrlen = addrlen;

	return (clpkt);
}
