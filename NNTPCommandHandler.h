/***************************************************************************
                          NNTPCommand.h  -  description                              
                             -------------------                                         
    begin                : Sun Jul 18 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifndef NNTPCOMMANDHANDLER_H
#define NNTPCOMMANDHANDLER_H

class NNTPCommandHandler; // Forward Declaration

#include "debugging.h"
#include "NNTPRetrieveManager.h"
#include "CommandHandler.h"
#include "NNTPProxy.h"
#include "IniFile.h"
#include "NewsKiller.h"

class NNTPCommandHandler : public CommandHandler
{
public: 
    NNTPCommandHandler (NNTPRetrieveManager* retrieveManager,
                        CommandQueue *commandQueue, 
                        NewsKiller   *killer, 
                        IniFile      *iniFile,
                        string        newsServer);
                        
    ~NNTPCommandHandler();

    NNTPProxy * GetNNTPProxy();
    
    IniFile * GetIniFile();
    
    bool 
    DoWeKeepThisArticle(NEWSArticle * article);

    void
    ArticleFileHasBeenWritten(string fileName);
    
	void AbortChildren();

private:
    NNTPRetrieveManager* fRetrieveManager;
    NewsKiller *fKiller;
    IniFile    *fIniFile;
    NNTPProxy  *nntpConnection;
};

#endif




