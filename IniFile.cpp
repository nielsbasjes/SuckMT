//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : IniFile.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/11/18 22:47:36 $
//  $Revision: 1.5 $
//  $RCSfile: IniFile.cpp,v $
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

#include <fstream.h>
#include <stdio.h>  
#include <stdlib.h>  
#include "IniFile.h"
#include "Tokenize.h"
#include "Printable.h"

//-------------------------------------------------------------------------

IniFile::IniFile(string filename)
{
    fFilename = filename;
    if (fFilename != "")
        ReadFile(fFilename);
}

//-------------------------------------------------------------------------

IniFile::~IniFile()
{
    if (fFilename != "")
        WriteFile(fFilename);
    EraseEverything();
}

//-------------------------------------------------------------------------
// Read the specified file into the settings database
// Returns true if file succesfully read
//         false otherwise (like : syntax error / no such file)

bool
IniFile::ReadFile(string filename)
{
    ifstream inFile(filename.c_str());

    bool showWhatIsRead = false;

    if (!inFile.good())
        return false;

    Section * currentSection = NULL;

    char buffer[10000];
    int lineNr = 0;

    while(!inFile.eof())
    {
        lineNr++;
        inFile.getline(buffer,9999);
        
        string work_buffer(buffer);
        
        // Skip leading spaces and tabs
        RemoveLeadingSpaces(work_buffer);

        // Oeps this is the end of the line
        if (work_buffer.begin() == work_buffer.end())
        {
            continue; // This was an empty line
        }

        if (*(work_buffer.begin()) == '#')
        {   
            // This is a comment line
            // Comments are NOT stored an will NOT be 
            // written back to any new file.
            continue;
        }

        if (*(work_buffer.begin()) == '[')
        {
            // Skip the '['
            work_buffer.erase(work_buffer.begin());

            // Skip leading spaces and tabs
            RemoveLeadingSpaces(work_buffer);
            RemoveTrailingSpaces(work_buffer);
            
            // This is the start of a new section
            string token(work_buffer.begin(),work_buffer.find("]"));

            RemoveTrailingSpaces(token);

            // Get or Make the requested section
            currentSection = AddSection(token);

            if (showWhatIsRead)
            {
                cout << endl << "[" << token << "]" << endl;
            }

            if (currentSection == NULL)
            {   // Fatal error.
                cout << "Couldn't find or create section \"" << token
                     << "\" in file \"" << filename 
                     << "\" at line " << lineNr << "." << endl;
                return false;
            }
        }
        else
        {
            if (currentSection == NULL)
            {
                cout << "Value without a section in \"" << filename
                     << "\" at line " << lineNr << "." << endl;
                return false;
            }

            vector<string> tokens = GetTokens(work_buffer,'=');
            
            string settingName  = tokens[0];      // Everything before the first '='
            string settingValue = "";
            if (tokens.size() == 2)
                settingValue = tokens[1]; // Everything after the first '=';

            RemoveLeadingSpaces(settingName);
            RemoveTrailingSpaces(settingName);

            RemoveLeadingSpaces(settingValue);
            RemoveTrailingSpaces(settingValue);

            if (settingName  == "IniFile Verbose Output")
            {
                if (settingValue == "Enable")
                {
                    showWhatIsRead = true;
                }
                else
                {
                    showWhatIsRead = false;
                }
            }


            if (showWhatIsRead)
            {
                cout << settingName << " = " <<  settingValue << endl;
            }

            if (!SetValue(currentSection,settingName,settingValue))
            {
                cout << "Unable to set or create the value \"" << settingName.c_str() 
                     << "\" in file \"" << filename.c_str() 
                     << "\" at line " << lineNr << "." << endl;
                return false;
            }

        }
        
    }
    return true;
}

//-------------------------------------------------------------------------
// Write all settings to the specified file
// Returns true if file succesfully written
//         false otherwise.
bool
IniFile::WriteFile(string filename)
{
    ofstream outFile(filename.c_str());
    if (!outFile.good())
        return false;

//    outFile 
//      << "#=====================================" << endl
//      << "# This file was read and written using" << endl
//      << "# Niels Basjes` C++ IniFile class     " << endl
//      << "#-------------------------------------" << endl
//      << "# (C) 1999 by ir. Niels Basjes        " << endl
//      << "# http://www.wirehub.nl/~basjesn      " << endl
//      << "#=====================================" << endl
//      << endl;
    
    return Print(outFile);
}

