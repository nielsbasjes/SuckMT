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

#include <sys/stat.h>
#include <fstream.h>

#ifdef WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif

#include "SuckDefines.h"
#include "omnithread.h"
#include "NNTPGetArticleCommand.h"
#include "NNTPProxy.h"
#include "NNTPCommandHandler.h"
#include "NEWSArticle.h"
#include "TraceLog.h"
#include "StatisticsKeeper.h"
 
static bool FileExists(char *fileName)
{
    struct stat fileStatus;
    if (stat(fileName,&fileStatus) == 0)
        return true;
    return false;
}

//-------------------------------------------------------------------------

FUNCTION_START(NNTPGetArticleCommand::NNTPGetArticleCommand(NEWSArticle * theArticle))
{
    article = theArticle;
}
FUNCTION_END

//-------------------------------------------------------------------------
bool 
FUNCTION_START(NNTPGetArticleCommand::Execute(CommandHandler * currentHandler))
{
    NNTPCommandHandler * myHandler = 
//            dynamic_cast<NNTPCommandHandler*> currentHandler;
            (NNTPCommandHandler*) currentHandler;

    if (myHandler == NULL)
        return false;
    
    NNTPProxy * nntpProxy = 
            myHandler->GetNNTPProxy();

    if (nntpProxy == NULL)
        return false;

    IniFile *iniFile = myHandler->GetIniFile();

    if (iniFile != NULL)
    {

// TODO: This section contains a race condition between the Get and the Set.
        long currentMax;
        if (iniFile->GetValue(SUCK_GROUPS,article->primaryNewsGroup,currentMax))
        {
            if (currentMax < (long)article->articleNr)
                iniFile->SetValue(SUCK_GROUPS,
                                  article->primaryNewsGroup,
                                  article->articleNr);
        }        
        else
            iniFile->SetValue(SUCK_GROUPS,
                              article->primaryNewsGroup,
                              article->articleNr);
    }

//    char * fileName = GetArticleFileName();

    strstream filenameStr;

    string baseDirectory;
    if(iniFile->GetValue(SUCK_CONFIG,SUCK_DIR,baseDirectory))
        filenameStr << baseDirectory << "/" ;

    filenameStr << article->primaryNewsGroup << "_" << article->articleNr << ends;

    char * fileName = filenameStr.str();

    // First we check if the specified file already exists.
    // This can happen when we restart a killed session
    if (FileExists(fileName))
    {
        STAT_AddValue("Articles Skipped",1);
        
        // Although this session didn't write the file it still needs
        // to be post processed
        myHandler->ArticleFileHasBeenWritten(fileName);

        delete fileName;
        delete article;
        return true; // Done
    }

	// Based on the XOVER fields --> Do we continue or kill this one ?
    if (!myHandler->DoWeKeepThisArticle(article) || !KeepRunning())
    {   
        delete fileName;
        delete article;
        return true; // Done
    }

    // Get the headers of the article
    if (!nntpProxy->GetArticleHead(article))
    {   
        cout << endl << "ERROR GETTING ARTICLE HEAD: " << article->messageID << endl;
        STAT_AddValue("Articles ERROR",1);

        delete fileName;
        delete article;
        return false; // Done
    }

    // Based on the headers --> Do we continue or kill this one ?
    if (!myHandler->DoWeKeepThisArticle(article) || !KeepRunning())
    {   
        //cout << "KILLED ARTICLE: " << article->messageID << endl;
        STAT_AddValue("Articles Killed",1);

        delete fileName;
        delete article;
        return true; // Done
    }

    // Get the body of the article
    if (!nntpProxy->GetArticleBody(article))
    {   
        cout << "ERROR GETTING ARTICLE HEAD: " << article->messageID << endl;
        STAT_AddValue("Articles ERROR",1);

        delete fileName;
        delete article;
        return false; // Done
    }

    if (KeepRunning())
    {
        // Ok, we got the article. Now we store it.
        ofstream outFile(fileName);
        outFile << article->header  << "\r\n";
        outFile << article->body    << "\r\n";
        STAT_AddValue("Articles Written",1);
        myHandler->ArticleFileHasBeenWritten(fileName);
    }
    
    delete fileName;
    delete article;
    return true;
}
FUNCTION_END

//-------------------------------------------------------------------------

