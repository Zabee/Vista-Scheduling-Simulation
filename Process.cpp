#include "readyQueue.cpp"
#include<iostream>
#include<pthread.h>
#include<stdlib.h>

using namespace std;

void * runner(void *);

class PROCESS
{
	pthread_t tid;
	pthread_attr_t attr;
	public:		QUEUE *q;
			int q_index;		//q_index is an index where(in which ready queue 0 or 1) the process resides.
			int burst_time;
			int pid; 		//Using pid we can access particular thread or process as index from queue.
			unsigned int priority;		//Hihger in the number higher in the priority.
			bool real;
			int io;
			int brk_pnt;
		void init(int);		//index for 'INDEX'
		void start()
		{
			pthread_create(&tid, &attr, runner, (void *) (this));
		}
		virtual void run()
		{
		};
		void join()
		{
			pthread_join(tid, NULL);
		}
		void sleep(double s)
		{
			usleep(s);
		}
};
void PROCESS :: init(int i)
{
		pthread_attr_init(&attr);		//Get the default attributes.
		brk_pnt = 0;
		io = 0;
		real = false;
		pid = i;	
		burst_time = rand()%20;
		priority =  rand()%31;		//Assigning the PRIORITY.
		if(priority<2)
			priority = 2;
		if(priority > 15)
			real = true;
		else
		{
			real = false;
			if(rand() % 3 ==0)
			{
				brk_pnt = rand() % burst_time;
				io = 1;
			}
		}
}

void * runner(void *t)
{
	PROCESS *thr = (PROCESS *)t;
	thr->run();
}

void initiate(PROCESS *a, int n)		//Sorts the threads.
{
	int i, j;
	PROCESS t;
	for(i=0; i<n-1; i++)
		for(j=0; j<n-i-1; j++)
		{
			if(a[j].priority < a[j+1].priority)
			{
				     t = a[j];
				  a[j] = a[j+1];
				a[j+1] = t;
			}
		}
}