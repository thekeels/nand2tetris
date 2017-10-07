#include <iostream>
#include <fstream>
#include <string>
using namespace std;

enum commandType { A_Command, C_Command, L_Command, Comment, Empty_Command}; // 0 = A_Command, 1 = C_Command, 2 = L_Command, 3 = Comment, 4 = Empty_Command
class Parser
{
	
	string inputFileName;
	ifstream inputFileStream;
	string currentCommand;
	size_t firstCharacter;
	size_t lastCharacter;

	// string currentSymbol;

public:
	string destField, compField, jumpField;
	Parser() {}
	Parser(string filename) : inputFileName(filename) {
		cout << "Instantiating Parser" << endl;      
		cout << "Opening " << filename << " for parsing..." << endl;
		inputFileStream.open(inputFileName);
		if (inputFileStream.good())
		{
			cout << "File successfully opened..." << endl;
		}
		else
			cout << "Unable to open file" << endl;
		//auto back_to_cstr = filename.c_str();
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
	string advance()

	{
		destField = compField = jumpField = "";
		if (!inputFileStream.eof())
		{
			getline(inputFileStream, currentCommand);
			cout << '\n' << currentCommand << '\n';
		}
		else
			cout << "Reached end of file.." << endl;
		return currentCommand;

	}
	commandType commandType()
	{

		cout << "Current command is " << currentCommand << endl;
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
		lastCharacter = currentCommand.find_last_not_of(" \t\r\n");
		if (currentCommand.at(lastCharacter) == ')')
		{
			return currentCommand.substr(firstCharacter + 1, lastCharacter-1);
		}
		else
		{
			return currentCommand.substr(firstCharacter + 1, lastCharacter);
		}
	}
	string dest()
	{
		string delimiter = "=";
		size_t pos = 0;
		// string token;
		cout << currentCommand << endl;
		while ((pos = currentCommand.find(delimiter)) != string::npos) {
			destField = currentCommand.substr(0, pos);
			cout << destField << endl;
			currentCommand.erase(0, pos + delimiter.length());
		}
		cout << currentCommand << endl;
		return destField;
	}

	string comp()
	{
		string delimiter = ";";
		size_t pos = 0;
		// string token;
		cout << currentCommand << endl;
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
				cout << compField << endl;
				currentCommand.erase(0, pos + delimiter.length());
			}
			cout << currentCommand << endl;
			return compField;
		}
	}
	string jump()
	{
		cout << currentCommand << endl;
		return currentCommand;
	}
};

class SymbolTable
{


public:
	SymbolTable() {}
	void addEntry()
	{

	}
	void contains()
	{

	}
	void getAddress()
	{

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
	//******************************************
	//*********CODE TO PRINT CMD LINE ARGUMENTS
	//******************************************
	/*
	std::cout << "Have " << argc << " arguments:" << std::endl;
	for (int i = 0; i < argc; ++i) {
	std::cout << argv[i] << std::endl;
	}
	*/
	//******************************************
	//******************************************
	if (argc != 2)
	{
		cout << "You must input a single assembly (.asm) file!" << endl;
		return 1;
	}
	string filename(argv[1]);
	Parser AsmParser(filename);
	Code Code;
	string parsedSymbol, parsedDest, parsedComp, parsedJump;
	string codeDest, codeComp, codeJump;
	string C_code;

	while (AsmParser.hasMoreCommands())
	{

		AsmParser.advance();
		commandType currentCommandType = AsmParser.commandType();
		cout << "Command Type is : " << currentCommandType << endl;
		if (currentCommandType == L_Command || currentCommandType == A_Command)
		{
			parsedSymbol = AsmParser.symbol();
			cout << "Symbol is: " << parsedSymbol << endl;
		}
		if (currentCommandType == C_Command)
		{
			parsedDest = AsmParser.dest();
			parsedComp = AsmParser.comp();
			parsedJump = AsmParser.jump();
			cout << "Dest is: " << parsedDest << endl;
			cout << "Comp is: " << parsedComp << endl;
			cout << "Jump is: " << parsedJump << endl;
			codeDest = Code.dest(parsedDest);
			codeComp = Code.comp(parsedComp);
			codeJump = Code.jump(parsedJump);
			if ((codeDest == "Invalid") || (codeComp == "Invalid") || (codeJump == "Invalid"))
			{
				cout << "Error, invalid entry!" << endl;
				return 1;
			}
			else C_code = "111" + codeComp + codeDest + codeJump;
			cout << "C_code = " << C_code << endl;
		}

	}



return 0;
}
