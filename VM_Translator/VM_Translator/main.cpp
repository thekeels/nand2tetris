#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include <vector>
#include <algorithm>
#include <iterator>

#include <io.h>
using namespace std;

enum commandType { C_ARITHMETIC, C_PUSH, C_POP, C_LABEL, C_GOTO, C_IF, C_FUNCTION, C_RETURN, C_CALL, C_ERROR, C_NULL };
// 0 = C_ARITHMETIC, 1 = C_PUSH, 2 = C_POP, 3 = C_LABEL, 4 = C_GOTO, 5 = C_IF, 6 = C_FUNCTION, 7 = C_RETURN, 8 = C_CALL 9 = C_ERROR, 10 = C_NULL

class Parser
{

	string inputFileName;
	ifstream inputFileStream;
	size_t firstCharacter;
	vector<string> tokens;

	//	size_t lastCharacter;

public:
	string argument1;// arg2;
	int argument2, argument3;
	string currentCommand;
	//	string destField, compField, jumpField;
	Parser() {}
	Parser(string filename) : inputFileName(filename)
	{
		cout << "Instantiating Parser..." << endl;
		cout << "Opening " << filename << " for parsing..." << endl;
		inputFileStream.open(inputFileName);
		if (inputFileStream.good())
		{
			cout << "File successfully opened..." << endl;
		}
		else
			cout << "Unable to open file" << endl;
	}
	~Parser()
	{
		cout << "Destroying Parser" << endl;
		inputFileStream.close();
	}

	bool hasMoreCommands()
	{
		if (inputFileStream.eof())
			return false;
		else
			return true;
	}
	void advance()

	{

		//destField = compField = jumpField = "";
		//vector<string> tokens;
		if (!inputFileStream.eof())
		{
			getline(inputFileStream, currentCommand);
			currentCommand = currentCommand.substr(0, currentCommand.find("//"));
			istringstream iss(currentCommand);
			tokens = { istream_iterator<string>{iss},istream_iterator<string>{} };

		}
		else
			cout << "Reached end of file.." << endl;
		return;
	}

