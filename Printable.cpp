//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : Printable.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/18 21:27:49 $
//  $Revision: 1.2 $
//  $RCSfile: Printable.cpp,v $
//  $Author: niels $
//=========================================================================

#ifdef WIN32
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
