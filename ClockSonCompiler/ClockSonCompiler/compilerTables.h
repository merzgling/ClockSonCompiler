#pragma once

#include <string>
#include <map>
#include <vector>

using namespace std;

const int numberOfReservedWords = 13;
extern string reservedWords[numberOfReservedWords];
const int numberOfLex = 57;
extern string lex[numberOfLex];

extern map <string, int> nameOfLexToIndex;

const int numberOfOperations = 47;
extern string operation[numberOfOperations];
extern map <string, int> nameOfOperationToIndex;

extern vector <pair <int, int> > OPS;

void initCompilerTables();

int pushStringConstant(string s);

string getStringConstant(int x);

int pushDoubleConstant(double s);

double getDoubleConstant(int x);