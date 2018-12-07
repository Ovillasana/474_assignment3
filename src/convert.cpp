#include "convert.h"
#include "node.h"

vector<string> lines;
vector<Node> nodes;
vector<string> outLines;
vector<string> inputOutputs;
vector<string> vars;
vector<string> inputs;
vector<string> outputs;
vector<string> regs;

string opp(string line);
int cycle(string line);
string convertTypes(string in);

Convert::Convert() {} //default constructor

bool Convert::readInFile(ifstream &input, ofstream &output, int latency) {
	int totalLines = 0;
	int count = 0;

	for (string inLine; getline(input, inLine); totalLines++) {
		string line = inLine;
		string outLine = inLine;
		if (line.compare("") != 0) { // check if its not an empty line

			if ((line.find("input") != string::npos) || (line.find("output") != string::npos) || (line.find("variable") != string::npos)) {
				outLine = convertTypes(inLine);
			}
			if (line.find("=") != string::npos) { // operation line
				Node node = Node();
				node.name = "v" + to_string(count);
				count++;
				node.line = line;
				node.out = line.substr(0, line.find(" = ")); // ouput variable
				line.erase(0, line.find(" = ") + 3);
				string opperation = opp(line);	//opperation
				node.operation = opperation;
				node.cycle = cycle(opperation);
				node.in1 = line.substr(0, line.find(opperation) - 1); //input 1
				line.erase(0, line.find(opperation) + opperation.size() + 1);
				node.in2 = line; //input 2
				nodes.push_back(node);
			}

			lines.push_back(outLine);
		}
	}

	//remove spaces from nodes names
	for (unsigned int i = 0; i < nodes.size(); i++) {
		
		for (unsigned int j = 0; j < nodes[i].in2.length(); j++) {
			if (nodes[i].in2[j] == ' ') {
				nodes[i].in2.erase(j, 1);
				j--;
			}
		}
		
	}

	//find precursors
	for (unsigned int i = 0; i < nodes.size(); i++) {
		for (unsigned int j = 0; j < i; j++) {
			if ((nodes[i].in1.find(nodes[j].out) != string::npos) || (nodes[i].in2.find(nodes[j].out) != string::npos)) {
				nodes[i].precursor.push_back(nodes[j].name);
			}
		}
	}
	//find successors
	for (unsigned int i = 0; i < nodes.size(); i++) {
		for (unsigned int j = i; j < nodes.size(); j++) {
			if ((nodes[j].in1.compare(nodes[i].out) == 0) || (nodes[j].in2.compare(nodes[i].out) == 0)){
				//(nodes[j].in1.find(nodes[i].out) != string::npos) || (nodes[j].in2.find(nodes[i].out) != string::npos)) {
				nodes[i].successor.push_back(nodes[j].name);
			}
		}
	}

	if (this->alap(latency)) {
		this->listR(latency);
		this->printHLSM(output, latency);
	}
	else {
		cout << "LIST_R is not achivable with given latency." << endl;
		cout << "Press enter and try again" << endl;
		return false;
	}
}//readInFile

bool Convert::alap(int latency) {
	vector<Node> visited;
	vector<Node> unvisited = nodes;
	string visitedNodes = "";

	for (unsigned int i = latency; i > 0; i--) {
		for (unsigned int j = 0; j < unvisited.size(); j++) {
			if (unvisited[j].successor.size() == 0) {
				unvisited[j].ALAPtime = i + 1 - unvisited[j].cycle;
				visitedNodes += unvisited[j].name + " ";
				visited.push_back(unvisited[j]);
				unvisited.erase(unvisited.begin() + j);
				j--;
			}
			else {
				if (!(visited.size() == 0)) {
					int successorNodesCount = unvisited[j].successor.size();
					int foundCount = 0;
					int latestTime = latency;

					//check if all successor nodes have been visited
					for (unsigned int k = 0; k < unvisited[j].successor.size(); k++) {
						for (unsigned int p = 0; p < visited.size(); p++) {
							if ((unvisited[j].successor[k].find(visited[p].name)) != string::npos) {
								//found = true;

								foundCount++;
								if (visited[p].ALAPtime < latestTime) {
									latestTime = visited[p].ALAPtime;
								}
								break;
							}
						}
					}

					if (foundCount == successorNodesCount) {
						//check the latest successor node 
						if (unvisited[j].cycle > 1) {
							if (i == latestTime) {
								unvisited[j].ALAPtime = latestTime - unvisited[j].cycle;
							}
							else {
								unvisited[j].ALAPtime = latestTime - unvisited[j].cycle;
							}
						}
						else{
								unvisited[j].ALAPtime = latestTime - unvisited[j].cycle;
						}
						visitedNodes += unvisited[j].name + " ";
						visited.push_back(unvisited[j]);
						unvisited.erase(unvisited.begin() + j);
						j--;
					}
				}
			}
		}
	}

	if (visited.size() != nodes.size()) {
		return false;
	}
	
	for (unsigned int i = 0; i < visited.size(); i++) {
		for (unsigned int j = 0; j < nodes.size(); j++) {
			if (visited[i].name.compare(nodes[j].name) == 0) {
				if (visited[i].ALAPtime <= 0) {
					return false;
				}
				nodes[j].ALAPtime = visited[i].ALAPtime;
				break;
			}
		}
	}
}