//------------------------------------------------------
// Prints all settings to the specified stream
// Returns true if file succesfully written
//         false otherwise.
bool
IniFile::Print(ostream & os)
{
    omni_mutex_lock lock(valuesMutex);
    list<string>::iterator sectionIter;
    list<string>::iterator valuesIter;

    for(sectionIter  = sectionsOrder.begin();
        sectionIter != sectionsOrder.end();
        sectionIter ++)
    {
        Section * thisSection = fSections[*sectionIter];
        
        if (thisSection == NULL)
        {
            cout << "FATAL ERROR: sectionsOrder contains section that doesn't exist." << endl;
            return false;
        }
        
        os << "[" << (*sectionIter).c_str() << "]" << endl;
        
        int sectionMaxVarNameLen = thisSection->maxVariableNameLen;
        
        for(valuesIter  = thisSection->variablesOrder.begin();
            valuesIter != thisSection->variablesOrder.end();
            valuesIter ++)
        {
            os << (*valuesIter).c_str();
            for (int i  = valuesIter->length() ; 
                     i <= sectionMaxVarNameLen ; 
                     i++)
                 os << " ";
            os << " = " << ((*thisSection)[*valuesIter]).c_str() << endl;
        }
        os << endl;
    }
    
    return true;
}

//-------------------------------------------------------------------------
// Get the specified setting from the specified section
// The value is returned in the value parameter
// Returns true if the setting exists --> value is filled
// Returns false if the setting does not exist --> value is unchanged
bool
IniFile::GetValue(string section, string name, string &value)
{
    omni_mutex_lock lock(valuesMutex);
    return GetValue(GetSection(section),name,value);
}

//-------------------------------------------------------------------------
// Get the specified setting from the specified section
// The value is returned in the value parameter
// Returns true if the setting exists --> value is filled
// Returns false if the setting does not exist --> value is unchanged
bool
IniFile::GetValue(Section * theSection, string name, string &value)
{
    if (theSection == NULL)
        return false; // Section doesn't exist 
        
    if (theSection->find(name) == theSection->end())
        return false; // Value doesn't exist 

    value = (*theSection)[name];
    return true;    
}

//------------------------------------------------------
// Get the specified setting from the specified section
// The value is returned in the value parameter
// Returns true if the setting exists and it is a number 
//                          --> value is filled
// Returns false if the setting does not exist or not a number 
//                          --> value is 0 if not exists
//                          --> value is 1 if exists
bool
IniFile::GetValue(string section, string name, long &value)
{
    omni_mutex_lock lock(valuesMutex);
    return GetValue(GetSection(section),name,value);
}

//------------------------------------------------------
// Get the specified setting from the specified section
// The value is returned in the value parameter
// Returns true if the setting exists and it is a number 
//                          --> value is filled
// Returns false if the setting does not exist or not a number 
//                          --> value is 0 if not exists
//                          --> value is 1 if exists
bool
IniFile::GetValue(Section * theSection, string name, long &value)
{
    if (theSection == NULL)
        return false;

    string string_value;
    if (!GetValue(theSection, name, string_value))
    {
        value = 0;    // NON existence indicator
        return false; // Doesn't exist
    }

    value = 1;    // Existence indicator
        
    const char * beginptr = string_value.c_str();
    char       * endptr   = NULL;
    
    if (*beginptr == 0x00) // The existing value is an empty string
        return false; // We interpret an empty string as NOT a number

    long currentNumberValue = 
            strtol(string_value.c_str(),&endptr, 10);

    //  man strtol says: "Thus, if *nptr is not `\0' but **endptr 
    //  is `\0' on return, the entire string is valid.
    if (*endptr != 0x00 /*|| errno == ERANGE*/ )
        return false;

    value = currentNumberValue;
    
    return true;
}

