#pragma once
#ifndef CONVERT_H
#define CONVERT_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include "node.h"

using namespace std;

class Convert {
private:
public:
	Convert();
	void readInFile(ifstream &input, int latency);
	void standardTests();
	string classify(string line);
	string convertIf(ifstream &input);
	string convertFor();
	string latency();
	string standard();
	void alap(int latency);
	
};

#endif