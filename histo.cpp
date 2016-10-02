// History Displayer
// Produces an Ascii-graphic representation of the history of a process
// within a simulation of job scheduling.  The history is recorded as a
// series of time stamps, represented here as integer time values and
// characters representing state.
// Paramaters:
//     history	input procList array	linked lists of events and times
//     size	input int		number of elements in array
//     start	input int		beginning of time frame of interest
//     stop	input int		end of time frame of interest
// Pre-Conditions:
//     The elements in 'history' are in non-decreasing order
//     'history' ends with an entry with 'Q' as its state
//     There is at least one entry before the 'Q'
//     'start' and 'stop' are both positive integers, with start < stop
//     NOTE:  either 'start' or 'stop' may exceed the largest time in 'history'
// Results:
//     A display of between 20 and 50 printable characters representing
//     the job history within the time range of interest, using the
//     characters stored within 'history'.  Events outside the actual 
//     range of the process history will be displayed with blanks.

#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;

#include "process.h"
#include "proctree.h"

void displayHistory(Process *history[], int size, int start, int stop)
{
	char display[50];			// to hold the output
	int outpos;				// next output position to fill
	//int scan;				// next input data item to scan

	char  currentState;			// current process state in history
	int time;				// current time examined
	int range = stop - start;		// total time period
	int increment = 1 + range / 40;	// round fractions upwards

	int TotTurnTime = 0;  //average turnaround time
	int TotRespTime = 0;  //average response time
	int MaxRespTime = 0;  //max response time
	int NonIntProcs = 0;  //amount of non interactive processes
	int IntProcs = 0;     //amount of interactive processes
	int respAvg = 0;      //response average for extra cred and averagetime
	int deviation = 0;    //standard deviation
	int devitimes = 0;    //occurences of deviation
	//create proctree or linked list to store values
	ProcTree resptimes, devs;

	cout << "Time:  " << setw(4) << start <<
		setw(range / increment) << stop << endl;

	for (int j = 0; j<size; j++)
	{
		for (int i = 0; i<50; i++)		// clear display
			display[i] = ' ';

		//ProcList &pl = history[j]->getLog();	// find out about this task
		ProcIterator iter = history[j]->getLog().begin();	// starting at its first point

		//  Identify where to begin recording output data, and
		//  what time to begin analyzing (tracking may start early)
		//iter = pl.begin();
		if (iter.time() > start)		// has not yet started yet
			outpos = (iter.time() - start) / increment;	// skip spaces
		else
			outpos = 0;				// start at beginning of display
		time = start + outpos*increment;	// identify smulation time

		//  Identify where first applicable at appears
		//  (tracking may start in the middle)
		currentState = iter.state();	// initialize if loop never repeats
		while (time <= stop && time >= iter.time())
		{
			currentState = iter.state();	// 
			iter.advance();
		}
		//  currentState represents the process state at 'time'
		//  and changes from that state at iter.time()

		// collect relevant data into the output array
		// There should be no need to test for pl.end() -- it is here as a safeguard
		//    in case a program error prevents 'Q' from appearing in the list
		// It is NEVER a good habit to add code to one function to address or mask
		//    a bug in another function -- the correct thing to do is fix the bug!
		//    So, if you really want to know that your code elsewhere is correct,
		//    remove this test if you copy this file!      
		while (time <= stop && currentState != 'Q' && iter != history[j]->getLog().end())
		{
			// currentState extends to iter.time(), so record it until
			// that time is reached.

			while (time <= stop && time < iter.time())
			{
				display[outpos] = currentState;
				outpos++;
				time += increment;
			}
			currentState = iter.state();	// change to this state
			iter.advance();			// and find when it ends

			// It is possible that the last increment jumped over a state
			// so we will record any missing X's in last output position
			// It is also very possible that many states got missed,
			// so we will attempt to examine all of them for X's
			while (currentState != 'Q' && time >= iter.time())
			{
				if (currentState == 'X')	// missed this execution
					display[outpos - 1] = 'X';	// record very short CPU burst
				currentState = iter.state();	// what did we change to?
				iter.advance();			// find where that ends
			}

		}

		display[outpos] = '\0';
		cout << "History:  "/*<< of Proc("<<j<<"): "*/<< display << endl;
	}
	/*
	determine when the process ends
	determine whether the process was interactive or NAW
	add the time it took to run
	keep count of how many processes have finished
	*/

	//DETERMINE DIF BETWEEN INTERACTIVE AND NONINTERACTIVE
	//do this by looping thru the process to see if there is an i
	//if there isnt an i by the time you hit q, its noninteractive

	//create a new loop so as not to interfere with the original
	for (int i = 0; i < size; i++){
		//case check for interactiveness
		bool interactive=history[i]->isInteractive();
		
		//redeclare iterator
		ProcIterator iter = history[i]->getLog().begin();
		currentState = iter.state();

		//for testing purposes
		//cout << history[i]->getLog() << endl;

		//if it is interactive, find when I becomes no longer I
		//then, subtract the new time by the original I time
		if (interactive){
			char past = ' '; //previous letter
			int pastTime = 0; //time of previous letter
			bool firstpass= true; //handle first x

			while (currentState != 'Q' && iter != history[i]->getLog().end()){
				//if the previous character was an I, and the current is an X
				//take the time of the next I and subtract it from the I before the X
				//and add the X time with it
				//DOUBLE CHECK ARE INFACT -X- AND NOT X-

				past = currentState;
				//when it's x, don't change the time, keep it at the - before the X
				if (currentState!='X')
				pastTime = iter.time();

				iter.advance();
				currentState = iter.state();

				//if X has just been passed  MAKE SURE ITS NOT FIRST X
				if (past == 'X' && currentState!='Q'){
					if (!firstpass){
						//DO I FACTOR IN X TIME SOMEHOW
						TotRespTime += abs(iter.time() - pastTime);
						IntProcs++;
						//insert into tree or list
						resptimes.insert(abs(iter.time() - pastTime));
						devs.insert (abs(iter.time() - pastTime));
					}
					else
						firstpass = false;				
				}
			}
		}
		//for the non-interactive times, simply subtract the end time by the start time and add it upon the average
		//also increment the occurrences of each process end
		else{
			TotTurnTime += abs(history[i]->getLog().tailTime() - history[i]->getLog().leadTime());
			NonIntProcs++;
		}
	}

	//standard dev calculations
	//find the average response time
	respAvg = TotRespTime / IntProcs;
	//while devs still has values
	while (!devs.empty()){
		//remove the top and check
		int value = devs.removeMax();
		//if it deviates, do that math thingy
		if (value != respAvg){
			//math thingy
			deviation += abs(respAvg-value)*abs(respAvg-value);
			//increment occurences of deviations
			devitimes++;
		}
	}
	//cout << TotRespTime << "  " << IntProcs << endl;

	cout << "Average Turnaround Time is " << TotTurnTime / NonIntProcs << endl;

	//Average Response Time
	//measure from conclusion of each console interact to to beginning of next (between I's)

	cout << "Average Response Time is " <<respAvg << endl;

	//Maximum Response Time
	//find maximum response time of the interactive

	cout << "Max Response Time is " << resptimes.removeMax() << endl;

	//Standard Deviatation
	//Sqrt(Avg of (Each individual Deviation)^2) of the response times

	cout << "Standard Deviation is " << sqrt(deviation/devitimes) << endl << endl;	
}