//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPCommandHandler.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/12/02 22:34:16 $
//  $Revision: 1.6 $
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

    void
    ArticleFileHasBeenWritten(string fileName);

    void AbortChildren();

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
