//=========================================================================
//                 Copyright (C)1999-2000 by Niels Basjes
//                  SuckMT Website : http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPGetArticleCommand.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2002/02/18 23:08:43 $
//  $Revision: 1.16 $
//  $RCSfile: NNTPGetArticleCommand.cpp,v $
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

#ifdef _MSC_VER
#pragma warning( disable : 4786 ) 
#endif

//-------------------------------------------------------------------------

#include <sys/stat.h>
#include <fstream>
#include <strstream>

#include "SuckDefines.h"
#include <omnithread.h>
#include "NNTPGetArticleCommand.h"
#include "NNTPProxy.h"
#include "NNTPCommandHandler.h"
#include "NEWSArticle.h"
#include "TraceLog.h"
#include "StatisticsKeeper.h"


//-------------------------------------------------------------------------

enum {
    GAC_BEGIN = 0,              // Normal starting point
    GAC_FILE_DOES_NOT_EXIST,    // We know the file doesn't exist
    GAC_NEED_DOWNLOAD,          // We know we want to download this message
    GAC_CHECKED_XOVER_HEADERS,  // We know we want to download this message
    GAC_HAVE_ARTICLE_HEAD,      // We have the article head
    GAC_CHECKED_HEADERS,        // We know we want to download this message
    GAC_HAVE_ARTICLE_BODY,      // We have the article body
    GAC_CHECKED_BODY,           // We know we want to download this message
    GAC_WROTE_FILE              // The message has been written
};

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
    fProgress = GAC_BEGIN;
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
    delete[] tmpFileName;

    // This switch is required to be able to download
    // this message if the connection failed.
    // Without this switch the message will be skipped due to calling 
    // DoWeNeedToDownloadThisArticle for the second time
    switch(fProgress)
    {
    case GAC_BEGIN: // Normal starting point
        // First we check if the specified file already exists.
        // This can happen when we restart a killed session
        if (FileExists(fileName))
        {
            // Although this session didn't write the file it still needs
            // to be post processed and therefor registered as 'stored'
            myHandler->ArticleHasBeenStored(article,fileName);
            STAT_AddValue("Articles Skipped",1);
            return true; // Done
        }

        fProgress = GAC_FILE_DOES_NOT_EXIST;

    //--------------------------
    case GAC_FILE_DOES_NOT_EXIST: // We know the file doesn't exist
        // Check if this article hasn't already been downloaded
        if (!myHandler->DoWeNeedToDownloadThisArticle(article))
        {
            STAT_AddValue("Articles Skipped",1);
            return true; 
        }

        fProgress = GAC_NEED_DOWNLOAD;

    //--------------------------
    case GAC_NEED_DOWNLOAD: // We know we want to download this message
        // Based on the XOVER fields --> Do we continue or kill this one ?
        if (!myHandler->DoWeKeepThisArticle(article))
        {   
            // The central handler must know the article has been killed
            myHandler->ArticleHasBeenKilled(article);
            STAT_AddValue("Articles Killed",1);
            return true; // Done
        }

        fProgress = GAC_CHECKED_XOVER_HEADERS;

    //--------------------------
    case GAC_CHECKED_XOVER_HEADERS: // We know we want to download this message
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

        fProgress = GAC_HAVE_ARTICLE_HEAD;

    //--------------------------
    case GAC_HAVE_ARTICLE_HEAD: // We have the article head

        // Based on the headers --> Do we continue or kill this one ?
        if (!myHandler->DoWeKeepThisArticle(article))
        {   
            // The central handler must know the article has been killed
            myHandler->ArticleHasBeenKilled(article);
            STAT_AddValue("Articles Killed",1);
            return true; // Done
        }

        fProgress = GAC_CHECKED_HEADERS;

    //--------------------------
    case GAC_CHECKED_HEADERS: // We know we want to download this message

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

        fProgress = GAC_HAVE_ARTICLE_BODY;

    //--------------------------
    case GAC_HAVE_ARTICLE_BODY: // We have the article body

        // Based on the actual content --> Do we continue or kill this one ?
        if (!myHandler->DoWeKeepThisArticle(article))
        {   
            // The central handler must know the article has been killed
            myHandler->ArticleHasBeenKilled(article);
            STAT_AddValue("Articles Killed",1);
            return true; // Done
        }

        fProgress = GAC_CHECKED_BODY;

    //--------------------------
    case GAC_CHECKED_BODY: // We know we want to download this message
        // Ok, we got the article. Now we store it.
        {
        ofstream outFile(fileName.c_str());
        bool keep_CarriageReturn = true;
        string separator;

        iniFile->GetValue(SUCK_CONFIG,SUCK_KEEP_CR,keep_CarriageReturn); 
        
        if (keep_CarriageReturn)
            separator = "\r\n";
        else            
            separator = "\n";

        outFile << article->GetHeader()  << separator;
        outFile << article->GetBody()    << separator;
        
        }
        // The central handler must know the article has been stored
        myHandler->ArticleHasBeenStored(article,fileName);
        STAT_AddValue("Articles Written",1);

        fProgress = GAC_WROTE_FILE;

    //--------------------------
    case GAC_WROTE_FILE: // The message has been written
    default: ; // Unknown value --> do nothing
    }
            
    return true;
}

//-------------------------------------------------------------------------

// End of the file NNTPGetArticleCommand.cpp
//=========================================================================
