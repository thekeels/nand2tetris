#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
#include <algorithm>
using namespace std;

enum commandType { A_Command, C_Command, L_Command, Comment, Empty_Command}; // 0 = A_Command, 1 = C_Command, 2 = L_Command, 3 = Comment, 4 = Empty_Command
class Parser
{
	
	string inputFileName;
	ifstream inputFileStream;
	size_t firstCharacter;
	size_t lastCharacter;

public:
	string currentCommand;
	string destField, compField, jumpField;
	Parser() {}
	Parser(string filename) : inputFileName(filename) {
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
	void reset()
	{
		inputFileStream.clear();
		inputFileStream.seekg(0, ios::beg);
		return;
	}
	bool constantCheck(string &A_commandSymbol)
	{
		if ((A_commandSymbol.at(0) == '0') || (A_commandSymbol.at(0) == '1') || (A_commandSymbol.at(0) == '2') \
			|| (A_commandSymbol.at(0) == '3') || (A_commandSymbol.at(0) == '4') || (A_commandSymbol.at(0) == '5') \
			|| (A_commandSymbol.at(0) == '6') || (A_commandSymbol.at(0) == '7') || (A_commandSymbol.at(0) == '8') \
			|| (A_commandSymbol.at(0) == '9'))
		{
			return true;
		}
		else return false;
	}
	bool hasMoreCommands()
	{
		if (inputFileStream.eof())
			return false;
		else
			return true;
	}
	string advance()

	{
		destField = compField = jumpField = "";
		if (!inputFileStream.eof())
		{
			getline(inputFileStream, currentCommand);
			currentCommand = currentCommand.substr(0, currentCommand.find("//"));
			currentCommand.erase(remove_if(currentCommand.begin(), currentCommand.end(), isspace), currentCommand.end());
		}
		else
			cout << "Reached end of file.." << endl;
		return currentCommand;

	}
	commandType commandType()
	{
		firstCharacter = currentCommand.find_first_not_of(" \t\r\n");
		if (string::npos == firstCharacter)
		{
			return Empty_Command;
		}
		if (currentCommand.at(firstCharacter) == '@')
		{
			return A_Command;
		}
		else if (currentCommand.at(firstCharacter) == '(')
		{
			return L_Command;
		}
		else if (currentCommand.at(firstCharacter) == '/')
		{
			return Comment;
		}
		else return C_Command;

	}
	string symbol()
	{
		currentCommand.erase(remove_if(currentCommand.begin(), currentCommand.end(), isspace),currentCommand.end()); 
		lastCharacter = currentCommand.find_last_not_of(" \t\r\n");
		if (currentCommand.at(lastCharacter) == ')')
		{
			return currentCommand.substr(1, lastCharacter-1);
		}
		else
		{
			return currentCommand.substr(1, lastCharacter);
		}
	}
	string dest()
	{
		string delimiter = "=";
		size_t pos = 0;
		while ((pos = currentCommand.find(delimiter)) != string::npos) {
			destField = currentCommand.substr(0, pos);
			currentCommand.erase(0, pos + delimiter.length());
		}
		return destField;
	}

	string comp()
	{
		string delimiter = ";";
		size_t pos = 0;
		if (currentCommand.find(delimiter) == string::npos)
		{
			compField = currentCommand;
			currentCommand = "";
			return compField;
		}
		else
		{
			while ((pos = currentCommand.find(delimiter)) != string::npos) {
				compField = currentCommand.substr(0, pos);
				currentCommand.erase(0, pos + delimiter.length());
			}
			return compField;
		}
	}
	string jump()
	{
		return currentCommand;
	}
};

class SymbolTable
{
	vector<string> symbolNameArray; // declaration
	vector<int> symbolAddressArray; // declaration

public:
	SymbolTable() {
		symbolNameArray = { "R0","R1","R2","R3","R4","R5","R6","R7","R8","R9","R10","R11","R12","R13","R14","R15",\
			"SP","LCL","ARG","THIS","THAT","SCREEN","KBD" };
		symbolAddressArray = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,1,2,3,4,16384,24576 };
	}
	~SymbolTable()
	{

	}
	void addEntry(string inputSymbol, int symbolAddress)
	{
		symbolNameArray.push_back(inputSymbol);
		symbolAddressArray.push_back(symbolAddress);
		return;
	}
	void updateEntry(string inputSymbol, int symbolAddress)
	{
		vector<string>::iterator iter = find(symbolNameArray.begin(), symbolNameArray.end(), inputSymbol);
		size_t index = std::distance(symbolNameArray.begin(), iter);
		symbolAddressArray.at(index) = symbolAddress;
		return;
	}
	bool contains(string inputSymbol)
	{
		if (find(symbolNameArray.begin(), symbolNameArray.end(), inputSymbol) != symbolNameArray.end())
		{
			return true; // Element in vector.
		}
		else 
		return false;
	}
	int getAddress(string inputSymbol)
	{
		vector<string>::iterator iter = find(symbolNameArray.begin(), symbolNameArray.end(), inputSymbol);
		size_t index = std::distance(symbolNameArray.begin(), iter);
		if (index == symbolNameArray.size())
		{
			//invalid
		}
		return symbolAddressArray.at(index);
	}

};

