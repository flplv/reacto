/* reacto_factory_config.h is a local header that will configure
 *  static allocations made by the factory.h module.
 * It is always necessary to include reacto_factory_config.h before
 *  reacto/factory.h */
#include "reacto_factory_config.h"
#include <reacto/factory.h>

/* This will create static instances of the data used by the factory.
 * It can only be included once in whole executable. */
#include <reacto/factory.template>

#include "get_input.h"
#include "rot13_stage.h"
#include "output_stage.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static struct reacto_queues_context queues_context = {

    /* Installing the queue handlers */
    .queue_handlers = {
        /* Queues handlers for the first queue */
        [0] = {
            rot13_stage_handler
        },
        /* Queue handlers for the second queue */
        [1] = {
            output_stage_handler
        },
    },
    .queue_buffer_sizes = {
        [0] = rot13_stage_buffer_size,
        [1] = output_stage_buffer_size
    },
};

static const struct reacto_timed_queue_context timed_queue_context =
{
    .high_priority = true,
};

static void sleep_handler (main_loop_t * loop)
{
    uint32_t sleep = main_loop_sleep_timeout(loop);
    usleep(sleep * 1000);
}

int main()
{
    int r = reacto_factory(main_loop_strategy_priority_queue,
                           &timed_queue_context,
                           &queues_context);
    if (r < 0) {
        printf ("reacto_factory failed with %d.", r);
        exit(EXIT_FAILURE);
    }

    get_input_init();
    main_loop_set_sleep_handler(reacto_factory_main_loop(), sleep_handler);
    main_loop_run(reacto_factory_main_loop());
    exit(EXIT_SUCCESS);
}
