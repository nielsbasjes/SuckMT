/***************************************************************************
                          main.cpp  -  description                              
                             -------------------                                         
    begin                : Sat Jul  3 01:47:46 MEST 1999
                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <signal.h>
#include <iostream.h>
#include <iomanip.h>

#include "debugging.h"
#include "TraceLog.h"
#include "NNTPRetrieveManager.h"
#include "StatisticsKeeper.h"
#include "IniFile.h"
#include "SuckDefines.h"

//-------------------------------------------------------------------------
void
FUNCTION_START(Copyright())
{
    cout << setprecision(2) // for the version displaying
         << "+=================================================+" << endl
         << "| Suck MT "<< SUCKMT_VERSION <<" - A Multi Threaded suck replacement |" << endl
         << "+=================================================+" << endl
         << "| (C) 1999 by ir. Niels Basjes                    |" << endl
         << "| http://www.wirehub.nl/~basjesn/suckmt           |" << endl
         << "+=================================================+" << endl;
}
FUNCTION_END

//-------------------------------------------------------------------------
void
FUNCTION_START(Usage())
{
    cout << "+=================================================+" << endl
         << "| Usage: suckmt [-i <file>] [-n <group>] [-init]  |" << endl
         << "| -i <filename>   Use specified file as ini file  |" << endl
         << "| -n <groupname>  Add the specied group           |" << endl
         << "| -init           Fill ini file with defaults     |" << endl
         << "+=================================================+" << endl;
}
FUNCTION_END

//-------------------------------------------------------------------------
void
FUNCTION_START(InitializeIniFile(IniFile &settings))
{
    string dummy;
    time_t now = time(NULL);
    char * nowStr = ctime(&now);
    nowStr[24] = 0x00; // Remove the endl symbol from the time string
 
    // Setting copyright information 
    settings.SetValue(SUCK_COPYRIGHT,SUCK_AUTHOR,
            "Suck MT was written by ir. Niels Basjes (C) 1999");
    settings.SetValue(SUCK_COPYRIGHT,SUCK_LICENSE,
            "Suck MT is distributed under the GNU Public License.");
    settings.SetValue(SUCK_COPYRIGHT,SUCK_WEBSITE,
            "http://www.wirehub.nl/~basjesn/suckmt");
    settings.SetValue(SUCK_COPYRIGHT,SUCK_EMAIL,
            "SuckMT@Basjes.nl");
    
    // Setting default values if they don't exist yet
    if (!settings.GetValue(SUCK_COPYRIGHT,SUCK_FIRST_VERSION,dummy))
        settings.SetValue(SUCK_COPYRIGHT,SUCK_FIRST_VERSION,SUCKMT_VERSION);
    
    if (!settings.GetValue(SUCK_INSTALL,SUCK_INSTALL_DATE,dummy))
        settings.SetValue(SUCK_INSTALL,SUCK_INSTALL_DATE,nowStr);

    if (!settings.GetValue(SUCK_CONFIG,SUCK_NEWS_SERVER,dummy))
        settings.SetValue(SUCK_CONFIG,SUCK_NEWS_SERVER,"news");

    if (!settings.GetValue(SUCK_CONFIG,SUCK_DIR,dummy))
        settings.SetValue(SUCK_CONFIG,SUCK_DIR,"/tmp/");

    if (!settings.GetValue(SUCK_CONFIG,SUCK_BATCH_FILE,dummy))
        settings.SetValue(SUCK_CONFIG,SUCK_BATCH_FILE,"/tmp/suckmtbatch");

    if (!settings.GetValue(SUCK_CONFIG,SUCK_THREADS,dummy))
        settings.SetValue(SUCK_CONFIG,SUCK_THREADS,3);

    if (!settings.GetValue(SUCK_GLOBAL_KILL_RULES,SUCK_MAX_LINES,dummy))
        settings.SetValue(SUCK_GLOBAL_KILL_RULES,SUCK_MAX_LINES,-1);

    // Keep statistics
    settings.SetValue(SUCK_COPYRIGHT,SUCK_LATEST_VERSION,SUCKMT_VERSION);
    settings.SetValue(SUCK_INSTALL,SUCK_LASTRUN_DATE,nowStr);
    settings.AddValue(SUCK_INSTALL,SUCK_RUN,1);

    // Check if the groups section exists
    list<string> groups;
    if (!settings.GetVariableNames(SUCK_GROUPS,groups))
        settings.CreateSection(SUCK_GROUPS);

}
FUNCTION_END

//-------------------------------------------------------------------------

static NNTPRetrieveManager * retrieveManagerToSignal = NULL;
static void SignalHandler(int /*sig_num*/)
{
    cout << "Received STOP signal." << endl << flush;
    if (retrieveManagerToSignal == NULL)
        exit(1); // No clean signalling option .. just die now.

    retrieveManagerToSignal->Abort();
}

