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

enum tokenType { KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST, TOKEN_NULL, TOKEN_ERROR};
// 0 = KEYWORD, 1= SYMBOL, 2 = IDENTIFIER, 3 = INT_CONST, 4 = STRING_CONST

class JackTokenizer
{

	string inputFileName;
	ifstream inputFileStream;
	size_t firstCharacter;
	vector<char> tokens;
	vector<string> tokenString;
	//	size_t lastCharacter;

public:
	string argument1;// arg2;
	int argument2, argument3;
	string currentCommand;
	string token;
	//	string destField, compField, jumpField;
	JackTokenizer() {}
	JackTokenizer(string filename) : inputFileName(filename)
	{
		cout << "Instantiating Tokenizer..." << endl;
		cout << "Opening " << filename << " for parsing..." << endl;
		inputFileStream.open(inputFileName);
		if (inputFileStream.good())
		{
			cout << "File successfully opened..." << endl;
		}
		else
			cout << "Unable to open file" << endl;
	}
	~JackTokenizer()
	{
		cout << "Destroying Tokenizer" << endl;
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

		if (!inputFileStream.eof())
		{
			getline(inputFileStream, currentCommand);
			currentCommand = currentCommand.substr(0, currentCommand.find("//"));
			currentCommand = currentCommand.substr(0, currentCommand.find("/*"));
			size_t lastCharacter = currentCommand.find_last_not_of(" \t\r\n");
			size_t firstCharacter = currentCommand.find_first_not_of(" \t\r\n");

			if (string::npos != firstCharacter) {
				// currentCommand.substr(firstCharacter, lastCharacter - firstCharacter);
				size_t range = lastCharacter - firstCharacter + 1;
				currentCommand = currentCommand.substr(firstCharacter, range);
			}
			//size_t keywordPosition = currentCommand.find_first_of(" ");
			//token = currentCommand.substr(firstCharacter, keywordPosition - 1);
			istringstream iss(currentCommand);
			istreambuf_iterator<char>eos;
			istreambuf_iterator<char>itt(iss);
			string test;
			//istream_iterator<string> eos;
			//istream_iterator<string> itt(iss);
			while (itt != eos)
			{ 
				while ((*itt != ' ') && (*itt != '{') && (*itt != '(') && (*itt != '=') && (*itt != '}') && (*itt != ')') && (*itt != ';') && (*itt != ',') && (*itt != '[') && (*itt != ']') && (*itt != '-') && (*itt != '+') && (*itt != '*') && (*itt != '|') && (*itt != '/') && (*itt != '.'))
				{
					if (*itt == '\"')
					{
						do {
							tokens.push_back(*itt);
							cout << *itt;
							++itt;
						} while (*itt != '\"');
						tokens.push_back(*itt);
						++itt;
					}
					else
					{
						tokens.push_back(*itt);
						cout << *itt;
						++itt;
					}
				}
				test = string(tokens.data(), tokens.size());
				if (test != "")
				{
					tokenString.push_back(test);
				}
				tokens.clear();
				
				tokens.push_back(*itt);
				test = string(tokens.data(), tokens.size());
				if ((test != "") && (test!=" "))
				{
					tokenString.push_back(test);
				}
				tokens.clear();
				++itt;

				
			}
			token = "TEST";
			//tokens = { istream_iterator<string>{iss},istream_iterator<string>{} };

		}
		else
			cout << "Reached end of file.." << endl;
		return;
	}


