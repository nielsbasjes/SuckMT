//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPCommandHandler.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/29 20:12:33 $
//  $Revision: 1.3 $
//  $RCSfile: NNTPCommandHandler.h,v $
//  $Author: niels $
//=========================================================================

#ifndef NNTPCOMMANDHANDLER_H
#define NNTPCOMMANDHANDLER_H

class NNTPCommandHandler; // Forward Declaration

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

// End of the file NNTPCommandHandler.h
//=========================================================================
