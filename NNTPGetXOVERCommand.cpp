//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPGetXOVERCommand.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/29 20:12:36 $
//  $Revision: 1.3 $
//  $RCSfile: NNTPGetXOVERCommand.cpp,v $
//  $Author: niels $
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
//cout << endl << "CREATED XOVER COMMAND FOR GROUP:" << newsGroup << " @ " << startAtArticlenr << endl << endl << endl << flush;

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

//    cout << endl << "Getting message overview for " << fNewsGroup << endl << flush;

    vector<NEWSArticle*> newsArticles;
    
    if (!nntpProxy->GetGroupOverview(fNewsGroup,myHandler,fStartAtArticlenr))
    {
        lprintf(LOG_ERROR,"Error while calling GetGroupOverview");
        return false;
    }

//    cout << endl << "Group " <<fNewsGroup << " has " << newsArticles.size() << " new messages." << endl << flush;
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
