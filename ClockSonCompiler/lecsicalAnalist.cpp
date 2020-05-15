#include "lecsicalAnalist.h"

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>

#include "compilerTables.h"

using namespace std;


string inputText;
int currentInputTextCharacter = 0;
bool endText = false;
void scanerInit()
{
	ifstream myfile;
	myfile.open("input.txt");

	string s;
	while (getline(myfile, s))
	{
		inputText += s;
		inputText += '\n';
	}

	myfile.close();
}

char nextCharacter()
{
	char res = -1;
	if (currentInputTextCharacter < inputText.size())
	{
		res = inputText[currentInputTextCharacter];
		currentInputTextCharacter++;
	}
	else
		endText = true;
	if (endText)
		return -1;

	if (res == '\t')
		return nextCharacter();
	else
		return res;
}

int currentLine = 0;
int currentLetter = 0;

void backCharacter()
{
	currentInputTextCharacter--;
	currentLetter--;
}





void error(int x, int y)
{
	printf("Error at line : %d, character: %d\n", x, y);
}


void error()
{
	error(currentLine, currentLetter);
}


string name;
int intNumber;
double realNumber;
double realPow;


vector <vector <int> > stateTable;
map <string, int> nonterminals, terminals;
int n, m;


void clearData()
{
	name = "";
	intNumber = 0;
	realNumber = 0;
	realPow = 1;
}

bool inited = false;
void init()
{
	ifstream myfile;
	myfile.open("lecsicalTable.txt");

	myfile >> n >> m;
	vector <vector <string> > temp(n - 1, vector <string>(m - 1));
	stateTable.resize(n-1, vector <int> (m));
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			string s;
			myfile >> s;
			if (i > 0 && j > 0)
				temp[i - 1][j - 1] = s;
			if (i == 0 && j > 0)
				terminals[s] = j - 1;
			if (j == 0 && i > 0)
				nonterminals[s] = i - 1;
		}
	}
	myfile.close();

	nonterminals["Z"] = n-1;
	nonterminals["Z*"] = n;

	for (int i = 0; i < n-1; i++)
	{
		for (int j = 0; j < m-1; j++)
		{
			if (temp[i][j] == "-")
				stateTable[i][j] = -1;
			else
				stateTable[i][j] = nonterminals[temp[i][j]];
		}
	}
	clearData();
}




void addCharToName(char c)
{
	name += c;
}

void addCharToIntNumber(char c)
{
	intNumber *= 10;
	intNumber += c - '0';
}

void addCharToRealNumber(char c)
{
	realPow *= 0.1;
	realNumber += realPow * (c - '0');
}



int getTerm(char c)
{
	if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
		return 0;
	if (c >= '0' && c <= '9')
		return 1;

	char chars[24] = { '.', ' ', '\n', '\'', ';', '=', '/', '*', '+', '-',
						'>', '<', '!', '&', '|', '^', '%', '[', ']', '{', '}', 
						'(', ')', ',' };

	for (int i = 0; i < 24; i++)
	{
		if (c == chars[i])
			return i + 2;
	}
}

pair <int, int> getNameResult()
{
	for (int i = 0; i < numberOfReservedWords; i++)
	{
		if (name == reservedWords[i])
			return make_pair(nameOfLexToIndex[name], 0);
	}

	return make_pair(nameOfLexToIndex["name"], pushStringConstant(name));
	// нужен указатель на имя в таблице вторым аргументом вернуть
}

pair <int, int> getIntNumberResult()
{
	return make_pair(1, intNumber);
}

pair <int, int> getRealNumberResult()
{
	return make_pair(2, pushDoubleConstant(realNumber + intNumber));
}

pair <int, int> getStringResult()
{
	string temp = name;
	name = "";
	for (int i = 1; i < temp.size(); i++)
		name += temp[i];
	return make_pair(3, pushStringConstant(name));
}

