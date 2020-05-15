#include <stack>

#include <iostream>
#include <fstream>
#include <set>

#include "syntaxAnalist.h"

using namespace std;

const int numbersOfNonTerminals = 26;
const int numberOfSimanticPrograms = 5;

map <string, int> indexOfNonTerminal;
map <string, int> columnOfLex;

stack <pair <int, pair <int, int> > > avtomatMagazin;
stack <string> operationsMagazin;
stack <int> OPSPointersMagazin;

set <int> noneActionNonTerminals;

string table[numbersOfNonTerminals][numberOfLex];

void SP0()
{
	OPSPointersMagazin.push(OPS.size());
	OPS.push_back(make_pair(5, -1));
	OPS.push_back(make_pair(0, nameOfOperationToIndex["jf"]));
}

void SP1()
{
	int t = OPSPointersMagazin.top();
	OPSPointersMagazin.pop();
	OPS[t].second = OPS.size() + 1;

	OPSPointersMagazin.push(OPS.size());
	OPS.push_back(make_pair(5, -1));
	OPS.push_back(make_pair(0, nameOfOperationToIndex["j"]));
}

void SP2()
{
	int t = OPSPointersMagazin.top();
	OPSPointersMagazin.pop();
	OPS[t].second = OPS.size() - 1;
}

void SP3()
{
	OPSPointersMagazin.push(OPS.size());
}

void SP4()
{
	int t = OPSPointersMagazin.top();
	OPSPointersMagazin.pop();
	OPS[t].second = OPS.size() + 1;

	t = OPSPointersMagazin.top();
	OPSPointersMagazin.pop();
	OPS.push_back(make_pair(5, t - 1));
	OPS.push_back(make_pair(0, nameOfOperationToIndex["j"]));
}

void (*simanticPrograms[numberOfSimanticPrograms])();


bool errorState = false;


void syntaxError()
{
	cout << "Syntax";
	error();
	errorState = true;
	// реализовать возврат ошибки
}

void initSyntaxAnalist()
{
	ifstream myfile;
	myfile.open("syntaxTable.txt");

	string s;
	for (int i = 0; i < numbersOfNonTerminals + 1; i++)
	{
		for (int j = 0; j < numberOfLex + 1; j++)
		{
			string temp = "";
			myfile >> s;
			if (s[0] == '\'')
			{
				for (int i = 1; i < s.size(); i++)
					temp += s[i];
				s = temp;
			}

			for (int i = 0; i < s.size(); i++)
			{
				if (s[i] == '_')
					s[i] = ' ';
			}

			if (j == 0 && i > 0)
				indexOfNonTerminal[s] = i - 1;
			if (i == 0 && j > 0)
				columnOfLex[s] = j - 1;
			if (i > 0 && j > 0)
				table[i - 1][j - 1] = s;
		}
	}

	noneActionNonTerminals.insert(indexOfNonTerminal["U"]);
	noneActionNonTerminals.insert(indexOfNonTerminal["V"]);
	noneActionNonTerminals.insert(indexOfNonTerminal["W"]);
	noneActionNonTerminals.insert(indexOfNonTerminal["Z"]);
	noneActionNonTerminals.insert(indexOfNonTerminal["H"]);
	noneActionNonTerminals.insert(indexOfNonTerminal["E"]);

	simanticPrograms[0] = SP0;
	simanticPrograms[1] = SP1;
	simanticPrograms[2] = SP2;
	simanticPrograms[3] = SP3;
	simanticPrograms[4] = SP4;

	myfile.close();
}


void makeAndPushOP(string s)
{
	operationsMagazin.push(s);
}

void makeAndPushAM(string s)
{
	if (s == "a")
		avtomatMagazin.push(make_pair(1, make_pair(0, 0)));
	else
	if (s == "n")
		avtomatMagazin.push(make_pair(1, make_pair(1, 0)));
	else
	if (s == "d")
		avtomatMagazin.push(make_pair(1, make_pair(2, 0)));
	else
	if (s == "s")
		avtomatMagazin.push(make_pair(1, make_pair(3, 0)));
	else
	if (nameOfLexToIndex.count(s) > 0)
	{
		avtomatMagazin.push(make_pair(1, make_pair(nameOfLexToIndex[s], 0)));
	}
	else
		if (indexOfNonTerminal.count(s) > 0)
		{
			avtomatMagazin.push(make_pair(0, make_pair(indexOfNonTerminal[s], 0)));
		}
		else
			syntaxError();
}

