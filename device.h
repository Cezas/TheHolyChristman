// Device Driver Simulation
// A simplified version of the behavior of a device driver.
// It is notified of when a request arrives, and then it
// determines when that action begins and ends.

#include "proclist.h"
class Process;

//IMPORTANT: each time request is called for either class, update the ID #
class Device
{
	//changed private to protected and voids to virtual void for Console class
protected:
	ProcList requests;
	char action;		// letter used for display
	int duration;		// how long the operation might take
	int readyTime;		// when next operation may start

public:
	Device(char a, int d) : action(a), duration(d), readyTime(0){}
	 virtual void restart()
	{
		readyTime = 0;
	}
	 virtual void request(int, int, Process*[], ProcList &);
	 //for overriding
	 virtual void removeBank(int){};
};


//this should be able to do input/output as well as actually inserting the process
class Console :public Device{
	//when this is called
private:
	//int nextTime; //modified with readytime to determine when a process should be added
	int procBank[10];  //each index will hold that type of process, 0 will be comps, 1 will be downs
	//2 will be interacts, by briefly checking the contents of each index, the total amount of processes can be determined
	int currID;
	int arriveTime;
public:
	//console constructor
	Console(char a, int d) :Device(a, d){
		//nextTime = 0;
		for (int i = 0; i < sizeof(procBank) / sizeof(int); i++)
			procBank[i] = 0;
		currID = 0;
		arriveTime = 0;
	};

	//void calcTime(int&); //updates nextTime and returns it, takes clock as parameter
	int howManyProcs(int); //returns how many of that process, takes type as parameter
	void addBank(int, int);  //throws the process into numProcs for organization, parameters of type and id
	void removeBank(int);  //remove proc from bank when it finishes, parameters of id

	//will need to look thru numProcs to determine what processes there are
	int nextProcess();  //decides what process to run next in int forme


	//creates a new process and puts it into the scheduler
	//the user gets to decide what type of process should be created
	//NOTE:also need to figure out if the clock is necessary for proper creation/insertion
	void create(int, int, Process*[]);
	//overrides the device request
	void request(int, int, Process*[], ProcList &);
	void restart();
};

extern Device disk, net, cpu, console;
extern Console user;