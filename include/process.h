//
// Created by Wenxin Zheng on 2021/3/5.
//

#ifndef ACMOS_SPR21_PROCESS_H
#define ACMOS_SPR21_PROCESS_H

#include <list.h>
#include <pagetable.h>

typedef enum state { UNUSED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE, IDLE } process_state_t;
typedef enum file_type{PUTC} file_type_t;

// Saved registers for kernel context switches.
struct context {
	uint64 ra;
	uint64 sp;

	// callee-saved
	uint64 s0;
	uint64 s1;
	uint64 s2;
	uint64 s3;
	uint64 s4;
	uint64 s5;
	uint64 s6;
	uint64 s7;
	uint64 s8;
	uint64 s9;
	uint64 s10;
	uint64 s11;
};

// per-process data for the trap handling code in trampoline.S.
// sits in a page by itself just under the trampoline page in the
// user page table. not specially mapped in the kernel page table.
// the sscratch register points here.
// uservec in trampoline.S saves user registers in the trapframe,
// then initializes registers from the trapframe's
// kernel_sp, kernel_hartid, kernel_satp, and jumps to kernel_trap.
// usertrapret() and userret in trampoline.S set up
// the trapframe's kernel_*, restore user registers from the
// trapframe, switch to the user page table, and enter user space.
// the trapframe includes callee-saved user registers like s0-s11 because the
// return-to-user path via usertrapret() doesn't return through
// the entire kernel call stack.
struct trapframe {
	/*   0 */ uint64 kernel_satp;   // kernel page table
	/*   8 */ uint64 kernel_sp;     // top of process's kernel stack
	/*  16 */ uint64 kernel_trap;   // usertrap()
	/*  24 */ uint64 epc;           // saved user program counter
	/*  32 */ uint64 kernel_hartid; // saved kernel tp
	/*  40 */ uint64 ra;
	/*  48 */ uint64 sp;
	/*  56 */ uint64 gp;
	/*  64 */ uint64 tp;
	/*  72 */ uint64 t0;
	/*  80 */ uint64 t1;
	/*  88 */ uint64 t2;
	/*  96 */ uint64 s0;
	/* 104 */ uint64 s1;
	/* 112 */ uint64 a0;
	/* 120 */ uint64 a1;
	/* 128 */ uint64 a2;
	/* 136 */ uint64 a3;
	/* 144 */ uint64 a4;
	/* 152 */ uint64 a5;
	/* 160 */ uint64 a6;
	/* 168 */ uint64 a7;
	/* 176 */ uint64 s2;
	/* 184 */ uint64 s3;
	/* 192 */ uint64 s4;
	/* 200 */ uint64 s5;
	/* 208 */ uint64 s6;
	/* 216 */ uint64 s7;
	/* 224 */ uint64 s8;
	/* 232 */ uint64 s9;
	/* 240 */ uint64 s10;
	/* 248 */ uint64 s11;
	/* 256 */ uint64 t3;
	/* 264 */ uint64 t4;
	/* 272 */ uint64 t5;
	/* 280 */ uint64 t6;
};
typedef struct process {
    struct list_head thread_list;
    process_state_t process_state;
    // 以下部分请根据自己的需要自行填充
	int pid;
	pagetable_t pagetable;
} process_t;

// 状态可以根据自己的需要进行修改
typedef process_state_t thread_state_t;

typedef struct thread {
    struct list_head process_list_thread_node;
    thread_state_t thread_state;
    struct list_head sched_list_thread_node;
    // 以下部分请根据自己的需要自行填充
	int tid;
	struct process *fa;
	uint64 kstack;
	struct trapframe *trapframe;
	struct context context;
} thread_t;

process_t *alloc_proc(const char* bin, thread_t **thr);
bool load_thread(file_type_t type);
void sched_enqueue(thread_t *target_thread);
thread_t *sched_dequeue();
bool sched_empty();
void sched_start();
void sched_init();
void proc_init();
void trap_init_vec();
thread_t *mythread();
void yield();
#endif  // ACMOS_SPR21_PROCESS_H
