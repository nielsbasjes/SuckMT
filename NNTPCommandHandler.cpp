/***************************************************************************
                          NNTPCommandHandler.cpp  -  description                              
                             -------------------                                         
    begin                : Sun Jul 18 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

#include "NNTPCommandHandler.h"

//-------------------------------------------------------------------------

FUNCTION_START(NNTPCommandHandler::NNTPCommandHandler(NNTPRetrieveManager* retrieveManager,CommandQueue * commandQueue,NewsKiller *killer, IniFile *iniFile, string newsServer): CommandHandler(commandQueue))
{
    fRetrieveManager = retrieveManager;
    nntpConnection   = new NNTPProxy(newsServer);
    fIniFile         = iniFile;
    fKiller          = killer;
}
FUNCTION_END

//-------------------------------------------------------------------------

FUNCTION_START(NNTPCommandHandler::~NNTPCommandHandler())
{
    if (nntpConnection != NULL)
        delete (nntpConnection);
}
FUNCTION_END

//-------------------------------------------------------------------------

NNTPProxy * 
FUNCTION_START(NNTPCommandHandler::GetNNTPProxy())
{
    return nntpConnection;
}
FUNCTION_END

//-------------------------------------------------------------------------

IniFile * 
FUNCTION_START(NNTPCommandHandler::GetIniFile())
{
    return fIniFile;
}
FUNCTION_END

//-------------------------------------------------------------------------

bool 
FUNCTION_START(NNTPCommandHandler::DoWeKeepThisArticle(NEWSArticle * article))
{
    return fKiller->DoWeKeepThisArticle(article);
}
FUNCTION_END

//-------------------------------------------------------------------------

void
FUNCTION_START(NNTPCommandHandler::ArticleFileHasBeenWritten(string fileName))
{
    fRetrieveManager->ArticleFileHasBeenWritten(fileName);
}
FUNCTION_END

//-------------------------------------------------------------------------

void 
FUNCTION_START(NNTPCommandHandler::AbortChildren())
{
    if (nntpConnection != NULL)
		nntpConnection->Abort();
}
FUNCTION_END

//-------------------------------------------------------------------------