//------------------------------------------------------
// Get the list of all the variables in the specified section
// The list of names is returned in the variableNames parameter
// Returns true if the section exists --> value is filled
// Returns false if the section does not exist
//                          --> value is unchanged
bool
IniFile::GetVariableNames(string section,list<string> &variableNames)
{
    Section* theSection = GetSection(section);
    
    if (theSection == NULL)
        return false; // Section doesn't exist
    
    variableNames.clear();
    variableNames = theSection->variablesOrder;

    return true;
}

//------------------------------------------------------
// Get the list of all the sections
// The list of names is returned in the sectionNames parameter
// Returns true if succes --> value is filled
// Returns false in case of error --> value is unchanged
bool
IniFile::GetSectionNames(list<string> &sectionNames)
{
    sectionNames.clear();
    sectionNames = sectionsOrder;
    return true;
}

//------------------------------------------------------
// Add the specified section
// Returns true if created, false if already existed
bool
IniFile::CreateSection(string section)
{
    Section* theSection = GetSection(section);
    
    if (theSection != NULL)
        return false; // Section already exists

    if (AddSection(section) == NULL)
        return false; // Couldn't create
    
    return true; // Created
}

//------------------------------------------------------
// Set the specified setting in the specified section
// If the section doesn't exist it is created
// Returns true if succes
// Returns false in case of error.
bool
IniFile::SetValue(string section, string name, string value)
{
    omni_mutex_lock lock(valuesMutex);
    Section * theSection=AddSection(section);

    if (theSection == NULL)
        return false;

    return SetValue(theSection,name,value);
}

//------------------------------------------------------
// Set the specified setting in the specified section
// If the section doesn't exist it is created
// Returns true if succes
// Returns false in case of error.
bool
IniFile::SetValue(string section, string name, long value)
{
    omni_mutex_lock lock(valuesMutex);
    Section * theSection=AddSection(section);

    if (theSection == NULL)
        return false;

    return SetValue(theSection,name,value);
}

//------------------------------------------------------
// Set the specified setting in the specified section
// If the section doesn't exist it is created
// Returns true if succes
// Returns false in case of error.
bool
IniFile::SetValue(Section * theSection, string name, long value)
{
    char cstr[50];
    sprintf(cstr,"%ld",value);
    string newValue = cstr;
    
    return SetValue(theSection,name,newValue);
}

//------------------------------------------------------
// Set the specified setting in the specified section
// If the section doesn't exist it is created
// Returns true if succes
// Returns false in case of error.
bool
IniFile::SetValue(Section * theSection, string name, string value)
{
    if (theSection == NULL)
        return false;
    
    // If it doesn't exist yte we need to add an entry to the
    // variables order thingy. 
    if (theSection->find(name) == theSection->end())
    {
        theSection->variablesOrder.push_back(name);
        if (theSection->maxVariableNameLen < name.length())
        {
            theSection->maxVariableNameLen = name.length();
        }
    }

    (*theSection)[name] = value;

    return true;    
}

//------------------------------------------------------
// Add the specified number to the specified setting 
// If the section doesn't exist it is created
// If the setting doesn't exist it is created
// If the existing setting is a number the value will be added
// If the existing setting is NOT a number false is returned
// Returns true if succes
// Returns false in case of error.
bool
IniFile::AddValue(string section, string name, long value)
{
    long   currentValue;
    
    if (!GetValue(section, name, currentValue))
    {   // This means its the current setting is not a number
        if (currentValue == 0)
        {   // The value just doesn't exist
            // So we can just set it.
            return SetValue(section,name,value);
        }
        else
        {   // The value DOES exist but its a string
            return false;
        }
    }

    return SetValue(section,name,currentValue + value);
}

//------------------------------------------------------
// Set lowest value of the specified and the current value.
// If the section doesn't exist it is created
// If the setting doesn't exist it is created
// If the existing setting is a number the value will be added
// If the existing setting is NOT a number false is returned
// Returns true if succes
// Returns false in case of error.
bool
IniFile::MinValue(string section, string name, long value)
{
    omni_mutex_lock lock(valuesMutex);
    Section * theSection=AddSection(section);

    if (theSection == NULL)
        return false;
        
    long   currentValue;

    if (!GetValue(theSection, name, currentValue))
    {   // This means its the current setting is not a number
        if (currentValue == 0)
        {   // The value just doesn't exist
            // So we can just set it.
            return SetValue(theSection,name,value);
        }
        else
        {   // The value DOES exist but its a string
            return false;
        }
    }

    if (currentValue > value)
        return SetValue(theSection,name,value);
    return true;
}

