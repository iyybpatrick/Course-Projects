#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BEGIN 1
#define END 2
#define INTEGER 3
#define IF 4
#define THEN 5
#define ELSE 6
#define FUNCTION 7
#define READ 8
#define WRITE 9
#define IDFINDER 10
#define CONST 11
#define EQUAL 12
#define NE 13
#define LE 14
#define LESS 15
#define GE 16
#define GRATE 17
#define SUB 18
#define MUL 19
#define ASSIGN 20
#define LPAR 21
#define RPAR 22
#define SEM 23
#define EOLNNU 24
#define EOLN "EOLN"
#define EOFNU 25
#define EOFS "EOF"

#define errprint(x,y) ({printf("%s",x);exit(y);})
#define MAXLINE 20 
#define MAXLENGTH 20 //longest string


int is_letter(char ch);
int is_number(char ch);
int is_keyword(char* str);
void writeKey(const char* token, int type, FILE* dydFile);
int morphology(FILE* pasFile, FILE* dydFile, FILE* errFile);
void writeError(const char* error, int line, FILE* errFile);

char buf[MAXLINE];
char token[MAXLENGTH];

int main(int argc, char **argv){
	if (argc != 2)
		errprint("Usage:morphology <filename>", 1);

	char fileName; 
	FILE* pasFile; //sourse file pointer
	FILE* dydFile; //target file pointer
	FILE* errFile; //error file pointer

	char fileNameBuf[100];
	if ((pasFile = fopen(argv[1], "r")) == NULL)
		errprint("Can't open the pas file", 1);

	strncpy(fileNameBuf, argv[1], strchr(argv[1], '.') - argv[1]);
	if ((dydFile = fopen(strcat(fileNameBuf, ".dyd"), "w")) == NULL)
		errprint("Can't create the dyd file", 1);
	memset(fileNameBuf, 0, sizeof(fileNameBuf));
	strncpy(fileNameBuf, argv[1], strchr(argv[1], '.') - argv[1]);
	if ((errFile = fopen(strcat(fileNameBuf, ".err"), "w")) == NULL)
		errprint("Can't create the err file", 1);

	morphology(pasFile, dydFile, errFile);
	fclose(pasFile);
	fclose(dydFile);
	fclose(errFile);

}

int morphology(FILE* pasFile, FILE* dydFile, FILE* errFile){
	char ch;    //current char
	int index; //char index
	int type; // char type
	int line = 0;

	ch = fgetc(pasFile);


	while (1){
		if (ch == EOF){
			
			writeKey(EOFS, EOFNU, dydFile);
			break;
		}
		if (ch == ' ' || ch == '\t'){
			ch = fgetc(pasFile);
			continue;
		}
		index = 0;
		memset(token, 0, sizeof(token));
		if (is_letter(ch)){
			type = 0;
			token[index++] = ch;
			token[index] = '\0';

			ch = fgetc(pasFile);
			while (is_letter(ch) || is_number(ch)){
				token[index++] = ch;
				token[index] = '\0';
				ch = fgetc(pasFile);
				if ((type = is_keyword(token)) != 0){
					writeKey(token, type, dydFile);
					break;
				}
			}
			if (type == 0){
				writeKey(token, IDFINDER, dydFile);
			}

			continue;
		}
		if (is_number(ch)){
			token[index++] = ch;
			ch = fgetc(pasFile);
			while (is_number(ch)){
				token[index++] = ch;
				ch = fgetc(pasFile);
			}
			token[index] = '\0';
			writeKey(token, CONST, dydFile);
			continue;
		}
		token[index++] = ch;
		token[index] = '\0';
		switch (ch){
		case '=':
			writeKey(token, EQUAL, dydFile);
			break;
		case '-':
			writeKey(token, SUB, dydFile);
			break;
		case '*':
			writeKey(token, MUL, dydFile);
			break;
		case '(':
			writeKey(token, LPAR, dydFile);
			break;
		case ')':
			writeKey(token, RPAR, dydFile);
			break;
		case ';':
			writeKey(token, SEM, dydFile);
			break;
		case '!':
			writeKey(token, SEM, dydFile);
			break;
		case '<':
			ch = fgetc(pasFile);
			if (ch == '='){
				token[index++] = ch;
				token[index] = '\0';
				writeKey(token, LE, dydFile);
				break;
			}
			else if (ch == '>'){
				token[index++] = ch;
				token[index] = '\0';
				writeKey(token, NE, dydFile);
			}
			else{
				token[index] = '\0';
				writeKey(token, LESS, dydFile);
				continue;
			}
			break;
		case '>':
			ch = fgetc(pasFile);
			if (ch == '='){
				token[index++] = ch;
				token[index] = '\0';
				writeKey(token, GE, dydFile);
			}
			else{
				token[index] = '\0';
				writeKey(token, GRATE, dydFile);
				continue;
			}
			break;
		case ':':
			ch = fgetc(pasFile);
			if (ch == '='){
				token[index++] = ch;
				token[index] = '\0';
				writeKey(token, ASSIGN, dydFile);
			}
			else{
				//error
				writeError("lack of '=' after ':'", line, errFile);
				continue;
			}
			break;
		case '\n':
			line++;
			writeKey(EOLN, EOLNNU, dydFile);
			break;
		default:
			fprintf(errFile, "***Line:%d  invalid token \"%c\"\n", line, ch);
			break;
		}
		ch = fgetc(pasFile);
	}
}

int is_letter(char ch){
	if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z'))
		return 1;
	else
		return 0;
}

int is_number(char ch){
	if (ch <= '9' && ch >= '0')
		return 1;
	else
		return 0;
}
int is_keyword(char* str){
	if (strcmp(str, "begin") == 0)
		return BEGIN;
	else if (strcmp(str, "end") == 0)
		return END;
	else if (strcmp(str, "integer") == 0)
		return INTEGER;
	else if (strcmp(str, "if") == 0)
		return IF;
	else if (strcmp(str, "then") == 0)
		return THEN;
	else if (strcmp(str, "else") == 0)
		return ELSE;
	else if (strcmp(str, "function") == 0)
		return FUNCTION;
	else if (strcmp(str, "read") == 0)
		return READ;
	else if (strcmp(str, "write") == 0)
		return WRITE;
	//  else if(strcmp(str,"program")==0)
	//      return PROGRAM;
	//  else if(strcmp(str,"process")==0)
	//      return PROCESS;
	else
		return 0;
}

void writeKey(const char* token, int type, FILE* dydFile){
	int length = strlen(token);
	int i;
	memset(buf, '\0', sizeof(buf));
	for (i = 0; i<length; i++){
		buf[15 - length + i + 1] = token[i];
	}
	buf[16] = '\0';
	sprintf(buf + 17, "%2d", type);
	buf[19] = '\n';
	for (i = 0; i<19; i++){
		if (buf[i] == '\0')
			buf[i] = ' ';
	}
	if (fputs(buf, dydFile) == -1)
	{
		perror("227 wrong\n");
	}
	memset(buf, 0, sizeof(buf));
}

void writeError(const char* error, int line, FILE* errFile){
	fprintf(errFile, "***Line:%d  %s\n", line, error);
}