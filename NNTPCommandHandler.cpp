//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPCommandHandler.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/03/19 12:21:43 $
//  $Revision: 1.8 $
//  $RCSfile: NNTPCommandHandler.cpp,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

//-------------------------------------------------------------------------

#include "SuckDefines.h"
#include "NNTPCommandHandler.h"

//-------------------------------------------------------------------------

NNTPCommandHandler::NNTPCommandHandler
        (NNTPRetrieveManager *retrieveManager,
         CommandQueue        *commandQueue,
         NewsKiller          *killer, 
         IniFile             *iniFile)
         : CommandHandler(commandQueue)
{
    fRetrieveManager = retrieveManager;
    fIniFile         = iniFile;
    fKiller          = killer;
   
    nntpConnection = new NNTPProxy(fIniFile,id());
}

//-------------------------------------------------------------------------

NNTPCommandHandler::~NNTPCommandHandler()
{
    if (nntpConnection != NULL)
        delete (nntpConnection);
}

//-------------------------------------------------------------------------

NNTPProxy * 
NNTPCommandHandler::GetNNTPProxy()
{
    return nntpConnection;
}

//-------------------------------------------------------------------------

IniFile * 
NNTPCommandHandler::GetIniFile()
{
    return fIniFile;
}

//-------------------------------------------------------------------------

bool 
NNTPCommandHandler::DoWeKeepThisArticle(NEWSArticle * article)
{
    return fKiller->DoWeKeepThisArticle(article);
}

//-------------------------------------------------------------------------
// Store the specified messageID so we can check if we get any duplicates
// Returns 
//  - true  if the messageID was unknown and it has been stored.
//  - false if the messageID was already present and it has not been stored.
bool
NNTPCommandHandler::DoWeNeedToDownloadThisArticle(NEWSArticle * article)
{
    return fRetrieveManager->DoWeNeedToDownloadThisArticle(article);
}

//-------------------------------------------------------------------------
// Make a note this article has been stored in the specified file
void
NNTPCommandHandler::ArticleHasBeenStored(NEWSArticle * article,string filename)
{
    fRetrieveManager->ArticleHasBeenStored(article,filename);
}

//-------------------------------------------------------------------------

// Make a note this article has been stored
void
NNTPCommandHandler::ArticleHasBeenKilled(NEWSArticle * article)
{
    fRetrieveManager->ArticleHasBeenKilled(article);
}

//-------------------------------------------------------------------------

void 
NNTPCommandHandler::AbortChildren()
{
    CommandHandler::AbortChildren();
    if (nntpConnection != NULL)
        nntpConnection->Abort();
}

//-------------------------------------------------------------------------

// End of the file NNTPCommandHandler.cpp
//=========================================================================
