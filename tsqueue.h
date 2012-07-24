//=========================================================================
//                 Copyright (C)1999-2012 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT
//                       Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Project: SuckMT, a multithreaded suck replacement
//  Language  : C++
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//   If you reuse code from SuckMT you are required to put a notice of 
//   this fact in both your manual and about box.
//
//=========================================================================

#ifndef __TSQUEUE_H__
#define __TSQUEUE_H__

#include <queue>
#include <omnithread.h>

//-------------------------------------------------------------------------

template <class T, class C = std::deque<T> >
class thread_safe_queue
{
protected:
    omni_mutex qmutex;
    C c;

public:
    typedef typename C::value_type value_type;
    typedef typename C::size_type  size_type;
    typedef C container_type;

    explicit thread_safe_queue(const C &a = C()) : c(a) { };

    //--------------------

    bool empty() const
    {
//        omni_mutex_lock lock(qmutex);
        return c.empty();
    }

    //--------------------

    size_type size() const
    {
//        omni_mutex_lock lock(qmutex);
        return c.size();
    }

    //--------------------

    value_type & front()
    {
        omni_mutex_lock lock(qmutex);
        return c.front();
    }

    //--------------------

    const value_type & front() const
    {
//        omni_mutex_lock lock(qmutex);
        return c.front();
    }

    //--------------------

    value_type & back()
    {
        omni_mutex_lock lock(qmutex);
        return c.back();
    }

    //--------------------

    const value_type & back() const
    {
//        omni_mutex_lock lock(qmutex);
        return c.back();
    }

    //--------------------

    void push( const value_type & x )
    {
        omni_mutex_lock lock(qmutex);
        c.push_back(x);
    }

    //--------------------

    void pop()
    {
        omni_mutex_lock lock(qmutex);
        c.pop_front();
    }

    //--------------------
};

//-------------------------------------------------------------------------

#endif // __TSQUEUE_H__

// End of the file tsqueue.h
//=========================================================================
