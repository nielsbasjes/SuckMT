/***************************************************************************
                          Abortable.h  -  description                              
                             -------------------                                         
    begin                : Sun Aug 1 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/


#ifndef ABORTABLE_H
#define ABORTABLE_H

class Abortable 
{
public: 
	Abortable();
	virtual ~Abortable();
    
    void Abort();
    bool KeepRunning();
    virtual void AbortChildren();
    
private:
    bool keepRunning;
};

#endif
