/***************************************************************************
                          Command.cpp  -  description                              
                             -------------------                                         
    begin                : Sun Jul 18 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

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

FUNCTION_START(NNTPGetXOVERCommand::NNTPGetXOVERCommand(string newsGroup, long startAtArticlenr))
{
//cout << endl << "CREATED XOVER COMMAND FOR GROUP:" << newsGroup << " @ " << startAtArticlenr << endl << endl << endl << flush;

    fNewsGroup        = newsGroup;
    fStartAtArticlenr = startAtArticlenr;
}
FUNCTION_END

//-------------------------------------------------------------------------
bool 
FUNCTION_START(NNTPGetXOVERCommand::Execute(CommandHandler * currentHandler))
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
            //cout << "KILLED ARTICLE: " << article->messageID << endl;
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
FUNCTION_END

//-------------------------------------------------------------------------
