/*
 * Low priority thread L acquires a lock, then blocks waiting for
 * the condition.  Medium priority thread M then blocks waiting for
 * the same condition.  Next, high priority thread H attempts to
 * acquire the lock, donating its priority to L.
 *
 * Next, the main thread signals the condition, waking up L.  L
 * releases the lock, which wakes up H.  H signals the condition,
 * waking up M. H terminates, then M, then L, and finally the
 * main thread.
 * 
 * Derived from priority_donate_sema by Godmar Back <gback@cs.vt.edu>
 * 
 * Modifications Copyright (C) 2017 David C. Harrison. All rights reserved.
 */
#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/condvar.h"
#include "threads/lock.h"
#include "threads/thread.h"

struct lock_and_cond {
    struct lock thread_lock;
    struct lock condvar_lock;
    struct condvar condvar;
};

static thread_func l_thread_func;
static thread_func m_thread_func;
static thread_func h_thread_func;

void test_priority_donate_condvar(void)
{
    ASSERT(thread_get_priority() == PRI_DEFAULT);

    struct lock_and_cond ls;

    lock_init(&ls.thread_lock);
    lock_init(&ls.condvar_lock);
    condvar_init(&ls.condvar);

    thread_create("low", PRI_DEFAULT + 1, l_thread_func, &ls);
    thread_create("med", PRI_DEFAULT + 3, m_thread_func, &ls);
    thread_create("high", PRI_DEFAULT + 5, h_thread_func, &ls);
    lock_acquire(&ls.condvar_lock);

    msg("Thread %s signaling", thread_current()->name);
    condvar_signal(&ls.condvar, &ls.condvar_lock);
    lock_release(&ls.condvar_lock);
    msg("Thread %s finished", thread_current()->name);
}

static void l_thread_func(void *ls_)
{
    struct lock_and_cond *ls = ls_;

    lock_acquire(&ls->thread_lock);
    msg("Thread %s acquired lock", thread_current()->name);
    lock_acquire(&ls->condvar_lock);
    condvar_wait(&ls->condvar, &ls->condvar_lock);
    lock_release(&ls->condvar_lock);
    msg("Thread %s woke up", thread_current()->name);
    lock_release(&ls->thread_lock);
    msg("Thread %s finished", thread_current()->name);
}

static void m_thread_func(void *ls_)
{
    struct lock_and_cond *ls = ls_;

    lock_acquire(&ls->condvar_lock);
    condvar_wait(&ls->condvar, &ls->condvar_lock);
    lock_release(&ls->condvar_lock);
    msg("Thread %s finished", thread_current()->name);
}

static void h_thread_func(void *ls_)
{
    struct lock_and_cond *ls = ls_;

    lock_acquire(&ls->thread_lock);
    msg("Thread %s acquired lock", thread_current()->name);

    lock_acquire(&ls->condvar_lock);
    msg("Thread %s signaling", thread_current()->name);
    condvar_signal(&ls->condvar, &ls->condvar_lock);
    lock_release(&ls->condvar_lock);
    lock_release(&ls->thread_lock);
    msg("Thread %s finished", thread_current()->name);
}
