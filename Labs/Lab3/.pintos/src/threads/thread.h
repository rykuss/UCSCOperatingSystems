/* 
 * This file is derived from source code for the Pintos
 * instructional operating system which is itself derived
 * from the Nachos instructional operating system. The 
 * Nachos copyright notice is reproduced in full below. 
 *
 * Copyright (C) 1992-1996 The Regents of the University of California.
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software
 * and its documentation for any purpose, without fee, and
 * without written agreement is hereby granted, provided that the
 * above copyright notice and the following two paragraphs appear
 * in all copies of this software.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE
 * AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS"
 * BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
 * MODIFICATIONS.
 *
 * Modifications Copyright (C) 2017 David C. Harrison. All rights reserved.
 */

#ifndef THREADS_THREAD_H
#define THREADS_THREAD_H

#include <debug.h>
#include <list.h>
#include <stdint.h>

#ifdef VM
#include "vm/page.h"
#endif

/* States in a thread's life cycle. */
enum thread_status
  {
    THREAD_RUNNING,     /* Running thread. */
    THREAD_READY,       /* Not running but ready to run. */
    THREAD_BLOCKED,     /* Waiting for an event to trigger. */
    THREAD_DYING        /* About to be destroyed. */
  };

// Thread identifier type.
// You can redefine this to whatever type you like. 
typedef int tid_t;
#define TID_ERROR ((tid_t) -1)          /* Error value for tid_t. */

/* Thread priorities. */
#define PRI_MIN 0                       /* Lowest priority. */
#define PRI_DEFAULT 31                  /* Default priority. */
#define PRI_MAX 63                      /* Highest priority. */

/* 
// A kernel thread or user process.
//
//   Each thread structure is stored in its own 4 kB page.  The
//   thread structure itself sits at the very bottom of the page
//  (at offset 0).  The rest of the page is reserved for the
//   thread's kernel stack, which grows downward from the top of
//   the page(at offset 4 kB).  Here's an illustration:
//
//        4 kB +---------------------------------+
//             |          kernel stack           |
//             |                |                |
//             |                |                |
//             |                V                |
//             |         grows downward          |
//             |                                 |
//             |                                 |
//             |                                 |
//             |                                 |
//             |                                 |
//             |                                 |
//             |                                 |
//             |                                 |
//             +---------------------------------+
//             |              magic              |
//             |                :                |
//             |                :                |
//             |               name              |
//             |              status             |
//        0 kB +---------------------------------+
//
//   The upshot of this is twofold:
//
//      1. First, `struct thread' must not be allowed to grow too
//         big.  If it does, then there will not be enough room for
//         the kernel stack.  Our base `struct thread' is only a
//         few bytes in size.  It probably should stay well under 1
//         kB.
//
//      2. Second, kernel stacks must not be allowed to grow too
//         large.  If a stack overflows, it will corrupt the thread
//         state.  Thus, kernel functions should not allocate large
//         structures or arrays as non-static local variables.  Use
//         dynamic allocation with malloc() or palloc_get_page()
//         instead.
//
//   The first symptom of either of these problems will probably be
//   an assertion failure in thread_current(), which checks that
//   the `magic' member of the running thread's `struct thread' is
//   set to THREAD_MAGIC.  Stack overflow will normally change this
//   value, triggering the assertion. 
//
//   The `elem' member has a dual purpose.  It can be an element in
//   the run queue(thread.c), or it can be an element in a
//   semaphore wait list(synch.c).  It can be used these two ways
//   only because they are mutually exclusive: only a thread in the
//   ready state is on the run queue, whereas only a thread in the
//   blocked state is on a semaphore wait list. 
*/
struct thread
  {
    // Owned by thread.c. 
    tid_t tid;                  // Thread identifier
    enum thread_status status;  // Thread state
    char name[16];              // Name(for debugging purposes)
    uint8_t *stack;             // Saved stack pointer
    int priority;               // Priority
    struct list_elem allelem;   // List element for all threads list

    struct list_elem waitelem;  // List element, stored in the wait_list queue 
    int64_t sleep_endtick;      // The tick after which the thread should wakeup 

    // Shared between thread.c and smeaphore.c. 
    struct list_elem elem;      // List element for the semaphore wiaitng list or the global ready_list

    // Owned by userprog/process.c. 
    uint32_t *pagedir;     // Pointer to the page directory
    uint8_t *current_esp;  // "Executable Stack Pointer" 
                           // i.e the current value of the user program’s stack pointer
    struct dir *cwd;	   // Current Working Directory, if any

    // Owned by thread.c. 
    unsigned magic;        // Detects stack overflow. 
  };

// If false(default), use round-robin scheduler.
// If true, use multi-level feedback queue scheduler.
// Controlled by kernel command-line option "-o mlfqs"
extern bool thread_mlfqs;

void thread_init(void);
void thread_start(void);

void thread_tick(int64_t tick);
void thread_print_stats(void);

typedef void thread_func(void *aux);
tid_t thread_create(const char *name, int priority, thread_func *, void *);

void thread_block(void);
void thread_unblock(struct thread *);

void thread_sleep_until(int64_t wake_tick);

struct thread *thread_current(void);
tid_t thread_tid(void);
const char *thread_name(void);

void thread_exit(void) NO_RETURN;
void thread_yield(void);

// Performs some operation on thread t, given auxiliary data AUX. 
typedef void thread_action_func(struct thread *t, void *aux);
void thread_foreach(thread_action_func *, void *);

int thread_get_priority(void);
void thread_set_priority(int);
void thread_priority_donate(struct thread *, int priority);

int thread_get_nice(void);
void thread_set_nice(int);
int thread_get_recent_cpu(void);
int thread_get_load_avg(void);

#endif // threads/thread.h 
