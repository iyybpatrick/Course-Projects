#include <windows.h> 
#include <time.h> 
#include <string> 
#include <iostream> 
#include <assert.h>  
using namespace std; 

bool kuaizi[5];//Label whether kuaizi is occupied, FALSE means occupied

CRITICAL_SECTION cs; //semaphore


class man
{ 
private: 
int number;
int status;//Philosophers' status, 0 means waiting, 1 means eating.

bool lefthand;//status of lefthand kuaizi for every philosopher. true means unoccupied, false means occupied.
bool righthand; 

public:
man(int num=0) { lefthand=true; righthand=false; status=2; number=num; } 
// to avoid dead lock, every philosopher start with picking his left hand kuaizi in default.
int find() const { return number; } 
int getinfo() const { return status; } 

void Change() ; 
}; 


void man::Change()
{
	EnterCriticalSection(&cs); //get in critical section
	if (status == 1) //kuaizi is being occupied.


	assert(!kuaizi[number]); 
	kuaizi[number]=true;  //kuaizi not being occupied.
	if(number==1) 
	{ 
	assert(!kuaizi[5]);
	kuaizi[5] = true;
	} 
	else 
	{
	kuaizi[number - 1] = true;
	status = 2; //status 2 : thingking
	srand(time(NULL) * 1000); //get random seed
	Sleep(900); //sleep for a while, so that random seed for two times are different.
	} 
	else if(status==2) //Thinking
	{ 
	status = 0; //change status to wait.
	} 
	else if(status==0) //waiting 
	{ 
	if (number == 1)
	{
		if (kuaizi[1] && kuaizi[5])
	    {
			kuaizi[1] = false;
			kuaizi[5] = false;
			status = 1;
			srand(time(NULL) * 1000);
			Sleep(900);
	    }
	}
	else
	{
		if (kuaizi[number] && kuaizi[number - 1])
		{
			kuaizi[number] = false;
			kuaizi[number - 1] = false;
			status = 1;
			srand(time(NULL) * 1000);
			Sleep(200);
		}
	}
}
LeaveCriticalSection (&cs) ; 
} 
string print(man *pA) 
{
	pA->Change();
	int i = pA->getinfo();
	string str;
	if (i == 0)
		str = "waiting";
	else if (i == 1)
		str = "eating";
	else str = "thinking";
return str; 
} 

int main() 
{ 
	for (int j = 1; j <= 5; j++)
		kuaizi[j] = true; //init
	man A1(1), A2(2), A3(3), A4(4), A5(5);
	InitializeCriticalSection(&cs); //init critical section
	while (1)
	{
		cout << "Second " << clock() / 1000 << " , status of philosophers: " << endl;  //clock¼ÆÊ±º¯Êý
		cout << A1.find() << "philosopher" << " status : " << print(&A1) << endl;
		cout << A2.find() << "philosopher" << " status : " << print(&A2) << endl;
		cout << A3.find() << "philosopher" << " status : " << print(&A2) << endl;
		cout << A4.find() << "philosopher" << " status : " << print(&A2) << endl;
		cout << A5.find() << "philosopher" << " status : " << print(&A2) << endl;
		cout<<"-----------------------------------------------------"<<endl;
		Sleep(20); 
	} 
	DeleteCriticalSection (&cs) ; //quit 
	return 0; 
}
