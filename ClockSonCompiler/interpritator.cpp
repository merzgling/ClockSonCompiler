#include <stack>
#include <iostream>
#include <sstream>

#include "compilerTables.h"
#include "interpritator.h"

stack <pair <int, int> > operandsMagazin;
void(*operationFunctions[numberOfOperations])();

int currenElementOPS = 0;

// memory //

map <string, pair <int, int> > indexOfVar;

vector <int> intVar;
vector <vector <int> > int1Var;
vector <vector <vector <int> > > int2Var;
vector <double> realVar;
vector <vector <double> > real1Var;
vector <vector <vector <double> > > real2Var;

////////////

void interpritatorError()
{
	cout << "Interpritator error in " << currenElementOPS << " element: " + operation[OPS[currenElementOPS].second] << '\n';
}

pair <int, int> getOperand()
{
	pair <int, int> res = operandsMagazin.top();
	operandsMagazin.pop();
	return res;
}

pair <string, pair <int, int> > getNameAndIndexes(string s)
{
	int i1 = -1;
	int i2 = -1;
	int c = 0;
	string name = "";
	while (c < s.size())
	{
		if (s[c] == ',')
		{
			i1 = 0;
			c++;
			break;
		}
		name += s[c];
		c++;
	}

	if (i1 == 0)
		while (c < s.size())
		{
			if (s[c] == ',')
			{
				i2 = 0;
				c++;
				break;
			}
			
			i1 *= 10;
			i1 += s[c] - '0';

			c++;
		}
	
	if (i2 == 0)
		while (c < s.size())
		{
			i2 *= 10;
			i2 += s[c] - '0';
			c++;
		}

	return make_pair(name, make_pair(i1, i2));
}

pair <int, int> readVal(pair <int, int> in)
{
	if (in.first == 1)
	{
		pair <string, pair <int, int> > var = getNameAndIndexes(getStringConstant(in.second));
		pair <int, int> type = indexOfVar[var.first];
		switch (type.first)
		{
		case 0:
			return make_pair(0, intVar[type.second]);
			break;
		case 1:
			if (var.second.first >= int1Var[type.second].size())
			{
				interpritatorError();
				return make_pair(-1, -1);
			}
			return make_pair(0, int1Var[type.second][var.second.first]);
			break;
		case 2:
			if (var.second.first >= int2Var[type.second].size() || var.second.second >= int2Var[type.second][var.second.first].size())
			{
				interpritatorError();
				return make_pair(-1, -1);
			}
			return make_pair(0, int2Var[type.second][var.second.first][var.second.second]);
			break;
		case 3:
			return make_pair(1, pushDoubleConstant(realVar[type.second]));
			break;
		case 4:
			if (var.second.first >= real1Var[type.second].size())
			{
				interpritatorError();
				return make_pair(-1, -1);
			}
			return make_pair(1, pushDoubleConstant(real1Var[type.second][var.second.first]));
			break;
		case 5:
			if (var.second.first >= real2Var[type.second].size() || var.second.second >= real2Var[type.second][var.second.first].size())
			{
				interpritatorError();
				return make_pair(-1, -1);
			}
			return make_pair(1, pushDoubleConstant(real2Var[type.second][var.second.first][var.second.second]));
			break;
		default:
			break;
		}
	}
	if (in.first == 2)
	{
		return make_pair(in.first - 2, in.second);
	}
	if (in.first == 3)
	{
		return make_pair(in.first - 2, in.second);
	}
	if (in.first == 4)
	{
		return make_pair(in.first - 2, in.second);
	}

	interpritatorError();
	return make_pair(-1, -1);
}

