/***************************************************************************
                          CommandProcessor.cpp  -  description                              
                             -------------------                                         
    begin                : Wed Jul 28 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif


#include "debugging.h"
#include "CommandHandler.h"

//-------------------------------------------------------------------------

FUNCTION_START(CommandHandler::CommandHandler(CommandQueue * commandQueue))
{
    myCommandQueue = commandQueue;
//    cout << "Starting Command Handler" << endl << flush;
    busy = false;
    currentCommand = NULL;
    start_undetached();
}
FUNCTION_END

//-------------------------------------------------------------------------

FUNCTION_START(CommandHandler::~CommandHandler())
{
    // Nothing to do
}
FUNCTION_END

//-------------------------------------------------------------------------

// Returns true if succes, false if failed
bool
FUNCTION_START(CommandHandler::AddCommand(Command * newCommand))
{
    if (newCommand == NULL)
        return false;

    myCommandQueue->AddCommand(newCommand);

    return true;
}
FUNCTION_END

//-------------------------------------------------------------------------

void* 
FUNCTION_START(CommandHandler::run_undetached(void* /*arg*/))
{
    busy = false;

    currentCommandMutex.lock();
    currentCommand = myCommandQueue->GetCommand();
    currentCommandMutex.unlock();

    while (KeepRunning())
    {
        if(currentCommand == NULL)
        {
            sleep(0, 100000); // 100000 nanoseconds = 0.1 seconds        
            currentCommandMutex.lock();
            currentCommand = myCommandQueue->GetCommand();
            currentCommandMutex.unlock();
        }
        else
        {
            busy = true;
            if(currentCommand->Execute(this) == false)
            {
                cout << "FATAL ERROR !!!!!!" << endl << flush;
                busy = false;
                return NULL;
            }
            
            currentCommandMutex.lock();
            delete (currentCommand);
            if (KeepRunning())
                currentCommand = myCommandQueue->GetCommand();
            else
                currentCommand = NULL;
            currentCommandMutex.unlock();
            busy = false;
        }
    }

	currentCommandMutex.lock();
    if (currentCommand != NULL)    
        delete (currentCommand);
	currentCommandMutex.unlock();
    
    return NULL;
}
FUNCTION_END

//-------------------------------------------------------------------------

bool 
FUNCTION_START(CommandHandler::IsBusy())
{
    return busy;
}
FUNCTION_END

//-------------------------------------------------------------------------

void 
FUNCTION_START(CommandHandler::AbortChildren())
{
    omni_mutex_lock lock(currentCommandMutex);
    if (currentCommand != NULL)
        currentCommand->Abort();
}
FUNCTION_END
