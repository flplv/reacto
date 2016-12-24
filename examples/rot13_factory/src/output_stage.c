#include <reacto/queue.h>
#include "reacto_factory_config.h"
#include <reacto/factory.h>
#include "output_stage.h"

#include <stdio.h>

struct stage_data
{
    char c;
};

static struct stage_data stage_buffer[output_stage_buffer_size];

int output_stage_handler (queue_t * queue)
{
    struct stage_data ev;
    queue_peek(queue, stage_buffer, &ev);
    putchar(ev.c);
    return 0;
}

void output_stage_push (char c)
{
    struct stage_data ev;
    ev.c = c;
    queue_push(reacto_factory_queue(1), stage_buffer, ev);
}