void Convert::listR(int latency) {
	unsigned int usingMult = 0;
	unsigned int usingAdder = 0;
	unsigned int usingDiv = 0;
	unsigned int usingLogic = 0;
	unsigned int lowestMult = 100;
	unsigned int lowestAdder = 100;
	unsigned int lowestDiv = 100;
	unsigned int lowestLogic = 100;
	unsigned int lowestMultNode = 0;
	unsigned int lowestAdderNode = 0;
	unsigned int lowestDivNode = 0;
	unsigned int lowestLogicNode = 0;
	unsigned int soonestTime = 100;
	for (unsigned int j = 0; j < nodes.size(); j++) {
		if (nodes[j].ALAPtime < soonestTime) {
			soonestTime = nodes[j].ALAPtime;
		}
	}
	for (unsigned int j = 0; j < nodes.size(); j++) {
		nodes[j].slack = nodes[j].ALAPtime;
		nodes[j].scheduledTime = 100;
	}
	for (unsigned int i = 1; i <= latency; i++) {
		if (usingMult != 0) {
			usingMult -= 1;
		}
		if (usingAdder != 0) {
			usingAdder -= 1;
		}
		if (usingDiv != 0) {
			usingDiv -= 1;
		}
		if (usingLogic != 0) {
			usingLogic -= 1;
		}
		if (i == 1) {
			for (unsigned int j = 0; j < nodes.size(); j++) {
				if (nodes[j].slack != 0 && nodes[j].slack < 1000) {
					nodes[j].slack -= 1;
				}
				if (nodes[j].slack == 0) {
					nodes[j].scheduledTime = i;
					nodes[j].slack = 1337;
				}
				if (nodes[j].scheduledTime != 100) {
					if (nodes[j].operation == "-" || nodes[j].operation == "+") {
						usingAdder = 1;
					}
					else if (nodes[j].cycle == 2) {
						usingMult = 2;
					}
					else if (nodes[j].cycle == 3) {
						usingDiv = 3;
					}
					else if (nodes[j].cycle == 1) {
						usingLogic = 1;
					}
				}
				if (usingAdder == 0 && (nodes[j].operation == "-" || nodes[j].operation == "+")) {
					if (nodes[j].slack < lowestAdder) {
						lowestAdder = nodes[j].slack;
						lowestAdderNode = j;
					}
				}
				else if (usingMult == 0 && nodes[j].cycle == 2) {
					if (nodes[j].slack < lowestMult) {
						lowestMult = nodes[j].slack;
						lowestMultNode = j;
					}
				}
				else if (usingDiv == 0 && nodes[j].cycle == 3) {
					if (nodes[j].slack < lowestDiv) {
						lowestDiv = nodes[j].slack;
						lowestDivNode = j;
					}
				}
				else if (usingLogic == 0 && nodes[j].cycle == 1) {
					if (nodes[j].slack < lowestLogic) {
						lowestLogic = nodes[j].slack;
						lowestLogicNode = j;
					}
				}
			}
			if (usingAdder == 0) {
				if (nodes[lowestAdderNode].slack <= (soonestTime - 1)) {
					usingAdder = 1;
					nodes[lowestAdderNode].scheduledTime = i;
					nodes[lowestAdderNode].slack = 1337;
				}


			}
			if (usingMult == 0) {
				if (nodes[lowestMultNode].slack <= (soonestTime - 1)) {
					usingMult = 2;
					nodes[lowestMultNode].scheduledTime = i;
					nodes[lowestMultNode].slack = 1337;
				}
			}
			if (usingDiv == 0) {
				if (nodes[lowestDivNode].slack <= (soonestTime - 1)) {
					usingDiv = 3;
					nodes[lowestDivNode].scheduledTime = i;
					nodes[lowestDivNode].slack = 1337;
				}
			}
			if (usingLogic == 0) {
				if (nodes[lowestLogicNode].slack <= (soonestTime - 1)) {
					usingLogic = 1;
					nodes[lowestLogicNode].scheduledTime = i;
					nodes[lowestLogicNode].slack = 1337;
				}
			}
		}
		else {
			for (unsigned int j = 0; j < nodes.size(); j++) {
				if (nodes[j].slack != 0 && nodes[j].slack < 1000) {
					nodes[j].slack -= 1;
				}
				if (nodes[j].slack == 0) {
					nodes[j].scheduledTime = i;
					nodes[j].slack = 1337;
				}
				if (nodes[j].scheduledTime != 100 && nodes[j].slack < 1000) {
					if (nodes[j].operation == "-" || nodes[j].operation == "+") {
						usingAdder = 1;
					}
					else if (nodes[j].cycle == 2) {
						usingMult = 2;
					}
					else if (nodes[j].cycle == 3) {
						usingDiv = 3;
					}
					else if (nodes[j].cycle == 1) {
						usingLogic = 1;
					}
				}
			}
		}

	}
	/*for (unsigned int j = 0; j < nodes.size(); j++) {
		cout << "Node " << j << " ALAP timeslot = " << nodes[j].ALAPtime << endl;
	}
	cout << endl << endl;
	for (unsigned int j = 0; j < nodes.size(); j++) {
		cout << "Node " << j << " final scheduled time occurs at timeslot = " << nodes[j].scheduledTime << endl;
	}*/
}

