/*
 * See Lab 2 Tutorial Slides for details
 */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/lock.h"
#include "threads/thread.h"

static thread_func b_thread_func;
static thread_func c_thread_func;

static struct lock lock;

void test_priority_donate_single(void) 
{
    thread_set_priority(20);

    lock_init(&lock);
    lock_acquire(&lock);

    thread_create("Thread B", 40, b_thread_func, NULL);
    thread_create("Thread C", 30, c_thread_func, NULL);

    lock_release(&lock);
    msg("Thread A finished.");
}

static void b_thread_func(void *aux UNUSED) 
{
    lock_acquire(&lock);
    lock_release(&lock);
    msg("Thread B finished.");
}

static void c_thread_func(void *aux UNUSED) 
{
    msg("Thread C finished.");
}
