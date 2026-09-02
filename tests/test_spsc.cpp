#include "spsc_queue.hpp"
#include<iostream>
#include<thread>
#include<cstdint>
#include<optional>

static int g_fail = 0;
#define CHECK(cond)                                                 \
    do{                                                             \
        if(!(cond)){                                                  \
            std::cout<<"FAIL:"<<#cond<<"(line "<<__LINE__<<")\n";   \
            ++g_fail;                                               \
        }                                                           \
    }while(0)


void test_push_pop(){
    SPSCQueue<int, 4> q;

        CHECK((q.push(1)));
        CHECK(q.push(2));
        CHECK(q.push(3));
        CHECK(q.push(4));

        // Queue should now be full.
        CHECK(!q.push(5));

        auto v1 = q.pop();
        CHECK(v1.has_value());
        CHECK(v1.value() == 1);

        auto v2 = q.pop();
        CHECK(v2.has_value());
        CHECK(v2.value() == 2);

        auto v3 = q.pop();
        CHECK(v3.has_value());
        CHECK(v3.value() == 3);

        auto v4 = q.pop();
        CHECK(v4.has_value());
        CHECK(v4.value() == 4);

        // Queue should now be empty.
        auto empty = q.pop();
        CHECK(!empty.has_value());

}

void test_fifo_order()
{
    SPSCQueue<int, 8> q;

    for (int i = 0; i < 8; ++i)
        CHECK(q.push(i));

    for (int i = 0; i < 8; ++i)
    {
        auto result = q.pop();

        CHECK(result.has_value());

        if (result.has_value())
            CHECK(result.value() == i);
    }

    CHECK(!q.pop().has_value());
}

void test_spsc_single_thread()
{
    SPSCQueue<int, 8> q;

    CHECK(q.push(10));
    CHECK(q.push(20));

    auto v1 = q.pop();
    CHECK(v1.has_value());

    if (v1.has_value())
        CHECK(v1.value() == 10);

    auto v2 = q.pop();
    CHECK(v2.has_value());

    if (v2.has_value())
        CHECK(v2.value() == 20);

    CHECK(!q.pop().has_value());
}

int main(){

        test_push_pop();
        test_fifo_order();
        test_spsc_single_thread();
        if (g_fail == 0)
        {
            std::cout << "All tests passed!\n";
            return 0;
        }
        std::cout << g_fail << " test(s) failed.\n";
        return 1;

}