class Code
{


public:
	Code() {}
	string dest(string destFieldInput)
	{
		if (destFieldInput == "M")
		{
			return "001";
		}
		else if (destFieldInput == "D")
		{
			return "010";
		}
		else if (destFieldInput == "MD")
		{
			return "011";
		}
		else if (destFieldInput == "A")
		{
			return "100";
		}
		else if (destFieldInput == "AM")
		{
			return "101";
		}
		else if (destFieldInput == "AD")
		{
			return "110";
		}
		else if (destFieldInput == "AMD")
		{
			return "111";
		}
		else if (destFieldInput == "")
		{
			return "000";
		}
		else return "Invalid";
	}
	string comp(string compFieldInput)
	{
		if (compFieldInput == "0")
		{
			return "0101010";
		}
		else if (compFieldInput == "1")
		{
			return "0111111";
		}
		else if (compFieldInput == "-1")
		{
			return "0111010";
		}
		else if (compFieldInput == "D")
		{
			return "0001100";
		}
		else if (compFieldInput == "A")
		{
			return "0110000";
		}
		else if (compFieldInput == "!D")
		{
			return "0001101";
		}
		else if (compFieldInput == "!A")
		{
			return "0110001";
		}
		else if (compFieldInput == "-D")
		{
			return "0001111";
		}
		else if (compFieldInput == "-A")
		{
			return "0110011";
		}
		else if ((compFieldInput == "D+1") || (compFieldInput == "1+D"))
		{
			return "0011111";
		}
		else if ((compFieldInput == "A+1") || (compFieldInput == "1+A"))
		{
			return "0110111";
		}
		else if ((compFieldInput == "D-1") || (compFieldInput == "-1+D"))
		{
			return "0001110";
		}
		else if ((compFieldInput == "A-1") || (compFieldInput == "-1+A"))
		{
			return "0110010";
		}
		else if ((compFieldInput == "D+A") || (compFieldInput == "A+D"))
		{
			return "0000010";
		}
		else if ((compFieldInput == "D-A") || (compFieldInput == "-A+D"))
		{
			return "0010011";
		}
		else if ((compFieldInput == "A-D") || (compFieldInput == "-D+A"))
		{
			return "0000111";
		}
		else if ((compFieldInput == "D&A") || (compFieldInput == "A&D"))
		{
			return "0000000";
		}
		else if ((compFieldInput == "D|A") || (compFieldInput == "A|D"))
		{
			return "0010101";
		}
		else if (compFieldInput == "M")
		{
			return "1110000";
		}
		else if (compFieldInput == "!M")
		{
			return "1110001";
		}
		else if (compFieldInput == "-M")
		{
			return "1110011";
		}
		else if ((compFieldInput == "M+1") || (compFieldInput == "1+M"))
		{
			return "1110111";
		}
		else if ((compFieldInput == "M-1") || (compFieldInput == "-1+M"))
		{
			return "1110010";
		}
		else if ((compFieldInput == "D+M") || (compFieldInput == "M+D"))
		{
			return "1000010";
		}
		else if ((compFieldInput == "D-M") || (compFieldInput == "-M+D"))
		{
			return "1010011";
		}
		else if ((compFieldInput == "M-D") || (compFieldInput == "-D+M"))
		{
			return "1000111";
		}
		else if ((compFieldInput == "D&M") || (compFieldInput == "M&D"))
		{
			return "1000000";
		}
		else if ((compFieldInput == "D|M") || (compFieldInput == "M|D"))
		{
			return "1010101";
		}
		else return "Invalid";
	}
	string jump(string jumpFieldInput)
	{
		if (jumpFieldInput == "JGT")
		{
			return "001";
		}
		else if (jumpFieldInput == "JEQ")
		{
			return "010";
		}
		else if (jumpFieldInput == "JGE")
		{
			return "011";
		}
		else if (jumpFieldInput == "JLT")
		{
			return "100";
		}
		else if (jumpFieldInput == "JNE")
		{
			return "101";
		}
		else if (jumpFieldInput == "JLE")
		{
			return "110";
		}
		else if (jumpFieldInput == "JMP")
		{
			return "111";
		}
		else if (jumpFieldInput == "")
		{
			return "000";
		}
		else return "Invalid";
	}
};

