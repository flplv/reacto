#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include <reacto/reusables/linked_list.hpp>

extern "C"
{
    #include <reacto/reusables/signal_slot.h>
}

extern "C"
int handler (slot_t * slot)
{
    mock().actualCall("handler")
        .withPointerParameter("slot", slot);

    return mock().returnValue().getIntValue();
}

TEST_GROUP(SignalSlotDemo)
{
    signal_t signal;
    slot_t slot;

    void setup ()
    {
        signal_init(&signal);
        slot_init(&slot, handler);
    }

    void teardown ()
    {
        slot_deinit(&slot);
        signal_deinit(&signal);

        mock().checkExpectations();
        mock().clear();
    }
};

TEST(SignalSlotDemo, emit_invalid_handler)
{
    mock().expectNoCall("handler");

    slot_connect(&slot, &signal);
    slot.handler = 0;

    signal_emit(&signal);
    signal_emit(&signal);
    signal_emit(&signal);
    signal_emit(&signal);
    signal_emit(&signal);
    signal_emit(&signal);
    signal_emit(&signal);
    signal_emit(&signal);
    signal_emit(&signal);
    signal_emit(&signal);
    slot_disconnect(&slot, &signal);
}

TEST(SignalSlotDemo, emit_a_lot)
{
    mock().expectNCalls(10, "handler")
            .withParameter("slot", &slot)
            .andReturnValue(0);

    slot_connect(&slot, &signal);
    signal_emit(&signal);
    signal_emit(&signal);
    signal_emit(&signal);
    signal_emit(&signal);
    signal_emit(&signal);
    signal_emit(&signal);
    signal_emit(&signal);
    signal_emit(&signal);
    signal_emit(&signal);
    signal_emit(&signal);
    slot_disconnect(&slot, &signal);
}

TEST(SignalSlotDemo, emit_to_lots)
{
    slot_t slots[10];

    mock().strictOrder();

    for (int i = 0; i < 10; i++)
    {
        slot_init(&slots[i], handler);
        slot_connect(&slots[i], &signal);

        mock().expectOneCall("handler")
                .withParameter("slot", &slots[i])
                .andReturnValue(0);
    }

    signal_emit(&signal);


    CHECK_EQUAL(10, linked_list_count(signal.root, ll));
    signal_deinit(&signal);
    CHECK_EQUAL(0, linked_list_count(signal.root, ll));

    for (int i = 0; i < 10; i++)
    {
        CHECK_EQUAL(0, slots[i].connection);
        CHECK_EQUAL(1, linked_list_count(&slots[i], ll));
        slot_deinit(&slots[i]);
    }
}

TEST(SignalSlotDemo, handler_return_1)
{
    slot_t slots[10];

    mock().strictOrder();

    for (int i = 0; i < 10; i++)
    {
        slot_init(&slots[i], handler);
        slot_connect(&slots[i], &signal);

        if (i <= 5)
        {
            mock().expectOneCall("handler")
                    .withParameter("slot", &slots[i])
                    .andReturnValue( i == 5 ? 1 : 0);
        }
    }

    signal_emit(&signal);

    for (int i = 0; i < 10; i++)
    {
        slot_deinit(&slots[i]);
    }
}

TEST(SignalSlotDemo, disconnect_from_deinit)
{
    mock().expectNCalls(1, "handler")
            .withParameter("slot", &slot);

    slot_connect(&slot, &signal);
    signal_emit(&signal);

    slot_deinit(&slot);
    CHECK_EQUAL(0, slot.connection);
    CHECK_EQUAL(0, slot.handler);
    CHECK_EQUAL(0, linked_list_count(signal.root, ll));

    signal_emit(&signal);
}

TEST(SignalSlotDemo, bad_disconnect)
{
    mock().expectOneCall("_log_file_line")
            .withParameter("msg", "Error: Provided signal is not connected to slot, cannot disconnect.")
            .ignoreOtherParameters();


    slot_disconnect(&slot, &signal);
}

TEST(SignalSlotDemo, inits)
{
    CHECK_EQUAL(0, slot.connection);
    CHECK_EQUAL((void*) handler, (void*) slot.handler);
    CHECK_EQUAL(0, linked_list_count(signal.root, ll));
}

TEST(SignalSlotDemo, connect)
{
    slot_connect(&slot, &signal);
    slot_connect(&slot, &signal);
    slot_connect(&slot, &signal);
    slot_connect(&slot, &signal);
    CHECK_EQUAL(&signal, slot.connection);
    CHECK_EQUAL(signal.root, &slot);
    CHECK_EQUAL(1, linked_list_count(signal.root, ll));
    CHECK_EQUAL(1, linked_list_count(&slot, ll));
}