		commandType commandType()
		{
			//firstCharacter = currentCommand.find_first_not_of(" \t\r\n");
			if (tokens.empty())
			{
				return C_NULL;
			}
			else
			{
				if ((tokens[0] == "ADD") || (tokens[0] == "SUB") || (tokens[0] == "add") || (tokens[0] == "sub") || (tokens[0] == "EQ") || (tokens[0] == "eq") || (tokens[0] == "GT") || (tokens[0] == "gt") || (tokens[0] == "LT") || (tokens[0] == "lt") || (tokens[0] == "AND") || (tokens[0] == "and") || (tokens[0] == "NOT") || (tokens[0] == "not") || (tokens[0] == "OR") || (tokens[0] == "or") || (tokens[0] == "NEG") || (tokens[0] == "neg"))
				{
					return C_ARITHMETIC;
				}
				else if ((tokens[0] == "PUSH") || (tokens[0] == "push"))
				{
					return C_PUSH;
				}
				else if ((tokens[0] == "POP") || (tokens[0] == "pop"))
				{
					return C_POP;
				}
				else if ((tokens[0] == "LABEL") || (tokens[0] == "label"))
				{
					return C_LABEL;
				}
				else if ((tokens[0] == "GOTO") || (tokens[0] == "goto"))
				{
					return C_GOTO;
				}
				else if ((tokens[0] == "IF-GOTO") || (tokens[0] == "if-goto"))
				{
					return C_IF;
				}
				else if ((tokens[0] == "FUNCTION") || (tokens[0] == "function"))
				{
					return C_FUNCTION;
				}
				else if ((tokens[0] == "RETURN") || (tokens[0] == "return"))
				{
					return C_RETURN;
				}
				else if ((tokens[0] == "CALL") || (tokens[0] == "call"))
				{
					return C_CALL;
				}
				else return C_ERROR;
			}
		}
		string arg1(void)
		{
			argument1 = tokens[1];
			return argument1;
		}
		int arg2(void)
		{
			argument2 = stoi(tokens[2]);
			return argument2;
		}

};
class CodeWriter
{
	int localBase = 1;
	int argumentBase = 2;
	int thisBase = 3;
	int thatBase = 4;
	string outputFileName;
	ofstream outputFileStream;
	int jeqCounter = 0;
	int jleCounter = 0;
	int jgeCounter = 0;
	
//	vector<string> symbolNameArray; // declaration
//	vector<int> symbolAddressArray; // declaration
//
public:
	string staticVariable;
	CodeWriter(string filename) 
	{
		outputFileName = (filename.substr(0, filename.find("."))) + ".asm";
		setFileName(outputFileName);
		staticVariable = filename;
	}
	void setFileName(string outputFileName)
	{
		outputFileStream.open(outputFileName, ofstream::out);
		return;
	}
	void Close()
	{
		outputFileStream.close();
		return;
	}
	void writeArithmetic(string command)
	{
		if ((command == "add") || (command == "ADD"))
		{
			// ASSEMBLY code to ADD
			outputFileStream << "@SP\nAM=M-1\nD=M\nA=A-1\nM=M+D" << endl;
		}
		else if ((command == "sub") || (command == "SUB"))
		{
			// ASSEMBLY code to SUB
			outputFileStream << "@SP\nAM=M-1\nD=M\nA=A-1\nM=M-D" << endl;
		}
		else if ((command == "EQ") || (command == "eq"))
		{
			// ASSEMBLY code to EQ
			outputFileStream << "@SP\nAM=M-1\nD=M\nA=A-1\nD=M-D\nM=-1\n@Jeq" << jeqCounter << "\nD;JEQ" << endl;
			outputFileStream << "\n@SP\nA=M-1\nM=0\n(Jeq" << jeqCounter << ")" << endl;
			jeqCounter++;
		}
		else if ((command == "GT") || (command == "gt"))
		{
			// ASSEMBLY code to GT
			outputFileStream << "@SP\nAM=M-1\nD=M\nA=A-1\nD=M-D\nM=-1\n@Jgt" << jgeCounter << "\nD;JGT" << endl;
			outputFileStream <<"\n@SP\nA=M-1\nM=0\n(Jgt" << jgeCounter << ")" << endl;
			jgeCounter++;
		}
		else if ((command == "LT") || (command == "lt"))
		{
			// ASSEMBLY code to LT
			outputFileStream << "@SP\nAM=M-1\nD=M\nA=A-1\nD=M-D\nM=-1\n@Jlt" << jleCounter << "\nD;JLT" << endl;
			outputFileStream << "\n@SP\nA=M-1\nM=0\n(Jlt" << jleCounter << ")" << endl;
			jleCounter++;
		}
		else if ((command == "AND") || (command == "and"))
		{
			// ASSEMBLY code to and
			outputFileStream << "@SP\nAM=M-1\nD=M\nA=A-1\nM=M&D" << endl;
		}
		else if ((command == "OR") || (command == "or"))
		{
			// ASSEMBLY code to OR
			outputFileStream << "@SP\nAM=M-1\nD=M\nA=A-1\nM=M|D" << endl;
		}
		else if ((command == "NEG") || (command == "neg"))
		{
			// ASSEMBLY code to NEG
			outputFileStream << "@SP\nA=M-1\nM=-M" << endl;
		}
		else if ((command == "NOT") || (command == "not"))
		{
			// ASSEMBLY code to not
			outputFileStream << "@SP\nA=M-1\nM=!M" << endl;
		}
		else {}
		return;
	}
	void writePushPop(commandType pushOrPop, string segment, int index)
	{
		string segmentTranslated;
		int segmentBaseAddress = 0;
		int segmentAddress;

		// static, this, local, argument, that, constant, pointer, or temp
		if (segment == "static")
		{
			outputFileStream << "@" << staticVariable << "." << index << endl;
			outputFileStream << "D=A\n@R13\nM=D\nA=M\nD=M" << endl;
		}
		else if (segment == "this")
		{
			outputFileStream << "@THIS\nD=M\n@" << index << endl;
			outputFileStream << "D=A+D\n@R13\nM=D\nA=M\nD=M" << endl;
		}
		else if (segment == "local")
		{
			outputFileStream << "@LCL\nD=M\n@" << index << endl;
			outputFileStream << "D=A+D\n@R13\nM=D\nA=M\nD=M" << endl;
		}
		else if (segment == "argument")
		{
			outputFileStream << "@ARG\nD=M\n@" << index << endl;
			outputFileStream << "D=A+D\n@R13\nM=D\nA=M\nD=M" << endl;
		}
		else if (segment == "that")
		{
			outputFileStream << "@THAT\nD=M\n@" << index << endl;
			outputFileStream << "D=A+D\n@R13\nM=D\nA=M\nD=M" << endl;
		}
		else if (segment == "constant")
		{
			segmentAddress = index;
			outputFileStream << "@" << segmentAddress << endl;
			outputFileStream << "D=A\n";
		}
		else if (segment == "pointer")
		{
			outputFileStream << "@3\nD=A\n@" << index << endl;
			outputFileStream << "D=A+D\n@R13\nM=D\nA=M\nD=M" << endl;
		}
		else if (segment == "temp")
		{
			outputFileStream << "@5\nD=A\n@" << index << endl;
			outputFileStream << "D=A+D\n@R13\nM=D\nA=M\nD=M" << endl;
		}
		else
		{
			// segmentBaseAddress = 256;
		}
		segmentAddress = segmentBaseAddress + index;
		if (pushOrPop == C_PUSH)
		{
			// ASSEMBLY code to PUSH

			outputFileStream << "@SP\nA=M\nM=D\nA=A+1\nD=A\n@SP\nM=D" << endl;
		}
		else if (pushOrPop == C_POP)
		{
			// ASSEMBLY code to POP
			outputFileStream << "@SP\nM=M-1\nA=M\nD=M\n@R13\nA=M\nM=D" << endl; 
		}
		else {}
		return;
	}
	~CodeWriter()
	{	}
};