void writeVal(pair <int, int> cell, pair <int, int> val)
{
	if (cell.first != 1)
		return interpritatorError();

	pair <string, pair <int, int> > var = getNameAndIndexes(getStringConstant(cell.second));
	pair <int, int> type = indexOfVar[var.first];

	if ((type.first == 0 || type.first == 1 || type.first == 2) && val.first == 0)
	{
		switch (type.first)
		{
		case 0:
			intVar[type.second] = val.second;
			break;
		case 1:
			int1Var[type.second][var.second.first] = val.second;
			break;
		case 2:
			int2Var[type.second][var.second.first][var.second.second] = val.second;
			break;
		default:
			break;
		}
		return;
	}
	
	if ((type.first == 3 || type.first == 4 || type.first == 5) && val.first == 1)
	{
		switch (type.first)
		{
		case 3:
			realVar[type.second] = getDoubleConstant(val.second);
			break;
		case 4:
			real1Var[type.second][var.second.first] = getDoubleConstant(val.second);
			break;
		case 5:
			real2Var[type.second][var.second.first][var.second.second] = getDoubleConstant(val.second);
			break;
		default:
			break;
		}
		return;
	}

	return interpritatorError();
}

//////////////////// сами операции /////////

void unarMinusOperation()
{	
	pair<int, int> operand = readVal(getOperand());
	if (operand.first == 0)
		operandsMagazin.push(make_pair(2, -operand.second));
	else
		operandsMagazin.push(make_pair(3, pushDoubleConstant(-getDoubleConstant(operand.second))));
}

void writeOperation()
{
	pair <int, int> operand = readVal(getOperand());
	if (operand.first == 0)
		cout << operand.second;
	if (operand.first == 1)
		cout << getDoubleConstant(operand.second);
	if (operand.first == 2)
	{
		string s = getStringConstant(operand.second);
		for (int i = 0; i < s.size(); i++)
		{
			if (s[i] == '\\' && i != s.size() - 1)
				if (s[i + 1] == 'n')
				{
					cout << '\n';
					i++;
					continue;
				}
			cout << s[i];
		}
	}
}

void createIntVarOperation()
{
	pair <int, int> operand = getOperand();
	if (operand.first != 1)
		return interpritatorError();
	string name = getStringConstant(operand.second);
	if (indexOfVar.count(name) > 0)
		return interpritatorError();

	indexOfVar[name] = make_pair(0, intVar.size());
	intVar.push_back(0);
}

void createInt1VarOperation()
{
	pair <int, int> operand = getOperand();
	if (operand.first != 1)
		return interpritatorError();
	string name = getStringConstant(operand.second);
	if (indexOfVar.count(name) > 0)
		return interpritatorError();

	indexOfVar[name] = make_pair(1, int1Var.size());
	int1Var.push_back(vector <int> (0));
}

void createInt2VarOperation()
{
	pair <int, int> operand = getOperand();
	if (operand.first != 1)
		return interpritatorError();
	string name = getStringConstant(operand.second);
	if (indexOfVar.count(name) > 0)
		return interpritatorError();

	indexOfVar[name] = make_pair(2, int2Var.size());
	int2Var.push_back(vector <vector <int> > (0, vector <int> (0)));
}

void createRealVarOperation()
{
	pair <int, int> operand = getOperand();
	if (operand.first != 1)
		return interpritatorError();
	string name = getStringConstant(operand.second);
	if (indexOfVar.count(name) > 0)
		return interpritatorError();

	indexOfVar[name] = make_pair(3, realVar.size());
	realVar.push_back(0);
}

void createReal1VarOperation()
{
	pair <int, int> operand = getOperand();
	if (operand.first != 1)
		return interpritatorError();
	string name = getStringConstant(operand.second);
	if (indexOfVar.count(name) > 0)
		return interpritatorError();

	indexOfVar[name] = make_pair(4, real1Var.size());
	real1Var.push_back(vector <double> (0));
}

void createReal2VarOperation()
{
	pair <int, int> operand = getOperand();
	if (operand.first != 1)
		return interpritatorError();
	string name = getStringConstant(operand.second);
	if (indexOfVar.count(name) > 0)
		return interpritatorError();

	indexOfVar[name] = make_pair(5, real2Var.size());
	real2Var.push_back(vector <vector <double> > (0, vector <double> (0)));
}


