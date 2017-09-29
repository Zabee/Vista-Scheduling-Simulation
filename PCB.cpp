#include "NTThread.cpp"
#include<iostream>
#include<time.h>
#define MAX_P 10		//Number of Random Processes / Threads to be created.

using namespace std;

int poi=0;
int time_slice=3;		//5 miliseconds is the time slice.
int rtq_cnt=0, dq_cnt =0;
int i, j, k, t, promoted=0;
QUEUE rq[2], ioq;	//rq[0] is Round Robin (REALTIME Process) Queue  and rq[1] is FIFO (Dynamic Process) Queue.
void RRFIFO();
int aa=0;
class PCB : public PROCESS
{

	public:	double s, e, dur, time_in_exec;
		void run();
};
PCB p[MAX_P];			//Process pool or thread pool has been created.

void display();		//Forward Referencing.
void addToQueue(PCB *, int, QUEUE *);

void execute(PCB *prs, int x)
{
	double z;
		if(prs->io == 1)
		{
			cout<<"\t\t      PROCESSOR\n";
			if(prs->brk_pnt >= time_slice)
			{
				z = prs->brk_pnt;
				z -= time_slice;
				cout<<"\t\t ___________________\n";
				cout<<"\t\t|                   |\n";
				cout<<"\t\t|                   |   Burst Time           : "<<prs->brk_pnt<<endl;
				cout<<"\t\t|	  P"<<prs->pid<<"\t    |\n";
				cout<<"\t\t|                   |   Remaining Burst Time : "<<z<<endl;
				cout<<"\t\t|                   |\n";
				cout<<"\t\t|___________________|   Priority             : "<<prs->priority<<"\n\n\n";
				sleep(time_slice);
				prs->burst_time -= time_slice;
				prs->brk_pnt -= time_slice;
			}
			else
			{
				z = prs->brk_pnt;
				z -= prs->brk_pnt;
				cout<<"\t\t ___________________\n";
				cout<<"\t\t|                   |\n";
				cout<<"\t\t|                   |   Burst Time           : "<<prs->brk_pnt<<endl;
				cout<<"\t\t|	  P"<<prs->pid<<"\t    |\n";
				cout<<"\t\t|                   |   Remaining Burst Time : "<<z<<endl;
				cout<<"\t\t|                   |\n";
				cout<<"\t\t|___________________|   Priority             : "<<prs->priority<<"\n\n\n";
				sleep(prs->brk_pnt);
				prs->burst_time -= prs->brk_pnt;
				prs->brk_pnt -= prs->brk_pnt;
				rq[1].Q[x] = -1;
				int u; 
				cin>>u;
				prs->priority += 10;
				//promoted = 1;
				if(prs->priority >31)
					prs->priority = 31;
				if(prs->priority <=15)
					prs->priority = 16;
				dq_cnt--;
				prs->io = 0;			// I/O Serviced.
				addToQueue(prs, prs->pid, rq);
					RRFIFO();
			}//End I/O if i.e. io == 1.
			int go;
			cin>>go;				
		}
	else	//If no I/O is needed to complete its execution then.
		{
			cout<<"\t\t      PROCESSOR\n";
			if( time_slice <=prs->burst_time)
			{	
				z = prs->burst_time;
				z -= time_slice;
				cout<<"\t\t ___________________\n";
				cout<<"\t\t|                   |\n";
				cout<<"\t\t|                   |   Burst Time           : "<<prs->burst_time<<endl;
				cout<<"\t\t|	  P"<<prs->pid<<"\t    |\n";
				cout<<"\t\t|                   |   Remaining Burst Time : "<<z<<endl;
				cout<<"\t\t|                   |\n";
				cout<<"\t\t|___________________|   Priority             : "<<prs->priority<<"\n\n\n";
				sleep(time_slice);
				prs->burst_time -= time_slice;
			}
			else
			{
				z = prs->burst_time;
				z -= prs->burst_time;
				cout<<"\t\t ___________________\n";
				cout<<"\t\t|                   |\n";
				cout<<"\t\t|                   |   Burst Time           : "<<prs->burst_time<<endl;
				cout<<"\t\t|	  P"<<prs->pid<<"\t    |\n";
				cout<<"\t\t|                   |   Remaining Burst Time : "<<z<<endl;
				cout<<"\t\t|                   |\n";
				cout<<"\t\t|___________________|   Priority             : "<<prs->priority<<"\n\n\n";
				cout.flush();
				sleep(prs->burst_time);
				prs->burst_time -= prs->burst_time;
			}
		}
}

void PCB :: run ()		//Each PROCESS.
{
		cout<<"\t\tProcess with PID: "<<pid<<" is under execution\n\n";
		bool x;
		double z;
		while(x && burst_time>0)
		{
			e = clock();
			z = (e-s)/CLOCKS_PER_SEC/1000;
			if(z>=time_slice)
			{
				burst_time -= time_slice;
				usleep((MAX_P-1)*500);
			}
		}
}

