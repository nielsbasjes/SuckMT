//=========================================================================
//                 Copyright (C)1999-2000 by Niels Basjes
//                  SuckMT Website : http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : HeaderMatcher.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2001/08/27 19:13:43 $
//  $Revision: 1.13 $
//  $RCSfile: HeaderMatcher.cpp,v $
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

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

//-------------------------------------------------------------------------

#include <string>
#include <time.h>
#include "SuckDefines.h"
#include "HeaderMatcher.h"
#include "Tokenize.h"
#include "TraceLog.h"

//-------------------------------------------------------------------------
// This class reads the specified value from the specified IniFile
// and logs this event by itself when a matching query is performed.
//-------------------------------------------------------------------------

HeaderMatcher::HeaderMatcher(
        IniFile *settings,
        const string sectionName,
        const string valueName)
    : Printable("HeaderMatcher")
    , ArticleImpactChecker(settings)
{
    fObjectIsValid  = true;

#ifdef USE_BOOST_REGEX
    fRegExpression = NULL;
#endif

    // Copy the parameters
    fIniSectionName = sectionName;
    fIniValueName   = valueName;

    // Initialize the Cooked values
    fHeaderName    = "";
    fHeaderValue   = "";
    fLastMatchDate = fNow;
    fMatchCount    = 0;
    fImpactValue   = 100; // Default Impact value
    fSearchCaseINSensitive = false;

    // Quick check of the parameters
    if (fSettings    == NULL ||
        fIniSectionName == ""   ||
        fIniValueName   == "")
    {
        fObjectIsValid = false;
        return;
    }

    // ----------
    // Split the fIniValueName into the required parts
    // There should be two possibilities:
    // Casesensitive matching
    //    fieldname:valuetomatch
    // Case INsensitive matching
    //    ~fieldname:valuetomatch

    // First check if searching case insensitive.
    string parseValueName = fIniValueName;
    if (parseValueName[0] == '~')
    {
        fSearchCaseINSensitive = true;
        // Remove "~"
        parseValueName.erase(parseValueName.begin()); 
    }

    // Now split at the first ':'
    unsigned long colonPosition = parseValueName.find(":");

    if (colonPosition == parseValueName.npos)
    {   // The header doesn't include a ':' --> Not a header !! 
        fObjectIsValid = false;
        Lerror << "Invalid \"" << sectionName 
               << "\" header matching line : \"" 
               << valueName << "\"." << endl << flush;
        return;
    }

    fHeaderName.assign (parseValueName,0,colonPosition);
    fHeaderValue.assign(parseValueName.begin() + colonPosition + 1,parseValueName.end());


#ifdef USE_BOOST_REGEX
    // ----------
    // Try to compile the header value that should be matched
    regbase::flag_type cflags = regbase::normal;
    if (fSearchCaseINSensitive)
        cflags |= regbase::icase;

    try 
    {
        fRegExpression = new regex(fHeaderValue.c_str(), cflags);
    }

    catch (bad_expression)
    {
        fObjectIsValid = false;
        Lerror << "Invalid regular expression \"" << fHeaderValue 
               << "\". Check the suckmt config file" << endl << flush;
        char inistr[100];
        std::sprintf(inistr,"%s ; %5ld ; %5ld ; INVALID REGULAR EXPRESSION !!",fLastMatchDate.c_str(),fMatchCount,fImpactValue);
        fSettings->SetValue(fIniSectionName, fIniValueName, inistr);
        return;
    }
#else
    // ----------
    // Try to compile the header value that should be matched
    int cflags = REG_EXTENDED;
    if (fSearchCaseINSensitive)
        cflags |= REG_ICASE;

    // Make sure the variable has been nullified
    ::memset(&fRegExpression,0,sizeof(fRegExpression));

    if (regcomp (&fRegExpression, fHeaderValue.c_str(), cflags) != 0)
    {
        Lerror << "Parsing regular expression:" << endl 
             << "[" << sectionName << "]:" << valueName << " = " << fHeaderValue 
             << endl << flush;
        fObjectIsValid = false;
        return;
    }
#endif

    // ----------
    // Now get the statistics parameters for the matching process
    if (fSettings->GetValue(fIniSectionName,fIniValueName,fIniValue))
    {
        // ----------
        // Split the value into the required parts
        vector<string> iniValueList = GetTokens(fIniValue,';');

        switch (iniValueList.size())
        {
            case 3: StringToLong(iniValueList[2],fImpactValue);
            case 2: StringToLong(iniValueList[1],fMatchCount);
            case 1: RemoveLeadingSpaces(iniValueList[0]);
                    RemoveTrailingSpaces(iniValueList[0]);
                    fLastMatchDate = iniValueList[0];
        }
    }

    char inistr[100];
    std::sprintf(inistr,"%s ; %5ld ; %5ld",fLastMatchDate.c_str(),fMatchCount,fImpactValue);
    fSettings->SetValue(fIniSectionName, fIniValueName, inistr);
}

