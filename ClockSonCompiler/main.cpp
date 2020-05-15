#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>

#include <vector>
#include <string>

using namespace std;

#include "compilerTables.h"
#include "lecsicalAnalist.h"
#include "syntaxAnalist.h"
#include "interpritator.h"

void testLex()
{

	// вывод списка лексем
	pair <int, int> currentLex;
	while ((currentLex = nextLex()) != make_pair(-1, -1))
	{
		if (currentLex.first == 0 || currentLex.first == 1
			|| currentLex.first == 2 || currentLex.first == 3)
		{
			cout << "(" << lex[currentLex.first] << ", ";
			if (currentLex.first == 0)
				cout << getStringConstant(currentLex.second);
			if (currentLex.first == 1)
				cout << currentLex.second;
			if (currentLex.first == 2)
				cout << getDoubleConstant(currentLex.second);
			if (currentLex.first == 3)
				cout << getStringConstant(currentLex.second);
			cout << ")";
		}
		else
			cout << lex[currentLex.first];
		cout << ", ";
	}
}

void testOPS()
{
	for (int i = 0; i < OPS.size(); i++)
	{
		if (OPS[i].first == 0)
			cout << operation[OPS[i].second];
		if (OPS[i].first == 1)
			cout << getStringConstant(OPS[i].second);
		if (OPS[i].first == 2)
			cout << OPS[i].second;
		if (OPS[i].first == 3)
			cout << getDoubleConstant(OPS[i].second);
		if (OPS[i].first == 4)
			cout << "\'" << getStringConstant(OPS[i].second) << "\'";
		if (OPS[i].first == 5)
			cout << "p" << OPS[i].second;

		cout << ' ';
	}
}

int main()
{
	freopen("output.txt", "w", stdout);
	initCompilerTables();
	scanerInit();
	initSyntaxAnalist();
	initInterpritator();

	generateOPS();

	//testLex();

	testOPS();
	cout << "\n\n";
	interpritateOPS();
	return 0;
}