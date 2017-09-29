/*Ready Queue and Blocked Queue, which are circular in nature.
Simulated for Windows NT server's Ready Queue and Blocked Queue*/

#include<iostream>
#define MAX_Q 30

using namespace std;

class QUEUE
{

	public:	int rear, front, cnt;
		int size;
		int Q[MAX_Q];
		void addToReadyQueue(int);
		void init();
		int  qDelete();
		void display();
};

void QUEUE :: init()
{
	rear = front =-1;
	this->size = MAX_Q;
	cnt = 0;
	for(int i=0; i<MAX_Q; i++)
		Q[i] = -1;		//QUEUE is empty
}

void QUEUE :: display()
{
	if (cnt == 0)
		cout<<"Queue is empty\n";
	else
	{
		for(int i=0; i<cnt; i++)
		{
			cout<<"| "<<Q[front]<<" |";
			front = (front +1) % MAX_Q;
		}
	}
}
void QUEUE :: addToReadyQueue(int pid)
{
		if(cnt == MAX_Q)
			cout<<"Queue is full try with another Queue\n";
		else
		{
			rear = (rear+1) % MAX_Q;
			Q[rear] = pid;
			cnt++;
		}
}

int QUEUE :: qDelete()
{
	int item;
	if(cnt == 0)
		return -1;
	item = Q[front];
	Q[front] = -1;
	front = (front +1) % MAX_Q;
	cnt--;
	return item;
}