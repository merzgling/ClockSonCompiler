#include <vector>

#include "compilerTables.h"

using namespace std;

string reservedWords[numberOfReservedWords] = { 
	"int", "int1", "int2", "real", "real1", "real2", "if", "else", "while", "write", "read", "mem1", "mem2" };

string lex[numberOfLex] = { 
	"name", "int number", "real number", "string constant",
	reservedWords[0], reservedWords[1], reservedWords[2], reservedWords[3],
	reservedWords[4], reservedWords[5], reservedWords[6], reservedWords[7],
	reservedWords[8], reservedWords[9], reservedWords[10], reservedWords[11],
	reservedWords[12], "=", "+=", "-=", "*=", "/=", "&=", "|=",
	"^=", ">>=", "<<=", "%=", "++", "--", "+", "-", "*", "/", "%", "!", "&", "|", "^",
	">>", "<<", "==", "!=", ">", "<", ">=", "<=", "&&", "||", "{", "}", "[", "]", "(", 
	")", ",", ";" };

map <string, int> nameOfLexToIndex;

string operation[numberOfOperations] = 
			{	"=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=",
				"++", "--", "+", "-", "*", "/", "%", "&", "|", "^", "<<", ">>", "!", "&&", "||", "==",
				"!=", "<", ">", "<=", ">=", "i", "i1", "i2", "r", "r1", "r2", "m1", "m2", "wr",
				"rd", "j", "jf", "u-", "ix1", "ix2"};
map <string, int> nameOfOperationToIndex;

vector <pair <int, int> > OPS;


void initCompilerTables()
{
	for (int i = 0; i < numberOfLex; i++)
	{
		nameOfLexToIndex[lex[i]] = i;
	}

	for (int i = 0; i < numberOfOperations; i++)
	{
		nameOfOperationToIndex[operation[i]] = i;
	}
}

vector <string> stringConstantsTable;
vector <double> doubleConstantsTable;


int pushStringConstant(string s)
{
	stringConstantsTable.push_back(s);
	return stringConstantsTable.size() - 1;
}

string getStringConstant(int x)
{
	return stringConstantsTable[x];
}

int pushDoubleConstant(double s)
{
	doubleConstantsTable.push_back(s);
	return doubleConstantsTable.size() - 1;
}

double getDoubleConstant(int x)
{
	return doubleConstantsTable[x];
}