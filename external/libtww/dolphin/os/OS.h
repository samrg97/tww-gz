#ifndef OS_H_
#define OS_H_

#include "../gctypes.h"

// move stuff to appropriate headers later

#define OS_MESSAGE_NON_BLOCKING 0
#define OS_MESSAGE_BLOCKING 1

struct OSThread;
struct OSMutex {
    u8 unk[24];
};

struct OSMutexLink {
    struct OSMutex* prev;
    struct OSMutex* next;
};

struct OSMutexQueue {
    struct OSMutex* prev;
    struct OSMutex* next;
};

struct OSContext {
    u32 gpr[32];
    u32 cr;
    u32 lr;
    u32 ctr;
    u32 xer;
    double fpr[32];
    u32 padding_1;
    u32 fpscr;
    u32 srr0;
    u32 srr1;
    u16 mode;
    u16 state;
    u32 gqr[8];
    u32 padding_2;
    double ps[32];
};

typedef void (*OSSwitchThreadCallback)(OSThread* from, OSThread* to);

struct OSThreadLink {
    struct OSThread* prev;
    struct OSThread* next;
};

struct OSThreadQueue {
    struct OSThread* head;
    struct OSThread* tail;
};

struct OSCond {
    struct OSThreadQueue queue;
};

typedef void* OSMessage;

struct OSMessageQueue {
    struct OSThreadQueue sending_queue;
    struct OSThreadQueue receiving_queue;
    void** message_array;
    s32 num_messages;
    s32 first_index;
    s32 num_used;
};

typedef u32 OSTick;

typedef int64_t OSTime;

#define OSBusClock ((uint32_t)tww_os_bus_clock_addr)

typedef OSTime (*OSGetTime)(void);
#define OSGetTime ((OSGetTime)tww_os_get_time_addr)

struct OSCalendarTime {
    s32 seconds;
    s32 minutes;
    s32 hours;
    s32 day_of_month;
    s32 month;
    s32 year;
    s32 week_day;
    s32 year_day;
    s32 milliseconds;
    s32 microseconds;
};

typedef s32 OSHeapHandle;

typedef enum OSSoundMode {
    SOUND_MODE_MONO = 0,
    SOUND_MODE_STEREO = 1,
} OSSoundMode;

typedef u16 OSThreadState;
#define OS_THREAD_STATE_UNINITIALIZED 0
#define OS_THREAD_STATE_READY 1
#define OS_THREAD_STATE_RUNNING 2
#define OS_THREAD_STATE_WAITING 4
#define OS_THREAD_STATE_DEAD 8

struct OSThread {
    OSContext context;
    OSThreadState state;
    u16 attributes;
    s32 suspend_count;
    u32 effective_priority;
    u32 base_priority;
    void* exit_value;
    OSThreadQueue* queue;
    OSThreadLink link;
    OSThreadQueue join_queue;
    OSMutex* mutex;
    OSMutexQueue owned_mutexes;
    OSThreadLink active_threads_link;
    u8* stack_base;
    u8* stack_end;
    u8* error_code;
    void* data[2];
};

#endif