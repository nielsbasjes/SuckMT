/***************************************************************************
                          Vectorprint.h  -  description                              
                             -------------------                                         
    begin                : Sun Jul 18 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifndef VECTOR_PRINT_H
#define VECTOR_PRINT_H

#include <vector>
#include <ostream.h>

//--------------------------------------------------------------------

template <class T>
ostream& operator<< (ostream &os, const vector<T> &itemVector);

//--------------------------------------------------------------------
#endif




