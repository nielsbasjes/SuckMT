//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPGetArticleCommand.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/03/19 12:21:48 $
//  $Revision: 1.9 $
//  $RCSfile: NNTPGetArticleCommand.cpp,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

//-------------------------------------------------------------------------

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

//-------------------------------------------------------------------------
 
static bool 
FileExists(string fileName)
{
    struct stat fileStatus;
    if (stat(fileName.c_str(),&fileStatus) == 0)
        return true;
    return false;
}

//-------------------------------------------------------------------------

NNTPGetArticleCommand::NNTPGetArticleCommand(NEWSArticle * theArticle)
{
    article = theArticle;
}

//-------------------------------------------------------------------------

NNTPGetArticleCommand::~NNTPGetArticleCommand()
{
    if (article != NULL)
        delete article;
}

//-------------------------------------------------------------------------
bool 
NNTPGetArticleCommand::Execute(CommandHandler * currentHandler)
{
    NNTPCommandHandler * myHandler = 
//            dynamic_cast<NNTPCommandHandler*> currentHandler;
            (NNTPCommandHandler*) currentHandler;

    if (myHandler == NULL)
        return false;
    
    NNTPProxy * nntpProxy = 
            myHandler->GetNNTPProxy();

    if (nntpProxy == NULL || !(nntpProxy->IsConnected()))
        return false;

    IniFile *iniFile = myHandler->GetIniFile();

    if (iniFile != NULL)
    {
        iniFile->MaxValue(SUCK_GROUPS,
                          article->fPrimaryNewsGroup,
                          article->fArticleNr);
    }

    strstream filenameStr;

    string baseDirectory = "/tmp/"; // Default value
    if(iniFile->GetValue(SUCK_CONFIG,SUCK_DIR,baseDirectory))
        filenameStr << baseDirectory << "/" ;

    filenameStr << article->fPrimaryNewsGroup << "_" 
                << article->fArticleNr << ends;

    char * tmpFileName = filenameStr.str();
    string fileName(tmpFileName);
    delete tmpFileName;

    // First we check if the specified file already exists.
    // This can happen when we restart a killed session
    if (FileExists(fileName))
    {
        STAT_AddValue("Articles Skipped",1);
        
        // Although this session didn't write the file it still needs
        // to be post processed
        myHandler->ArticleHasBeenStored(article,fileName);

        return true; // Done
    }

    // Check if this article hasn't already been downloaded
    if (!myHandler->DoWeNeedToDownloadThisArticle(article))
    {
        STAT_AddValue("Articles Skipped",1);
        return true; 
    }

    // Based on the XOVER fields --> Do we continue or kill this one ?
    if (!myHandler->DoWeKeepThisArticle(article))
    {   
        // The central handler must know the article has been killed
        myHandler->ArticleHasBeenKilled(article);
        return true; // Done
    }

    // Continue running ??
    if (!KeepRunning())
    {   
        return true; // Done
    }

    // Get the headers of the article
    if (!nntpProxy->GetArticleHead(article))
    {   
        Lerror << "ERROR GETTING ARTICLE HEAD: " 
               << article->fMessageID << endl << flush;
        STAT_AddValue("Articles ERROR",1);

        return false; // Done
    }

    // Based on the headers --> Do we continue or kill this one ?
    if (!myHandler->DoWeKeepThisArticle(article))
    {   
        // The central handler must know the article has been killed
        myHandler->ArticleHasBeenKilled(article);
        return true; // Done
    }

    // Continue running ??
    if (!KeepRunning())
    {   
        return true; // Done
    }

    // Get the body of the article
    if (!nntpProxy->GetArticleBody(article))
    {   
        Lerror << "ERROR GETTING ARTICLE BODY: " 
               << article->fMessageID << endl << flush;
        STAT_AddValue("Articles ERROR",1);

        return false; // Done
    }

    // Based on the actual content --> Do we continue or kill this one ?
    if (!myHandler->DoWeKeepThisArticle(article))
    {   
        // The central handler must know the article has been killed
        myHandler->ArticleHasBeenKilled(article);
        return true; // Done
    }

    // Ok, we got the article. Now we store it.
    ofstream outFile(fileName.c_str());
    outFile << article->GetHeader()  << "\r\n";
    outFile << article->GetBody()    << "\r\n";

    // The central handler must know the article has been stored
    myHandler->ArticleHasBeenStored(article,fileName);
            
    return true;
}

//-------------------------------------------------------------------------

// End of the file NNTPGetArticleCommand.cpp
//=========================================================================
