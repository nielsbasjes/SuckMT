//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPGetArticleCommand.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/11/18 22:54:25 $
//  $Revision: 1.6 $
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
FileExists(char *fileName)
{
    struct stat fileStatus;
    if (stat(fileName,&fileStatus) == 0)
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

    string baseDirectory;
    if(iniFile->GetValue(SUCK_CONFIG,SUCK_DIR,baseDirectory))
        filenameStr << baseDirectory << "/" ;

    filenameStr << article->fPrimaryNewsGroup << "_" << article->fArticleNr << ends;

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
        return true; // Done
    }

    // Based on the XOVER fields --> Do we continue or kill this one ?
    if (!myHandler->DoWeKeepThisArticle(article) || !KeepRunning())
    {   
        delete fileName;
        return true; // Done
    }

    // Get the headers of the article
    if (!nntpProxy->GetArticleHead(article))
    {   
//        cout << endl << "ERROR GETTING ARTICLE HEAD: " << article->fMessageID << endl;
        STAT_AddValue("Articles ERROR",1);

        delete fileName;
        return false; // Done
    }

    // Based on the headers --> Do we continue or kill this one ?
    if (!myHandler->DoWeKeepThisArticle(article) || !KeepRunning())
    {   
        delete fileName;
        return true; // Done
    }

    // Get the body of the article
    if (!nntpProxy->GetArticleBody(article))
    {   
//        cout << "ERROR GETTING ARTICLE BODY: " << article->fMessageID << endl;
        STAT_AddValue("Articles ERROR",1);

        delete fileName;
        return false; // Done
    }

    if (KeepRunning())
    {
        // Ok, we got the article. Now we store it.
        ofstream outFile(fileName);
        outFile << article->GetHeader()  << "\r\n";
        outFile << article->GetBody()      << "\r\n";
        STAT_AddValue("Articles Written",1);
        myHandler->ArticleFileHasBeenWritten(fileName);
    }
    
    delete fileName;
    return true;
}

//-------------------------------------------------------------------------

// End of the file NNTPGetArticleCommand.cpp
//=========================================================================
