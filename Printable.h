//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : Printable.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/11/18 22:53:22 $
//  $Revision: 1.4 $
//  $RCSfile: Printable.h,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifndef __PRINTABLE_H__
#define __PRINTABLE_H__

//-------------------------------------------------------------------------

#include <string>
#include <vector>
#include <ostream.h>

//-------------------------------------------------------------------------

// Warning Dirty hack to skip the std namespace in Visual C++ 6.0
#ifdef __WIN32__
#define string std::string
#define vector std::vector
#endif

//--------------------------------------------------------------------

#define DEFINE_PRINTABLE_OPERATORS(className) \
    ostream& operator<< (ostream &os, const className &className##Object);\
    ostream& operator<< (ostream &os, const className *className##Object);\
    ostream& operator<< (ostream &os, const vector<className*> &className##Vector);\
    ostream& operator<< (ostream &os, const vector<className*> *className##Vector);

#define IMPLEMENT_PRINTABLE_OPERATORS(className) \
    ostream& operator<< (ostream &os, const className &className##Object)\
        { os << (const Printable*)(&className##Object); return os;}\
    ostream& operator<< (ostream &os, const className *className##Object)\
        { os << (const Printable*)(className##Object); return os;}\
    ostream& operator<< (ostream &os, const vector<className*> &className##Vector)\
        { os << (const vector<Printable*>*)(&className##Vector); return os;}\
    ostream& operator<< (ostream &os, const vector<className*> *className##Vector)\
        { os << (const vector<Printable*>*)(className##Vector); return os;}

//-------------------------------------------------------------------------

class Printable
{
public: 
    Printable(string className = "Unknown");
    virtual ~Printable();
    virtual void Print(ostream &os) const;
    void PrintClassName(ostream &os) const;
    
private:
    string thisClassName;
};

//-------------------------------------------------------------------------

DEFINE_PRINTABLE_OPERATORS(Printable)

ostream& operator<< (ostream &os, const string &stringObject);

//-------------------------------------------------------------------------

#endif // __PRINTABLE_H__

// End of the file Printable.h
//=========================================================================