void PlusOperation()
{
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = readVal(getOperand());

	if (operand1.first == 0)
		if (operand2.first == 0)
			operandsMagazin.push(make_pair(2, operand1.second + operand2.second));
		else
			operandsMagazin.push(make_pair(3, pushDoubleConstant(getDoubleConstant(operand2.second) + operand1.second)));
	else
		if (operand2.first == 0)
			operandsMagazin.push(make_pair(3, pushDoubleConstant(getDoubleConstant(operand1.second) + operand2.second)));
		else
			operandsMagazin.push(make_pair(3, pushDoubleConstant(getDoubleConstant(operand2.second) + getDoubleConstant(operand1.second))));

}

void minusOperation()
{
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = readVal(getOperand());

	if (operand1.first == 0)
		if (operand2.first == 0)
			operandsMagazin.push(make_pair(2, operand1.second - operand2.second));
		else
			operandsMagazin.push(make_pair(3, pushDoubleConstant(-getDoubleConstant(operand2.second) + operand1.second)));
	else
		if (operand2.first == 0)
			operandsMagazin.push(make_pair(3, pushDoubleConstant(getDoubleConstant(operand1.second)- operand2.second)));
		else
			operandsMagazin.push(make_pair(3, pushDoubleConstant(-getDoubleConstant(operand2.second) + getDoubleConstant(operand1.second))));

}

void multOperation()
{
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = readVal(getOperand());

	if (operand1.first == 0)
		if (operand2.first == 0)
			operandsMagazin.push(make_pair(2, operand1.second * operand2.second));
		else
			operandsMagazin.push(make_pair(3, pushDoubleConstant(getDoubleConstant(operand2.second) * operand1.second)));
	else
		if (operand2.first == 0)
			operandsMagazin.push(make_pair(3, pushDoubleConstant(getDoubleConstant(operand1.second) * operand2.second)));
		else
			operandsMagazin.push(make_pair(3, pushDoubleConstant(getDoubleConstant(operand2.second) * getDoubleConstant(operand1.second))));

}

void divOperation()
{
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = readVal(getOperand());

	if (operand1.first == 0)
		if (operand2.first == 0)
			operandsMagazin.push(make_pair(2, operand1.second / operand2.second));
		else
			operandsMagazin.push(make_pair(3, pushDoubleConstant(double(operand1.second) / getDoubleConstant(operand2.second))));
	else
		if (operand2.first == 0)
			operandsMagazin.push(make_pair(3, pushDoubleConstant(getDoubleConstant(operand1.second) / operand2.second)));
		else
			operandsMagazin.push(make_pair(3, pushDoubleConstant(getDoubleConstant(operand1.second) / getDoubleConstant(operand2.second))));

}

void modOperation()
{
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = readVal(getOperand());

	if (operand1.first == 0)
		if (operand2.first == 0)
		{
			operandsMagazin.push(make_pair(2, operand1.second % operand2.second));
			return;
		}

	interpritatorError();
}

void bitAndOperation()
{
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = readVal(getOperand());

	if (operand1.first == 0)
		if (operand2.first == 0)
		{
			operandsMagazin.push(make_pair(2, operand1.second & operand2.second));
			return;
		}
		
	interpritatorError();
}

void bitOrOperation()
{
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = readVal(getOperand());

	if (operand1.first == 0)
		if (operand2.first == 0)
		{
			operandsMagazin.push(make_pair(2, operand1.second | operand2.second));
			return;
		}

	interpritatorError();
}

void bitXorOperation()
{
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = readVal(getOperand());

	if (operand1.first == 0)
		if (operand2.first == 0)
		{
			operandsMagazin.push(make_pair(2, operand1.second ^ operand2.second));
			return;
		}

	interpritatorError();
}

void bitRightOperation()
{
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = readVal(getOperand());

	if (operand1.first == 0)
		if (operand2.first == 0)
		{
			operandsMagazin.push(make_pair(2, operand1.second >> operand2.second));
			return;
		}

	interpritatorError();
}

void bitLeftOperation()
{
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = readVal(getOperand());

	if (operand1.first == 0)
		if (operand2.first == 0)
		{
			operandsMagazin.push(make_pair(2, operand1.second << operand2.second));
			return;
		}

	interpritatorError();
}


void equalOperation()
{
	pair <int, int> operand2 = getOperand();
	pair <int, int> operand1 = getOperand();

	operand2 = readVal(operand2);
	writeVal(operand1, operand2);
}

