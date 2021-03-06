//				Package : omnithread
// omnithread/posix.h		Created : 7/97 lars immisch lars@ibp.de
//
//    Copyright (C) 1994,1995,1996, 1997 Immisch, becker & Partner
//
//    This file is part of the omnithread library
//
//    The omnithread library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Library General Public
//    License as published by the Free Software Foundation; either
//    version 2 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Library General Public License for more details.
//
//    You should have received a copy of the GNU Library General Public
//    License along with this library; if not, write to the Free
//    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  
//    02111-1307, USA
//
//
// OMNI thread implementation classes for posix threads
//

#ifndef __omnithread_mach_h_
#define __omnithread_mach_h_

#include <mach/cthreads.h>

extern "C" void* omni_thread_wrapper(void* ptr);

#define OMNI_MUTEX_IMPLEMENTATION			\
    struct mutex mach_mutex;

#define OMNI_CONDITION_IMPLEMENTATION			\
    struct condition mach_cond;

#define OMNI_SEMAPHORE_IMPLEMENTATION			\
    omni_mutex m;					\
    omni_condition c;					\
    int value;


#define OMNI_THREAD_IMPLEMENTATION			\
    cthread_t mach_thread;				\
    static int mach_priority(priority_t);		\
    friend void* omni_thread_wrapper(void* ptr);

#endif
