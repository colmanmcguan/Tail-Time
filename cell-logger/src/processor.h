#ifndef PROCESSOR_H_
#define PROCESSOR_MANAGER_H_

void ack_new_trace(int sockfd, struct addr *dst, char *payload);
int process_clpacket(struct clpacket *clpkt);
int get_cell(int sockfd);

#endif
