//=========================================================================
//                 Copyright (C)1999-2000 by Niels Basjes
//                  SuckMT Website : http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPProxy.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2001/08/26 20:40:52 $
//  $Revision: 1.10 $
//  $RCSfile: NNTPProxy.h,v $
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

#ifndef __NNTPPROXY_H__
#define __NNTPPROXY_H__

//-------------------------------------------------------------------------

class NNTPProxy; // Forward Declaration

#include <algorithm>
#include <vector>
#include <string> 
#include "AsciiLineSocket.h"
#include "NEWSArticle.h"
#include "Abortable.h"
#include "NNTPCommandHandler.h"
#include "IniFile.h"

//-------------------------------------------------------------------------

using namespace std;

//-------------------------------------------------------------------------

struct GroupInfo
{
    string name;
    unsigned long last;
    unsigned long first;
    string flags;
    unsigned long last_we_have; // 0 means we have none
};

ostream& operator<<(ostream &os, const GroupInfo &group);
ostream& operator<<(ostream &os, const vector<GroupInfo> &groups);

//--------------------------------------------------------------------

class NNTPProxy  : public Abortable
{
public:
    // All parameters required for construction are 
    // available in the IniFile.
    NNTPProxy(IniFile  *settings);
    virtual ~NNTPProxy();

    bool
    IsConnected();

    // Retrieves the complete list of all groups on the server
    bool 
    GetGroups(vector<GroupInfo> &groups);

    // Sets the current group
    bool
    SetCurrentGroup(string groupName,GroupInfo &groupInfo);

    // Retrieves the complete list of the overview database
    bool 
    GetGroupOverview(string                groupName,
                     vector<NEWSArticle*> &newsArticle, 
                     long                  startAtArticlenr = -1);

    // Retrieves the complete list of the overview database
    // Instead of storing the articles in a vector they are stored
    // with the specified commandhandler
    bool 
    GetGroupOverview(string                groupName,
                     NNTPCommandHandler   *commandHandler, 
                     long                  startAtArticlenr = -1);

    // Retrieves an article by ID
    bool
    GetArticle(string articleId,string &article);

    // Retrieves an article by number in the group
    bool
    GetArticle(int articleNumber,string &article);

    // Retrieves the header of an article
    bool
    GetArticleHead(string articleId,string &articleHead);

    // Retrieves the header of an article
    bool
    GetArticleHead(NEWSArticle * article);

    // Retrieves the body of an article
    bool
    GetArticleBody(string articleId,string &articleBody);

    // Retrieves the body of an article
    bool
    GetArticleBody(NEWSArticle * article);

private:
    AsciiLineSocket * nntp;
    IniFile         * fSettings;
    string            currentGroup;
    int               fConnectionNr;

    bool              fKeep_CarriageReturn;

    // Implements the common part of both versions of GetGroupOverview
    // returns 0 if error else returns 1 if OK or 2 if no messages.
    int
    COMMON_GetGroupOverview(string groupName,
                            long   startAtArticlenr = -1);

    bool
    Send_MODE_READER();

    // Tries to login to the server
    // Returns true if success
    // Returns false if failed ---> also disconnected.
    bool
    Login(string user,string pass);

    static int Next_ID;
};

//------------------------------------------------------------------------

#endif // __NNTPPROXY_H__

// End of the file NNTPProxy.h
//=========================================================================
