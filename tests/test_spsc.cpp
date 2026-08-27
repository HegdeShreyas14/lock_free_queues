#include "spsc_queue.hpp"


int main(){
    SPSCQueue<int , 8> q;
    q.pusher(69);
    auto v = q.popper();
    return 0;
}
