#include <reacto/queue.h>
#include "reacto_factory_config.h"
#include <reacto/factory.h>
#include <reacto/reusables/macros.h>

#include "rot13_stage.h"
#include "output_stage.h"

struct stage_data
{
    char c;
};

static struct stage_data stage_buffer[rot13_stage_buffer_size];

static char * upper_array = "ABCDEFGHIJKLMNOPQRSTUVXYZ";
static char * lower_array = "abcdefghijklmnopqrstuvxyz";

static char rot13 (char c)
{
    if (c >= 'a' && c <= 'z')
        return lower_array[(c - 'a' + 13) % (int)ARRAY_SIZE(lower_array)];

    if (c >= 'A' && c <= 'Z')
        return upper_array[(c - 'A' + 13) % (int)ARRAY_SIZE(upper_array)];

    return c;
}

int rot13_stage_handler (queue_t * queue)
{
    struct stage_data ev;
    queue_peek(queue, stage_buffer, &ev);
    output_stage_push (rot13(ev.c));
    return 0;
}

void rot13_stage_push (char c)
{
    struct stage_data ev;
    ev.c = c;
    queue_push(reacto_factory_queue(0), stage_buffer, ev);
}
