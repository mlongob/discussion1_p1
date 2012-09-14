#include <iostream>
#include <cstdlib>
#include <cassert>
#include <ctime>
#include <string>
#include "thread.h"

using namespace std;

#define DISPLAY1() do { \
	cout << "[TH " << thread_num << "; count = " << \
		count << "] A = LOAD[tally] = " << a << endl; } while(false)
		
#define DISPLAY2() do { \
	cout << "[TH " << thread_num << "; count = " << \
		count << "] A = A + 1 = " << a << endl; } while(false)
		
#define DISPLAY3() do { \
	cout << "[TH " << thread_num << "; count = " << \
		count << "] STORE(A => tally) = " << tally << endl; } while(false)

inline void ask_yield()
{
	string input;
	do
	{
		cout << "Yield now? [y/n] ";
		getline(cin,input);
	} while(input != "y" && input != "n" && input != "");
	if(input == "y")
	{
		thread_yield();
	}
}

/* Global variables */
int n = 20;
volatile int tally = 0;

/*
 * Thread start function
 */
void total(void *arg)
{
	int thread_num = (int)arg;
	int count;  /* local variable */
	for(count = 0; count < n; count++)
	{
		register int a;
		a = tally;			/* Display this on the console => */ DISPLAY1();
		ask_yield();
		a = a + 1;			/* Display this on the console => */ DISPLAY2();
		tally = a;			/* Display this on the console => */ DISPLAY3();
		ask_yield();
	}
}

/*
 * Main function
 */
void parent(void *arg)
{
	tally = 0; /* initializing a global variable */
	
	/*
	 * First, we create a thread, thread1; it starts executing from the 
	 * function "total", defined above.
	 */
	int thread1 = thread_create((thread_startfunc_t) total, (void*)0);
	assert(thread1 >= 0);
	
	/*
	 * The call to new Thread returns immediately and we create another thread.
	 * thread1 starts executing from the function "total", defined above.
	 */
	int thread2 = thread_create((thread_startfunc_t) total, (void*)1);
	assert(thread2 >= 0);
	
	/*
	 * Wait until both Threads are done executing.
	 */
	int ret = thread_join(thread1);
	assert(ret == 0);
	ret = thread_join(thread2);
	assert(ret == 0);
	
	cout << "tally = " << tally << endl;
}

int main()
{
	const int cpus = 1;
	const bool sync = false;
	const bool async = false;
	const int seed = time(NULL);
	
	boot_cpus(cpus, (thread_startfunc_t) parent, NULL, sync, async, seed);
	return 0;
}
