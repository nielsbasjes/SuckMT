//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPCommandHandler.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/18 21:27:34 $
//  $Revision: 1.2 $
//  $RCSfile: NNTPCommandHandler.cpp,v $
//  $Author: niels $
//=========================================================================

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

#include "NNTPCommandHandler.h"

//-------------------------------------------------------------------------

NNTPCommandHandler::NNTPCommandHandler(NNTPRetrieveManager* retrieveManager,CommandQueue * commandQueue,NewsKiller *killer, IniFile *iniFile, string newsServer): CommandHandler(commandQueue)
{
    fRetrieveManager = retrieveManager;
    nntpConnection   = new NNTPProxy(newsServer);
    fIniFile         = iniFile;
    fKiller          = killer;
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

void
NNTPCommandHandler::ArticleFileHasBeenWritten(string fileName)
{
    fRetrieveManager->ArticleFileHasBeenWritten(fileName);
}

//-------------------------------------------------------------------------

void 
NNTPCommandHandler::AbortChildren()
{
    if (nntpConnection != NULL)
		nntpConnection->Abort();
}

//-------------------------------------------------------------------------

// End of the file NNTPCommandHandler.cpp
//=========================================================================
