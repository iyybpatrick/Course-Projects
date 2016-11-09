#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<cstring>
#include "Promenade.h"
using namespace std;

int detect_first_num(char* buffer){//detect the first number in the input buffer	
	int sum = 0;
	for (int i = 2; buffer[i] >= '0' && buffer[i] <= '9'; i++){
		sum = sum * 10 + buffer[i] - '0';
	}
	return sum;
}

int detect_second_num(char* buffer){//detect the second number in the input buffer	
	int sum = 0;
	int i = 2;
	while (i<strlen(buffer) && buffer[i] >= '0' && buffer[i] <= '9')i++;
	i++;//the space
	for (; i<strlen(buffer) && buffer[i] >= '0' && buffer[i] <= '9'; i++){
		sum = sum * 10 + buffer[i] - '0';
	}
	return sum;
}

int main(int argc, char* argv[])
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

	Promenade *prom = new Promenade();

	char buffer[1000];
	while (fgets(buffer, 1000, input_file) != NULL){
		Promenade *g = new Promenade();
		int num, num2;
		int i = 2, pos = 0, flag = 0;
		switch (buffer[0]){
					
		case 'P':// one person joined
			num = detect_first_num(buffer);
			num2 = detect_second_num(buffer);
			prom->one_person_joined(num,num2);
			break;
		case 'G':// one group joined			
			while (i < strlen(buffer)){
				if (buffer[i]<'0' || buffer[i]>'9')flag++;
				if (flag == 2){
					num = detect_first_num(buffer + pos);
					num2 = detect_second_num(buffer + pos);
					g->one_person_joined(num, num2);
					flag = 0;
					pos = i-1;
				}
				i++;
			}
			prom->one_group_joined(g);
			break;
		case 'A':// absorb the energy
			num = detect_first_num(buffer);
			num2 = detect_second_num(buffer);
			prom->absorb_energy(num, num2);
			break;
		case 'C':// calculate people below the number of energy
			num = detect_first_num(buffer);
			cout <<"The number of people below "<<num<<" energy is: "<< prom->calculate_people_below(num)<< endl;
			break;
		case 'W':// the weakest person's id
			cout << "The weakest person's id is: " << prom->get_weakest_person() << endl;
			break;
		case 'N':// the number of people in the prom 
			cout << "The number of people in Elizabeth promenade is: " << prom->size() << endl;
			break;
		}
	}

	fclose(input_file);
	return 0;
}