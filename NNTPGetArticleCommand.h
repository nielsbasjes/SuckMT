//=========================================================================
//                 Copyright (C)1999-2005 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT
//                       Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Project: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Id: NNTPGetArticleCommand.h 6 2004-12-20 20:53:20Z niels $
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