int main(int argc, const char *argv[])  // alternatively: int main(int argc, char** argv)
										// argc = number of strings pointed to by argv
										// argv holds the array of inputs to the program, argv[0] holds "Assembler", argv[1] holds command line input
{
	void DumpEntry(_finddata_t &data);
	if (argc != 2)
	{
		cout << "You must input a single vm file (.vm) or a directory containing multiple vm files!" << endl;
		return 1;
	}
	//int instructionCounter = 0;
	//int currentSymbolAddress = 0;
	//int symbolCounter = 16;

	string filename(argv[1]);
	string inputFileName, inputDirectory;

	vector<string> fileList;
	_finddata_t data;
	size_t last_slash_idx = filename.find_last_of("\\/");
	//size_t fileStartPos = filename.find_last_of("/\\");
	// CODE to check -- is input a directory or single .vm file?
	if (filename.find(".vm")!=string::npos)
	{

		bool directoryCheck = false;
		filename.erase(0, last_slash_idx + 1);
		//const size_t period_idx = filename.rfind('.'); // Remove extension if present.
		//filename.erase(period_idx);
		//inputFileName = (filename.substr(last_slash_idx+1, filename.find(".vm")));
		fileList.push_back(filename);
		inputFileName = filename;
	}
	else
	{
		// Fancy code from http://www.dummies.com/programming/cpp/how-to-get-the-contents-of-a-directory-in-c/ to find files in a directory
		if (filename.back() = '\\')
		{
			const size_t backslashIndex = filename.rfind('\\');
			filename.erase(backslashIndex);
			last_slash_idx = filename.find_last_of("\\/");
		}
		inputDirectory = filename.substr(last_slash_idx+1);
		inputFileName = inputDirectory;
		filename = filename + "\\*.vm";
		intptr_t ff = _findfirst(filename.c_str(), &data);
		if (ff != -1)
		{
			int res = 0;
			while (res != -1)
			{
				DumpEntry(data);
				//string temp = data.name;
				//temp = temp.substr(0, temp.find(".vm"));
				fileList.push_back(data.name);
				res = _findnext(ff, &data);
			}
			_findclose(ff);
		}
	}

	if (fileList.size() == 0)
	{
		cout << "No .vm files were located. Exiting..." << endl;
		return 0;
	}

	
	CodeWriter CodeWriter(inputFileName);
	
	for (int i = 0; i < fileList.size(); i++) {
		Parser VMParser(fileList[i]);
		CodeWriter.staticVariable = fileList[i];
		CodeWriter.staticVariable = CodeWriter.staticVariable.substr(0, CodeWriter.staticVariable.find("."));
		while (VMParser.hasMoreCommands())
		{
			//	currentCode = "";
			VMParser.advance();
			commandType currentCommandType = VMParser.commandType();
			if (currentCommandType != C_NULL)
			{
				if (currentCommandType == C_ARITHMETIC)
				{
					CodeWriter.writeArithmetic(VMParser.currentCommand);
				}
				else if (currentCommandType == C_RETURN) {}
				else VMParser.arg1();
				if ((currentCommandType == C_PUSH) || (currentCommandType == C_POP) || (currentCommandType == C_FUNCTION) || (currentCommandType == C_CALL))
				{
					VMParser.arg2();
				}
				else {}

				if ((currentCommandType == C_PUSH) || (currentCommandType == C_POP))
				{
					CodeWriter.writePushPop(currentCommandType, VMParser.argument1, VMParser.argument2);
				}

				else {}

			}
		}
	}
	CodeWriter.Close();

	return 0;
}

void DumpEntry(_finddata_t &data)
{
	//string createtime(ctime(&data.time_create));
	//cout << Chop(createtime) << "t";
	cout << data.size << "t";
	if ((data.attrib & _A_SUBDIR) == _A_SUBDIR)
	{
		cout << "[" << data.name << "]" << endl;
	}
	else
	{
		cout << data.name << endl;
	}
}