//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPCommandHandler.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/03/28 20:06:19 $
//  $Revision: 1.8 $
//  $RCSfile: NNTPCommandHandler.h,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifndef __NNTPCOMMANDHANDLER_H__
#define __NNTPCOMMANDHANDLER_H__

//-------------------------------------------------------------------------

class NNTPCommandHandler; // Forward Declaration

//-------------------------------------------------------------------------

#include "NNTPRetrieveManager.h"
#include "CommandHandler.h"
#include "NNTPProxy.h"
#include "IniFile.h"
#include "NewsKiller.h"

//-------------------------------------------------------------------------

class NNTPCommandHandler : public CommandHandler
{
public: 
    NNTPCommandHandler (NNTPRetrieveManager* retrieveManager,
                        CommandQueue *commandQueue, 
                        NewsKiller   *killer, 
                        IniFile      *iniFile);

    ~NNTPCommandHandler();

    NNTPProxy * GetNNTPProxy();

    IniFile * GetIniFile();

    bool 
    DoWeKeepThisArticle(NEWSArticle * article);

    // Store the specified messageID so we can check if we get any duplicates
    // Returns 
    //  - true  if the messageID was unknown and it has been stored.
    //  - false if the messageID was already present and it has not been stored.
    bool
    DoWeNeedToDownloadThisArticle(NEWSArticle * article);

    // Make a note this article has been stored in the specified file
    void
    ArticleHasBeenStored(NEWSArticle * article,string filename);

    // Make a note this article has been stored
    void
    ArticleHasBeenKilled(NEWSArticle * article);

    // Forward the Abort command to all child objects
    void 
    AbortChildren();

protected:

    bool 
    CommandHandlerCanContinue();

private:
    NNTPRetrieveManager* fRetrieveManager;
    NewsKiller *fKiller;
    IniFile    *fIniFile;
    NNTPProxy  *nntpConnection;
};

//-------------------------------------------------------------------------

#endif // __NNTPCOMMANDHANDLER_H__

// End of the file NNTPCommandHandler.h
//=========================================================================
