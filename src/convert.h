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
	bool readInFile(ifstream &input, ofstream &output, int latency);
	bool alap(int latency);
	void listR(int latency);
	void printHLSM(ofstream &output, int latency);
	bool readInputFile(ifstream &input, ofstream &output);	//reads the input file
	void checkInitializationVars(string inputLine);
	bool variableExists(string inputLine);

};

#endif