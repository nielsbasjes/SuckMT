/***************************************************************************
                          NEWSArticle.cpp  -  description                              
                             -------------------                                         
    begin                : Sun Jul 18 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/


#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

#include "NEWSArticle.h"
#include "Tokenize.h"
//--------------------------------------------------------------------

FUNCTION_START(NEWSArticle::NEWSArticle(): Printable("NEWSArticle"))
{
    // Nothing to do
}
FUNCTION_END

//--------------------------------------------------------------------

FUNCTION_START(NEWSArticle::~NEWSArticle())
{
    // Nothing to do
}
FUNCTION_END

//--------------------------------------------------------------------

FUNCTION_START(NEWSArticle::NEWSArticle(string newsgroup, string serverLine): Printable("NEWSArticle"))
{
    primaryNewsGroup = newsgroup;
    SetXOVERLine(serverLine.c_str());
}
FUNCTION_END

//--------------------------------------------------------------------

IMPLEMENT_PRINTABLE_OPERATORS(NEWSArticle)

void 
FUNCTION_START(NEWSArticle::Print (ostream &os) const)
{
    os  << "NEWSArticle= {"                    << endl
        << "   articleNr  = " << articleNr  << endl
        << "   subject    = " << subject    << endl
        << "   sender     = " << sender     << endl
        << "   date       = " << date       << endl
        << "   messageID  = " << messageID  << endl
        << "   references = " << references << endl
        << "   bytes      = " << bytes      << endl
        << "   lines      = " << lines      << endl
        << "   xrefHeader = " << xrefHeader << endl
        << "!!! SOME FIELDS ARE MISSING !!!"   << endl
        << "}";
}
FUNCTION_END

//--------------------------------------------------------------------

void 
FUNCTION_START(NEWSArticle::SetXOVERLine(const char * line))
{
	vector<string> tokens = GetTokens(line,'\t');
    sscanf(tokens[0].c_str(),"%ld",&articleNr);
    subject = tokens[1].c_str();
    sender = tokens[2].c_str();
    date = tokens[3].c_str();
    messageID = tokens[4].c_str();
    references = tokens[5].c_str();
    sscanf(tokens[6].c_str(),"%ld",&bytes);
    sscanf(tokens[7].c_str(),"%ld",&lines);
    xrefHeader = tokens[8].c_str();
}
FUNCTION_END

//--------------------------------------------------------------------

bool
FUNCTION_START(NEWSArticle::operator== (const string& someMessageID) const)
{
    return (messageID == someMessageID);
}
FUNCTION_END

//--------------------------------------------------------------------
