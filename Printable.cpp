//=========================================================================
//                 Copyright (C)1999-2005 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT
//                       Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Project: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Id: Printable.cpp 17 2008-08-17 15:29:35Z niels $
//  $Date: 2008-08-17 17:29:35 +0200 (Sun, 17 Aug 2008) $
//  $Revision: 17 $
//  $LastChangedBy: niels $
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

#ifdef _MSC_VER
#pragma warning( disable : 4786 ) 
#endif

//-------------------------------------------------------------------------

#include "Printable.h"

//-------------------------------------------------------------------------

ostream& 
operator<< (ostream &os, const string &stringObject)
{
    os << stringObject.c_str();
    return os;
}

//-------------------------------------------------------------------------

Printable::Printable(string className)
{
    thisClassName = className;
}

//-------------------------------------------------------------------------

Printable::~Printable()
{
    // Nothing to do.   
}

//-------------------------------------------------------------------------

void 
Printable::Print(ostream &os) const
{
    os << "### The function \"void " << thisClassName 
       << "::Print(ostream &os)\" is NOT IMPLEMENTED !!!" << endl;
}

//-------------------------------------------------------------------------

void 
Printable::PrintClassName(ostream &os) const
{
    os << thisClassName;
}

//-------------------------------------------------------------------------

ostream& 
operator<< (ostream &os, const Printable &printableObject)
{
    printableObject.Print(os);
    return os;
}

//-------------------------------------------------------------------------

ostream& 
operator<< (ostream &os, const Printable *printableObject)
{
    if (printableObject == NULL)
        os << "#@NULL@#";
    else
        os << *printableObject;
    
    return os;
}

//-------------------------------------------------------------------------

ostream& 
operator<< (ostream &os, const vector<Printable*> &printableObjectVector)
{
    vector<Printable*>::const_iterator itemIter;

    bool first = true;
    os << "@@@@@@@@@@@@@@@@@@@@" << endl;
    for(itemIter  = printableObjectVector.begin();
        itemIter != printableObjectVector.end();
        itemIter ++)
    {
        if (first)
        {
            os << "--------------------" << endl;
            first = false;
        }
        os << (*itemIter) << endl << flush ;
    }

    os << "@@@@@@@@@@@@@@@@@@@@" << endl << flush ;
    return os;
}

//-------------------------------------------------------------------------

ostream& 
operator<< (ostream &os, const vector<Printable*> *printableObjectVector)
{
    if (printableObjectVector == NULL)
        os << "#@NULL@#";
    else
        os << *printableObjectVector;
    
    return os;
}

//-------------------------------------------------------------------------

// End of the file Printable.cpp
//=========================================================================
