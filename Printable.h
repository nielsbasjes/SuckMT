/***************************************************************************
                          Printable.h  -  description                              
                             -------------------                                         
    begin                : Sun Jul 18 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifndef PRINTABLE_H
#define PRINTABLE_H

#include <string>
#include <vector>
#include <ostream.h>

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

DEFINE_PRINTABLE_OPERATORS(Printable)

ostream& operator<< (ostream &os, const string &stringObject);

#endif