//-------------------------------------------------------------------------

static void SetSignalHandler(NNTPRetrieveManager * nntpRetrieveManager)
{
    retrieveManagerToSignal = nntpRetrieveManager;
    signal(SIGINT,  SignalHandler);
//    signal(SIGHUP,  SignalHandler);
//    signal(SIGABRT, SignalHandler);
//    signal(SIGTERM, SignalHandler);
}

//-------------------------------------------------------------------------

FUNCTION_START(int main(int argc, char** argv))
{
    string iniFileName = SUCK_CONFIG_FILE;
    bool   writeDefaultInitFile = false;
    bool   downloadMessages = true;

    IniFile settings;
    
    vector<string> groupsToAdd;
    
//    SetCurrentLogLevel(LOG_EVERYTHING);
//    StartTraceLog("sucknntp.log");

    Copyright();
    
    for (int i = 1 ; i < argc ; i++)
    {
        // INIFILE SETTING ??
        if (!strcmp(argv[i],"-i"))
        {
            if ( argc >= i+2 )
            {
                iniFileName = argv[i+1];
                i++;
                continue;
            }
            else
            {
                cout << "Invalid parameter: Expected a filename after -i" << endl;
                Usage();
                return -1;
            }
        }
        else
        
        // ADD a GROUP ??
        if (!strcmp(argv[i],"-n"))
        {
            if ( argc >= i+2 )
            {
                downloadMessages = false;
                groupsToAdd.push_back(argv[i+1]);
                i++;
                continue;
            }
            else
            {
                cout << "Invalid parameter: Expected a filename after -n" << endl;
                Usage();
                return -1;
            }
        }

        // Create default INIFile ??
        if (!strcmp(argv[i],"-init"))
        {
            writeDefaultInitFile = true;
            continue;
        }

        cout << "Unknown commandline parameter " << argv[i] << endl;
        Usage();
        return -1;
    }
    
    cout << "Reading config file \"" << iniFileName.c_str() << "\"" << endl 
         << flush;
         
    if(!settings.ReadFile(iniFileName))
    {
        if (writeDefaultInitFile)
        {
            cout << "Writing a default file settings file \"" << iniFileName << "\"." << endl;
            cout << "Remember to check the settings file." << endl;
            InitializeIniFile(settings);
            settings.WriteFile(iniFileName);
            return -1;
        }
        cout << "Unable to read settings file \"" << iniFileName << "\"." << endl;
        Usage();
        return -1;
    }
    else
    if (writeDefaultInitFile)
    {
        cout << "The specified settings file \"" << iniFileName << "\" "
             << "already exists and will NOT be overwritten." << endl;
        return -1;
    }

    InitializeIniFile(settings);

    vector<string>::iterator newGroupIter;
    
    for ( newGroupIter  = groupsToAdd.begin();
          newGroupIter != groupsToAdd.end();
          newGroupIter++ )
    {
        string dummyValue;
    
        if (settings.GetValue(SUCK_GROUPS,(*newGroupIter),dummyValue))
        {
            cout << "Group \"" << *newGroupIter << "\" already exists." << endl;
        }
        else
        {
            cout << "Adding new group \"" << *newGroupIter << "\"." << endl;
            settings.SetValue(SUCK_GROUPS,(*newGroupIter),-1);
        }
    }

    if (downloadMessages)
    {
        NNTPRetrieveManager nntpRetrieveManager(settings);
    
        SetSignalHandler(&nntpRetrieveManager);

        KeepStatistics(10000); // 10000 == Show every 1 second

        nntpRetrieveManager.WaitForCompletion();

        cout << endl;
        //    statisticsKeeper->Print(cout);
    
        EndStatistics();
        
        // Write the ini file back if we were not aborted
        if (nntpRetrieveManager.KeepRunning())
	    {
		    cout << "All downloads were completed successfully." << endl << flush;
            settings.WriteFile(iniFileName); 
	    }
	    else
	    {
		    cout << "Downloads were aborted by user." << endl << flush;
	    }
    }
    else
    {
        settings.WriteFile(iniFileName);
    }

    return EXIT_SUCCESS;
}
FUNCTION_END

//-------------------------------------------------------------------------
