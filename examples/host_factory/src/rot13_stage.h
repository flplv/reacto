#ifndef rot13_H_
#define rot13_H_

#include <reacto/queue.h>

#define rot13_stage_buffer_size 2

int rot13_stage_handler (queue_t * queue);
void rot13_stage_push (char c);

#endif /* rot13_H_ */