void Convert::printHLSM(ofstream &output, int latency) {
	cout << endl << endl << endl;

	string moduleName = "`timescale 1ns / 1ps\nmodule HLSM (Clk, Rst, Start, Done, ";
	for (unsigned int i = 0; i < inputOutputs.size(); i++) {
		if (i == inputOutputs.size() - 1) { // erase the last comma
			moduleName += inputOutputs[i];
		} 
		else {
			moduleName += (inputOutputs[i] + ", ");
		}
	}

	moduleName += ");\n";
	outLines.push_back(moduleName);
	
	string inOutReg = "input Clk, Rst, Start;\noutput reg Done;\n";
	for (unsigned int i = 0; i < vars.size(); i++) {
		inOutReg += (vars[i] + ";\n");
	}
	outLines.push_back(inOutReg);

	int bits = ceil(log2(latency + 2)); // to fixed

	string stateInit = "reg [" + to_string(bits) + ":0] State, NextState;\n\n";
	stateInit += "parameter sWait = 0, sFinal = 1;\nparameter";
	for (unsigned int i = 0; i <= latency; i++) {
		if (i == latency) {
			stateInit += " S" + to_string(i + 1) + " = " + to_string(i + 2);
		}
		else {
			stateInit += " S" + to_string(i + 1) + " = " + to_string(i + 2) + ",";
		}
	}
	stateInit += ";\n\n";
	outLines.push_back(stateInit);

	string block = "always@(State, Start, ";
	for (unsigned int i = 0; i < inputs.size(); i++) {
		if (i == inputs.size() - 1) {
			block += inputs[i] + ")begin\n";
		}
		else {
			block += inputs[i] + ", ";
		}
	}
	outLines.push_back(block);

	string state = "case (State)\n\n";

	state += "sWait: begin\nDone = 0;\n";
	/*for (unsigned int i = 0; i < outputs.size(); i++) {
		state += outputs[i] + " = 0;\n";
	}*/
	for (unsigned int i = 0; i < regs.size(); i++) {
		if (regs[i].find(",") != string::npos) {
			string line = regs[i];
			size_t pos = 0;
			string token;
			while (((pos = line.find(",")) != string::npos)) {
				token = line.substr(0, pos);
				state += token + " = 0;\n";
				line.erase(0, pos + 2);
			}
			state += line + " = 0;\n";
		}
		else {
			state += regs[i] + " = 0;\n";
		}
	}
	state += "if (Start == 1)\nNextState <= S1;\nelse\nNextState <= sWait;\nend\n\n";
	outLines.push_back(state);

	int temp = bits;

	for (unsigned int i = 0; i < latency; i++) {
		string newState = "S" + to_string(i+1) + ": begin\n";
		for (unsigned int j = 0; j < nodes.size(); j++) {
			if (nodes[j].scheduledTime == i+1) {
				newState += nodes[j].line + ";\n";
			}
		}
		if (i == latency - 1) {
			newState += "NextState <= sFinal;\nend\n\n";
		}
		else {
			newState += "NextState <= S" + to_string(i + 2) + ";\nend\n\n";
		}
		outLines.push_back(newState);
	}

	string fin = "sFinal: begin\nDone = 1;\nNextState <= sWait;\nend\n\n";
	outLines.push_back(fin);

	string enddd = "endcase\nend\n\nalways@(posedge Clk)begin\nif (Rst == 1) begin\nDone = 0;\n";
		for (unsigned int i = 0; i < regs.size(); i++) {
			if (regs[i].find(",") != string::npos) {
				string line = regs[i];
				size_t pos = 0;
				string token;
				while (((pos = line.find(",")) != string::npos)) {
					token = line.substr(0, pos);
					enddd += token + "= 0;\n";
					line.erase(0, pos + 2);
				}
				enddd += line + " = 0;\n";
			}
			else {
				enddd += regs[i] + " = 0;\n";
			}
		}

	enddd += "State <= sWait;\nend\nelse\nState <= NextState;\nend\n\nendmodule";
	outLines.push_back(enddd);

	for (unsigned int i = 0; i < outLines.size(); i++) {
		output << outLines[i];
	}
}