//-------------------------------------------------------------------------

HeaderMatcher::~HeaderMatcher()
{
    // Free the memory allocated inside the compiled expression
#ifdef USE_BOOST_REGEX
    if (fRegExpression != NULL)
        delete (fRegExpression);
#else
//    regfree (&fRegExpression);
#endif
}

//-------------------------------------------------------------------------

#ifdef USE_BOOST_REGEX
// Required callback function for the grep implementation
static bool grep_callback(match_results<string::iterator, regex::alloc_type> & /*what*/ ) 
{ 
   return true; 
}   
#endif

long
HeaderMatcher::GetImpactValue(
        NEWSArticle * article,
        const string &headerName,
        string &matchlog)
{
    if (!fObjectIsValid || article == NULL)
        return 0; // Not valid -> no Impact

    // Is this the right header ??
    if (fHeaderName != headerName)
    {
        return 0; // 0 == no impact
    }

    string headerValue;

    bool thisIsTheBody = false;

    if (article->GetState() == NA_COMPLETE && 
        headerName          == SUCK_ARTICLEBODY)
    {
        headerValue = article->GetBody();
        thisIsTheBody = true;
    }
    else
    // Retrieves the value of the specified header field 
    // returns false if the header doesn't exist
    if (!article->GetHeaderField(fHeaderName, headerValue))
    {
        return 0;
    }

    // Match regular expression to the actual value
#ifdef USE_BOOST_REGEX
    if (!regex_grep(grep_callback,headerValue.begin(),headerValue.end(),
                    *fRegExpression,boost::match_default | boost::match_any))
#else
    if (regexec (&fRegExpression, headerValue.c_str(), 0, NULL, 0) == REG_NOMATCH)
#endif
    {
        return 0; // 0 == no impact
    }

    // If we get here we have a matching string ... jippee .... sorry.
    valuesMutex.lock();
    fLastMatchDate = fNow;
    fMatchCount ++;
    valuesMutex.unlock();

    char inistr[100];
    std::sprintf(inistr,"%s ; %5ld ; %5ld",fLastMatchDate.c_str(),fMatchCount,fImpactValue);
    fSettings->SetValue(fIniSectionName, fIniValueName, inistr);

    strstream tmpstream;
    if (thisIsTheBody)
    {
        tmpstream << "MATCHED Article Body: \"" 
            << fHeaderValue << "\" = \"" << headerValue << "\"" << ends;
    }
    else
    {
        tmpstream << "MATCHED Header: " << fHeaderName << ": \"" 
            << fHeaderValue << "\" = \"" << headerValue << "\"" << ends;
    }
    char * tmpcharp = tmpstream.str();
    matchlog += tmpcharp;

    delete[] tmpcharp;
    
    return fImpactValue; 
}

//--------------------------------------------------------------------

// This function returns the name of the header
// for which this ArticleImpactChecker will check.
// Returns true if headerName is filled.
// Returns false if this ArticleImpactChecker will check
// Several headers
bool
HeaderMatcher::GetMatchingHeader(string & headerName)
{
    headerName = fHeaderName;
    return true;
}

//--------------------------------------------------------------------

IMPLEMENT_PRINTABLE_OPERATORS(HeaderMatcher);

void 
HeaderMatcher::Print (ostream &os) const
{
    os  << "HeaderMatcher= ("<<(fObjectIsValid?"1":"0")<<"){" << endl
        << "  RAW:  [" << fIniSectionName << " = " << fIniValueName  << "] =" 
        << fIniValue << endl
        << "  COOK: "  << endl
        << "     fHeaderName    = " << fHeaderName << endl
        << "     fHeaderValue   = " << fHeaderValue << endl
        << "     fLastMatchDate = " << fLastMatchDate << endl
        << "     fMatchCount    = " << fMatchCount << endl
        << "     fNow           = " << fNow << endl
        << "}" << endl;
}

//-------------------------------------------------------------------------

// End of the file HeaderMatcher.cpp
//=========================================================================
