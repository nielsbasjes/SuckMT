//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : tsqueue.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/18 21:28:11 $
//  $Revision: 1.2 $
//  $RCSfile: tsqueue.h,v $
//  $Author: niels $
//=========================================================================

#ifndef __tsqueue_h__
#define __tsqueue_h__

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

#endif // __tsqueue_h__

// End of the file tsqueue.h
//=========================================================================