void addToQueue(PCB *p, QUEUE *q, int n)	//Array of Processes and Array of QUEUES. 'n' maximum processes.
{
	for(int i=0; i<n; i++)
	{
		if(p[i].priority >15)
		{
			q[0].addToReadyQueue(p[i].pid);
			p[i].q_index = 0;
			rtq_cnt ++;
		}	
		else if(p[i].priority <=15 && p[i].priority>=2) //base prirority is 2.
		{
			q[1].addToReadyQueue(p[i].pid);
			p[i].q_index = 1;

			dq_cnt++;
		}
		
	}
}

	//A process and Array of QUEUES.
void addToQueue(PCB *prs, int index, QUEUE *q)	
{
	if(prs->priority >15)
	{
		q[0].addToReadyQueue(prs->pid);
		prs->q_index = 0;
		rtq_cnt++;
		cout<<"Real Time process COUNT :"<<rtq_cnt<<endl;
	}
	else if(prs->priority <=15 && prs->priority>=2) 
//base prirority is 2.
	{
			q[1].addToReadyQueue(prs->pid);
			prs->q_index = 1;
			dq_cnt++;
			cout<<"Dynamic  process COUNT :"<<dq_cnt<<endl;
	}
}

void RRFIFO()
{
	int zr=0; bool br=true, chk=0;
	while(br)
	{
		for(i=0; i<rq[0].size; i++)		
//rtq_cnt number of Real Time Processes.
		{
			if(rq[0].Q[i] != -1 && p[rq[0].Q[i]].burst_time>0)
			{
			   cout<<"Real Time burst time :"<<p[rq[0].Q[i]].burst_time<<endl;
			   execute(&p[rq[0].Q[i]], i);	//Service Each Processes in round robin fashion by taking them from reayQueue 0.	   
			   system("clear");
			   chk=1;
			}   
		}
		zr=0;
		for(i=0; i<rq[0].size; i++)
		{
			if(rq[0].Q[i] != -1)
			{
			   if(p[rq[0].Q[i]].burst_time==0 )
			   {
			   	//cout<<"Checking Burst time: "<<p[rq[0].Q[i]].burst_time<<endl;
				zr++;
				if(zr>=(rtq_cnt))			
//If all processes / threads completed their execution then break the loop.
				{
					br=false;
					break;
				}
		           }		
			}
			else
				break;
		}		
		if(chk == 0)
		break;
	}
	rtq_cnt = 0;
	//Reset the entire RR queue.
	rq[0].init();	
	int two;		
	cout<<"RT Process Queue Serviced\nDynamic Processes:"<<dq_cnt<<endl;	
	zr=0; br=true; chk=0;
	while(br)
	{
		for(i=0; i<rq[1].size; i++)		
//rtq_cnt number of Real Time Processes.
		{
			if(rq[1].Q[i] != -1 )
			{    
			   if(p[rq[1].Q[i]].burst_time>0)       
			   { 
			      cout<<"Dynamic burst time :"<<p[rq[1].Q[i]].burst_time<<endl;
			      execute(&p[rq[1].Q[i]], i);	//Service Each Processes in round robin fashion by taking them from reayQueue 0.   
      			      system("clear");
			      chk = 1;
			   }	
			}   
		}
		zr=0;
		for(i=0; i<rq[1].size; i++)
		{
			if(rq[1].Q[i] != -1)
			{
			   if(p[rq[1].Q[i]].burst_time==0 )
			   {
				zr++;
				if(zr>=(dq_cnt))			
//If all processes / threads completed their execution then break the loop.
				{
					br=false;
					break;
				}
		           }		
			}
			else
				break;
		}
		if(chk == 0)
			break;
	}
}

