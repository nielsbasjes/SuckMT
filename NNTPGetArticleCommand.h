/***************************************************************************
                          NNTPGetArticleCommand.h  -  description                              
                             -------------------                                         
    begin                : Sun Jul 18 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifndef NNTPGetARTICLECOMMAND_H
#define NNTPGetARTICLECOMMAND_H

class NNTPGetArticleCommand; // Forward Declaration

#include "debugging.h"
#include "Command.h"
#include "CommandHandler.h"
#include "NEWSArticle.h"

class NNTPGetArticleCommand : public Command
{
public:
    NNTPGetArticleCommand(NEWSArticle * theArticle);
    virtual bool Execute(CommandHandler * currentHandler);
private:
    NEWSArticle * article;
};

#endif




