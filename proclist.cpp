#include <iostream>
using namespace std;

// List of Process ProcList Information
// This is a simple linked list, all of whose elements record
// information about a process in a simulated process scheduler.
// This list data structure will be used for three different purposes:
// -- a record of the past history of a particular process
// -- a collection for the job scheduler of processes currently wishing to run
// -- a task list for known future events for the simulation

#include "proclist.h"

// First, some helper functions to display the data

ostream &operator<<(ostream &stream, ProcListElement &ele)
{
	stream << "(" << ele.procID << "," << ele.state << ","
		<< ele.time << ")";
	return stream;
}

ostream &operator<<(ostream &stream, ProcList &hist)
{
	for (ProcListElement *curr = hist.head; curr != NULL; curr = curr->next)
		stream << *curr;
	stream << endl;
	return stream;
}

// And some functionality for the list itself.
// First some easy iterator definitions:
ProcIterator ProcList::begin()
{
	return ProcIterator(this, head);
}

ProcIterator ProcList::end()
{
	return ProcIterator(this, NULL);
}

//  push a new element to the back end of the list
//  Primarily used for First-In, First-Out ordering
void ProcList::pushBack(int procId, int time, char state)
{
	ProcListElement *newEle = new ProcListElement(procId, time, state);
	if (head == NULL)
		head = tail = newEle;
	else
	{
		tail = tail->next = newEle;
	}
}

void ProcList::popFront(int &procId, char &state)
{
	ProcListElement *removal = head;
	head = head->next;
	if (head == NULL)
		tail = NULL;

	procId = removal->procID;
	state = removal->state;
	delete removal;
}

void ProcList::insert(int procId, int time, char state)
{
	ProcListElement *newEle = new ProcListElement(procId, time, state);
	if (head == NULL)
		head = tail = newEle;
	else if (time < head->time)
	{
		newEle->next = head;
		head = newEle;
	}
	else if (time >= tail->time)
	{
		tail = tail->next = newEle;
	}
	else
	{
		ProcListElement *scan;
		for (scan = head; scan->next->time < time; scan = scan->next) {}
		newEle->next = scan->next;
		scan->next = newEle;
	}
}

// EXTRA CREDIT
// Condenses the information in the linked list after it is collected.
// 1) If two adjacent cells have the same time information,
//    the first can be removed (did not actually occupy that state)
// 2) If two adjacent cells have the same state recorded,
//    the second can be removed (did not actually change state)
void ProcList::condense()
{

	ProcListElement *checktemp, *movetemp;
	checktemp = head;
	movetemp = checktemp;

	//create a break statement for while
	while (movetemp != NULL){
		//create links before breaking
		/*if (checktemp->next->time == checktemp->next->next->time){
		checktemp->next = movetemp->next;
		if (movetemp->time == movetemp->next->time){
		popFront(movetemp->procID, movetemp->state);
		}
		}*/

		if (movetemp->state == movetemp->state){
			popFront(movetemp->next->procID, movetemp->next->state);
		}
		movetemp = movetemp->next;
		checktemp = movetemp;
	}


}