//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPProxy.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/10/07 19:43:14 $
//  $Revision: 1.3 $
//  $RCSfile: NNTPProxy.h,v $
//  $Author: niels $
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

//-------------------------------------------------------------------------

// Warning Dirty hack to skip the std namespace in Visual C++ 6.0
#ifdef __WIN32__
#define string std::string
#endif

//--------------------------------------------------------------------

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
    NNTPProxy(string serverName);
    virtual ~NNTPProxy();

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
    string currentGroup;

    // Implements the common part of both versions of GetGroupOverview
    // returns 0 if error else returns 1 if OK or 2 if no messages.
    int
    COMMON_GetGroupOverview(string groupName,
                            long   startAtArticlenr = -1);
};

//------------------------------------------------------------------------

#endif // __NNTPPROXY_H__

// End of the file NNTPProxy.h
//=========================================================================
