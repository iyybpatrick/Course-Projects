#include "stdlib.h"
#include "stdio.h"
#include "iostream.h"
#include "string"

#define NULL 0
using namespace std;
char account=-1;
int count=0; //file number
struct file{
	char name[10];
	int code;//safety code 0-R 1-W 2-RW
	int size;
	char pwrite;//pointer write
};
struct dir{
	char name[10];
	struct file *list;//target pointer
};
//init running files AFD
struct file 
one[10]={{"a1",0,10,0},{"a2",2,10,0},{"a3",1,10,0},{"0",0,0,0},{"0",0,0,0},
		{"a4",2,10,0},{"a5",2,10,0},{"a6",2,10,0},{"0",0,0,0},{"0",0,0,0}};
struct file 
two[10]={{"b1",0,10,0},{"0",0,0,0},{"b2",2,10,0},{"b3",1,10,0},{"0",0,0,0},
		{"b4",2,10,0},{"b5",2,10,0},{"b6",2,10,0},{"b7",2,10,0},{"0",0,0,0}	};
		
struct file 
three[10]={{"c1",0,10,0},{"c2",2,10,0},	{"0",0,0,0},{"c3",1,10,0},{"0",0,0,0},
			{"c4",2,10,0},{"c5",2,10,0},{"c6",2,10,0},{"c7",1,10,0},{"0",0,0,0}};
		
struct file 
four[10]={{"0",0,0,0},{"d1",0,10,0},{"d2",2,10,0},	{"0",0,0,0},{"d3",1,10,0},
			{"d4",2,10,0},{"d5",2,10,0},{"d6",2,10,0},{"d7",1,10,0},{"0",0,0,0}};
		
struct file 
fine[10]={{"e1",0,10,0},{"e2",2,10,0},{"e3",1,10,0},{"e4",2,10,0},{"e5",2,10,0},
{"e6",2,10,0},{"e7",1,10,0},{"0",0,0,0},{"0",0,0,0},{"0",0,0,0}};   		
struct file 
six[10]={{"f1",0,10,0},{"f2",2,10,0},{"f3",1,10,0},	{"0",0,0,0},{"0",0,0,0},
			{"f4",2,10,0},{"f5",2,10,0},{"f6",2,10,0},{"f7",1,10,0},{"0",0,0,0}};
	
struct file 
seven[10]={{"g1",0,10,0},{"g2",2,10,0},{"0",0,0,0},{"0",0,0,0},{"g3",1,10,0},
			{"g4",2,10,0},{"g5",2,10,0},{"g6",2,10,0},{"g7",1,10,0},{"0",0,0,0}
			};
struct file 
eight[10]={{"0",0,0,0},{"0",0,0,0},{"h1",0,10,0},{"h2",2,10,0},{"h3",1,10,0},
			{"h4",2,10,0},{"h5",2,10,0},{"h6",2,10,0},{"h7",1,10,0},{"0",0,0,0}
			};
struct file 
nine[10]={{"j1",0,10,0},{"j2",2,10,0},	{"0",0,0,0},{"0",0,0,0},{"j3",1,10,0},
			{"j4",2,10,0},{"j5",2,10,0},{"j6",2,10,0},{"j7",1,10,0},{"0",0,0,0}};
struct file
ten[10]={{"k1",0,10,0},{"k2",2,10,0},{"k3",1,10,0},{"k4",2,10,0},{"0",0,0,0},
			{"0",0,0,0},{"k5",2,10,0},{"k6",2,10,0},{"k7",1,10,0},{"0",0,0,0}
			};
struct file UFD[10];
struct file AFD[5]={{"0",0,0,0},{"0",0,0,0},{"0",0,0,0},{"0",0,0,0},{"0",0,0,0}};
//init user name
struct dir 
usr[10]={{"number1",one},{"number2",two},{"number3",three},{"number4",four},
			{"number5",fine},{"number6",six},{"number7",seven},{"number8",eight},
			{"number9",nine},{"number10",ten}};
struct dir MDF[1]={{"0",0}};

void	running();
//declare command function
void 	Delete();	
void	Write();    
void	Open();		
void	Close();
void	Byebye();	
void	Read();     
void	Create();	
void    Input();

void main(){    
	account=-1;
	Input();
	running();
	cout<<"操作己结束"<<endl;
}