void equalPlusOperation()
{
	pair <int, int> operand2 = getOperand();
	pair <int, int> operand1 = getOperand();

	pair <int, int> temp = readVal(operand1);
	temp.first += 2;

	operandsMagazin.push(operand1);
	operandsMagazin.push(temp);
	operandsMagazin.push(operand2);

	PlusOperation();
	equalOperation();
}

void equalMinusOperation()
{
	pair <int, int> operand2 = getOperand();
	pair <int, int> operand1 = getOperand();

	pair <int, int> temp = readVal(operand1);
	temp.first += 2;

	operandsMagazin.push(operand1);
	operandsMagazin.push(temp);
	operandsMagazin.push(operand2);

	minusOperation();
	equalOperation();
}

void equalMultOperation()
{
	pair <int, int> operand2 = getOperand();
	pair <int, int> operand1 = getOperand();

	pair <int, int> temp = readVal(operand1);
	temp.first += 2;

	operandsMagazin.push(operand1);
	operandsMagazin.push(temp);
	operandsMagazin.push(operand2);

	multOperation();
	equalOperation();
}

void equalDivOperation()
{
	pair <int, int> operand2 = getOperand();
	pair <int, int> operand1 = getOperand();

	pair <int, int> temp = readVal(operand1);
	temp.first += 2;

	operandsMagazin.push(operand1);
	operandsMagazin.push(temp);
	operandsMagazin.push(operand2);

	divOperation();
	equalOperation();
}

void equalModOperation()
{
	pair <int, int> operand2 = getOperand();
	pair <int, int> operand1 = getOperand();

	pair <int, int> temp = readVal(operand1);
	temp.first += 2;

	operandsMagazin.push(operand1);
	operandsMagazin.push(temp);
	operandsMagazin.push(operand2);

	modOperation();
	equalOperation();
}

void equalBitAndOperation()
{
	pair <int, int> operand2 = getOperand();
	pair <int, int> operand1 = getOperand();

	pair <int, int> temp = readVal(operand1);
	temp.first += 2;

	operandsMagazin.push(operand1);
	operandsMagazin.push(temp);
	operandsMagazin.push(operand2);

	bitAndOperation();
	equalOperation();
}

void equalBitOrOperation()
{
	pair <int, int> operand2 = getOperand();
	pair <int, int> operand1 = getOperand();

	pair <int, int> temp = readVal(operand1);
	temp.first += 2;

	operandsMagazin.push(operand1);
	operandsMagazin.push(temp);
	operandsMagazin.push(operand2);

	bitOrOperation();
	equalOperation();
}

void equalBitXorOperation()
{
	pair <int, int> operand2 = getOperand();
	pair <int, int> operand1 = getOperand();

	pair <int, int> temp = readVal(operand1);
	temp.first += 2;

	operandsMagazin.push(operand1);
	operandsMagazin.push(temp);
	operandsMagazin.push(operand2);

	bitXorOperation();
	equalOperation();
}

void equalBitRightOperation()
{
	pair <int, int> operand2 = getOperand();
	pair <int, int> operand1 = getOperand();

	pair <int, int> temp = readVal(operand1);
	temp.first += 2;

	operandsMagazin.push(operand1);
	operandsMagazin.push(temp);
	operandsMagazin.push(operand2);

	bitRightOperation();
	equalOperation();
}

void equalBitLeftOperation()
{
	pair <int, int> operand2 = getOperand();
	pair <int, int> operand1 = getOperand();

	pair <int, int> temp = readVal(operand1);
	temp.first += 2;

	operandsMagazin.push(operand1);
	operandsMagazin.push(temp);
	operandsMagazin.push(operand2);

	bitLeftOperation();
	equalOperation();
}


void plusPlusOperation()
{
	pair <int, int> operand2 = make_pair(2, 1);
	pair <int, int> operand1 = getOperand();

	pair <int, int> temp = readVal(operand1);
	temp.first += 2;

	operandsMagazin.push(operand1);
	operandsMagazin.push(temp);
	operandsMagazin.push(operand2);

	PlusOperation();
	equalOperation();
}

