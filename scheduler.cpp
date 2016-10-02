#include "scheduler.h"
#include "device.h"


//should notice and update when a new process has been added
void Scheduler::runScheduler(Process *tasks[], int arrival[], int size)
{
	int pid;			// process wanting action
	int newpid;			// newly arriving process
	Device *nextAct;		// and the action it wants
	char discard;		// discard unwanted return from popFront

	cout << name << ":" << endl;	// what are we scheduling now?
	for (int i = 0; i < size; i++)
	{
		future.insert(i, arrival[i], 'X');	// all want to run
		tasks[i]->restart();			// and start at beginning
		tasks[i]->addLog(arrival[i], '-');	// might have to wait
	}
	disk.restart();		// make all these start over again
	net.restart();
	console.restart();
	user.restart();

	clock = 0;		// initialize the clock

	while (!future.empty() || !noneReady())
	{
		if (noneReady())		// nothing avaliable now
		{				//    but something in the future
			clock = future.leadTime();		// go do that future
			future.popFront(pid, discard);	// get that task
			addProcess(pid);			// which is ready to run
		}
		else 				// else run with what we have
		{
			chooseProcess(pid);		// run this one
			tasks[pid]->run(clock, allowance(), nextAct);

			//  some tasks may have arrived in the meantime, so get those
			while (!future.empty() && clock >= future.leadTime())
			{
				// al we want here is process id's; discard the other value
				future.popFront(newpid, discard);
				addProcess(newpid);
			}

			//  as far as the task that was just executing, it can
			//  work some more, after waiting its turn
			//  (which is why this appears after that last while loop)
			if (nextAct == &cpu)
				addProcess(pid);
			else if (nextAct != NULL)
				nextAct->request(pid, clock, tasks, future);
		}
	}
}


void SRT::addProcess(int ID){
	//The heap should contain procID's, related to the processes with least remtime to most
	//Add each process to the heap via its procID(ID's should be unique #s)
	//Begin by setting the modifiable position to the nextEmpty cell
	//as long as the heap does not run out AND the parent process has more remtime
	//swap the parent and new value, with new value in parent position
	//set the available position of heap to the ID
	//find a nextEmpty space
	pos = nextEmpty; 
	while ((pos > 1) && procs[heap[pos / 2]]->getRemTime() > procs[ID]->getRemTime()){
		heap[pos] = heap[pos / 2];
		pos = pos / 2;
	}
	heap[pos]= ID;
	nextEmpty++;
}

void SRT::chooseProcess(int& procID){//remove the ProcID of minimum remTime
	//save root val
	//switch the values from the last node(rightmost) and the root
	//reorganize the tree (while the swapped node's time is greater than both children, swap with lesser child value and move it downward)
	procID = heap[1];
	bool done = false; //loop condition
	int child = 2;  //left child of root
	pos = 1;
	nextEmpty--;  //gonna get rid of that last node
	heap[pos] = heap[nextEmpty];
	while (child < nextEmpty && !done){  //stops when at bottom or done
		//if right child is smol
		if (child + 1 < nextEmpty && procs[heap[child]]->getRemTime() > procs[heap[child+1]]->getRemTime())
			child++;  //move down this way
		//if parent is smol, we done
		if (procs[heap[child]]->getRemTime() > procs[heap[pos]]->getRemTime())
			done = true;
		//otherwise, keep swapping and moving
		else{
			swap(heap[child], heap[pos]);
			pos = child;
			child = 2 * pos;
		}
	}

}