//------------------------------------------------------
// Set highest value of the specified and the current value.
// If the section doesn't exist it is created
// If the setting doesn't exist it is created
// If the existing setting is a number the value will be added
// If the existing setting is NOT a number false is returned
// Returns true if succes
// Returns false in case of error.
bool
IniFile::MaxValue(string section, string name, long value)
{
    omni_mutex_lock lock(valuesMutex);
    Section * theSection=AddSection(section);

    if (theSection == NULL)
        return false;
        
    long   currentValue;

    if (!GetValue(theSection, name, currentValue))
    {   // This means its the current setting is not a number
        if (currentValue == 0)
        {   // The value just doesn't exist
            // So we can just set it.
            return SetValue(theSection,name,value);
        }
        else
        {   // The value DOES exist but its a string
            return false;
        }
    }

    if (currentValue < value)
        return SetValue(theSection,name,value);
    return true;
}


//-------------------------------------------------------------------------
// Erase the specified setting
// Returns true if erased, false if not present.
bool
IniFile::EraseValue(string section, string name)
{
    omni_mutex_lock lock(valuesMutex);
    Section* theSection = GetSection(section);
    
    if (theSection == NULL)
        return false; // Section doesn't exist

    map<string,string>::iterator valueIter = theSection->find(name);

    if (valueIter == theSection->end())
        return false; // Value doesn't exist
        
    // Remove the value
    theSection->erase(valueIter);

    list<string>::iterator listIter;
    for(listIter  = theSection->variablesOrder.begin();
        listIter != theSection->variablesOrder.end();
        listIter ++)
    {
        if (*listIter == name)
        {
            theSection->variablesOrder.erase(listIter);
            return true;
        }
    }    
   
    // We shouldn't get here
    
    return false;
}

//-------------------------------------------------------------------------
// Erase the specified section and all settings in that section
// Returns true if erased, false if not present.
bool
IniFile::EraseSection(string section)
{
    omni_mutex_lock lock(valuesMutex);

    Section * theSection=GetSection(section);

    if (theSection == NULL)
        return false;
        
    delete (theSection);
    
    fSections.erase(section);

    list<string>::iterator listIter;
    for(listIter  = sectionsOrder.begin();
        listIter != sectionsOrder.end();
        listIter ++)
    {
        if (*listIter == section)
        {
            sectionsOrder.erase(listIter);
            return true;
        }
    }    

    // We shouldn't get here
    
    return false;
}

//-------------------------------------------------------------------------
// Erase all sections and all settings in them
// Returns true if erased, false if already empty.
bool
IniFile::EraseEverything()
{
    omni_mutex_lock lock(valuesMutex);
    
    map<string,Section*>::iterator sectionIter;

    for(sectionIter  = fSections.begin();
        sectionIter != fSections.end();
        sectionIter ++)
    {
       delete (sectionIter->second);
    }

    // Erase all sections
    fSections.clear();
    sectionsOrder.clear();
    
    return true;
}

//-------------------------------------------------------------------------
// Get the specified section
// Returns NULL or a pointer to the specified section
IniFile::Section*
IniFile::GetSection(string section)
{
    map<string,Section*>::const_iterator sectionIter = fSections.find(section);
    if (sectionIter == fSections.end())
        return NULL;
    return sectionIter->second;
}

//-------------------------------------------------------------------------
// Add the specified section if it doesn't exist yet
// Returns NULL or a pointer to the specified section
IniFile::Section *
IniFile::AddSection(string section)
{
    Section * theSection=GetSection(section);

    if (theSection == NULL)
    {   // We need to create a new one
        theSection = new Section();
        fSections[section] = theSection;
        sectionsOrder.push_back(section);
        theSection->maxVariableNameLen = 0;
    }
    
    return theSection;
}

//-------------------------------------------------------------------------

// End of the file IniFile.cpp
//=========================================================================