void minusMinusOperation()
{
	pair <int, int> operand2 = make_pair(2, -1);
	pair <int, int> operand1 = getOperand();

	pair <int, int> temp = readVal(operand1);
	temp.first += 2;

	operandsMagazin.push(operand1);
	operandsMagazin.push(temp);
	operandsMagazin.push(operand2);

	PlusOperation();
	equalOperation();
}


void andOperation()
{
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = readVal(getOperand());

	if (operand1.first == 0)
		if (operand2.first == 0)
		{
			if (operand1.second == 0 || operand2.second == 0)
				operandsMagazin.push(make_pair(2, 0));
			else
				operandsMagazin.push(make_pair(2, 1));
			return;
		}
		
	return interpritatorError();
}

void orOperation()
{
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = readVal(getOperand());

	if (operand1.first == 0)
		if (operand2.first == 0)
		{
			if (operand1.second == 1 || operand2.second == 1)
				operandsMagazin.push(make_pair(2, 1));
			else
				operandsMagazin.push(make_pair(2, 0));
			return;
		}

	return interpritatorError();
}

void equalEqualOperation()
{
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = readVal(getOperand());

	if (operand1.first == 0)
		if (operand2.first == 0)
		{
			if (operand1.second != operand2.second)
				operandsMagazin.push(make_pair(2, 0));
			else
				operandsMagazin.push(make_pair(2, 1));
			return;
		}

	return interpritatorError();
}

void notEqualOperation()
{
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = readVal(getOperand());

	if (operand1.first == 0)
		if (operand2.first == 0)
		{
			if (operand1.second == operand2.second)
				operandsMagazin.push(make_pair(2, 0));
			else
				operandsMagazin.push(make_pair(2, 1));
			return;
		}

	return interpritatorError();
}

void moreOperation()
{
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = readVal(getOperand());

	if (operand1.first == 0)
		if (operand2.first == 0)
		{
			if (operand1.second > operand2.second)
				operandsMagazin.push(make_pair(2, 1));
			else
				operandsMagazin.push(make_pair(2, 0));
			return;
		}
	double a1 = 0;
	double a2 = 0;

	if (operand1.first == 0)
		a1 = operand1.second;
	else
		a1 = getDoubleConstant(operand1.second);

	if (operand2.first == 0)
		a2 = operand2.second;
	else
		a2 = getDoubleConstant(operand2.second);

	if (a1 > a2)
		operandsMagazin.push(make_pair(2, 1));
	else
		operandsMagazin.push(make_pair(2, 0));
}

void moreEqualOperation()
{
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = readVal(getOperand());

	if (operand1.first == 0)
		if (operand2.first == 0)
		{
			if (operand1.second >= operand2.second)
				operandsMagazin.push(make_pair(2, 1));
			else
				operandsMagazin.push(make_pair(2, 0));
			return;
		}

	double a1 = 0;
	double a2 = 0;

	if (operand1.first == 0)
		a1 = operand1.second;
	else
		a1 = getDoubleConstant(operand1.second);

	if (operand2.first == 0)
		a2 = operand2.second;
	else
		a2 = getDoubleConstant(operand2.second);

	if (a1 >= a2)
		operandsMagazin.push(make_pair(2, 1));
	else
		operandsMagazin.push(make_pair(2, 0));
	
}

void lessOperation()
{
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = readVal(getOperand());

	if (operand1.first == 0)
		if (operand2.first == 0)
		{
			if (operand1.second < operand2.second)
				operandsMagazin.push(make_pair(2, 1));
			else
				operandsMagazin.push(make_pair(2, 0));
			return;
		}
	double a1 = 0;
	double a2 = 0;

	if (operand1.first == 0)
		a1 = operand1.second;
	else
		a1 = getDoubleConstant(operand1.second);

	if (operand2.first == 0)
		a2 = operand2.second;
	else
		a2 = getDoubleConstant(operand2.second);

	if (a1 < a2)
		operandsMagazin.push(make_pair(2, 1));
	else
		operandsMagazin.push(make_pair(2, 0));
}

