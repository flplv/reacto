#include "reacto_factory_config.h"
#include <reacto/context.h>
#include <reacto/queue.h>
#include "rot13_stage.h"

#include <stdio.h>
#include <sys/select.h>

static timed_event_t ev;

static char poll_input()
{
    fd_set rfds;
    struct timeval tv;
    int retval;

    /* Watch stdin (fd 0) to see when it has input. */
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);

    /* Wait up to 0 seconds. */
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    retval = select(1, &rfds, NULL, NULL, &tv);
    /* Don't rely on the value of tv now! */

    if (retval == -1)
        perror("select()");

    if (!retval)
        return 0;

    /* should not block */
    int c = getchar ();
    if (c < 0)
        return 0;

    return (char)c;
}

static void get_input_periodic_handler (timed_event_t * ev)
{
    char c;
    c = poll_input();

    if (c)
        rot13_stage_push(c);

    /* Relink to get called cyclicly */
    timed_queue_link(reacto_context_timed_queue(), ev);
}

void get_input_init()
{
    timed_event_init(&ev, 5, get_input_periodic_handler);
    timed_queue_link(reacto_context_timed_queue(), &ev);
    setvbuf(stdin, NULL, _IONBF, 0); //turn off buffering
}