void running(){ 
	char order;
	string temp="";
	int i;
	system("cls");

	cout<<endl<<"----------------------------------------------------------------"<<endl;
	printf("files of user %c are shown as follows：\n",account);
	//cout<<"用户"<<temp<<"的所有文件如下："<<endl;
	for(i=0;i<10;i++){                //显示用户的文件

		cout<<UFD[i].name<<"\t";
	}
	cout << endl << "----------------------------------------------------------------" << endl;
    cout<<"           Choose command（CODE）："<<endl;
	cout<<"c-Create	 d-Delete	o-Open	s-Close	 \nr-Read	  w-Write	b-Bye	t.change user"<<endl;
	cout << endl << "----------------------------------------------------------------" << endl;
	cout<<"请输入命令:"<<endl;
	cin>>order;                      //choose function
	switch(order){
		case 'c' :		Create();running();break;   
		case 'd' :		Delete();running();break;   
		case 'o' :		Open();running();break;     
		case 's' :		Close();running();break;   
		case 'r' :		Read();running();break;   
		case 'w' :		Write();running();break;  
		case 'b' :		Byebye();break;   
		case 't' :		Input();break;
		default  :		cout<<"Command Error,Please re-enter"<<endl<<endl;running();
	}



}

void  Delete(){   //delete
	char name[10];
	int i,flag;
	do{
		cout<<"Please in put file name： "<<endl<<endl;
		cin>>name;
		flag=0;
		for(i=0;i<10;i++){
			if(strcmp(UFD[i].name,name)){//file not found
				flag=1;		
			}
			else{
				flag=0;
				strcpy(UFD[i].name,"0");//clear file
				UFD[i].code=0;
				UFD[i].pwrite=0;
				UFD[i].size=0;
				cout<<"File has been deleted"<<endl;
				break;
			}
		}
		if(flag==1)
		cout<<"File not found，please re-enter"<<endl;	
	}while(flag==1);
	cout<<endl;
	//running();
		getchar();
}
void Open(){      
	char name[10];
	int i,j,flag;
	if(count==5){
		cout<<"Running queue is full."<<endl;
		//running();
	}
	else{
		do{
			cout<<"Please input file name："<<endl;
			cin>>name;
			flag=0;
			for(i=0;i<10;i++){
				if(strcmp(UFD[i].name,name)){  // file not found
					flag=1;		
				}
				else{
					flag=0;
					break;
				}
			}
			if(flag==1)
			cout<<"File not found，please re-enter"<<endl;	
		}while(flag==1);

		for(j=0;j<5;j++){
			if(strcmp(AFD[j].name,name))//如果没有这个文件
				flag=1;
			else{
				flag=0;
				break;
			}
		}
		if(flag==1)
			for(j=0;j<5;j++){
				if(!strcmp(AFD[j].name,"0")){
					AFD[j]=UFD[i];
					cout<<"File is open"<<endl;
					count++;
					break;
				}
			}
		else cout<<"File is in queue"<<endl;
		cout<<endl;
			getchar();
	//	running();
	}
}

void Close(){   //close
	char name[10];
	int j,flag;
	cout<<"Please input file name： "<<endl;
	cin>>name;
	flag=0;
	for(j=0;j<5;j++){
		if(strcmp(AFD[j].name,name)){//file not found
			flag=1;		
		}
		else{
			flag=0;
			strcpy(AFD[j].name,"0");//clear file
			AFD[j].code=0;
			AFD[j].pwrite=0;
			AFD[j].size=0;
			cout<<"File has been closed"<<endl;
			break;
		}
	}
	if(flag==1)
		cout<<"File not found，please re-enter"<<endl;	
	cout<<endl;
//	running();
		getchar();
}

void Byebye(){     //Save file directory
	int i,j;
	for(i=0;i<5;i++){
		for(j=0;j<10;j++){
			if(!strcmp(AFD[i].name,UFD[j].name)){
				UFD[i]=AFD[i];
				break;
			}
		}
	}	
	cout<<"File has been saved"<<endl;
	cout<<endl;
		getchar();
}

