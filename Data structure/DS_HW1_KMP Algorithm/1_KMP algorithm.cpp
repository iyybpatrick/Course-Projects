/*                                              Student Number：0440061                                 */
/*                                              Student Name ：Yang Yuebin                                  */
#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<vector>
#include<sstream>
using namespace std;
string write_out;
int flag = 0;
void get_next(char*t, int next[])   //t is used to save the intersted string. Next is used to save it matched value.

{

	int t_len = strlen(t);///t_len is to save the matched string's length 

	int i = 0;        

	int j = -1;     

	next[0] = -1; 



	while (i<t_len)    //while when no string match

	{

		if (j == -1 || t[i] == t[j])  //j==-1 means not matched

		{
			i++;
			j++;
			next[i] = j;
		}

		else
		{
			j = next[j];  //recursion
		}
	}
}
/*KMP algorithm*/
int KMP(char *s, char *t)  //s is the pointer pointing the whole string. t is for interested string

{

	int s_len = strlen(s);  //to save it's length

	int t_len = strlen(t);  //to save it's length

	int i = 0;   //i指向字符串

	int j = 0;   //j指向匹配字符串pat

	int *next = new int[t_len];    // this array is used to save the matching value of each character.

	get_next(t, next);   //get it's matching value. and save it to array next.

	if (t_len>s_len) return -1;  //如果pat的长度比原字符串还长，则不可能匹配成功，返回-1

	int count = 0;      
	int change_i = 0;  
	int count_space = 1;  //to save the number of space.
	int judge = -1;

	while (i <= s_len)   //still calculating

	{
		if (flag == 1 && s[i - 1] != ' ')
			flag = 0;


		if ((flag == 0) && (s[i - 1] == ' ') && (judge != i)) 
		{
			judge = i;    //因为每次读取空格的i值一定不同，所以读入i值保存在judge里，若下次i值相同，则count不再++
			count_space++;
			if (s[i - 1] == '\n')
				count_space++;
			
			flag = 1;

		}



		if (j == t_len)    //matched successfully. Then obtain the value of i,j.
		{
			char t[8];     //for trasforming string to int。
			sprintf_s(t, "%d", count_space);   
			write_out += t;     

			cout << count_space;   
			count++;

			j = 0;                  //restart matching
			i = change_i;           //get next i.


		}
		if (next[j] == 0)           //重新匹配时，i的值为离其最近的，next值为0的字符。
		{
			change_i = i;           //更新重新匹配时i的值
		}
		if (j == -1 || s[i] == t[j])    //recursion
		{
			i++;
			j++;
		}
		else      //when not matched, move backward
		{
			j = next[j];
		}
	}//end while

	if (i >= s_len)

		return count; 

	else

		return -1;

}
int get_n(char*s)    
{                    
	int i = 0;       
	while (s[i] != ' ')
	{
		i++;
	}

	return i;
}

int main(void)

{
	string file_in_name;       
	ifstream myfile_open;       
	clock_t start, finish;
	cout << "Print input file :（format: xxxx.txt）：" << endl;
	cin >> file_in_name;

	myfile_open.open(file_in_name);   
	if (myfile_open.is_open() == NULL)   
	{
		cout << "Cannot open file !" << endl;
		return -1;
	}
	stringstream buffer;
	buffer << myfile_open.rdbuf();
	string get_txt(buffer.str());
	//string get_txt;    
	//getline(myfile_open, get_txt);  


	char *s = (char*)get_txt.c_str();  

	int k = get_n(s);
	char *t = new char[k];
	for (int i = 0; i < k; i++)
		t[i] = s[i];
	t[k] = '\0';


	start = clock();                               
	KMP(s, t);
	finish = clock();//calculate time             
	cout << endl;
	cout << "--------------------------------------------------------------------------------";
	cout << endl;

	double totaltime;
	totaltime = (double)(finish - start) * 1000 / CLOCKS_PER_SEC;        //换算成ms
	cout << "Total time = " << endl;
	cout << totaltime << "ms" << endl;
	cout << "--------------------------------------------------------------------------------";

	/*save as output*/
	cout << "Print output file :（format :xxxx.txt）：" << endl;
	string file_out_name;
	cin >> file_out_name;
	ofstream myfile_save;
	myfile_save.open(file_out_name);
	myfile_save << totaltime << "ms" << endl;
	myfile_save << write_out;
	cout << " Done ! " << endl;
	return 0;

}