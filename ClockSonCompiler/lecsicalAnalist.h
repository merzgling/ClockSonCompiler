#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <iostream>
#include <cstdio>

using namespace std;

void scanerInit();

void error(int x, int y);

void error();

pair<int, int> nextLex();