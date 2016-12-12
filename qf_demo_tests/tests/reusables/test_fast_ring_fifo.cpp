#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

extern "C"
{
    #include <reusables/fast_ring_fifo.h>
}

TEST_GROUP(fast_ring_fifo)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(fast_ring_fifo, boundaries)
{
    fast_ring_fifo_t cut;
    int buffer[1026];
    buffer[0] = INT_MAX;
    buffer[1025] = INT_MAX;
    int * const buf = buffer + 1;
    int c;

    fast_ring_fifo_init(&cut, 1024);

    c = 0;
    while (!fast_ring_fifo_full(&cut))
    {
        buf[fast_ring_fifo_write_index(&cut)] = c++;
        fast_ring_fifo_write_increment(&cut);
    }

    CHECK_EQUAL(INT_MAX, buffer[0]);
    CHECK_EQUAL(INT_MAX, buffer[1025]);

}

TEST(fast_ring_fifo, read_write)
{
    fast_ring_fifo_t cut;

    char reference_buffer[] = "abcdefghijklmn";
    char buffer[16];

    fast_ring_fifo_init(&cut, 16);

    int i = 0;

    while (!fast_ring_fifo_full(&cut))
    {
        buffer[fast_ring_fifo_write_index(&cut)] = reference_buffer[i++];
        fast_ring_fifo_write_increment(&cut);
    }

    STRCMP_EQUAL(reference_buffer, buffer);
    CHECK_EQUAL(15, fast_ring_fifo_count(&cut));


    i = 0;

    while(!fast_ring_fifo_empty(&cut))
    {
        int n = i++;
        CHECK_EQUAL(reference_buffer[n], buffer[fast_ring_fifo_read_index(&cut)]);
        fast_ring_fifo_read_increment(&cut);
    }

    int n = 3;
    while (n--)
    {
        i = 0;

        while (!fast_ring_fifo_full(&cut))
        {
            buffer[fast_ring_fifo_write_index(&cut)] = reference_buffer[i++];
            fast_ring_fifo_write_increment(&cut);
        }

        CHECK_EQUAL(15, fast_ring_fifo_count(&cut));

        i = 0;

        while(!fast_ring_fifo_empty(&cut))
        {
            int m = i++;
            CHECK_EQUAL(reference_buffer[m], buffer[fast_ring_fifo_read_index(&cut)]);
            fast_ring_fifo_read_increment(&cut);
        }
    }
}