pair <int, int> getResult(pair <int, int> state)
{
	int x = state.first, y = state.second;

	// Возврат из начального состояния
	if (x == 0)
	{
		switch (y)
		{
		case 6: 
			return make_pair(nameOfLexToIndex[";"], 0);
			break;
		case 19:
			return make_pair(nameOfLexToIndex["["], 0);
			break;
		case 20:
			return make_pair(nameOfLexToIndex["]"], 0);
			break;
		case 21:
			return make_pair(nameOfLexToIndex["{"], 0);
			break;
		case 22:
			return make_pair(nameOfLexToIndex["}"], 0);
			break;
		case 23:
			return make_pair(nameOfLexToIndex["("], 0);
			break;
		case 24:
			return make_pair(nameOfLexToIndex[")"], 0);
			break;
		case 25:
			return make_pair(nameOfLexToIndex[","], 0);
			break;
		default:
			return make_pair(-1, -1);
			break;
		}
	}

	// Возврат из состояния идентификатора
	if (x == 1)
		return getNameResult();

	// Возврат из состояния целого числа
	if (x == 2)
		return getIntNumberResult();

	// Возврат из состояния вещественного числа
	if (x == 4)
		return getRealNumberResult();

	// Возврат из состояния строковой константы
	if (x == 19)
		return getStringResult();

	// Возврат ++
	if (y == 10 && x == 8)
		return make_pair(nameOfLexToIndex["++"], 0);
	// Возврат --
	if (y == 11 && x == 9)
		return make_pair(nameOfLexToIndex["--"], 0);
	// Возврат &&
	if (y == 15 && x == 13)
		return make_pair(nameOfLexToIndex["&&"], 0);
	// Возврат ||
	if (y == 16 && x == 14)
		return make_pair(nameOfLexToIndex["||"], 0);

	// Возврат операция + '='
	if (y == 7)
	{
		switch (x)
		{
		case 5:
			return make_pair(nameOfLexToIndex["=="], 0);
			break;
		case 6:
			return make_pair(nameOfLexToIndex["/="], 0);
			break;
		case 7:
			return make_pair(nameOfLexToIndex["*="], 0);
			break;
		case 8:
			return make_pair(nameOfLexToIndex["+="], 0);
			break;
		case 9:
			return make_pair(nameOfLexToIndex["-="], 0);
			break;
		case 10:
			return make_pair(nameOfLexToIndex[">="], 0);
			break;
		case 11:
			return make_pair(nameOfLexToIndex["<="], 0);
			break;
		case 12:
			return make_pair(nameOfLexToIndex["!="], 0);
			break;
		case 13:
			return make_pair(nameOfLexToIndex["&="], 0);
			break;
		case 14:
			return make_pair(nameOfLexToIndex["|="], 0);
			break;
		case 15:
			return make_pair(nameOfLexToIndex["^="], 0);
			break;
		case 16:
			return make_pair(nameOfLexToIndex["%="], 0);
			break;
		case 17:
			return make_pair(nameOfLexToIndex[">>="], 0);
			break;
		case 18:
			return make_pair(nameOfLexToIndex["<<="], 0);
			break;
		default:
			break;
		}
	}

	// Возврат операции
	switch (x)
	{
	case 5:
		return make_pair(nameOfLexToIndex["="], 0);
		break;
	case 6:
		return make_pair(nameOfLexToIndex["/"], 0);
		break;
	case 7:
		return make_pair(nameOfLexToIndex["*"], 0);
		break;
	case 8:
		return make_pair(nameOfLexToIndex["+"], 0);
		break;
	case 9:
		return make_pair(nameOfLexToIndex["-"], 0);
		break;
	case 10:
		return make_pair(nameOfLexToIndex[">"], 0);
		break;
	case 11:
		return make_pair(nameOfLexToIndex["<"], 0);
		break;
	case 12:
		return make_pair(nameOfLexToIndex["!"], 0);
		break;
	case 13:
		return make_pair(nameOfLexToIndex["&"], 0);
		break;
	case 14:
		return make_pair(nameOfLexToIndex["|"], 0);
		break;
	case 15:
		return make_pair(nameOfLexToIndex["^"], 0);
		break;
	case 16:
		return make_pair(nameOfLexToIndex["%"], 0);
		break;
	case 17:
		return make_pair(nameOfLexToIndex[">>"], 0);
		break;
	case 18:
		return make_pair(nameOfLexToIndex["<<"], 0);
		break;
	default:
		break;
	}
}

const int endState = 23;

pair <int, int> nextLex()
{
	if (!inited)
		init();

	int currentState = 0;
	pair <int, int> lastState;


	while (currentState != endState)
	{
		// Выход по условию конца входной цепочки
		char character = nextCharacter();
		if (character == '\n')
		{
			currentLine++;
			currentLetter = 0;
		}
		else
			currentLetter++;
		if (endText)
			return make_pair(-1, -1);

		// Преобразую символ входной цепочки в терминал лексической граматики
		int currentTerminal = getTerm(character);

		lastState = make_pair(currentState, currentTerminal);
		if (currentState < 0)
			error();
		currentState = stateTable[currentState][currentTerminal];

		if (currentState == 1)
			addCharToName(character);
		if (currentState == 2)
			addCharToIntNumber(character);
		if (currentState == 4)
			addCharToRealNumber(character);

		if (currentState == 19)
			addCharToName(character);

		if (currentState == endState + 1)
		{
			currentState--;
			backCharacter();
		}
	}

	pair <int, int> result = getResult(lastState);
	clearData();
	return result;
}