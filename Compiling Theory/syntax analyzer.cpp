#include"string.h"
#include"stdlib.h"
#include"stdio.h"
#include"ctype.h"
#include"assert.h"

#define INTS 0;


FILE* ifp = NULL;  //input pointer
FILE* errfp = NULL;
FILE* ofp = NULL;  //output pointer
FILE* varf = NULL;
FILE* prof = NULL;

int level = 0;
int varn = 0;
int pron = 0;
int line = 1;

//int p=0;
int is_int = 0;
int is_func[1];

char proname[16][16];//temporary save

char token[16];   
char type[16];
char character;  




typedef struct Var{
	char vname[16];//variable name
	char vproc[16];//variable process
	int vkind;//classify  0: variable    1:  formal parameter
	char vtype[10];//variable type
	int vlev;//variable level
	int vadr;//variable position
}Var;

typedef struct Pro{
	char pname[16];//process name
	char ptype[10];//process type
	int plev;      //process level
	int fadr;      //the position of first variable in variable table
	int ladr;      //the position of last variable in variable table
}Pro;

Var var[16];//variable table
Pro pro[16];//process table


void error(char *errtype){
	printf("Syn_error:%s\t\t\t", type);
	printf("%s\n", token);

	fprintf(errfp, "***LINE:%2d  %10s\n", line, errtype);
	//	fprintf(errfp, ":%s\n", token);
	//	fprintf(errfp, "its type is %s\n", type);
}
int checkInVar(){
	int check = 0;
	int i;
	for (i = 0; i < 16; i++){
		if (strcmp(var[i].vname, token) == 0 && var[i].vlev == level){
			check = 1;
		}
		if (strcmp(pro[i].pname, token) == 0 || strcmp(proname[i], token) == 0){
			check = 1;
			printf("%s,%s,%s\n", pro[i].pname, proname[i], token);
			//		p=1;
		}
	}
	if (check == 1)
		return 1;
	else
		return 0;
}

void addToVar(int kind){
	int check = 0;
	int checkp = 0;
	int i;
	for (i = 0; i < 16; i++){
		if (strcmp(var[i].vname, token) == 0 && var[i].vkind == kind){
			check = 1;
			printf("88token:%s\n", token);
		}
		if (strcmp(pro[i].pname, token) == 0 || strcmp(proname[i], token) == 0){
			checkp = 1;
			printf("91token:%s\n", token);
		}
	}

	if (kind == 1){ return; }
	if (is_int == 1){
		if (check == 0){
			if (checkp == 1)
			{
				error("it has been defined as function name");
			}
			else{
				printf("加入一个变量        ：%s\t %d \n", token, kind);
				memset(var[varn].vname, 0, sizeof(var[varn].vname));

				strcat(var[varn].vname, token);

				if (level == 1){//find variable's process
					strcat(var[varn].vproc, "main process");
					printf("main process;\n");
				}
				else{
					int i;
					for (i = pron; i >= 0; i--){
						if (pro[i].plev + 1 == level){
							strcat(var[varn].vproc, pro[i].pname);
						}
					}
				}

				var[varn].vkind = kind;
				strcat(var[varn].vtype, "integer");
				var[varn].vlev = level;
				var[varn].vadr = varn;
				varn++;
				is_int = 0;
				printf("vlevel:%d\n", var[varn - 1].vlev);
			}
		}
		else{ error("duplicated identification"); }
	}
	//	else{
	//		error("");
	//	}
}

void addToPro(int f, int l){
	
	strcat(pro[pron].ptype, "integer");
	//pro[pron].plev = level - 1;
	pro[pron].fadr = f;
	pro[pron].ladr = l;
	pron++;
}

void addToPro1(char * proname)
{
	printf("Add a process        ：%s\n", proname);
	pro[pron].plev = level;
	strcat(pro[pron].pname, proname);
}


//read-in one char everytime
void getString() {
	memset(token, 0, sizeof(token));
	memset(type, 0, sizeof(type));
	character = getc(ifp);
	//start reading from the first no-empty character	
	while (character == ' ')
	{
		character = getc(ifp);
	}
	while (1){
		if (character == ' ')
		{
			while (character == ' ')
				character = getc(ifp);
			while (character != '\n')
			{
				strcat(type, &character);
				if ((character = getc(ifp)) == EOF)
					exit(1);
			}
			break;
		}

		char * ct = &character;
		strcat(token, ct);
		if (strcmp(token, "EOLN") == 0)
		{
			line++;
			memset(token, 0, sizeof(token));
			memset(type, 0, sizeof(type));
			printf("125\n");
			while (character != '\n')
			{
				character = getc(ifp);
			}
			character = getc(ifp);
			while (character == ' ')
			{
				character = getc(ifp);
			}
			continue;
		}
		if ((character = getc(ifp)) == EOF)
			exit(1);
	}

}