void setup()
{
	cout<<"\n\nAll are in READY\n";
	cout<<"\t**********************************************************\n";
	cout<<"\tThread Type\t\tPriority\tPID\tBurst Time\n";
	cout<<"\t**********************************************************\n";
	for(i=0; i<MAX_P; i++)
	{
		if(p[i].real == true)
		{
			cout<<"\t\b\b* Real time thread\t   "<<p[i].priority<<"\t\t  "<<p[i].pid<<"\t   "<<p[i].burst_time<<endl;
		}
		else
			cout<<"\tDynamic thread  \t   "<<p[i].priority<<"\t\t  "<<p[i].pid<<"\t   "<<p[i].burst_time<<endl;
	}
	cout<<"\t**********************************************************\n\n";	

	cout<<"Before Sorting both the QUEUES\n";
	cout<<"\tRound Robin READY QUEUE for Real Time Threads\n";
	cout<<"\t**********************************************************\n";
	cout<<"\tThread Type\t\tPriority\tPID\tBurst Time\n";
	cout<<"\t**********************************************************\n";
	for(i=0; i<(rq[0].size); i++)
		if(rq[0].Q[i] != -1)
		cout<<"\tReal Time  \t\t   "<<p[rq[0].Q[i]].priority<<"\t\t  "<<p[rq[0].Q[i]].pid<<"\t   "<<p[rq[0].Q[i]].burst_time<<endl;
	cout<<"\t**********************************************************\n\n";

	cout<<"\n\tFIFO READY QUEUE for Dynamic Threads\n";
	cout<<"\t**********************************************************\n";
	cout<<"\tThread Type\t\tPriority\tPID\tBurst Time\n";
	cout<<"\t**********************************************************\n";
	for(i=0; i<(rq[1].size); i++)
		if(rq[1].Q[i] != -1)
		cout<<"\tDynamic    \t\t   "<<p[rq[1].Q[i]].priority<<"\t\t  "<<p[rq[1].Q[i]].pid<<"\t   "<<p[rq[1].Q[i]].burst_time<<endl;
	cout<<"\t**********************************************************\n\n";


	for(int z=0; z<2; z++)		//Sorting the both queues on their priority. 
	{	
		for(i=0; i<rq[z].size-1; i++)
			for(j=0; j<rq[z].size-i-1; j++)
			{
				if(p[rq[z].Q[j]].priority < p[rq[z].Q[j+1]].priority && (rq[z].Q[j] != -1 && rq[z].Q[j+1] != -1  ))
				{
					t = rq[z].Q[j];
					rq[z].Q[j] = rq[z].Q[j+1];
					rq[z].Q[j+1] = t;
				}
			}
	}
	cout<<"After Sorting both the QUEUES\n";	
	display();
}


void display()
{	
	cout<<"\tRound Robin READY QUEUE for Real Time Threads\n";
	cout<<"\t**********************************************************\n";
	cout<<"\tThread Type\t\tPriority\tPID\tBurst Time\n";
	cout<<"\t**********************************************************\n";
	for(i=0; i<(rq[0].size); i++)
		if(rq[0].Q[i] != -1)
		cout<<"\tReal Time  \t\t   "<<p[rq[0].Q[i]].priority<<"\t\t  "<<p[rq[0].Q[i]].pid<<"\t   "<<p[rq[0].Q[i]].burst_time<<endl;
	cout<<"\t**********************************************************\n\n";
	if(promoted == 0)
	{
		cout<<"\n\tFIFO READY QUEUE for Dynamic Threads\n";
		cout<<"\t**********************************************************\n";
		cout<<"\tThread Type\t\tPriority\tPID\tBurst Time\n";
		cout<<"\t**********************************************************\n";
		for(i=0; i<(rq[1].size); i++)
			if(rq[1].Q[i] != -1)
			{
				cout<<"\tDynamic    \t\t   "<<p[rq[1].Q[i]].priority<<"\t\t  "<<p[rq[1].Q[i]].pid<<"\t   "<<p[rq[1].Q[i]].burst_time;
				if(p[rq[1].Q[i]].io == 1)
					cout<<"--------->Needs I/O\n";
				else
					cout<<endl;
			}
	cout<<"\t**********************************************************\n\n";
	}
}
int main()
{					
//Generates random number of processes with their random attributes. 
	for(i=0; i<MAX_P; i++)	
	//'i' is the pid for each process and index in queue initially.
		p[i].init(i);		
//Here each processes is a THREAD runs concurrently (which is considered as in Ready Queue) 
	for(i=0; i<2; i++)
		rq[i].init();		
//Ready 'Ready Queue'. Size of each queue 30 processes / theads. Size is specified in "readyQueue.cpp" file.
	ioq.init();			//Ready 'I/O queue'.		
	addToQueue(p, rq, MAX_P);	
//Adding each process to appropriate queue (such as RR and FIFO).
	setup();			
//Make ready all the process sort them and display them in order.
	int z;				
cout<<"Ready any value followed by enter key:(5 zoom in):";
//It starts sleeping when it has been scheduled. (For or as my Convinience).
	cin>>z;				//Service the Real Time Processes 
//in Round Robin Fashion.  
system("clear");		//Service the Dynamic Process in Round 
//Robin Fashion except, promoting and demoting them
	RRFIFO();
	cout<<"All are serviced (only for the demostrate)\n";	
// by increasing and decreasing their prirority.
rq[0].init();			//At the begining of time quantum each process diplays a message that it has been scheduled (meantime it will be sleeping).
	rq[1].init();			//At the end of time quantum each 
	promoted=0;			process dispalys a message that it 
has been taken out in both RR and FIFO.
//It displays the Burst time of individual process with the remaining burst time. (Execute and realize these things).
	display();			
	return 0;
}
