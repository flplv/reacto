#ifndef SECOND_STAGE_H_
#define SECOND_STAGE_H_

#include <reacto/queue.h>

#define output_stage_buffer_size 2

int output_stage_handler (queue_t * queue);
void output_stage_push (char c);

#endif /* SECOND_STAGE_H_ */
