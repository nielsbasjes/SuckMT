/***************************************************************************
                          Abortable.cpp  -  description                              
                             -------------------                                         
    begin                : Sun Aug 1 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

#include "debugging.h"
#include "Abortable.h"

//-------------------------------------------------------------------------

FUNCTION_START(Abortable::Abortable())
{
    keepRunning = true;
}
FUNCTION_END

//-------------------------------------------------------------------------

FUNCTION_START(Abortable::~Abortable())
{
    // Nothing to to
}
FUNCTION_END

//-------------------------------------------------------------------------

void
FUNCTION_START(Abortable::Abort())
{
    if (keepRunning)
    {
        keepRunning = false;
        AbortChildren();
    }
}
FUNCTION_END

//-------------------------------------------------------------------------

bool
FUNCTION_START(Abortable::KeepRunning())
{
    return keepRunning;
}
FUNCTION_END

//-------------------------------------------------------------------------

void
FUNCTION_START(Abortable::AbortChildren())
{
    // Nothing to to
}
FUNCTION_END

//-------------------------------------------------------------------------

