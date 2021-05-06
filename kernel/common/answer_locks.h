//
// Created by Wenxin Zheng on 2021/4/21.
//

#ifndef ACMOS_SPR21_ANSWER_LOCKS_H
#define ACMOS_SPR21_ANSWER_LOCKS_H


int lock_init(struct lock *lock){
    
    /* Your code here */
    lock->cpuid=-1;
    lock->locked=0;
    if(nlock >= MAXLOCKS) BUG("Max lock count reached.");
    locks[nlock++] = lock;
    return 0;
}

void acquire(struct lock *lock){
    /* Your code here */
    while(__sync_lock_test_and_set(&(lock->locked),1));
    lock->cpuid=cpuid();
}

// Try to acquire the lock once
// Return 0 if succeed, -1 if failed.
int try_acquire(struct lock *lock){
    /* Your code here */
    if(__sync_lock_test_and_set(&(lock->locked),1)) return -1;
    lock->cpuid=cpuid();
    return 0;
}

void release(struct lock* lock){
    /* Your code here */
    if(!holding_lock(lock)) return;
    lock->cpuid=-1;
    __sync_lock_release(&(lock->locked));
}

int is_locked(struct lock* lock){
    return lock->locked;
}

// private for spin lock
int holding_lock(struct lock* lock){
    /* Your code here */
    return lock->cpuid==cpuid();
}

#endif  // ACMOS_SPR21_ANSWER_LOCKS_H
