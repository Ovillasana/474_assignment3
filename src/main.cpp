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
		if (cFile.peek() == std::ifstream::traits_type::eof()) {
			cout << "Your input file is empty" << endl;
			return 0;
		}
		if (conv.readInputFile(cFile, verilogFile) == false) {
			cout << "Error in your input file." << endl;
			cFile.close();
			return 0;
		}
		cFile.close();
		cFile.open(argv[1]);

		if (conv.readInFile(cFile, verilogFile, latency)) {
			cout << "SUCCESS" << endl;
			return EXIT_SUCCESS;
		}
		else {
			cFile.close();
			verilogFile.close();
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