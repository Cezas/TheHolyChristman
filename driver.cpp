// A quick driver to test the History Display routine
// It just fabricates a single job history to see what it does
//
#include <iostream>
using namespace std;


#include "scheduler.h"
#include "device.h"

//should only call shell process, which calls the user only once
int main()
{
	//max amount will be 10 stuffs
	const int maxProcs = 10;
	//use the 5 given policies
	Scheduler *scheds[] = 
	{ new FCFS(), new RoundRobin(), new Priority(), new Preempt(), new SRT() };
	//initialize tasks
	Process *tasks[maxProcs] = {NULL};
	//declare arrival
	int arrival[maxProcs];

	//uses each policy
		for (int i = 0; i < 5; i++)
	{
		//reset these
		tasks[0] =new Shell(0);
		arrival[0] = 0;   

		//only needs to call shell
		scheds[i]->runScheduler(tasks, arrival, 1);
		
		int filled = 0;//resets amount of processes

		//count how many processes there truly are
		for (int j = 0; j < maxProcs; j++)
		{
			if (tasks[j] != NULL)
				filled++;

			//cout << filled << endl;
		}
		
		//if size is too big, will be missing lots of important info
		displayHistory(tasks, filled, 0, 5000);
		//cout << tasks[0]->getLog() << endl;

		//empties tasks
		for (int k = 0; k < maxProcs; k++)
			tasks[k] = NULL;
	}
}