	tokenType commandType()
	{
		//firstCharacter = currentCommand.find_first_not_of(" \t\r\n");
		if (token.empty())
		{
			return TOKEN_NULL;
		}
		else
		{
			if ((token == "CLASS") || (token == "METHOD") || (token == "FUNCTION") || (token == "CONSTRUCTOR") || (token == "INT") || (token == "BOOLEAN") || (token == "CHAR") || (token == "VOID") || (token == "VAR") || (token == "STATIC") || (token == "FIELD") || (token == "LET") || (token == "DO") || (token == "IF") || (token == "ELSE") || (token == "WHILE") || (token == "RETURN") || (token == "TRUE") || (token == "FALSE") || (token == "NULL") || (token == "THIS"))
			{
				return KEYWORD;
			}
			else if ((token == "PUSH") || (token == "push"))
			{
				return SYMBOL;
			}
			else if ((token == "POP") || (token == "pop"))
			{
				return IDENTIFIER;
			}
			else if ((token == "LABEL") || (token == "label"))
			{
				return INT_CONST;
			}
			else if ((token == "LABEL") || (token == "label"))
			{
				return STRING_CONST;
			}
			else return TOKEN_ERROR;
		}
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
	int functionCounter = 0;
	//	vector<string> symbolNameArray; // declaration
	//	vector<int> symbolAddressArray; // declaration
	//
public:
	string staticVariable;
	CodeWriter(string directory, string filename)
	{
		filename = (filename.substr(0, filename.find("."))); // remove .jack if it is there
		outputFileName = directory + filename + ".vm";
		setFileName(outputFileName);
		writeBootstrapper();
		staticVariable = (filename.substr(0, filename.find(".")));
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
	void writeBootstrapper()
	{
		outputFileStream << "// Initializing boot strapper" << endl;
		outputFileStream << "@256\nD=A\n@SP\nM=D" << endl;
		outputFileStream << "// Calling Sys.init" << endl;
		writeCall("Sys.init", 0);
		return;
	}
	void writeArithmetic(string command)
	{
		if ((command == "add") || (command == "ADD"))
		{
			// ASSEMBLY code to ADD
			//outputFileStream << "@SP\nAM=M-1\nD=M\nA=A-1\nM=M+D" << endl;
			outputFileStream << "@SP\nAM=M-1\nD=M\nA=A-1\nM=M+D\n" << endl;
		}
		else if ((command == "sub") || (command == "SUB"))
		{
			// ASSEMBLY code to SUB
			//outputFileStream << "@SP\nAM=M-1\nD=M\nA=A-1\nM=M-D" << endl;
			outputFileStream << "@SP\nAM=M-1\nD=M\nA=A-1\nM=M-D\n" << endl;
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
			outputFileStream << "\n@SP\nA=M-1\nM=0\n(Jgt" << jgeCounter << ")" << endl;
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
	void writePushPop(tokenType pushOrPop, string segment, int index)
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
		if (pushOrPop == KEYWORD)
		{
			// ASSEMBLY code to PUSH

			//outputFileStream << "@SP\nA=M\nM=D\nA=A+1\nD=A\n@SP\nM=D" << endl;
			outputFileStream << "@SP\nA=M\nM=D\n@SP\nM=M+1" << endl;
		}
		else if (pushOrPop == SYMBOL)
		{
			// ASSEMBLY code to POP
			outputFileStream << "@SP\nM=M-1\nA=M\nD=M\n@R13\nA=M\nM=D" << endl;
		}
		else {}
		return;
	}

	void writeLabel(string label)
	{
		outputFileStream << "(" << staticVariable << "$" << label << ")" << endl;
		return;
	}
	void writeGoto(string label)
	{
		outputFileStream << "@" << staticVariable << "$" << label << "\n0;JMP" << endl;
		return;
	}
	void writeIf(string label)
	{
		outputFileStream << "@SP\nM=M-1\nA=M\nD=M\n@" << staticVariable << "$" << label << "\nD;JNE" << endl;
		return;
	}
	void writeCall(string functionName, int numArgs)
	{
		outputFileStream << "@returnAddress_" << functionCounter << endl;
		outputFileStream << "D=A" << endl;
		outputFileStream << "@SP\nA=M\nM=D\nA=A+1\nD=A\n@SP\nM=D" << endl; // PUSH RETURN ADDRESS
		outputFileStream << "@LCL\nD=M" << endl;
		outputFileStream << "@SP\nA=M\nM=D\nA=A+1\nD=A\n@SP\nM=D" << endl;  // PUSH LCL
		outputFileStream << "@ARG\nD=M" << endl;
		outputFileStream << "@SP\nA=M\nM=D\nA=A+1\nD=A\n@SP\nM=D" << endl; // PUSH ARG
		outputFileStream << "@THIS\nD=M" << endl;
		outputFileStream << "@SP\nA=M\nM=D\nA=A+1\nD=A\n@SP\nM=D" << endl; // PUSH THIS
		outputFileStream << "@THAT\nD=M" << endl;
		outputFileStream << "@SP\nA=M\nM=D\nA=A+1\nD=A\n@SP\nM=D" << endl; // PUSH THAT
		outputFileStream << "@" << numArgs << endl;
		outputFileStream << "D=A\n@5\nD=D+A\n@SP\nD=M-D\n@ARG\nM=D\n@SP\nD=M\n@LCL\nM=D" << endl;
		outputFileStream << "@" << functionName << endl;
		outputFileStream << "0;JMP" << endl;
		outputFileStream << "(returnAddress_" << functionCounter << ")" << endl;
		functionCounter++;
		// push return address  --> using the label declared below
		// push LCL			--> save LCL of the calling function
		// push ARG			--> save ARG of the calling function
		// push THIS		--> save THIS of the calling function
		// push THAT		--> save THAT of the calling function
		// ARG = SP-n-5		--> reposition ARG (n=number of args)
		// LCL = SP			--> reposition LCL
		// goto f			--> transfer control
		// (return-address) --> declare a label for return address

		return;
	}
	void writeReturn(void)
	{
		outputFileStream << "@LCL\nD=M\n@R14\nM=D\n@5\nAD=D-A\nD=M\n@R15\nM=D" << endl;
		outputFileStream << "@SP\nM=M-1\nA=M\nD=M\n@ARG\nA=M\nM=D\n@ARG\nD=M\n@SP\nM=D+1" << endl;
		outputFileStream << "@R14\nM=M-1\nA=M\nD=M\n@THAT\nM=D" << endl; // restore THAT
		outputFileStream << "@R14\nM=M-1\nA=M\nD=M\n@THIS\nM=D" << endl; // restore THIS
		outputFileStream << "@R14\nM=M-1\nA=M\nD=M\n@ARG\nM=D" << endl; // restore ARG
		outputFileStream << "@R14\nM=M-1\nA=M\nD=M\n@LCL\nM=D" << endl; // restore LCL
		outputFileStream << "@R15\nA=M\n0;JMP" << endl; // restore retADDR and jump
														// frame = LCL				// frame is a temp. variable
														// retAddr = *(frame - 5)	// retAddr is a temp. variable
														// *ARG = pop				// repositions the return value for the caller
														// SP = ARG + 1				// restores the caller’s SP
														// THAT = *(frame - 1)		// restores the caller’s THAT
														// THIS = *(frame - 2)		// restores the caller’s THIS
														// ARG = *(frame - 3)		// restores the caller’s ARG
														// LCL = *(frame - 4)		// restores the caller’s LCL
														// goto retAddr				// goto returnAddress
		return;
	}
	void writeFunction(string functionName, int numLocals)
	{
		outputFileStream << "(" << functionName << ")" << endl;
		//	repeat nVars times:
		//	push 0
		for (int i = 0; i < numLocals; i++)
		{
			outputFileStream << "@0\nD=A" << endl;
			outputFileStream << "@SP\nA=M\nM=D\nA=A+1\nD=A\n@SP\nM=D" << endl; // PUSH 0
		}
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
		cout << "You must input a single jack file (.jack) or a directory containing multiple jack files!" << endl;
		return 1;
	}
	//int instructionCounter = 0;
	//int currentSymbolAddress = 0;
	//int symbolCounter = 16;

	string filename(argv[1]);
	string inputFileName, asmFileName;
	string fileLocation;
	vector<string> fileList;
	_finddata_t data;
	size_t last_slash_idx = filename.find_last_of("\\/");
	//size_t fileStartPos = filename.find_last_of("/\\");
	// CODE to check -- is input a directory or single .vm file?
	if (filename.find(".jack") != string::npos)
	{
		const size_t backslashIndex = filename.rfind('\\');
		fileLocation = filename;
		fileLocation.erase(backslashIndex + 1);
		bool directoryCheck = false;
		filename.erase(0, last_slash_idx + 1);
		//const size_t period_idx = filename.rfind('.'); // Remove extension if present.
		//filename.erase(period_idx);
		//inputFileName = (filename.substr(last_slash_idx+1, filename.find(".vm")));
		fileList.push_back(filename);
		asmFileName = filename;
	}
	else
	{
		// Fancy code from http://www.dummies.com/programming/cpp/how-to-get-the-contents-of-a-directory-in-c/ to find files in a directory
		if (filename.back() == '\\')
		{
			const size_t backslashIndex = filename.rfind('\\');
			fileLocation = filename;
			filename.erase(backslashIndex);
			last_slash_idx = filename.find_last_of("\\/");
		}
		else
		{
			fileLocation = filename + "\\";
		}

		asmFileName = filename.substr(last_slash_idx + 1);
		inputFileName = asmFileName;
		filename = filename + "\\*.jack";
		intptr_t ff = _findfirst(filename.c_str(), &data);
		if (ff != -1)
		{
			int res = 0;
			while (res != -1)
			{
				//DumpEntry(data);
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
		cout << "No .jack files were located. Exiting..." << endl;
		return 0;
	}

	CodeWriter CodeWriter(fileLocation, asmFileName);
	string fileNameCurrent;
	for (int i = 0; i < fileList.size(); i++) {
		fileNameCurrent = fileLocation + fileList[i];
		JackTokenizer JackTokenizer(fileNameCurrent);
		CodeWriter.staticVariable = fileList[i];
		CodeWriter.staticVariable = CodeWriter.staticVariable.substr(0, CodeWriter.staticVariable.find("."));
		while (JackTokenizer.hasMoreCommands())
		{
			//	currentCode = "";
			JackTokenizer.advance();
			tokenType currentCommandType = JackTokenizer.commandType();
			if (currentCommandType != TOKEN_NULL)
			{
				if (currentCommandType == KEYWORD)
				{
					CodeWriter.writeArithmetic(JackTokenizer.currentCommand);
				}
				else 
				{
					CodeWriter.writeReturn();
				}
				

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