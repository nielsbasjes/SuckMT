//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPGetArticleCommand.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/10/22 20:37:41 $
//  $Revision: 1.5 $
//  $RCSfile: NNTPGetArticleCommand.h,v $
//  $Author: niels $
//=========================================================================

#ifndef NNTPGetARTICLECOMMAND_H
#define NNTPGetARTICLECOMMAND_H

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

#endif

//-------------------------------------------------------------------------

// End of the file NNTPGetArticleCommand.h
//=========================================================================