//read-in next char
void advance(){
	getString();
	printf("now read:%s,%s\n", token, type);
	fprintf(ofp, "%s\t", token);
	fprintf(ofp, "%s\n", type);
}

//for dispatch
void SP();
void DST();
void DST1();
void DS();
void V();
void ID();
void PM();
void FB();
void EST();
void EST1();
void ES();
void RS();
void WS();
void AS();
void AE();
void AE1();
void IT();
void IT1();
void F();
void FC();
void CS();
void CE();
void RO();


//process
void P(){
	SP();
}


//seperate process
void SP(){
	if (strcmp(type, "1") == 0){
		level++;
		advance();
		DST();

		EST();
		if (strcmp(type, "2") == 0){
			level--;
		}
		else error("lack of \"end\"");
	}
	else { printf("183err\n"); error("lack of \"begin\""); }
}

void DST(){
	DS();
	DST1();
}


void DST1(){
	if (strcmp(type, "23") == 0){
		advance();
		if (strcmp(type, "3") == 0){
			DS();
			DST1();
		}
	}
	else{ error("lack of \";\""); }
}

void DS(){
	if (strcmp(type, "3") == 0){
		is_int = 1;
		is_func[0] = 1;
		advance();
		if (strcmp(type, "7") == 0){
			advance();
			is_int = 0;
			is_func[1] = 1;
			memset(proname[level], 0, sizeof(type));
			strcat(proname[level], token);

			int f = 0;
			int l = 0;

			ID();
			addToPro1(proname[level]);//put process name to process table
			is_func[0] = 0;
			is_func[1] = 0;

			if (strcmp(type, "21") == 0){
				advance();
				f = varn;

				PM();
				if (strcmp(type, "22") == 0){
					advance();
					if (strcmp(type, "23") == 0){
						advance();
						FB();
						l = varn - 1;
					}
					else error("lack if \";\"");
				}
				else error("lack of \")\"");
			}
			else error("lack of \"(\"");

			addToPro(f, l);//put process other factors to process table
		}
		else{
			if (strcmp(type, "10") == 0)
				V();//process specification
			else error("lack of variable");
		}
	}
	else error("lack of integer");
}

//variable
void V(){
	addToVar(0);
	advance();
}
//add to variable table
void ID(){
	if (strcmp(type, "10") == 0){   //it is identifier
		advance();
	}
	else error("lack of variable");
}
//parameter
void PM(){
	addToVar(1);
	advance();
}
//function body
void FB(){
	if (strcmp(type, "1") == 0){
		level++;
		advance();
		DST();

		EST();
		if (strcmp(type, "2") == 0){
			level--;
			advance();
		}
		else error("lack of end");
	}
	else error("lack of begin");
}

//Execute statement table
void EST(){
	ES();
	EST1();
}

void EST1(){
	if (strcmp(type, "23") == 0){
		advance();
		ES();
		EST1();
	}
}

//Execute statement
void ES(){
	if (strcmp(type, "8") == 0){
		RS();
	}
	else if (strcmp(type, "9") == 0){
		WS();
	}
	else if (strcmp(type, "4") == 0){
		CS();
	}
	else{
		AS();
	}
}

//read
void RS(){
	advance();
	if (strcmp(type, "21") == 0){
		advance();
		int check = checkInVar();
		if (check == 0){
			char temp[20];
			sprintf(temp, "undefined variable:%s", token);
			error(temp);
		}
		advance();
		if (strcmp(type, "22") == 0)
			advance();
		else error("lack of \")\"");
	}
	else error("lack of \"(\"");
}

//write
void WS(){
	advance();
	if (strcmp(type, "21") == 0){
		advance();
		int check = checkInVar();
		if (check == 0){
			char temp[20];
			sprintf(temp, "undefined variable:%s", token);
			error(temp);
		}
		advance();
		if (strcmp(type, "22") == 0)
			advance();
		else error("lack of \")\"");
	}
	else error("lack of \"(\"");
}

//assignment
void AS(){
	//printf("%d\n",p);
	if (strcmp(type, "10") == 0)
	{	//p=0;
		int check = checkInVar();
		printf("412!!!!!!!!!!!!!!\n");
		if (check == 0)
		{
			char temp[20];
			sprintf(temp, "undefined variable:%s", token);
			error(temp);
		}
	}
	else{ error("lack of variable"); }
	//V();
	//	if(p==1){  //F is followed by ( only.
	//	p=0;
	//	printf("422!!!!!!!!!!!!\n");
	//	error("function name cannot be assigned");
	//	}
	advance();
	if (strcmp(type, "20") == 0){
		advance();
		AE();
	}
	else error("lack of \":=\"");
}

