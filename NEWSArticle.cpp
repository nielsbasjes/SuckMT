//=========================================================================
//                   Copyright (C) 2000 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NEWSArticle.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/03/02 20:51:29 $
//  $Revision: 1.12 $
//  $RCSfile: NEWSArticle.cpp,v $
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

#include "SuckDefines.h"
#include "NEWSArticle.h"
#include "Tokenize.h"

//--------------------------------------------------------------------

NEWSArticle::NEWSArticle(): 
        Printable("NEWSArticle")
{
    fNrOfNewsGroups = 1; // Let's assume a reasonable default
    
    fPrimaryNewsGroup = "";

    fState = NA_NOTHING;

    fKillMatched = false;
    fKeepMatched = false;
    fKillKeepScore = 0;
}

//--------------------------------------------------------------------

NEWSArticle::~NEWSArticle()
{
    // Nothing to do
}

//--------------------------------------------------------------------

NEWSArticle::NEWSArticle(string newsgroup, string serverLine): 
        Printable("NEWSArticle")
{
    fNrOfNewsGroups = 1; // Let's assume a reasonable default
    
    fState = NA_NOTHING;

    fKillMatched = false;
    fKeepMatched = false;
    fKillKeepScore = 0;

    fPrimaryNewsGroup = newsgroup;
    SetXOVERLine(serverLine.c_str());
}

//--------------------------------------------------------------------

IMPLEMENT_PRINTABLE_OPERATORS(NEWSArticle)

void 
NEWSArticle::Print (ostream &os) const
{
    os  << "NEWSArticle= {"                    << endl
        << "   articleNr  = " << fArticleNr  << endl
        << "   subject    = " << fSubject    << endl
        << "   sender     = " << fSender     << endl
        << "   date       = " << fDate       << endl
        << "   messageID  = " << fMessageID  << endl
        << "   references = { ("<<fParsedReferences.size()<<")" << endl;
        
    for(vector<string>::const_iterator
        itemIter  = fParsedReferences.begin();
        itemIter != fParsedReferences.end();
        itemIter ++)
    {
        os << "                 " << (*itemIter) << endl;
    }

    os  << "                }" << endl
        << "   bytes      = " << fBytes      << endl
        << "   lines      = " << fLines      << endl
        << "   xrefHeader = " << fXrefHeader << endl;
            
    if (fState == NA_ONLY_HEADER || fState == NA_COMPLETE)
    {
        os  << "   PARSED HEADER = {" << endl;
        for(vector<string>::const_iterator
            itemIter  = fParsedHeaderFields.begin();
            itemIter != fParsedHeaderFields.end();
            itemIter ++)
        {
            os << "     " << (*itemIter)
               << " : " << (fParsedHeaders.find(*itemIter))->second
               << endl<< flush;
        }
        os  << "                }" << endl;
    }    
    os  << "}" << endl << flush ;
}

//--------------------------------------------------------------------

long 
NEWSArticle::Score(long additionalScore)
{
    fKillKeepScore += additionalScore;
    return fKillKeepScore;
}

//--------------------------------------------------------------------

void 
NEWSArticle::SetXOVERLine(const char * line)
{
    vector<string> tokens = GetTokens(line,'\t');
    sscanf(tokens[0].c_str(),"%ld",&fArticleNr);
    fSubject = tokens[1].c_str();
    fSender = tokens[2].c_str();
    fDate = tokens[3].c_str();
    fMessageID = tokens[4].c_str();
    fReferences = tokens[5].c_str();
    sscanf(tokens[6].c_str(),"%ld",&fBytes);
    sscanf(tokens[7].c_str(),"%ld",&fLines);
    fXrefHeader = tokens[8].c_str();
    
    fParsedReferences = GetTokens(fReferences,' ');
    fState = NA_ONLY_XOVER;
}

//--------------------------------------------------------------------

bool 
NEWSArticle::StoreHeader(const string &line)
{
    // First we simply store the raw header string
    fHeader = line;

    // Now we try to disassemble the header and store it as such

    // Separate the lines
    vector<string> headerLines = GetTokens(fHeader,'\n');
    
    string fieldName  = "";
    string fieldValue = "";
    
    for(vector<string>::const_iterator
        itemIter  = headerLines.begin();
        itemIter != headerLines.end();
        itemIter ++)
    {
        string thisLine = (*itemIter);
        RemoveTrailingSpaces(thisLine);

        if (thisLine[0] != ' ' && thisLine[0] != '\t')
        {  // This means that this line is a new field
            int position = thisLine.find(":");

            fieldName.assign(thisLine,0,position);
            fieldValue.assign(thisLine.begin()+position+1,thisLine.end());

            RemoveLeadingSpaces(fieldName);
            RemoveTrailingSpaces(fieldName);

            RemoveLeadingSpaces(fieldValue);
            RemoveTrailingSpaces(fieldValue);
        
            // Store the field 
            fParsedHeaderFields.push_back(fieldName);
            fParsedHeaders[fieldName] = fieldValue;
        }
        else
        {  // This means that this line is a continuation of the previous line
            // Just append the extra data with a newline
            fParsedHeaders[fieldName] += "\r\n" + thisLine;
        }
    }

    string groupsField;
    if (GetHeaderField("Newsgroups",groupsField))
    {
            fNewsGroups = GetTokens(groupsField,',');
            fNrOfNewsGroups = fNewsGroups.size();
    }

    fState = NA_ONLY_HEADER;
    return true;
}

//--------------------------------------------------------------------

bool 
NEWSArticle::StoreBody(const string &line)
{
    fBody = line;
    fState = NA_COMPLETE;
    return true;
}

//--------------------------------------------------------------------

NewsArticleState 
NEWSArticle::GetState()
{
    return fState;
}

//--------------------------------------------------------------------

string
NEWSArticle::GetHeader()
{
    return fHeader;
}

//--------------------------------------------------------------------

bool
NEWSArticle::GetHeaderField(string headerName,string &value)
{
    if (fParsedHeaders.find(headerName) == fParsedHeaders.end())
    {
        if (headerName == "ArticleNr")
        {
            char str[50];
            sprintf(str,"%ld",fArticleNr);
            value = str;
            return true;
        }

        if (headerName == "Subject") 
        {
            value = fSubject;
            return true;
        }

        if (headerName == "From")
        {
            value = fSender;
            return true;
        }

        if (headerName == "Date")
        {
            value = fDate;
            return true;
        }

        if (headerName == "Message-ID")
        {
            value = fMessageID;
            return true;
        }

        if (headerName == "References")
        {
            value = fReferences;
            return true;
        }

        if (headerName == "Bytes")
        {
            char str[50];
            sprintf(str,"%ld",fBytes);
            value = str;
            return true;
        }

        if (headerName == "Lines")
        {
            char str[50];
            sprintf(str,"%ld",fLines);
            value = str;
            return true;
        }

        if (headerName == "Xref")
        {
            value = fXrefHeader;
            return true;
        }

        if (headerName == SUCK_ARTICLEBODY)
        {
            value = fBody;
            return true;
        }

        return false; // Value doesn't exist 
    }
    value = fParsedHeaders[headerName];
    return true;    
}

//--------------------------------------------------------------------

string 
NEWSArticle::GetBody()
{
    return fBody;
}

//--------------------------------------------------------------------

bool
NEWSArticle::operator== (const string& someMessageID) const
{
    return (fMessageID == someMessageID);
}

//--------------------------------------------------------------------

// End of the file NEWSArticle.cpp
//=========================================================================
