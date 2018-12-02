#ifndef NODE_H
#define NODE_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include "node.h"
#include "convert.h"


using namespace std;

class Node {

public:
	string name;
	string line;
	string operation;
	string out;
	string in1;
	string in2;
	int slack;
	int ALAPtime;
	int scheduledTime;
	int cycle;
	vector<string> precursor;
	vector<string> successor;
	Node();
	Node(string line, string operation, string out, string in1, string in2, int slack, int ALAPtime, int scheduledTime, int cycle);

};

#endif //NODE_H