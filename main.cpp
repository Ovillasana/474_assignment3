#include"convert.h"

using namespace std;

int main(int argc, char* argv[]) {

	if (argc != 4) {
		cout << "Incorrect command-line arguments." << endl;
		cout << "Correct format: hlsyn cFile latency verilogFile" << endl;
		return EXIT_FAILURE;
	}

	ifstream cFile;
	cFile.open(argv[1]);
	int latency = stoi(argv[2]);
	ofstream verilogFile;
	verilogFile.open(argv[3]);

	if (cFile.is_open() && verilogFile.is_open()) {
		Convert conv = Convert();
		if (conv.readInFile(cFile, verilogFile, latency)) {
			cin.get();
			return EXIT_SUCCESS;
		}
		else {
			cin.get();
			return EXIT_FAILURE;

		}

	}
	else {
		cout << "Could not open file : " << argv[1] << endl;
		cout << "Could not open file : " << argv[3] << endl;
		cout << "Make sure these files are in the same directory and try again." << endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}