//extra functions 
string convertTypes(string line) {
	string inString = line;
	string outString = "";

	if (line.find("variable") != string::npos) {
		inString.replace(0, 8, "reg");
	}
	string temp = "";
	//1 BIT
	if ((line.find("Int1") != string::npos) && !(line.find("Int16") != string::npos)) {

		// UNSIGNED INPUT
		if (line.find("UInt1") != string::npos) {
			//truncate up to UInt1
			string splitterSize = "UInt1 ";
			outString += inString.substr(0, inString.find(splitterSize)); // outstring = "input | output | wire"
			inString.erase(0, inString.find(splitterSize) + splitterSize.length());
			temp = inString;
			outString += inString;
		}
		//SIGNED INPUTS
		else {
			//truncate up to Int1
			string splitterSize = "Int1 ";
			outString += inString.substr(0, inString.find(splitterSize)); // outstring = "input | output | wire"
			inString.erase(0, inString.find(splitterSize) + splitterSize.length());
			 temp = inString;
			outString += "signed "; 
			outString += inString;
		}
	}

	// 2 BIT
	else if (line.find("Int2") != string::npos) {

		// UNSIGNED INPUT
		if (line.find("UInt2") != string::npos) {
			//truncate up to UInt2
			string splitterSize = "UInt2 ";
			outString += inString.substr(0, inString.find(splitterSize)); // outstring = "input | output | wire"
			inString.erase(0, inString.find(splitterSize) + splitterSize.length());
			 temp = inString;
			outString += "[1:0] "; 
			outString += inString;
		}
		//SIGNED INPUTS
		else {
			//truncate up to Int2
			string splitterSize = "Int2 ";
			outString += inString.substr(0, inString.find(splitterSize)); // outstring = "input | output | wire"
			string temp = outString;
			inString.erase(0, inString.find(splitterSize) + splitterSize.length());
			 temp = inString;
			outString += "signed [1:0] ";
			outString += inString;
		}
	}

	//8 BIT
	else if (line.find("Int8") != string::npos) {

		// UNSIGNED INPUT
		if (line.find("UInt8") != string::npos) {
			//truncate up to UInt8
			string splitterSize = "UInt8 ";
			outString += inString.substr(0, inString.find(splitterSize)); // outstring = "input | output | wire"
			inString.erase(0, inString.find(splitterSize) + splitterSize.length());
			 temp = inString;
			outString += "[7:0] ";
			outString += inString;
		}
		//SIGNED INPUTS
		else {
			//truncate up to Int8
			string splitterSize = "Int8 ";
			outString += inString.substr(0, inString.find(splitterSize)); // outstring = "input | output | wire"
			inString.erase(0, inString.find(splitterSize) + splitterSize.length());
			 temp = inString;
			outString += "signed [7:0] ";
			outString += inString;
		}
	}

	//16 BIT
	else if (line.find("Int16") != string::npos) {

		// UNSIGNED INPUT
		if (line.find("UInt16") != string::npos) {
			//truncate up to UInt16
			string splitterSize = "UInt16 ";
			outString += inString.substr(0, inString.find(splitterSize)); // outstring = "input | output | wire"
			inString.erase(0, inString.find(splitterSize) + splitterSize.length());
			 temp = inString;
			outString += "[15:0] ";
			outString += inString;
		}
		//SIGNED INPUTS
		else {
			//truncate up to Int16
			string splitterSize = "Int16 ";
			outString += inString.substr(0, inString.find(splitterSize)); // outstring = "input | output | wire"
			inString.erase(0, inString.find(splitterSize) + splitterSize.length());
			 temp = inString;
			outString += "signed [15:0] ";
			outString += inString;
		}
	}

	//32 BIT
	else if (line.find("Int32") != string::npos) {
		// UNSIGNED INPUT
		if (line.find("UInt32") != string::npos) {
			//truncate up to UInt32
			string splitterSize = "UInt32 ";
			outString += inString.substr(0, inString.find(splitterSize)); // outstring = "input | output | wire"
			inString.erase(0, inString.find(splitterSize) + splitterSize.length());
			 temp = inString;
			outString += "[32:0] ";
			outString += inString;
		}
		//SIGNED INPUTS
		else {
			//truncate up to Int32
			string splitterSize = "Int32 ";
			outString += inString.substr(0, inString.find(splitterSize)); // outstring = "input | output | wire"
			inString.erase(0, inString.find(splitterSize) + splitterSize.length());
			 temp = inString;
			outString += "signed [32:0] ";
			outString += inString;
		}
	}

	//64 BIT
	else if (line.find("Int64") != string::npos) {

		// UNSIGNED INPUT
		if (line.find("UInt64") != string::npos) {
			//truncate up to UInt64
			string splitterSize = "UInt64 ";
			outString += inString.substr(0, inString.find(splitterSize)); // outstring = "input | output | wire"
			inString.erase(0, inString.find(splitterSize) + splitterSize.length());
			 temp = inString;
			outString += "[63:0] ";
			outString += inString;
		}
		//SIGNED INPUTS
		else {
			//truncate up to Int64
			string splitterSize = "Int64 ";
			outString += inString.substr(0, inString.find(splitterSize)); // outstring = "input | output | wire"
			inString.erase(0, inString.find(splitterSize) + splitterSize.length());
			 temp = inString;
			outString += "signed [63:0] ";
			outString += inString;
		}
	}

	else {}

	
	if (!(outString.find("reg") != string::npos)) {
		inputOutputs.push_back(temp);
	}
	if (outString.find("input") != string::npos) {
		inputs.push_back(temp);
	}
	if (outString.find("output") != string::npos) {
		outputs.push_back(temp);
	}
	if (outString.find("output") != string::npos) {
		outString.replace(0, 6, "output reg");
	}
	if (outString.find("reg") != string::npos) {
		regs.push_back(temp);
	}
	

	vars.push_back(outString);
	return outString;
}

string opp(string line) {
	if (line.find("+") != string::npos) {
		return "+";
	}
	else if (line.find("-") != string::npos) {
		return "-";
	}
	else if (line.find("*") != string::npos) {
		return "*";
	}
	else if (line.find(">>") != string::npos) {
		return ">>";
	}
	else if (line.find("<<") != string::npos) {
		return "<<";
	}
	else if (line.find("==") != string::npos) {
		return "==";
	}
	else if (line.find("?") != string::npos) {
		return "?";
	}
	else if (line.find(">") != string::npos) {
		return ">";
	}
	else if (line.find("<") != string::npos) {
		return "<";
	}
	else if (line.find("/") != string::npos) {
		return "/";
	}
	else if (line.find("%") != string::npos) {
		return "%";
	}
	else {
		return "err";
	}
}

int cycle(string line) {
	if (line.find("*") != string::npos) {
		return 2;
	}
	else if ((line.find("/") != string::npos) || (line.find("%") != string::npos)) {
		return 3;
	}
	else if (line.find("err") != string::npos) {
		return 300;
	}
	else {
		return 1;
	}
}
