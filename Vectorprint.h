//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : Vectorprint.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/18 21:28:02 $
//  $Revision: 1.2 $
//  $RCSfile: Vectorprint.h,v $
//  $Author: niels $
//=========================================================================

#ifndef VECTOR_PRINT_H
#define VECTOR_PRINT_H

#include <vector>
#include <ostream.h>

//--------------------------------------------------------------------

template <class T>
ostream& 
operator<< (ostream &os, const vector<T> &itemVector);

//--------------------------------------------------------------------

#endif

// End of the file Vectorprint.h
//=========================================================================
