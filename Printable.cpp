/***************************************************************************
                          Printable.cpp  -  superclass for objects that want to be printable                              
                             -------------------                                         
    begin                : Sun Jul 18 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

#include "debugging.h"
#include "Printable.h"

ostream& operator<< (ostream &os, const string &stringObject)
{
	os << stringObject.c_str();
	return os;
}

//-------------------------------------------------------------------------

FUNCTION_START(Printable::Printable(string className))
{
    thisClassName = className;
}
FUNCTION_END

//-------------------------------------------------------------------------

FUNCTION_START(Printable::~Printable())
{
    // Nothing to do.   
}
FUNCTION_END

//-------------------------------------------------------------------------

FUNCTION_START(void Printable::Print(ostream &os) const)
{
    os << "### The function \"void " << thisClassName 
       << "::Print(ostream &os)\" is NOT IMPLEMENTED !!!" << endl;
}
FUNCTION_END

//-------------------------------------------------------------------------

FUNCTION_START(void Printable::PrintClassName(ostream &os) const)
{
    os << thisClassName;
}
FUNCTION_END

//-------------------------------------------------------------------------

FUNCTION_START(ostream& operator<< (ostream &os, const Printable &printableObject))
{
    printableObject.Print(os);
    return os;
}
FUNCTION_END

//-------------------------------------------------------------------------

FUNCTION_START(ostream& operator<< (ostream &os, const Printable *printableObject))
{
    if (printableObject == NULL)
        os << "#@NULL@#";
    else
        os << *printableObject;
    
    return os;
}
FUNCTION_END

//-------------------------------------------------------------------------

FUNCTION_START(ostream& operator<< (ostream &os, const vector<Printable*> &printableObjectVector))
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
FUNCTION_END

//-------------------------------------------------------------------------

FUNCTION_START(ostream& operator<< (ostream &os, const vector<Printable*> *printableObjectVector))
{
    if (printableObjectVector == NULL)
        os << "#@NULL@#";
    else
        os << *printableObjectVector;
    
    return os;
}
FUNCTION_END

//-------------------------------------------------------------------------