void Read(){     //Read file
	char name[10];
	int j,flag=0;
	cout<<"Please input file name： "<<endl;
	cin>>name;
	for(j=0;j<5;j++){	
		if(!strcmp(AFD[j].name,name)){//File found
			flag=1;	break;	
		}
	}
	if(flag==1)
		if(AFD[j].code==0||AFD[j].code==2){
			AFD[j].pwrite='R';
			cout<<"File is being reading"<<endl;
		}
		else cout<<"You are not authorized"<<endl;
	else{ 
			cout<<"File not found，please re-enter"<<endl;
	}
	cout<<endl;
	//running();
		getchar();
}
	
void Write(){      //write file
	char name[10];
	int j,flag=0;
	cout<<"Please input file name： "<<endl;
	cin>>name;
	for(j=0;j<5;j++){	
		if(!strcmp(AFD[j].name,name)){//File found
			flag=1;	break;	
		}
	}
	if(flag==1)
		if(AFD[j].code==1||AFD[j].code==2){
			AFD[j].pwrite='W';
			cout<<"File is being reading"<<endl;
		}
		else cout<<"You are not authorized"<<endl;
	else
		cout<<"File not found，please re-enter"<<endl;
	cout<<endl;
	//running();
		getchar();
}


void Create(){      //Create file
	char name[10];
	int i,j=0,flag;
	for(i=0;i<10;i++){
		if(!strcmp(UFD[i].name,"0"))
		j++;
	}
	if(j>9){
		cout<<"File queue is full !"<<endl;//cout<<j;
	//	running();
	}	
	else{	
		do{
			cout<<"Please input file name： "<<endl;
			cin>>name;
			flag=0;
			for(i=0;i<10;i++){
				if(!strcmp(UFD[i].name,name)){//duplication of name
					cout<<"duplication of name, please re-enter"<<endl;
					flag=1;	break;	
				}
			}
		}while(flag==1);
		for(i=0;i<10;i++){
			if(!strcmp(UFD[i].name,"0")){//Find a empty file
				strcpy(UFD[i].name,name);
				do{	
					cout<<"Please enter security code（0-R，1-W，2-RW）:"<<endl;
					cin>>UFD[i].code;
					if(UFD[i].code!=0&&UFD[i].code!=1&&UFD[i].code!=2)
					cout<<"wrong enter, please try again."<<endl;
				}while(UFD[i].code!=0&&UFD[i].code!=1&&UFD[i].code!=2);
				cout<<"Please input the size of this file（input number）:"<<endl;
				cin>>UFD[i].size;
				cout<<"Created"<<UFD[i].name<<endl;
				break;
			}	
		}
	//	running();
	}
		getchar();
}

void Input(){     //Input
	//char name;
	int i;
	system("cls");
	printf("\n");
	cout<< "----------------------------------------------------------------" << endl;
    printf("                        FILE  SYSTEM                                \n");
	cout << "----------------------------------------------------------------" << endl;
	cout<<endl<<"////////////////////////////////////////////////////////"<<endl;
	cout<<"	Please choose user（input numbers）："<<endl;
	cout<<"1.one  2.two	 3.three  4.four  5.fine  \n6.six	 7.seven  8.eight  9.nine  0.ten	"<<endl;
	cout<<"////////////////////////////////////////////////////////"<<endl<<endl;
	cout<<"Please input user name:";
	cin>>account;
	switch(account){
		case '1':		for(i=0;i<10;i++)UFD[i]=one[i];MDF[0]=usr[1];running();break;
		case '2':		for(i=0;i<10;i++)UFD[i]=two[i];MDF[0]=usr[1];running();break;
		case '3':		for(i=0;i<10;i++)UFD[i]=three[i];MDF[0]=usr[2];running();break;
		case '4':		for(i=0;i<10;i++)UFD[i]=four[i];MDF[0]=usr[3];running();break;
		case '5':		for(i=0;i<10;i++)UFD[i]=fine[i];MDF[0]=usr[4];running();break;
		case '6':		for(i=0;i<10;i++)UFD[i]=six[i];MDF[0]=usr[5];running();break;
		case '7':		for(i=0;i<10;i++)UFD[i]=seven[i];MDF[0]=usr[6];running();break;
		case '8':		for(i=0;i<10;i++)UFD[i]=eight[i];MDF[0]=usr[7];running();break;
		case '9':		for(i=0;i<10;i++)UFD[i]=nine[i];MDF[0]=usr[8];running();break;
		case '0':		for(i=0;i<10;i++)UFD[i]=ten[i];MDF[0]=usr[9];running();break;
		default :		cout<<"Wrong user name"<<endl;Input();
	}	
}
