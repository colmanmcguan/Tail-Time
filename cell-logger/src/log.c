#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

#include "clpacket.h"
#include "log.h"

FILE *fp;
clock_t start;

void init_log(char *filename)
{
	char logfile[1024];
	const char *logs = "./logs";
	struct stat sb;

	/* set up logs directory if needed */
	if (!(stat(logs, &sb) == 0 && S_ISDIR(sb.st_mode)))
		mkdir(logs, 755);

	/* create new log file */
	sprintf(logfile, "%s/%s", logs, filename);
	fp = fopen(logfile, "w");
	fprintf(fp, "time, direction\n");
	start = time(NULL);
}

void close_log() { fclose(fp); }

/* log the relative timestamp and direction */
void log_trace(unsigned short func, clock_t time, char *payload)
{
	double reltime;
	reltime = (double)(start - time) / CLOCKS_PER_SEC;
	fprintf(fp, "%lf, %s\n", reltime, payload);
}