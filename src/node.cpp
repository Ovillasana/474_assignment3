#include "node.h"

Node::Node() {

}
Node::Node(string line, string operation, string out, string in1, string in2, int slack, int ALAPtime, int scheduledTime, int cycle) {
	this->line = line;
	this->operation = operation;
	this->out = out;
	this->in1 = in1;
	this->in2 = in2;
	this->slack = slack;
	this->ALAPtime = ALAPtime;
	this->scheduledTime = scheduledTime;
	this->cycle = cycle;
}
