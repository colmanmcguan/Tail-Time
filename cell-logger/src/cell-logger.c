#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "log.h"
#include "network.h"
#include "processor.h"

int main(int argc, char **argv)
{
	int sockfd, ret, bad_packs;

	if (argc != 2) {
		fprintf(stderr, "Usage: cell-logger <log-file.csv>\n");
		exit(1);
	}

	printf("+--------------------------------+\n");
	printf("| Setting up cell logger\n");
	printf("+--------------------------------+\n");

	/* initialize log file */
	init_log(argv[1]);

	/* setup socket to receive on */
	sockfd = setup();
	bad_packs = 0;

	/* loop forever until control says we're done */
	for (;;) {
		ret = get_cell(sockfd);

		switch (ret) {
			case BAD_PACK:
				bad_packs++;
				break;
			case CLOSE_CONN:
				printf("+--------------------------------+\n");
				printf("| Received shutdown notice\n");
				printf("+--------------------------------+\n");
				printf("stats:\n");
				printf("\t%d bad packets\n", bad_packs);
				close_log();
				goto done;
		}
	}
	done:
		return (0);
}