int main(int argc, const char *argv[])  // alternatively: int main(int argc, char** argv)
// argc = number of strings pointed to by argv
// argv holds the array of inputs to the program, argv[0] holds "Assembler", argv[1] holds command line input
{

	if (argc != 2)
	{
		cout << "You must input a single assembly (.asm) file!" << endl;
		return 1;
	}
	int instructionCounter = 0;
	int currentSymbolAddress = 0;
	int symbolCounter = 16;
	string filename(argv[1]);
	string outputFileName;
	ofstream outputFileStream;
	Parser AsmParser(filename);
	Code Code;
	string parsedSymbol, parsedDest, parsedComp, parsedJump;
	string codeDest, codeComp, codeJump;
	string currentCode, symbolCoded;
	vector<string> codeList;
	SymbolTable AsmSymbolTable;
	/////////////////////////////////////////////////////////////
	////**********************************************************
	////FIRST PASS***********************************************
	// find the symbols, add them to symbol table
	int testCounter = 1;
	while (AsmParser.hasMoreCommands())
	{
		currentCode = "";
		AsmParser.advance();
		commandType currentCommandType = AsmParser.commandType();
		if (currentCommandType == A_Command)
		{
			instructionCounter++;
		}
		if (currentCommandType == L_Command)
		{
			parsedSymbol = AsmParser.symbol();

			if (AsmSymbolTable.contains(parsedSymbol))
			{
				AsmSymbolTable.updateEntry(parsedSymbol, instructionCounter);
			}
			else
			{
				AsmSymbolTable.addEntry(parsedSymbol, instructionCounter);
			}

		}
		if (currentCommandType == C_Command)
		{
			instructionCounter++;
		}
		testCounter++;
	}
	AsmParser.reset();
	/////////////////////////////////////////////////////////////
	////**********************************************************
	////SECOND PASS***********************************************
	while (AsmParser.hasMoreCommands())
	{
		currentCode = "";
		AsmParser.advance();
		commandType currentCommandType = AsmParser.commandType();
		//cout << "\nCurrent command is " << AsmParser.currentCommand << endl;
		//cout << "Command Type is : " << currentCommandType << endl;
		if (currentCommandType == A_Command)
		{
			parsedSymbol = AsmParser.symbol();
			//cout << "Symbol is: " << parsedSymbol << endl;
			if (AsmParser.constantCheck(parsedSymbol))
			{
				int decimalSymbol = stoi(parsedSymbol);
				//cout << "Symbol integer = " << decimalSymbol << endl;
				symbolCoded = bitset< 15 >(decimalSymbol).to_string();
			}
			else
			{
				if (AsmSymbolTable.contains(parsedSymbol)) {
					
					currentSymbolAddress = AsmSymbolTable.getAddress(parsedSymbol);
					symbolCoded = bitset< 15 >(currentSymbolAddress).to_string();
				}
				else
				{
							AsmSymbolTable.addEntry(parsedSymbol,symbolCounter);
							symbolCoded = bitset< 15 >(symbolCounter).to_string();
							symbolCounter++;
				
				}
			}
			currentCode = "0" + symbolCoded;
		}
		if (currentCommandType == L_Command)
		{
		}
		if (currentCommandType == C_Command)
		{
			parsedDest = AsmParser.dest();
			parsedComp = AsmParser.comp();
			parsedJump = AsmParser.jump();
			//cout << "Dest is: " << parsedDest << endl;
			//cout << "Comp is: " << parsedComp << endl;
			//cout << "Jump is: " << parsedJump << endl;
			codeDest = Code.dest(parsedDest);
			codeComp = Code.comp(parsedComp);
			codeJump = Code.jump(parsedJump);
			if ((codeDest == "Invalid") || (codeComp == "Invalid") || (codeJump == "Invalid"))
			{
				cout << "Error, invalid entry!" << endl;
				return 1;
			}
			else {
				currentCode = "111" + codeComp + codeDest + codeJump;
			}
		}
		// cout << "Current command code = " << currentCode << endl;
		if (currentCode != ""){ codeList.push_back(currentCode); }
		
	}
	AsmParser.reset();

	outputFileName = (filename.substr(0, filename.find("."))) + ".hack";
	outputFileStream.open(outputFileName, ofstream::out);
	for (unsigned int i = 0; i < codeList.size(); i++)
		outputFileStream << codeList[i] << endl;
	outputFileStream.close();

	return 0;
}

