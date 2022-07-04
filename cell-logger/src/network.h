#ifndef NETOWRK_H_
#define NETWORK_H_

#include "clpacket.h"

#define RECV_PORT "7000"

int setup(void);
struct clpacket * create_clpacket(void);
void destroy_clpacket(struct clpacket *clpkt);
void unserialize_clpkt(char *buf, struct clpacket *clpkt);
struct clpacket * get_clpacket(int sockfd);
int send_clpacket(int sockfd, struct addr *dst, struct clpacket *clpkt);

#endif
