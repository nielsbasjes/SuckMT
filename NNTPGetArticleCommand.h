//=========================================================================
//                 Copyright (C)1999-2000 by Niels Basjes
//                  SuckMT Website : http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPGetArticleCommand.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2004-12-20 21:09:23 +0100 (Mon, 20 Dec 2004) $
//  $Revision: 4 $
//  $RCSfile: NNTPGetArticleCommand.h,v $
//  $Author: $
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
