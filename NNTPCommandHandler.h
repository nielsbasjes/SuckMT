//=========================================================================
//                 Copyright (C)1999-2005 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT
//                       Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Project: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Id: NNTPCommandHandler.h 6 2004-12-20 20:53:20Z niels $
//  $Date: 2004-12-20 21:53:20 +0100 (Mon, 20 Dec 2004) $
//  $Revision: 6 $
//  $LastChangedBy: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//   If you reuse code from SuckMT you are required to put a notice of 
//   this fact in both your manual and about box.
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
