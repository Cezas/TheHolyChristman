#include "device.h"
#include "process.h"

//  Simulate a request to an I/O device
//  Parameters:
//      pid	(input int)		which process is making the request
//      clock	(input int)		time at which request is made
//      tasks	(modified array)	Process information (to record events)
//      future	(modified ProcList)	record when operation is complete
void Device::request(int pid, int clock, Process *tasks[], ProcList &future)
{	
	
	//if it is the cpu, place it back into the future list
	if (action == 'X'){
		future.insert(pid, clock, 'X');
		return;
	}

	if (readyTime <= clock)
		readyTime = clock;		// start acting now

	//readyTime += 1000;

	tasks[pid]->addLog(readyTime, action);
	readyTime+=duration;
	tasks[pid]->addLog(readyTime, '-');
	future.insert(pid, readyTime, action);	// run when done

}

//NEEDS TO COMPENSATE FOR ARRIVAL SOMEHOW
void Console::request(int pid, int clock, Process *tasks[], ProcList &future){
	//calling original request doesnt seem to work, have to manually implement regular request code
	//Device::request(pid, clock, tasks, future);

	if (readyTime <= clock)
		readyTime = clock;		// start acting now

	tasks[pid]->addLog(readyTime, action);
	readyTime += duration;
	//change the "arrival" to be 500 seconds after the start of the previous process
	//calcTime(arriveTime);
	//readyTime += arriveTime;

	//will return the amount of nonNulls
	currID = 0;

	while (tasks[currID] != NULL)
		currID++;

	//create the process and insert
	//if (nextProcess() != -1){
		create(currID, nextProcess(), tasks);
		//tasks[currID]->addLog(readyTime, '-');
		future.insert(currID, readyTime, action);
	//}
	tasks[pid]->addLog(readyTime + 5000, '-');
	future.insert(pid, readyTime - 2500, 'X');

}

void Console::create(int newID, int procType, Process *tasks[]){
	//first, find out what type of process it should be
	//then, create that new process
	//make sure that process is added to tasks[] with the correct pid
	//then the device request can be called to insert it into the scheduler
	//each time a proc is created, throw into the bank

	//IMPORTANT: also need to add in proper arrival time
		//can be done by modifying readytime+whatTime
	//after device is called, increment the ID
	//currID++;

	int choose = rand() % 2;

	//future insert is done outside after this
	if (procType == 0){

		if (choose==0)
			tasks[newID] = new Computation(newID);
		if (choose==1)
			tasks[newID] = new Symbolab(newID);

		//tasks[newID]->addLog(readyTime, '-');
	}
	if (procType == 1){
		if (choose==0)
			tasks[newID]= new Download(newID);
		if (choose == 1)
			tasks[newID] = new Video(newID);
		//tasks[newID]->addLog(readyTime, '-');
	}
	if (procType == 2){
		if (choose==0)
			tasks[newID] = new Interact(newID);
		if (choose == 1)
			tasks[newID] = new Video(newID); 
		//tasks[newID]->addLog(readyTime, '-');
	}
	//takes 100-400 time for a user to create a new process
	arriveTime += rand() % 300+100;
	tasks[newID]->addLog(readyTime+arriveTime, '-');
	//adds that process type to the bank
	addBank(procType, newID);

	//cout << newID << endl;
}


int Console::howManyProcs(int procType){
	int count = 0;
	for (int i = 0; i < sizeof(procBank) / sizeof(int); i++)
		if (procBank[i] == procType)
			count++;

	return count;
}

//WHEN A PROCESS FINISHED, REMOVE FROM BANK
void Console::addBank(int input, int ID){
	//throws the process into the bank to its index/type relation
	procBank[ID] = input;
}

void Console::removeBank(int ID){
	//when a process is finished, it is removed from the bank
	//cant use 0 as that is reserved for computation
	procBank[ID] = -1;
}

int Console::nextProcess(){
	//decides whether it should make a next process, and then returns what type of process
	//will return -1 if no more
	//see what processes are already there
	int comp, down, inter, input, modulo, split;
	comp = howManyProcs(0);
	down = howManyProcs(1);
	inter = howManyProcs(2);
	//for random gen
	modulo = 3;

	//only one interative should be active, so take that out of the random gen
	if (inter > 1)
		modulo--;

	//random gen
	input = rand() % modulo;

	//try to limit to 3 downloads at a time
	if (down > 3 && modulo == 1){
		//decide between using comp or inter
		split = rand() % 2;
		if (split == 0)
			input = 0;

		if (split == 1)
			input = 2;
		//if inter is chosen but violates the active clause, switch to comp
		if (inter >= 1 && input == 2)
			input = 0;
	}

		
	return input;
}

void Console::restart(){
	readyTime = 0;
	currID = 0;
	arriveTime = 0;
}

//  Each device records a letter to be used in the summary display
//  and the time the operation is expected to take
Device disk('D', 2000), 	// disk has 'slow' moving parts
net('N', 1000), 	// networks are faster nowadays// have to wait for user to respond
cpu('X', 500),
console('I', 1000);
//console object
Console user('U',20);
// not external, but used for type compatibility
//  When each process runs with the CPU, it will use this list to identify
//  what it wishes to do next (using the cpu object to continue running)
