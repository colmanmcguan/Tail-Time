#ifndef CONNECTION_MANAGER_H_
#define CONNECTION_MANAGER_H_

void ack_new_trace(char *payload);
int process_packet(struct packet *pkt, FILE *data);
int manage_connection(int sockfd, FILE *data);

#endif
