//=========================================================================
//                 Copyright (C)1999-2000 by Niels Basjes
//                  SuckMT Website : http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPGetXOVERCommand.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/05/05 20:03:13 $
//  $Revision: 1.6 $
//  $RCSfile: NNTPGetXOVERCommand.cpp,v $
//  $Author: niels $
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

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

#include "NNTPGetXOVERCommand.h"
#include "NNTPGetArticleCommand.h"
#include "NNTPProxy.h"
#include "NNTPCommandHandler.h"
#include "NEWSArticle.h"
#include "TraceLog.h"
#include "StatisticsKeeper.h"

//-------------------------------------------------------------------------

NNTPGetXOVERCommand::NNTPGetXOVERCommand(string newsGroup, long startAtArticlenr)
{
    Lcalls << "CREATED XOVER COMMAND FOR GROUP:" << newsGroup 
           << " @ " << startAtArticlenr << endl << flush;

    fNewsGroup        = newsGroup;
    fStartAtArticlenr = startAtArticlenr;
}

//-------------------------------------------------------------------------
bool 
NNTPGetXOVERCommand::Execute(CommandHandler * currentHandler)
{
    NNTPCommandHandler * myHandler = 
// TODO: figure out why this doesn't work.
//            dynamic_cast<NNTPCommandHandler*> currentHandler;
            (NNTPCommandHandler*) currentHandler;

    if (myHandler == NULL)
        return false;
    
    NNTPProxy * nntpProxy = 
            myHandler->GetNNTPProxy();

    if (nntpProxy == NULL)
        return false;

    Lcalls << "Getting message overview for " << fNewsGroup << endl << flush;

    vector<NEWSArticle*> newsArticles;
    
    if (!nntpProxy->GetGroupOverview(fNewsGroup,myHandler,fStartAtArticlenr))
    {
        Lerror << "Error while calling GetGroupOverview" << endl << flush;
        return false;
    }

    Lcalls << "Group " <<fNewsGroup << " has " << newsArticles.size() 
           << " new messages." << endl << flush;
/*
    vector<NEWSArticle*>::iterator articleIter;

    if (!KeepRunning())
    {
        for( articleIter  = newsArticles.begin();
             articleIter != newsArticles.end();
             articleIter ++)
        {
            delete *articleIter;
        }
        return false;
    }

    STAT_AddValue("Articles Present",newsArticles.size());

    for( articleIter  = newsArticles.begin();
         articleIter != newsArticles.end();
         articleIter ++)
    {
        NEWSArticle* article = *articleIter;
        if (!KeepRunning() || !myHandler->DoWeKeepThisArticle(article))
        {   
            STAT_AddValue("Articles Killed",1);
            //cout << "KILLED ARTICLE: " << article->fMessageID << endl;
            delete article;
        }
        else
        {
            myHandler->AddCommand(new NNTPGetArticleCommand(article));
        }
    }
*/
    return true;
}

//-------------------------------------------------------------------------

// End of the file NNTPGetXOVERCommand.cpp
//=========================================================================