void lessEqualOperation()
{
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = readVal(getOperand());

	if (operand1.first == 0)
		if (operand2.first == 0)
		{
			if (operand1.second <= operand2.second)
				operandsMagazin.push(make_pair(2, 1));
			else
				operandsMagazin.push(make_pair(2, 0));
			return;
		}
	double a1 = 0;
	double a2 = 0;

	if (operand1.first == 0)
		a1 = operand1.second;
	else
		a1 = getDoubleConstant(operand1.second);

	if (operand2.first == 0)
		a2 = operand2.second;
	else
		a2 = getDoubleConstant(operand2.second);

	if (a1 <= a2)
		operandsMagazin.push(make_pair(2, 1));
	else
		operandsMagazin.push(make_pair(2, 0));
}


void jumpOperation()
{
	pair <int, int> operand1 = getOperand();

	if (operand1.first == 5)
		currenElementOPS = operand1.second;
	else
		interpritatorError();
}

void jumpFalseOperation()
{
	pair <int, int> operand2 = getOperand();
	pair <int, int> operand1 = readVal(getOperand());

	if (operand2.first == 5 && operand1.first == 0)
	{
		if (operand1.second == 0)
			currenElementOPS = operand2.second;
	}
	else
		interpritatorError();
}


void mem1Operation()
{
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = getOperand();

	if (operand1.first == 1 && operand2.first == 0)
	{
		pair <int, int> t = indexOfVar[getNameAndIndexes(getStringConstant(operand1.second)).first];
		if (t.first == 1 || t.first == 4)
		{
			if (t.first == 1)
				int1Var[t.second].resize(operand2.second);
			if (t.first == 4)
				real1Var[t.second].resize(operand2.second);
		}
		else
			interpritatorError();
	}
	else
		interpritatorError();
}

void mem2Operation()
{
	pair <int, int> operand3 = readVal(getOperand());
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = getOperand();

	if (operand1.first == 1 && operand2.first == 0 && operand3.first == 0)
	{
		pair <int, int> t = indexOfVar[getNameAndIndexes(getStringConstant(operand1.second)).first];
		if (t.first == 2 || t.first == 5)
		{
			if (t.first == 2)
				int2Var[t.second].resize(operand2.second, vector <int> (operand3.second));
			
			if (t.first == 5)
				real2Var[t.second].resize(operand2.second, vector <double>(operand3.second));
		}
		else
			interpritatorError();
	}
	else
		interpritatorError();
}

void readOperation()
{
	pair <int, int> operand1 = getOperand();

	if (operand1.first == 1)
	{
		pair <int, int> t = indexOfVar[getNameAndIndexes(getStringConstant(operand1.second)).first];
		if (t.first < 3)
		{
			int c;
			cin >> c;
			writeVal(operand1, make_pair(0, c));
		}
		else
		{
			double c;
			cin >> c;
			writeVal(operand1, make_pair(1, pushDoubleConstant(c)));
		}
	}
	else
		interpritatorError();
}

void index1Operation()
{
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = getOperand();

	if (operand1.first == 1 && operand2.first == 0)
	{
		stringstream ss;
		ss << operand2.second;
		string s;
		ss >> s;
		operandsMagazin.push(make_pair(1, pushStringConstant(getStringConstant(operand1.second) + "," + s)));
	}
	else
		interpritatorError();
}

void index2Operation()
{
	pair <int, int> operand3 = readVal(getOperand());
	pair <int, int> operand2 = readVal(getOperand());
	pair <int, int> operand1 = getOperand();

	if (operand1.first == 1 && operand3.first == 0 && operand2.first == 0)
	{
		stringstream ss;
		ss << operand2.second;
		string s;
		ss >> s;
		s += ",";
		ss << operand3.second;
		ss >> s;
		operandsMagazin.push(make_pair(1, pushStringConstant(getStringConstant(operand1.second) + "," + s)));
	}
	else
		interpritatorError();
}

/////////////////////////////////////////////