void pushToAvtomatMagazin(string s)
{
	vector <string> AM, OP;
	bool justChar = false;

	string charactersSequance = "";
	if (s == "-")
		return syntaxError();
	if (s == "lambda")
		return;
	int currentChar = 0;
	bool readingFirstPart = true;

	while (currentChar < s.size())
	{
		if (s[currentChar] == '"')
		{
			currentChar++;
			continue;
		}

		if (s[currentChar] == '\\')
		{
			currentChar++;
			justChar = true;
			continue;
		}

		// переход от символов 
		if (s[currentChar] == ',' && !justChar)
		{
			currentChar++;
			readingFirstPart = false;
			continue;
		}

		//если больше одного симвала
		if (s[currentChar] == '[' && !justChar)
		{
			currentChar++;
			while (s[currentChar] != ']')
			{
				charactersSequance += s[currentChar];
				currentChar++;
			}
		}
		else
			charactersSequance += s[currentChar];

		currentChar++;
		justChar = false;

		if (readingFirstPart)
			AM.push_back(charactersSequance);
		else
			OP.push_back(charactersSequance);

		charactersSequance = "";
	}

	for (int i = AM.size() - 1; i >= 0; i--)
		makeAndPushAM(AM[i]);

	for (int i = OP.size() - 1; i >= 0; i--)
		makeAndPushOP(OP[i]);
}

void writeNextOperation(pair <int, int> lex)
{
	string topOperation = operationsMagazin.top();
	operationsMagazin.pop();

	if (topOperation == "")
		return;

	int simanticProgram = -1;
	if (topOperation[0] >= '0' && topOperation[0] <= '9')
	{
		simanticProgram = 0;
		for (int i = 0; i < topOperation.size(); i++)
		{
			simanticProgram *= 10;
			simanticProgram += topOperation[i] - '0';
		}
		return simanticPrograms[simanticProgram]();
	}

	if (nameOfOperationToIndex.count(topOperation))
		return OPS.push_back(make_pair(0, nameOfOperationToIndex[topOperation]));
	
	if (topOperation == "a")
		return OPS.push_back(make_pair(1, lex.second));
	if (topOperation == "n")
		return OPS.push_back(make_pair(2, lex.second));
	if (topOperation == "d")
		return OPS.push_back(make_pair(3, lex.second));
	if (topOperation == "s")
		return OPS.push_back(make_pair(4, lex.second));
}

void generateOPS()
{
	pair <int, int> currentLex;
	avtomatMagazin.push(make_pair(0, make_pair(indexOfNonTerminal["A"], 0)));// запихать начальный нетерминал
	operationsMagazin.push("");
	while ((currentLex = nextLex()) != make_pair(-1, -1))
	{
		if (avtomatMagazin.empty())
			return syntaxError(); // вернуть ошибку
		pair <int, pair <int, int> > magazinTop = avtomatMagazin.top();
		avtomatMagazin.pop();
		//writeNextOperation(currentLex);

		while (magazinTop.first == 0 && !errorState)// если не терминал
		{
			writeNextOperation(currentLex);
			pushToAvtomatMagazin(table[magazinTop.second.first][columnOfLex[lex[currentLex.first]]]);
			if (avtomatMagazin.empty())
				return syntaxError(); // вернуть ошибку
			magazinTop = avtomatMagazin.top();
			avtomatMagazin.pop();
		}

		if (magazinTop.second.first != currentLex.first)
		{
			return syntaxError();
		}
		else
			writeNextOperation(currentLex);
	}

	while (!avtomatMagazin.empty())
	{
		if (operationsMagazin.empty() || noneActionNonTerminals.count(avtomatMagazin.top().second.first) == 0)
			return syntaxError();
		else
		{
			writeNextOperation(currentLex);
			avtomatMagazin.pop();
		}
	}
	// если мазаин не пуст то вывести ошибку
}