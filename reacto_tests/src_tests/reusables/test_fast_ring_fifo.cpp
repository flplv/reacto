#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

extern "C"
{
    #include <reacto/reusables/fast_ring_fifo.h>
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


TEST(fast_ring_fifo, sizes)
{
    fast_ring_fifo_t cut;
    long long int sz = fast_ring_fifo_init(&cut, 0);
    CHECK_EQUAL(-1, sz);

    sz = fast_ring_fifo_init(&cut, 1);
    CHECK_EQUAL(-1, sz);

    sz = fast_ring_fifo_init(&cut, 2);
    CHECK_EQUAL(2, sz);

    sz = fast_ring_fifo_init(&cut, 3);
    CHECK_EQUAL(2, sz);

    sz = fast_ring_fifo_init(&cut, 4);
    CHECK_EQUAL(4, sz);

    sz = fast_ring_fifo_init(&cut, 7);
    CHECK_EQUAL(4, sz);

    sz = fast_ring_fifo_init(&cut, 8);
    CHECK_EQUAL(8, sz);

    sz = fast_ring_fifo_init(&cut, 9);
    CHECK_EQUAL(8, sz);

    sz = fast_ring_fifo_init(&cut, 15);
    CHECK_EQUAL(8, sz);

    sz = fast_ring_fifo_init(&cut, 16);
    CHECK_EQUAL(16, sz);

    sz = fast_ring_fifo_init(&cut, 17);
    CHECK_EQUAL(16, sz);

    sz = fast_ring_fifo_init(&cut, 31);
    CHECK_EQUAL(16, sz);

    sz = fast_ring_fifo_init(&cut, 32);
    CHECK_EQUAL(32, sz);

    sz = fast_ring_fifo_init(&cut, 255);
    CHECK_EQUAL(128, sz);

    sz = fast_ring_fifo_init(&cut, 256);
    CHECK_EQUAL(256, sz);

    sz = fast_ring_fifo_init(&cut, 0x200);
    CHECK_EQUAL(0x200, sz);

    sz = fast_ring_fifo_init(&cut, 0x400);
    CHECK_EQUAL(0x400, sz);

    sz = fast_ring_fifo_init(&cut, 0x800);
    CHECK_EQUAL(0x800, sz);

    sz = fast_ring_fifo_init(&cut, 0x80000000);
    CHECK_EQUAL((unsigned)0x80000000, (unsigned)sz);
}

TEST(fast_ring_fifo, boundaries)
{
    fast_ring_fifo_t cut;
    int buffer[1026];
    buffer[0] = INT_MAX;
    buffer[1025] = INT_MAX;
    int * const buf = buffer + 1;
    int c;

    int sz = fast_ring_fifo_init(&cut, 1024);
    CHECK_EQUAL(1024, sz);

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

    int sz = fast_ring_fifo_init(&cut, 16);
    CHECK_EQUAL(16, sz);

    int i = 0;

    while (!fast_ring_fifo_full(&cut))
    {
        buffer[fast_ring_fifo_write_index(&cut)] = reference_buffer[i++];
        fast_ring_fifo_write_increment(&cut);
    }

    STRCMP_EQUAL(reference_buffer, buffer);
    CHECK_EQUAL(16, fast_ring_fifo_count(&cut));


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

        CHECK_EQUAL(16, fast_ring_fifo_count(&cut));

        i = 0;

        while(!fast_ring_fifo_empty(&cut))
        {
            int m = i++;
            CHECK_EQUAL(reference_buffer[m], buffer[fast_ring_fifo_read_index(&cut)]);
            fast_ring_fifo_read_increment(&cut);
        }
    }
}