//arithmetic expression
void AE(){
	IT();
	AE1();
}

//arithmetic expression 1
void AE1(){
	if (strcmp(type, "18") == 0){
		advance();
		IT();
		AE1();
	}
}

//term
void IT(){
	F();
	IT1();
}

void IT1(){
	if (strcmp(type, "19") == 0){
		advance();
		F();
		IT1();
	}
}

//factor, including function call
void F(){
	if (strcmp(type, "10") == 0){         //if with ')' it's function call, otherwise，it's a variable
		int check = checkInVar();
		if (check == 0){
			char temp[20];
			sprintf(temp, "undefined variable:%s", token);
			error(temp);
		}
		advance();

		if (strcmp(type, "21") == 0){     //process function call  
			advance();
			AE();
			if (strcmp(type, "22") == 0){
				advance();
			}
		}

	}
	else if (strcmp(type, "11") == 0){
		advance();
	}
	else{ error("unknow factor"); }
}

//function call
void FC(){
	if (strcmp(type, "10") == 0){
		advance();
		if (strcmp(type, "21") == 0){
			advance();
			AE();
			if (strcmp(type, "22") == 0){
				advance();
			}
			else error("lack of \")\"");
		}
		else error("lack of \"(\"");
	}
	else error("lack of function name");
}

//condition sentence  
void CS(){
	advance();
	// no need to check 'if' because it has been checked at the ES().
	CE();
	if (strcmp(type, "5") == 0){
		advance();
		ES();
		if (strcmp(type, "6") == 0){
			advance();
			ES();
		}
		else error("lack of else");
	}
	else error("lack of then");
}

//condition expression
void CE(){
	AE();
	RO();
	AE();
}

//relation operator
void RO(){
	if (strcmp(type, "12") == 0)
		advance();
	else if (strcmp(type, "13") == 0)
		advance();
	else if (strcmp(type, "14") == 0)
		advance();
	else if (strcmp(type, "15") == 0)
		advance();
	else if (strcmp(type, "16") == 0)
		advance();
	else if (strcmp(type, "17") == 0)
		advance();
	else if (strcmp(type, "20") == 0)
		advance();
	else error("lack of relative operand");

}


int main(){

	if ((errfp = fopen("pasFile.err", "r+")) == NULL){
		printf("open file error: can't open input file! nanlyze.err \n");
		return  1;
	}
	fgetc(errfp);
	if (!feof(errfp)){
		printf("源文件有错！请先检查词法排除错误，再进行语法分析\n");
		exit(1);
	}

	//open input,output file
	if ((ifp = fopen("pasFile.dyd", "r")) == NULL){
		printf("open file error: can't open input file! nanlyze.dyd \n");
		return  1;
	}
	if ((ofp = fopen("syntax.dys", "w")) == NULL){
		printf("open file error: can't open input file! nanlyze.dys \n");
		return  1;
	}
	if ((varf = fopen("syntax.var", "w")) == NULL){
		printf("open file error: can't open input file! nanlyze.var \n");
		return  1;
	}
	if ((prof = fopen("syntax.pro", "w")) == NULL){
		printf("open file error: can't open input file! nanlyze.pro \n");
		return  1;
	}

	is_func[0] = 1;
	is_func[1] = 1;

	advance();
	//start syntax analysis
	P();
	//print outcome to output file
	rewind(errfp);
	fgetc(errfp);
	if (!feof(errfp)){
		printf("\nSyntax Analysis fail.\n");
		exit(1);
	}
	printf("\nCorrect Grammar！\n\n");
	printf(" process listed as follows: \n\n");

	int i;
	for (i = 0; i < varn; i++){
		fprintf(varf, "Process name  :%s\t", var[i].vname);
		fprintf(varf, "beloing process:%-10s\t", var[i].vproc);
		fprintf(varf, "process kind:%d\t", var[i].vkind);
		fprintf(varf, "process type:%s\t", var[i].vtype);
		fprintf(varf, "process level:%d\t", var[i].vlev);
		fprintf(varf, "process direction:%d\n", var[i].vadr);

	}
	for (i = 0; i < pron; i++){
		fprintf(prof, "Process name:%-10s\t", pro[i].pname);
		fprintf(prof, "Process type:%s\t", pro[i].ptype);
		fprintf(prof, "Process level:%d\t", pro[i].plev);
		fprintf(prof, "lastvar    :%d\t", pro[i].fadr);
		fprintf(prof, "firstvar   :%d\n", pro[i].ladr);
	}

	fclose(ifp);
	fclose(ofp);
	fclose(errfp);
	fclose(varf);
	fclose(prof);
	return 0;
}