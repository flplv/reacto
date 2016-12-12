#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "linked_list.hpp"

extern "C"
{
    #include <reusables/signal_slot_demo.h>
}

extern "C"
int handler (slot_demo_t * slot, int number, char * str)
{
    mock().actualCall("handler")
        .withPointerParameter("slot", slot)
        .withIntParameter("number", number)
        .withStringParameter("str", str);

    return mock().returnValue().getIntValue();
}

TEST_GROUP(SignalSlotDemo)
{
    signal_demo_t signal;
    slot_demo_t slot;

    void setup ()
    {
        signal_demo_init(&signal);
        slot_demo_init(&slot, handler);
    }

    void teardown ()
    {
        slot_demo_deinit(&slot);
        signal_demo_deinit(&signal);

        mock().checkExpectations();
        mock().clear();
    }
};

TEST(SignalSlotDemo, emit_invalid_handler)
{
    int number = INT_MAX;
    char str[] = "maria juana";

    mock().expectNoCall("handler");

    slot_demo_connect(&slot, &signal);
    slot.handler = 0;

    signal_demo_emit(&signal, number, str);
    signal_demo_emit(&signal, number, str);
    signal_demo_emit(&signal, number, str);
    signal_demo_emit(&signal, number, str);
    signal_demo_emit(&signal, number, str);
    signal_demo_emit(&signal, number, str);
    signal_demo_emit(&signal, number, str);
    signal_demo_emit(&signal, number, str);
    signal_demo_emit(&signal, number, str);
    signal_demo_emit(&signal, number, str);
    slot_demo_disconnect(&slot, &signal);
}

TEST(SignalSlotDemo, emit_a_lot)
{
    int number = INT_MAX;
    char str[] = "maria juana";

    mock().expectNCalls(10, "handler")
            .withParameter("slot", &slot)
            .withParameter("number", number)
            .withParameter("str", str)
            .andReturnValue(0);

    slot_demo_connect(&slot, &signal);
    signal_demo_emit(&signal, number, str);
    signal_demo_emit(&signal, number, str);
    signal_demo_emit(&signal, number, str);
    signal_demo_emit(&signal, number, str);
    signal_demo_emit(&signal, number, str);
    signal_demo_emit(&signal, number, str);
    signal_demo_emit(&signal, number, str);
    signal_demo_emit(&signal, number, str);
    signal_demo_emit(&signal, number, str);
    signal_demo_emit(&signal, number, str);
    slot_demo_disconnect(&slot, &signal);
}

TEST(SignalSlotDemo, emit_to_lots)
{
    int number = INT_MAX;
    char str[] = "maria juana";
    slot_demo_t slots[10];

    mock().strictOrder();

    for (int i = 0; i < 10; i++)
    {
        slot_demo_init(&slots[i], handler);
        slot_demo_connect(&slots[i], &signal);

        mock().expectOneCall("handler")
                .withParameter("slot", &slots[i])
                .withParameter("number", number)
                .withParameter("str", str)
                .andReturnValue(0);
    }

    signal_demo_emit(&signal, number, str);


    CHECK_EQUAL(10, linked_list_count(signal.root, ll));
    signal_demo_deinit(&signal);
    CHECK_EQUAL(0, linked_list_count(signal.root, ll));

    for (int i = 0; i < 10; i++)
    {
        CHECK_EQUAL(0, slots[i].connection);
        CHECK_EQUAL(1, linked_list_count(&slots[i], ll));
        slot_demo_deinit(&slots[i]);
    }
}

TEST(SignalSlotDemo, handler_return_1)
{
    int number = INT_MAX;
    char str[] = "maria juana";
    slot_demo_t slots[10];

    mock().strictOrder();

    for (int i = 0; i < 10; i++)
    {
        slot_demo_init(&slots[i], handler);
        slot_demo_connect(&slots[i], &signal);

        if (i <= 5)
        {
            mock().expectOneCall("handler")
                    .withParameter("slot", &slots[i])
                    .withParameter("number", number)
                    .withParameter("str", str)
                    .andReturnValue( i == 5 ? 1 : 0);
        }
    }

    signal_demo_emit(&signal, number, str);

    for (int i = 0; i < 10; i++)
    {
        slot_demo_deinit(&slots[i]);
    }
}

TEST(SignalSlotDemo, disconnect_from_deinit)
{
    int number = INT_MAX;
    char str[] = "maria juana";

    mock().expectNCalls(1, "handler")
            .withParameter("slot", &slot)
            .withParameter("number", number)
            .withParameter("str", str);

    slot_demo_connect(&slot, &signal);
    signal_demo_emit(&signal, number, str);

    slot_demo_deinit(&slot);
    CHECK_EQUAL(0, slot.connection);
    CHECK_EQUAL(0, slot.handler);
    CHECK_EQUAL(0, linked_list_count(signal.root, ll));

    signal_demo_emit(&signal, number, str);
}

TEST(SignalSlotDemo, bad_disconnect)
{
    mock().expectOneCall("_log_file_line")
            .withParameter("msg", "Error: Provided signal is not connected to slot, cannot disconnect.")
            .ignoreOtherParameters();


    slot_demo_disconnect(&slot, &signal);
}

TEST(SignalSlotDemo, inits)
{
    CHECK_EQUAL(0, slot.connection);
    CHECK_EQUAL((void*) handler, (void*) slot.handler);
    CHECK_EQUAL(0, linked_list_count(signal.root, ll));
}

TEST(SignalSlotDemo, connect)
{
    slot_demo_connect(&slot, &signal);
    slot_demo_connect(&slot, &signal);
    slot_demo_connect(&slot, &signal);
    slot_demo_connect(&slot, &signal);
    CHECK_EQUAL(&signal, slot.connection);
    CHECK_EQUAL(signal.root, &slot);
    CHECK_EQUAL(1, linked_list_count(signal.root, ll));
    CHECK_EQUAL(1, linked_list_count(&slot, ll));
}