void initInterpritator()
{
	operationFunctions[nameOfOperationToIndex["wr"]] = writeOperation;
	operationFunctions[nameOfOperationToIndex["rd"]] = readOperation;

	operationFunctions[nameOfOperationToIndex["i"]] = createIntVarOperation;
	operationFunctions[nameOfOperationToIndex["i1"]] = createInt1VarOperation;
	operationFunctions[nameOfOperationToIndex["i2"]] = createInt2VarOperation;
	operationFunctions[nameOfOperationToIndex["r"]] = createRealVarOperation;
	operationFunctions[nameOfOperationToIndex["r1"]] = createReal1VarOperation;
	operationFunctions[nameOfOperationToIndex["r2"]] = createReal2VarOperation;

	operationFunctions[nameOfOperationToIndex["+"]] = PlusOperation;
	operationFunctions[nameOfOperationToIndex["-"]] = minusOperation;
	operationFunctions[nameOfOperationToIndex["*"]] = multOperation;
	operationFunctions[nameOfOperationToIndex["/"]] = divOperation;
	operationFunctions[nameOfOperationToIndex["%"]] = modOperation;
	operationFunctions[nameOfOperationToIndex["&"]] = bitAndOperation;
	operationFunctions[nameOfOperationToIndex["|"]] = bitOrOperation;
	operationFunctions[nameOfOperationToIndex["^"]] = bitXorOperation;
	operationFunctions[nameOfOperationToIndex[">>"]] = bitRightOperation;
	operationFunctions[nameOfOperationToIndex["<<"]] = bitLeftOperation;

	operationFunctions[nameOfOperationToIndex["="]] = equalOperation;
	operationFunctions[nameOfOperationToIndex["+="]] = equalPlusOperation;
	operationFunctions[nameOfOperationToIndex["-="]] = equalMinusOperation;
	operationFunctions[nameOfOperationToIndex["*="]] = equalMultOperation;
	operationFunctions[nameOfOperationToIndex["/="]] = equalDivOperation;
	operationFunctions[nameOfOperationToIndex["%="]] = equalModOperation;
	operationFunctions[nameOfOperationToIndex["&="]] = equalBitAndOperation;
	operationFunctions[nameOfOperationToIndex["|="]] = equalBitOrOperation;
	operationFunctions[nameOfOperationToIndex["^="]] = equalBitXorOperation;
	operationFunctions[nameOfOperationToIndex[">>="]] = equalBitRightOperation;
	operationFunctions[nameOfOperationToIndex["<<="]] = equalBitLeftOperation;

	operationFunctions[nameOfOperationToIndex["u-"]] = unarMinusOperation;
	operationFunctions[nameOfOperationToIndex["++"]] = plusPlusOperation;
	operationFunctions[nameOfOperationToIndex["--"]] = minusMinusOperation;

	operationFunctions[nameOfOperationToIndex["&&"]] = andOperation;
	operationFunctions[nameOfOperationToIndex["||"]] = orOperation;
	operationFunctions[nameOfOperationToIndex["=="]] = equalEqualOperation;
	operationFunctions[nameOfOperationToIndex["!="]] = notEqualOperation;
	operationFunctions[nameOfOperationToIndex[">"]] = moreOperation;
	operationFunctions[nameOfOperationToIndex[">="]] = moreEqualOperation;
	operationFunctions[nameOfOperationToIndex["<"]] = lessOperation;
	operationFunctions[nameOfOperationToIndex["<="]] = lessEqualOperation;

	operationFunctions[nameOfOperationToIndex["j"]] = jumpOperation;
	operationFunctions[nameOfOperationToIndex["jf"]] = jumpFalseOperation;
	operationFunctions[nameOfOperationToIndex["m1"]] = mem1Operation;
	operationFunctions[nameOfOperationToIndex["m2"]] = mem2Operation;
	operationFunctions[nameOfOperationToIndex["ix1"]] = index1Operation;
	operationFunctions[nameOfOperationToIndex["ix2"]] = index2Operation;
}

void interpritateOPS()
{
	for (; currenElementOPS < OPS.size(); currenElementOPS++)
	{
		if (OPS[currenElementOPS].first == 0)
			operationFunctions[OPS[currenElementOPS].second]();
		else
			operandsMagazin.push(OPS[currenElementOPS]);
	}
}