#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "network.h"
#include "connmgr.h"

int main()
{
	int sockfd, ret, bad_packs;
	FILE *data;

	/* setup socket to receive on */
	sockfd = setup();
	bad_packs = 0;
	data = fopen("log.csv", "w");

	/* loop forever until control says we're done */
	for (;;) {
		ret = manage_connection(sockfd, data);

		switch (ret) {
			case BAD_PACK:
				bad_packs++;
				break;
			case CLOSE_CONN:
				printf("+------------------------+\n");
				printf("received shutdown notice\n");
				printf("+------------------------+\n");
				printf("stats:\n");
				printf("\t%d bad packets\n");
				printf("closing resources\n");
				close(sockfd);
				fclose(data);
				printf("done\n");
				goto done;
		}
	}
	done:
		return (0);
}
