//=========================================================================
//                 Copyright (C)1999-2005 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT
//                       Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Project: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Id: NNTPGetArticleCommand.h 17 2008-08-17 15:29:35Z niels $
//  $Date: 2008-08-17 17:29:35 +0200 (Sun, 17 Aug 2008) $
//  $Revision: 17 $
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

#ifndef __NNTPGETARTICLECOMMAND_H__
#define __NNTPGETARTICLECOMMAND_H__

//-------------------------------------------------------------------------

class NNTPGetArticleCommand; // Forward Declaration

//-------------------------------------------------------------------------

#include "Command.h"
#include "CommandHandler.h"
#include "NEWSArticle.h"

//-------------------------------------------------------------------------

class NNTPGetArticleCommand : public Command
{
public:
    NNTPGetArticleCommand(NEWSArticle * theArticle);
    ~NNTPGetArticleCommand();
    virtual bool Execute(CommandHandler * currentHandler);
private:
    NEWSArticle * article;
    int           fProgress;
};

//-------------------------------------------------------------------------

#endif // __NNTPGETARTICLECOMMAND_H__

// End of the file NNTPGetArticleCommand.h
//=========================================================================
