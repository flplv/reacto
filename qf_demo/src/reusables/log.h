#ifndef LOG_H_
#define LOG_H_

void _log_impl_file_line (const char * msg, const char * file, int line);
void _log_impl (const char * msg);

#ifndef DISABLE_LOG
#define log_error(___msg) _log_impl_file_line ("Error: " ___msg, __FILE__, __LINE__)
#define log(___msg) _log_impl (___msg)
#else
#define log_error(___msg) do{}while(0)
#define log(___msg) do{}while(0)
#endif
#endif /* LOG_H_ */
