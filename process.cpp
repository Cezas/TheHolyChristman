#include "process.h"
#include "device.h"

//  Run a Process for some time
//  The process would like to complete its current CPU burst,
//  but is currently only allowed what is granted to it.
//  Parameters:
//      clock           (modified int)  time on simulation clock
//      allowance       (input int)     time allowed to run with
//      next            (output Device) what device should be used next
//		next is from { disk, net, console, cpu )
//  Post-Condition:
//      the clock will have advanced until either the allowed time
//      has been used up, or the current CPU burst is complete
//      (whichever happens earlier).  There will be no idle CPU time.
//  Also:  The history log for this Process will record what is known
//      at this time

void Process::run(int &clock, int allowance, Device *&next)
{
	addLog(clock, 'X');		// running now

	//when the burst is to be completed
	if (allowance >= remainingTime)
	{
		clock += remainingTime;		// use all the time needed

		//change to next device
		next = nextRequest[currentCycle];

		//if its null, it has ended
		if (next == NULL){
			//remove this process from the procbank when completed
			//if (next->action)
			//nextRequest[currentCycle]->removeBank(getID());

			addLog(clock, 'Q');		// record completion
		}
		else{
			//new cycle
			currentCycle++;
			//new time for new cycle
			remainingTime = usages[currentCycle];
			//waiting
			addLog(clock, '-');
		}

	}
	else
	{
		//make sure it runs X
		next = &cpu;
		clock += allowance;		// uses the time that was given
		remainingTime -= allowance;	// and save some work for later
		addLog(clock, '-');		// wait to execute again
	}

}




Computation::Computation(int id)
{
	myId = id;
	bursts = 4 + rand() % 3;	// several lengthy CPU bursts
	for (int i = 0; i < bursts; i++)
	{
		usages[i] = 200 + rand() % 120;
		nextRequest[i] = &disk;	// some disk activity
	}
	nextRequest[bursts - 1] = NULL;	// all done!
}

Download::Download(int id)
{
	myId = id;
	bursts = 9;		// 4 chances to move data, then wrap up
	for (int i = 0; i < bursts; i++)
	{
		usages[i] = 40 + rand() % 20;   // not much CPU needed
		if (i % 2 == 0)
			nextRequest[i] = &net;	// alternate network
		else
			nextRequest[i] = &disk;	// and disk
	}
	nextRequest[bursts - 1] = NULL;	// all done!
}

Interact::Interact(int id)
{
	myId = id;
	bursts = 6;		// enough to simulate till others are all done
	for (int i = 0; i < bursts; i++)
	{
		usages[i] = 30 + rand() % 20;
		nextRequest[i] = &console;	// work with console
	}
	nextRequest[bursts - 1] = NULL;	// all done!
}

Shell::Shell(int id){
	myId = id;
	//index 0 for the call, index 1 for the end
	bursts = 6;
	for (int i = 0; i < bursts; i++)
	{
		usages[i] = 30 + rand() % 20; //this simulates the activity of the os
		nextRequest[i] = &user;   
	}
	nextRequest[bursts - 1] = NULL;    // all done!
}

//other stuffs
Video::Video(int id)
{
	myId = id;
	bursts = 7;		// 4 chances to move data, then wrap up
	for (int i = 0; i < bursts; i++)
	{
		usages[i] = 100 + rand() % 20;   // not much CPU needed
		if (i % 2 == 0)
			nextRequest[i] = &net;	// alternate network
		else
			nextRequest[i] = &disk;	// and disk
	}
	nextRequest[bursts - 1] = NULL;	// all done!
}

Game::Game(int id)
{
	myId = id;
	bursts = 15;		// 4 chances to move data, then wrap up
	for (int i = 0; i < bursts; i++)
	{
		usages[i] = 300 + rand() % 200;   // needs a LOT of cpu
		nextRequest[i] = &console;	// work with console
	}
	nextRequest[bursts - 1] = NULL;	// all done!
}

Symbolab::Symbolab(int id)
{
	myId = id;
	bursts = 2 + rand() % 4;	// several lengthy CPU bursts
	for (int i = 0; i < bursts; i++)
	{
		usages[i] = 300 + rand() % 150;
		nextRequest[i] = &disk;	// some disk activity
	}
	nextRequest[bursts - 1] = NULL;	// all done!
}