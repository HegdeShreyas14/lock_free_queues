# Notes 

- alignas(64) int x : meant to align 'x' to a memory address which is a multiple of 64 bytes since modern systems use 64 bytes cache lines , in the repo 
kCacheLine is the variable used to define 64 bytes as the value 

- size_t : unsigned integer data type which represents the size/number of bytes of any object in the memory , holds only non neg values
 It is used because it is guaranteed to hold the largest possible array/object on the host system

- the capacity of the queue is made capacity + 1 so that head == tail directly means that queue is empty and next(head) == tail directly means full , no need of flag to distinguish between full or empty

- both head and tail have separate cache line to prevent false sharing , if on the same cache line then the consumer can over write the producer and then go on a loop of invalidating the other , this is false sharing

- memory_order_acquire makes anything published before that block visible to the thread , like if a value written by a release is seen then the operations done before that release is also visible ; establishes this via the release-acquire synchronisation relationship

- verified memory ordering correct under ThreadSanitizer; single-threaded tests can't catch ordering bugs because there's no second thread to observe reordering
