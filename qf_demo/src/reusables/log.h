#ifndef LOG_H_
#define LOG_H_

__attribute__ ((format (printf, 3, 4))) void _log_file_line (const char * file, int line, const char * msg, ...);
__attribute__ ((format (printf, 1, 2))) void log_message (const char * msg, ...);

#define log_error(...) _log_file_line(__FILE__, __LINE__, "Error: " __VA_ARGS__)

#endif /* LOG_H_ */
