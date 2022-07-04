#ifndef LOG_H_
#define LOG_H_

void init_log(char *filename);
void close_log();
void log_trace(unsigned short func, time_t time, char *payload);

#endif
