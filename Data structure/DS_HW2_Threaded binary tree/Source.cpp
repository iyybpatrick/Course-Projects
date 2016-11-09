#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include "op_tree_totum.h"
using namespace std;

int detect_number(char* buffer){//detect the number in the input buffer	
	int sum = 0;
	for (int i = 2; buffer[i] >= '0' && buffer[i] <= '9'; i++){
		sum = sum * 10 + buffer[i] - '0';
	}
	return sum;
}


int main(int argc,char* argv[])
{
	
	if (argc != 2){
		printf("Input error: You have to key the [input filename]!!\n");
		exit(1);
	}

	FILE *input_file;

	if ((input_file = fopen(argv[1], "r")) == NULL){
		cout << "Cannot open input file!!\n";
		exit(1);
	}

	op_tree_totum *optt = new op_tree_totum();

	char buffer[100];
	while (fgets(buffer, 100, input_file) != NULL){
		switch (buffer[0]){
			int num;
			case 'I':// insert the node
				num = detect_number(buffer);
				optt->insertion(num);
				break;
			case 'D':// delete the node
				num = detect_number(buffer);
				optt->deletion(num);
				break;
			case 'C':// inorder path
				cout << "Change! Change myself into a cute mahou shoujo!!\n";
				cout << "The path: ";
				optt->inorder_run();
				cout << endl;
				break;
			case 'B':// reverseorder path
				cout << "Back! Back to the original life!!\n";
				cout << "The reverse path: ";
				optt->reverseorder_run();
				cout << endl;
				break;
		}
	}
	
	delete optt;
	fclose(input_file);
	return 0;
}