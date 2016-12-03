#ifndef LOG_H_
#define LOG_H_

void log_write (const char * msg, const char * file, int line);
void log_write2 (const char * msg);

#ifndef DISABLE_LOG
#define log_error(___msg) log_write ("Error: " ___msg, __FILE__, __LINE__)
#define log(___msg) log_write2 (___msg)
#else
#define log_error(___msg) do{}while(0)
#define log(___msg) do{}while(0)
#endif
#endif /* LOG_H_ */
