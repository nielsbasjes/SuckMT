//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : DuplicatesChecker.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/03/02 20:51:28 $
//  $Revision: 1.1 $
//  $RCSfile: DuplicatesChecker.h,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifndef __DuplicatesChecker_H__
#define __DuplicatesChecker_H__
    
class DuplicatesChecker; // Forward Declaration

//-------------------------------------------------------------------------

#ifdef WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif

#include <fstream.h>
#include <stdio.h>  
#include <stdlib.h>  
#include "regex.h"
#include "Abortable.h"
#include "IniFile.h"
#include "Printable.h"
#include "ArticleImpactChecker.h"

//-------------------------------------------------------------------------
// This class reads the specified value from the specified IniFile
// and logs this event by itself when a matching query is performed.

class DuplicatesChecker: 
        public Printable, 
        public ArticleImpactChecker, 
        public Abortable
{
public: 
    DuplicatesChecker(IniFile *settings);

//    DuplicatesChecker(const DuplicatesChecker &original);

    ~DuplicatesChecker();

    virtual long
    GetImpactValue(
        NEWSArticle * article,
        const string &headerName,
        string &matchlog);

    bool
    GetMatchingHeader(string & headerName);

    void Print (ostream &os) const;
        
private:
    omni_mutex valuesMutex;    
    
    // Message-ID storage system
    string     fRestartFileName;
    ofstream   fRestartFile;
    omni_mutex fRestartFileMutex;

    // The string is the messageID and 
    // the bool is always true except when not yet present
    map <string,bool>   fAllMessageIDs;
    omni_mutex          fMessageDBmutex;
};

DEFINE_PRINTABLE_OPERATORS(DuplicatesChecker)

//-------------------------------------------------------------------------

#endif // __HEADERCHECKER_H__

// End of the file DuplicatesChecker.h
//=========================================================================
