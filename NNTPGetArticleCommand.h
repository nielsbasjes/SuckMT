//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPGetArticleCommand.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/11/18 22:54:26 $
//  $Revision: 1.6 $
//  $RCSfile: NNTPGetArticleCommand.h,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
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
};

//-------------------------------------------------------------------------

#endif // __NNTPGETARTICLECOMMAND_H__

// End of the file NNTPGetArticleCommand.h
//=========================================================================
