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
enum varType { STATIC, FIELD, ARG, VAR, NONE,CLASS, SUBROUTINE, OBJECT};
enum tokenType { KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST, TOKEN_NULL, TOKEN_ERROR};
// 0 = KEYWORD, 1= SYMBOL, 2 = IDENTIFIER, 3 = INT_CONST, 4 = STRING_CONST
enum segmentType {CONSTseg, ARGseg, LOCALseg, STATICseg, THISseg, THATseg, POINTERseg, TEMPseg};
enum commandType { ADD, SUB, NEG, EQ, GT, LT, AND, OR, NOT };


class JackTokenizer
{

	string inputFileName;
	ifstream inputFileStream;
	size_t firstCharacter;
	vector<char> tokens;

public:
	vector<string> tokenString;
	vector<tokenType> tokenList;
	string argument1;
	int argument2, argument3;
	string currentCommand;
	string token;

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

			
			bool multilineComment = false;
			if (string::npos != currentCommand.find("/*")) // Search for "/*", if found, then we have to remove the multiline comment
			{
				multilineComment = true;
				while (multilineComment)
				{
					size_t commentStartPosition = currentCommand.find("/*");
					if (string::npos == commentStartPosition)
					{
						commentStartPosition = 0;
					}
					size_t commentEndPosition = currentCommand.find("*/");
					if (string::npos != commentEndPosition) // If we find the end of the multiline comment, then we set the flag to false
					{
						multilineComment = false;
						currentCommand.erase(commentStartPosition, commentEndPosition - commentStartPosition+2);
						return;
					}
					getline(inputFileStream, currentCommand);
				}

			}
			currentCommand = currentCommand.substr(0, currentCommand.find("//"));
			size_t lastCharacter = currentCommand.find_last_not_of(" \t\r\n");
			size_t firstCharacter = currentCommand.find_first_not_of(" \t\r\n");

			if (string::npos != firstCharacter) {
				size_t range = lastCharacter - firstCharacter + 1;
				currentCommand = currentCommand.substr(firstCharacter, range);
			}
			else return;
			istringstream iss(currentCommand);
			istreambuf_iterator<char>eos;
			istreambuf_iterator<char>itt(iss);
			string test;
			while (itt != eos)
			{ 
				while ((*itt != ' ') && (*itt != '{') && (*itt != '(') && (*itt != '=') && (*itt != '}') && (*itt != ')') && (*itt != ';') && (*itt != ',') && (*itt != '[') && (*itt != ']') && (*itt != '-') && (*itt != '+') && (*itt != '*') && (*itt != '|') && (*itt != '/') && (*itt != '.') && (*itt != '&') && (*itt != '<') && (*itt != '>') && (*itt != '~'))
				{
					if (*itt == '\"')
					{
						do {
							tokens.push_back(*itt);
							//cout << *itt;
							++itt;
						} while (*itt != '\"');
						tokens.push_back(*itt);
						++itt;
					}
					else
					{
						tokens.push_back(*itt);
						//cout << *itt;
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
		}
		else
			cout << "Reached end of file.." << endl;
		return;
	}

	tokenType tokenType(int j)
	{
		if (tokenString[j].empty())
		{
			return TOKEN_NULL;
		}
		else
		{
			if ((tokenString[j] == "class") || (tokenString[j] == "method") || (tokenString[j] == "function") || (tokenString[j] == "constructor") || (tokenString[j] == "int") || (tokenString[j] == "boolean") || (tokenString[j] == "char") || (tokenString[j] == "void") || (tokenString[j] == "var") || (tokenString[j] == "static") || (tokenString[j] == "field") || (tokenString[j] == "let") || (tokenString[j] == "do") || (tokenString[j] == "if") || (tokenString[j] == "else") || (tokenString[j] == "while") || (tokenString[j] == "return") || (tokenString[j] == "true") || (tokenString[j] == "false") || (tokenString[j] == "null") || (tokenString[j] == "this"))
			{
				return KEYWORD;
			}
			else if ((tokenString[j] == "{") || (tokenString[j] == "}") || (tokenString[j] == "(") || (tokenString[j] == ")") || (tokenString[j] == "[") || (tokenString[j] == "]") || (tokenString[j] == "-") || (tokenString[j] == ",") || (tokenString[j] == ";") || (tokenString[j] == "+") || (tokenString[j] == "-") || (tokenString[j] == "*") || (tokenString[j] == "/") || (tokenString[j] == "&") || (tokenString[j] == "|") || (tokenString[j] == "<") || (tokenString[j] == ">") || (tokenString[j] == "=") || (tokenString[j] == "~") || (tokenString[j] == "."))
			{
				return SYMBOL;
			}
			else if (string::npos != tokenString[j].find('/"'))
			{
				return STRING_CONST;
			}
			else if ((tokenString[j].at(0)=='0') || (tokenString[j].at(0) == '1') || (tokenString[j].at(0) == '2') || (tokenString[j].at(0) == '3') || (tokenString[j].at(0) == '4') || (tokenString[j].at(0) == '5') || (tokenString[j].at(0) == '6') || (tokenString[j].at(0) == '7') || (tokenString[j].at(0) == '8') || (tokenString[j].at(0) == '9'))
			{
				return INT_CONST;
			}
			else return IDENTIFIER;
		}
	}
};

class TokenWriter
{
	string outputFileNameT;
	ofstream outputFileStreamT;

public:
	string staticVariable;
	TokenWriter() {}
	TokenWriter(string directory, string filename)
	{
		filename = (filename.substr(0, filename.find("."))); // remove .jack if it is there
		outputFileNameT = directory + filename + "T.xml";
		setFileName(outputFileNameT);
		writeHeader();
		staticVariable = (filename.substr(0, filename.find(".")));
	}
	void setFileName(string outputFileName)
	{
		outputFileStreamT.open(outputFileName, ofstream::out);
		return;
	}
	void Close()
	{
		outputFileStreamT.close();
		return;
	}
	void writeHeader()
	{
		outputFileStreamT << "<tokens>" << endl;
		return;
	}
	void writeFooter()
	{
		outputFileStreamT << "</tokens>" << endl;
		return;
	}
	void writeKeyword(string input)
	{
		outputFileStreamT << "<keyword> " << input << " </keyword>" << endl;
		return;
	}
	void writeSymbol(string input)
	{
		if (input == "\"") { input = "&quot;"; }
		else if (input == "<") { input = "&lt;"; }
		else if (input == ">") { input = "&gt;"; }
		else if (input == "&") { input = "&amp;"; }
		else;
		outputFileStreamT << "<symbol> " << input << " </symbol>" << endl;
		return;
	}
	void writeIdentifier(string input)
	{
		outputFileStreamT << "<identifier> " << input << " </identifier>" << endl;
		return;
	}
	void writeStringConst(string input)
	{
		size_t inputSize = input.size();
		string temp = input.substr(1, inputSize - 2);
		outputFileStreamT << "<stringConstant> " << temp << " </stringConstant>" << endl;
		return;
	}
	void writeIntConst(string input)
	{
		outputFileStreamT << "<integerConstant> " << input << " </integerConstant>" << endl;
		return;
	}
	~TokenWriter()
	{	}
};

class VMWriter
{
	string outputFileNameVM;
	ofstream outputFileStreamVM;
	string convertedSegment;

public:
	//string staticVariable;
	segmentType currentSegment;
	VMWriter() {}
	VMWriter(string directory, string filename)
	{
		filename = (filename.substr(0, filename.find("."))); // remove .jack if it is there
		outputFileNameVM = directory + filename + ".vm";
		setFileName(outputFileNameVM);
		//writeHeader();
		//staticVariable = (filename.substr(0, filename.find(".")));
	}
	void setFileName(string outputFileName)
	{
		outputFileStreamVM.open(outputFileName, ofstream::out);
		return;
	}
	void Close()
	{
		outputFileStreamVM.close();
		return;
	}
	void findSegment(varType segment)
	{

		if (segment == ARG)
		{
			currentSegment = ARGseg;
		}
		else if (segment == VAR)
		{
			currentSegment = LOCALseg;
		}
		else if (segment == STATIC)
		{
			currentSegment = STATICseg;
		}
		else if (segment == FIELD)
		{
			currentSegment = THISseg;
		}

		else

			return;
	}
	void convertSegment(segmentType segment)
	{

		if (segment == STATICseg)
		{
			convertedSegment = "static";
		}
		else if (segment == LOCALseg)
		{
			convertedSegment = "local";
		}
		else if (segment == CONSTseg)
		{
			convertedSegment = "constant";
		}
		else if (segment == ARGseg)
		{
			convertedSegment = "argument";
		}
		else if (segment == THISseg)
		{
			convertedSegment = "this";
		}
		else if (segment == THATseg)
		{
			convertedSegment = "that";
		}
		else if (segment == POINTERseg)
		{
			convertedSegment = "pointer";
		}
		else if (segment == TEMPseg)
		{
			convertedSegment = "temp";
		}
		else

		return;
	}
	void writePush(segmentType segment, int index)
	{
		convertSegment(segment);
		outputFileStreamVM << "push" << " " << convertedSegment << " " << index << endl;
		return;
	}
	void writePop(segmentType segment, int index)
	{
		convertSegment(segment);
		outputFileStreamVM << "pop" << " " << convertedSegment << " " << index << endl;
		return;
	}
	void writeArithmetic(commandType command)
	{
		if (command == ADD)
		{
			outputFileStreamVM << "add" << endl;
		}
		else if (command == SUB)
		{
			outputFileStreamVM << "sub" << endl;
		}
		else if (command == NEG)
		{
			outputFileStreamVM << "neg" << endl;
		}
		else if (command == EQ)
		{
			outputFileStreamVM << "eq" << endl;
		}
		else if (command == GT)
		{
			outputFileStreamVM << "gt" << endl;
		}
		else if (command == LT)
		{
			outputFileStreamVM << "lt" << endl;
		}
		else if (command == AND)
		{
			outputFileStreamVM << "and" << endl;
		}
		else if (command == OR)
		{
			outputFileStreamVM << "or" << endl;
		}
		else if (command == NOT)
		{
			outputFileStreamVM << "not" << endl;
		}
		else
		return;
	}
	void writeLabel(string input)
	{
		outputFileStreamVM << "label"  <<  " " << input << endl;
		return;
	}
	void writeGoto(string input)
	{
		outputFileStreamVM << "goto" << " " << input << endl;
		return;
	}
	void writeIf(string input)
	{
		outputFileStreamVM << "if-goto" << " " << input << endl;
		return;
	}
	void writeCall(string input, int nArgs)
	{
		outputFileStreamVM << "call" << " " << input << " " << nArgs << endl;
		return;
	}
	void writeFunction(string input, int nLocals)
	{
		outputFileStreamVM << "function" << " " << input << " " << nLocals << endl;
		return;
	}
	void writeReturn()
	{
		outputFileStreamVM << "return" << endl;
		return;
	}
	~VMWriter()
	{	}
};

class SymbolTable
{
	vector<string> nameColClass;	// Creates a column with the variable names -- Class-scope Table
	vector<string> typeColClass;	// Creates a column with the type of the variable -- Class-scope Table
	vector<varType> kindColClass;	// Creates a column with the memory type of the variable -- Class-scope Table
	vector<int> indexColClass;		// Creates a column with the index of the variable -- Class-scope Table
	vector<string> nameColMethod;	// Creates a column with the variable names -- Method-scope Table
	vector<string> typeColMethod;	// Creates a column with the type of the variable -- Method-scope Table
	vector<varType> kindColMethod;	// Creates a column with the memory type of the variable -- Method-scope Table
	vector<int> indexColMethod;		// Creates a column with the index of the variable -- Method-scope Table



public:
	int staticCounterM = 0, argCounterM = 0, varCounterM = 0, fieldCounterM = 0; // Counters for method identifiers
	int staticCounterC = 0, argCounterC = 0, varCounterC = 0, fieldCounterC = 0; // Counters for method identifiers
	string printbeingDefined;
	string printCategory;
	string currentClassName;
	string currentSubName;
	string currentVarName;
	string currentFunction;
	string currentLet;
	bool inSubroutine = false; // false = class-scope, true = method-scope 
	bool beingDefined = false; // false = var being used, true = var being defined
	bool global = false;
	varType currentKind = NONE;
	string currentTypeOf = "";
	varType currentCategory = NONE;
	SymbolTable() {}
	void startSubroutine() 
	{
		nameColMethod.clear();
		typeColMethod.clear();
		kindColMethod.clear();
		indexColMethod.clear();
		inSubroutine = true;
		staticCounterM = 0, argCounterM = 0, varCounterM = 0, fieldCounterM = 0; // Counters for method identifiers
		return;
	}
	void checkDefined(string name)
	{
		global = false;
		if (inSubroutine)
		{
			if (std::find(nameColMethod.begin(), nameColMethod.end(), name) != nameColMethod.end())
			{
				// Element in vector.
				beingDefined = false;
			}
			else if (std::find(nameColClass.begin(), nameColClass.end(), name) != nameColClass.end())
			{
				// Element in vector.
				beingDefined = false;
				global = true;
			}
			else beingDefined = true;
		}
		else
		{
			if (std::find(nameColClass.begin(), nameColClass.end(), name) != nameColClass.end())
			{
				// Element in vector.
				beingDefined = false;
			}
			else beingDefined = true;
		}
		if (beingDefined)
		{
			printbeingDefined = "beingDefined";
		}
		else
		{
			printbeingDefined = "beingUsed";
		}
		return;
	}
	string PrintClassOrSub(varType kind)
	{
		string printClassOrSub;
		if (kind == CLASS)
		{
			printClassOrSub = "CLASS";
		}
		else if (kind == SUBROUTINE)
		{
			printClassOrSub = "SUBROUTINE";
		}
		else
		{
			printClassOrSub = "unknown";
		}
		return printClassOrSub;
	}


	string PrintCategory(string name)
	{
		int pos = findIndex(name);
		varType posCategory;
		string printCategory;
		if (inSubroutine && !global)
		{
			posCategory = kindColMethod[pos];
		}
		else
		{
			posCategory = kindColClass[pos];
		}
		if (posCategory == VAR)
		{
			printCategory = "VAR";
		}
		else if (posCategory == STATIC)
		{
			printCategory = "STATIC";
		}
		else if (posCategory == ARG)
		{
			printCategory = "ARG";
		}
		else if (posCategory == FIELD)
		{
			printCategory = "FIELD";
		}
		else
		{
			printCategory = "unknown";
		}
		return printCategory;
	}
	int findIndex(string name)
	{
		vector<string>::iterator indexReturn;
		int pos;
		if (inSubroutine && !global)
		{
			indexReturn = std::find(nameColMethod.begin(), nameColMethod.end(), name);
			pos = distance(nameColMethod.begin(), indexReturn);
			return pos;
		}
		else
		{
			indexReturn = std::find(nameColClass.begin(), nameColClass.end(), name);
			pos = distance(nameColClass.begin(), indexReturn);
			return pos;
		}
	}
	void Define(string name, string type, varType kind) 
	{
		if (kind == VAR)
		{
			if (inSubroutine)
			{
				kindColMethod.push_back(kind);
				indexColMethod.push_back(IndexOf(name));
				nameColMethod.push_back(name);
				typeColMethod.push_back(type);

				++varCounterM;
			}
			else
			{
				kindColClass.push_back(kind);
				indexColClass.push_back(IndexOf(name));
				nameColClass.push_back(name);
				typeColClass.push_back(type);

				++varCounterC;
			}
		}
		else if (kind == STATIC)
		{
			if (inSubroutine)
			{
				kindColMethod.push_back(kind);
				indexColMethod.push_back(IndexOf(name));
				nameColMethod.push_back(name);
				typeColMethod.push_back(type);

				++staticCounterM;
			}
			else
			{
				kindColClass.push_back(kind);
				indexColClass.push_back(IndexOf(name));
				nameColClass.push_back(name);
				typeColClass.push_back(type);

				++staticCounterC;
			}
		}
		else if (kind == ARG)
		{
			if (inSubroutine)
			{
				kindColMethod.push_back(kind);
				indexColMethod.push_back(IndexOf(name));
				nameColMethod.push_back(name);
				typeColMethod.push_back(type);

				++argCounterM;
			}
			else
			{
				kindColClass.push_back(kind);
				indexColClass.push_back(IndexOf(name));
				nameColClass.push_back(name);
				typeColClass.push_back(type);

				++argCounterC;
			}
		}
		else if (kind == FIELD)
		{
			if (inSubroutine)
			{
				kindColMethod.push_back(kind);
				indexColMethod.push_back(IndexOf(name));
				nameColMethod.push_back(name);
				typeColMethod.push_back(type);

				++fieldCounterM;
			}
			else
			{
				kindColClass.push_back(kind);
				indexColClass.push_back(IndexOf(name));
				nameColClass.push_back(name);
				typeColClass.push_back(type);

				++fieldCounterC;
			}
		}
		return;


	}
	int VarCount(varType kind)
	{
		if (inSubroutine)
		{
			if (kind == STATIC)
			{
				return staticCounterM;
			}
			else if (kind == FIELD)
				return fieldCounterM;
			else if (kind == VAR)
				return varCounterM;
			else if (kind == ARG)
				return argCounterM;
		}
		else
		{
			if (kind == STATIC)
			{
				return staticCounterC;
			}
			else if (kind == FIELD)
				return fieldCounterC;
			else if (kind == VAR)
				return varCounterC;
			else if (kind == ARG)
				return argCounterC;
		}
	}
	varType KindOf(string name) 
	{
		int pos = findIndex(name);
		varType posKind;

		if (inSubroutine && !global)
		{
			posKind = kindColMethod[pos];
		}
		else
		{
			posKind = kindColClass[pos];
		}
		
		return posKind;
	}
	string TypeOf(string name) 
	{
		int pos = findIndex(name);
		string typeIndex;
		varType kind = KindOf(name);
		if (inSubroutine && !global)
		{
			if (find(nameColMethod.begin(), nameColMethod.end(), name) != nameColMethod.end())
			{
				typeIndex = typeColMethod[pos];
			}
			else
			{
				typeIndex = "Unknown";
			}
		}
		else
		{
			if (find(nameColClass.begin(), nameColClass.end(), name) != nameColClass.end())
			{
				typeIndex = typeColClass[pos];
			}
			else
			{
				typeIndex = "Unknown";
			}
		}
		return typeIndex;
	}
	int IndexOf(string name)
	{
		int pos = findIndex(name);
		int posIndex;
		varType kind = KindOf(name);
		if (inSubroutine && !global)
		{
			if (find(nameColMethod.begin(), nameColMethod.end(), name) != nameColMethod.end())
			{
				posIndex = indexColMethod[pos];
			}
			else
			{
				if (kind == STATIC)
				{
					posIndex = staticCounterM;
				}
				else if (kind == ARG)
				{
					posIndex = argCounterM;
				}
				else if (kind == VAR)
				{
					posIndex = varCounterM;
				}
				else if (kind == FIELD)
				{
					posIndex = fieldCounterM;
				}
				else posIndex = 0;
			}
		}
		else
		{
			if (find(nameColClass.begin(), nameColClass.end(), name) != nameColClass.end())
			{
				posIndex = indexColClass[pos];
			}
			else
			{
				if (kind == STATIC)
				{
					posIndex = staticCounterC;
				}
				else if (kind == ARG)
				{
					posIndex = argCounterC;
				}
				else if (kind == VAR)
				{
					posIndex = varCounterC;
				}
				else if (kind == FIELD)
				{
					posIndex = fieldCounterC;
				}
				else posIndex = 0;
			}
		}
		return posIndex;
	}
	~SymbolTable() {}


};
class CompilationEngine : public JackTokenizer
{
	SymbolTable symbolTable;	
	string outputFileName, outputFileNameVM;
	ofstream outputFileStream;
	VMWriter VMWriter;
	string currentFileName;
	string functionType;
	int fieldCounter=0;
	vector<string> expressionOPVector;
public:
	int tokenCount = 0;
	int localCount = 0;
	int termCounter = 0;
	JackTokenizer* J;
	int argCount = 0;
	vector<int> labelCountVector;
	int ifCount=0;
	int whileCount = 0;
	bool letArray = false;
	string opType;

	CompilationEngine(string directory, string filename, JackTokenizer &Jack)
	{
		J = &Jack;
		filename = (filename.substr(0, filename.find("."))); // remove .jack if it is there
		currentFileName = filename;
		outputFileName = directory + filename + ".xml";
		outputFileNameVM = directory + filename + ".vm";
		setFileName(outputFileName);
		VMWriter.setFileName(outputFileNameVM);
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
	void CompileClass()
	{
		outputFileStream << "<class>" << endl;			// <class>
		writeKeyword(J->tokenString[tokenCount]);		// <keyword> class
		++tokenCount;			
		writeIdentifier(J->tokenString[tokenCount]);	// <identifier> Main
		symbolTable.currentClassName = J->tokenString[tokenCount];
		outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintClassOrSub(CLASS) << " , " << "No Index" << " , " << "CLASS/SUB-notdefined" << ")" << endl;
		++tokenCount;

		writeSymbol(J->tokenString[tokenCount]);		// <symbol> ;
		++tokenCount;
		while ((J->tokenString[tokenCount] == "static") || (J->tokenString[tokenCount] == "field"))
		{
			CompileClassVarDec();						// Prints out all the Class Variable Definitions
		}
		localCount = 0;
		while ((J->tokenString[tokenCount] == "function") || (J->tokenString[tokenCount] == "method") || (J->tokenString[tokenCount] == "constructor"))
		{
			CompileSubroutine();						// Moves on to print all the subroutines
		}
		writeSymbol(J->tokenString[tokenCount]);		// <symbol> }
		++tokenCount;
		outputFileStream << "</class>" << endl;			// </class>
	}
	void CompileClassVarDec()
	{
		outputFileStream << "<classVarDec>" << endl;	// <classVarDec>
		writeKeyword(J->tokenString[tokenCount]);		// "<keyword> static " || "<keyword field "
		// class var kind definition
		if (J->tokenString[tokenCount] == "static") { symbolTable.currentKind = varType::STATIC; }
		else
		{
			symbolTable.currentKind = FIELD;
			++fieldCounter;
		}
		// end class var kind definition
		++tokenCount;
		if (J->tokenList[tokenCount] == KEYWORD)
		{
			writeKeyword(J->tokenString[tokenCount]);	// prints <keyword> int/char/boolean
		}
		else
		{
			writeIdentifier(J->tokenString[tokenCount]);	// prints <identifier> className
			// CLASS symbol table state definition
			outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintClassOrSub(CLASS) << " , " << "No Index" << " , " << "CLASS/SUB-notdefined" << ")" << endl;
			
			// end CLASS symbol table state definition
		}
		++tokenCount;
		writeIdentifier(J->tokenString[tokenCount]);		// prints <identifier> varName
		// CLASS symbol table state definition
		symbolTable.currentTypeOf = J->tokenString[tokenCount-1];
		symbolTable.checkDefined(J->tokenString[tokenCount]);
		if (symbolTable.beingDefined)
		{
			symbolTable.Define(J->tokenString[tokenCount], symbolTable.currentTypeOf, symbolTable.currentKind);
		}
		outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintCategory(J->tokenString[tokenCount]) << " , " << symbolTable.IndexOf(J->tokenString[tokenCount]) << " , " << symbolTable.printbeingDefined << ")" << endl;
		// VMWriter.writePush(STATICseg,symbolTable.IndexOf(J->tokenString[tokenCount]));
		// end CLASS symbol table state definition
		++tokenCount;
		while (J->tokenString[tokenCount] == ",")
		{
			writeSymbol(J->tokenString[tokenCount]);		// <symbol> ,
			++tokenCount;
			++fieldCounter;
			writeIdentifier(J->tokenString[tokenCount]);	// prints remaining varNames, if they exist
			// CLASS symbol table state definition
			if (symbolTable.beingDefined)
			{
				symbolTable.Define(J->tokenString[tokenCount], symbolTable.currentTypeOf, symbolTable.currentKind);
			}
			outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintCategory(J->tokenString[tokenCount]) << " , " << symbolTable.IndexOf(J->tokenString[tokenCount]) << " , " << symbolTable.printbeingDefined << ")" << endl;
			//VMWriter.writePush(STATICseg, symbolTable.IndexOf(J->tokenString[tokenCount]));
			// end CLASS symbol table state definition
			++tokenCount;
		}
		writeSymbol(J->tokenString[tokenCount]);			// <symbol> ;
		++tokenCount;
		outputFileStream << "</classVarDec>" << endl;		// </classVarDec>
	}
	void CompileSubroutine()
	{
		symbolTable.startSubroutine();
		ifCount = 0;
		whileCount = 0;
		outputFileStream << "<subroutineDec>" << endl;		// <subroutineDec>
		writeKeyword(J->tokenString[tokenCount]);			// <keyword> constructor/method/function
		functionType = J->tokenString[tokenCount];
		++tokenCount;
		if (J->tokenList[tokenCount] == KEYWORD)
		{
			writeKeyword(J->tokenString[tokenCount]);		// prints a <keyword> void/int/char/boolean
		}
		else
		{
			writeIdentifier(J->tokenString[tokenCount]);	// prints <identifier> className
			// SUBROUTINE symbol table state definition
			outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintClassOrSub(CLASS) << " , " << "No Index" << " , " << "CLASS/SUB-notdefined" << ")" << endl;
			// end SUBROUTINE symbol table state definition
		}
		++tokenCount;
		writeIdentifier(J->tokenString[tokenCount]);		// <identifier> subroutineName
		// SUBROUTINE symbol table state definition	
		outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintClassOrSub(SUBROUTINE) << " , " << "No Index" << " , " << "CLASS/SUB-notdefined" << ")" << endl;
		symbolTable.currentSubName = J->tokenString[tokenCount];
		// end SUBROUTINE symbol table state definition
		++tokenCount;
		writeSymbol(J->tokenString[tokenCount]);			// <symbol> (
		++tokenCount;
		CompileParameterList();								// prints the Parameter List
		writeSymbol(J->tokenString[tokenCount]);			// <symbol> )
		++tokenCount;
		outputFileStream << "<subroutineBody>" << endl;		// <subroutineBody>
		writeSymbol(J->tokenString[tokenCount]);			// <symbol> {		
		++tokenCount;
		localCount = 0;
		while (J->tokenString[tokenCount] == "var")
		{
			CompileVarDec();								// prints all the VarDecs
		}
		if (functionType == "function")
		{
			VMWriter.writeFunction(symbolTable.currentClassName + "." + symbolTable.currentSubName, localCount);// need fix to set localCount?
		}
		else if (functionType == "constructor")
		{
			VMWriter.writeFunction(symbolTable.currentClassName + "." + symbolTable.currentSubName, localCount); // need to check localCount
			VMWriter.writePush(CONSTseg, fieldCounter);
			VMWriter.writeCall("Memory.alloc", 1);
			VMWriter.writePop(POINTERseg, 0);
			//for (int j = 0; j < varCount; j++)
			//{
			//	VMWriter.writePush(ARGseg, j);
			//	VMWriter.writePop(THISseg, j);
			//}
		}
		else if (functionType == "method")
		{
			VMWriter.writeFunction(symbolTable.currentClassName + "." + symbolTable.currentSubName, localCount); // need to check
			VMWriter.writePush(ARGseg, 0);
			VMWriter.writePop(POINTERseg, 0);
		}
		CompileStatements();								// prints all the statements
		writeSymbol(J->tokenString[tokenCount]);			// <symbol> }			
		++tokenCount;
		outputFileStream << "</subroutineBody>" << endl;	// </subroutineBody>
		outputFileStream << "</subroutineDec>" << endl;		// </subroutineDec>
		symbolTable.inSubroutine = false;
	}
	void CompileParameterList()
	{
		outputFileStream << "<parameterList>" << endl;		// <parameterList>
		if (J->tokenString[tokenCount] == ")")
		{
															// if parameter list is empty, do nothing
		}
		else
		{
			if (J->tokenList[tokenCount] == KEYWORD)
			{
				writeKeyword(J->tokenString[tokenCount]);		// prints <keyword> int/char/boolean
			}
			else
			{
				writeIdentifier(J->tokenString[tokenCount]);	// prints <identifier> className
				// Symbol table state definition
				outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintClassOrSub(SUBROUTINE) << " , " << "No Index" << " , " << "CLASS/SUB-notdefined" << ")" << endl;
				// End symbol table state definition
			}
			++tokenCount;
			writeIdentifier(J->tokenString[tokenCount]);		// prints <identifier> varName
			// Symbol table state definition
			symbolTable.currentKind = varType::ARG;
			symbolTable.currentTypeOf = J->tokenString[tokenCount - 1];
			symbolTable.currentCategory = symbolTable.currentKind;
			symbolTable.checkDefined(J->tokenString[tokenCount]);
			if (symbolTable.beingDefined)
			{
				symbolTable.Define(J->tokenString[tokenCount], symbolTable.currentTypeOf, symbolTable.currentKind);
			}
			outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintCategory(J->tokenString[tokenCount]) << " , " << symbolTable.IndexOf(J->tokenString[tokenCount]) << " , " << symbolTable.printbeingDefined << ")" << endl;
			// End symbol table state definition
			++tokenCount;
			while (J->tokenString[tokenCount] == ",")
			{
				writeSymbol(J->tokenString[tokenCount]);		// <symbol> ,
				++tokenCount;
				if (J->tokenList[tokenCount] == KEYWORD)
				{
					writeKeyword(J->tokenString[tokenCount]);		// prints <keyword> int/char/boolean
				}
				else
				{
					writeIdentifier(J->tokenString[tokenCount]);	// prints <identifier> className
					// Symbol table state definition
					outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintClassOrSub(SUBROUTINE) << " , " << "No Index" << " , " << "CLASS/SUB-notdefined" << ")" << endl;
					// End symbol table state definition
				}
				++tokenCount;
				writeIdentifier(J->tokenString[tokenCount]);		// prints <identifier> varName
				// Symbol table state definition
				symbolTable.currentKind = varType::ARG;
				symbolTable.currentTypeOf = J->tokenString[tokenCount - 1];
				symbolTable.currentCategory = symbolTable.currentKind;
				symbolTable.checkDefined(J->tokenString[tokenCount]);
				if (symbolTable.beingDefined)
				{
					symbolTable.Define(J->tokenString[tokenCount], symbolTable.currentTypeOf, symbolTable.currentKind);
				}
				outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintCategory(J->tokenString[tokenCount]) << " , " << symbolTable.IndexOf(J->tokenString[tokenCount]) << " , " << symbolTable.printbeingDefined << ")" << endl;
				// End symbol table state definition
				++tokenCount;
			}
		}
		outputFileStream << "</parameterList>" << endl;			// </parameterList>
	}
	void CompileVarDec()
	{

		outputFileStream << "<varDec>" << endl;
		writeKeyword(J->tokenString[tokenCount]);				// <keyword> var
		++tokenCount;
		// SUBROUTINE symbol table state definition
		symbolTable.currentKind = VAR;
		symbolTable.currentTypeOf = J->tokenString[tokenCount];
		symbolTable.checkDefined(J->tokenString[tokenCount]);
		// end SUBROUTINE symbol table state definition
		if (J->tokenList[tokenCount] == KEYWORD)
		{
			writeKeyword(J->tokenString[tokenCount]);		// prints <keyword> int/char/boolean
		}
		else
		{
			writeIdentifier(J->tokenString[tokenCount]);	// prints <identifier> className
			// Symbol table state definition
			outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintClassOrSub(CLASS) << " , " << "No Index" << " , " << "CLASS/SUB-notdefined" << ")" << endl;
			// End symbol table state definition
		}
		++tokenCount;
		++localCount;
		writeIdentifier(J->tokenString[tokenCount]);		// prints <identifier> varName
		// Symbol table state definition
		symbolTable.currentKind = VAR;
		symbolTable.currentTypeOf = J->tokenString[tokenCount - 1];
		symbolTable.currentCategory = symbolTable.currentKind;
		symbolTable.checkDefined(J->tokenString[tokenCount]);
		if (symbolTable.beingDefined)
		{
			symbolTable.Define(J->tokenString[tokenCount], symbolTable.currentTypeOf, symbolTable.currentKind);
		}
		outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintCategory(J->tokenString[tokenCount]) << " , " << symbolTable.IndexOf(J->tokenString[tokenCount]) << " , " << symbolTable.printbeingDefined << ")" << endl;
		// End symbol table state definition
		++tokenCount;
		while (J->tokenString[tokenCount] == ",")
		{
			writeSymbol(J->tokenString[tokenCount]);		// <symbol> ,
			++tokenCount;
			++localCount;
			writeIdentifier(J->tokenString[tokenCount]);	// prints remaining varNames, if they exist
			// Symbol table state definition
			if (symbolTable.beingDefined)
			{
				symbolTable.Define(J->tokenString[tokenCount], symbolTable.currentTypeOf, symbolTable.currentKind);
			}
			outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintCategory(J->tokenString[tokenCount]) << " , " << symbolTable.IndexOf(J->tokenString[tokenCount]) << " , " << symbolTable.printbeingDefined << ")" << endl;
			// End symbol table state definition
			++tokenCount;
		}
		writeSymbol(J->tokenString[tokenCount]);			// <symbol> ;
		++tokenCount;
		outputFileStream << "</varDec>" << endl;			// </varDec>
	}
	void CompileStatements()
	{
		outputFileStream << "<statements>" << endl;			// <statements>
		while ((J->tokenString[tokenCount] == "let") || (J->tokenString[tokenCount] == "if") || (J->tokenString[tokenCount] == "while") || (J->tokenString[tokenCount] == "do")||(J->tokenString[tokenCount] == "return"))
		{
			if (J->tokenString[tokenCount] == "let")	
			{
				CompileLet();								// prints Let
			}
			else if (J->tokenString[tokenCount] == "if")
			{
				CompileIf();								// prints If
			}
			else if (J->tokenString[tokenCount] == "while")
			{
				CompileWhile();								// prints While
			}
			else if (J->tokenString[tokenCount] == "do")
			{
				CompileDo();								// prints Do
			}
			else if (J->tokenString[tokenCount] == "return")
			{
				CompileReturn();							// prints Return
			}
			else {}
		}
		outputFileStream << "</statements>" << endl;		// </statements>
	}
	void CompileDo()
	{
		argCount = 0;
		outputFileStream << "<doStatement>" << endl;		// <doStatement>
		writeKeyword(J->tokenString[tokenCount]);			// <keyword> do
		++tokenCount;
		writeIdentifier(J->tokenString[tokenCount]);		// <identifer> subroutineName/className/varName
		// Symbol table state definition
		symbolTable.currentSubName = J->tokenString[tokenCount];
		symbolTable.checkDefined(J->tokenString[tokenCount]);
		if (symbolTable.beingDefined)
		{
			if (J->tokenString[tokenCount + 1] == ".")
			{
				symbolTable.currentKind = CLASS;
				symbolTable.currentSubName = J->tokenString[tokenCount];
			}
			else
			{
				symbolTable.currentKind = SUBROUTINE;
				symbolTable.currentSubName = symbolTable.currentClassName + "." + symbolTable.currentSubName;
				symbolTable.currentCategory = OBJECT;
			}
			outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintClassOrSub(symbolTable.currentKind) << " , " << "No Index" << " , " << "CLASS/SUB-notdefined" << ")" << endl;
			//VMWriter.writePush(POINTERseg, 0); // needs to push THIS
		}
		else
		{
			symbolTable.currentKind = symbolTable.KindOf(J->tokenString[tokenCount]);
			outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintCategory(J->tokenString[tokenCount]) << " , " << symbolTable.IndexOf(J->tokenString[tokenCount]) << " , " << symbolTable.printbeingDefined << ")" << endl;
			symbolTable.currentSubName = symbolTable.TypeOf(J->tokenString[tokenCount]);
			VMWriter.writePush(VMWriter.currentSegment, symbolTable.IndexOf(J->tokenString[tokenCount])); // needs to push
			if (symbolTable.currentKind == VAR)
			{
				++argCount; // If VAR, then we are dealing with an object and need to increment arg to account for "THIS"
			}
		}
		// End symbol table state definition
		++tokenCount;
		if (J->tokenString[tokenCount] == "(")
		{
			writeSymbol(J->tokenString[tokenCount]);			// <symbol> (
			++tokenCount;
			CompileExpressionList();							// calls expression list
			if (symbolTable.currentCategory == OBJECT)
			{
				VMWriter.writePush(POINTERseg, 0);
			}
			VMWriter.writeCall(symbolTable.currentSubName, 1); // need fix for nArgs
			writeSymbol(J->tokenString[tokenCount]);			// <symbol> )
			++tokenCount;
		}
		else
		{
			writeSymbol(J->tokenString[tokenCount]);			// <symbol> .
			++tokenCount;
			writeIdentifier(J->tokenString[tokenCount]);		// <identifer> subroutineName
			// Symbol table state definition
			outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintClassOrSub(SUBROUTINE) << " , " << "No Index" << " , " << "CLASS/SUB-notdefined" << ")" << endl;
			symbolTable.currentSubName = symbolTable.currentSubName + "." + J->tokenString[tokenCount];
			// End symbol table state definition
			++tokenCount;
			writeSymbol(J->tokenString[tokenCount]);			// <symbol> (
			++tokenCount;
			CompileExpressionList();							// calls expression list
			writeSymbol(J->tokenString[tokenCount]);			// <symbol> )
			VMWriter.writeCall(symbolTable.currentSubName, argCount); // need check for nArgs
			++tokenCount;
		}
		writeSymbol(J->tokenString[tokenCount]);				// <symbol> ;		
		++tokenCount;
		VMWriter.writePop(TEMPseg, 0);
		outputFileStream << "</doStatement>" << endl;			// </doStatement>
	}
	void CompileLet()
	{
		argCount = 0;
		outputFileStream << "<letStatement>" << endl;			// <letStatement>
		writeKeyword(J->tokenString[tokenCount]);				// <keyword> let
		++tokenCount;
		writeIdentifier(J->tokenString[tokenCount]);			// <identifier> varName
		// Symbol table state definition
		symbolTable.currentKind = VAR;
		symbolTable.currentTypeOf = J->tokenString[tokenCount - 1];
		symbolTable.currentCategory = symbolTable.currentKind;
		symbolTable.checkDefined(J->tokenString[tokenCount]);
		if (symbolTable.beingDefined)
		{
			symbolTable.Define(J->tokenString[tokenCount], symbolTable.currentTypeOf, symbolTable.currentKind);
		}
		else
		{
			symbolTable.currentKind = symbolTable.KindOf(J->tokenString[tokenCount]);
			symbolTable.currentLet = J->tokenString[tokenCount];

		}
		outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintCategory(J->tokenString[tokenCount]) << " , " << symbolTable.IndexOf(J->tokenString[tokenCount]) << " , " << symbolTable.printbeingDefined << ")" << endl;
		VMWriter.findSegment(symbolTable.currentKind);
		// End symbol table state definition
		++tokenCount;
		if (J->tokenString[tokenCount] == "[")					// if array, print the array notation
		{
			string arrayName = J->tokenString[tokenCount - 1];
			writeSymbol(J->tokenString[tokenCount]);			// <symbol> [
			++tokenCount;
			CompileExpression();								// print the expression
			writeSymbol(J->tokenString[tokenCount]);			// <symbol> ]
			VMWriter.findSegment(symbolTable.KindOf(arrayName));
			VMWriter.writePush(VMWriter.currentSegment, symbolTable.IndexOf(arrayName)); // need to push base adress of array
			VMWriter.writeArithmetic(ADD);
			++tokenCount;
			letArray = true;
		}
		else;
		

		writeSymbol(J->tokenString[tokenCount]);				// if no array, print <symbol> =
		++tokenCount;
		CompileExpression();
		symbolTable.checkDefined(symbolTable.currentLet);// print the expression
		VMWriter.findSegment(symbolTable.KindOf(symbolTable.currentLet));
		if (letArray)
		{
			VMWriter.writePop(TEMPseg, 0); // place = expression in temp segment
			VMWriter.writePop(POINTERseg, 1); // "THAT" access
			VMWriter.writePush(TEMPseg, 0); // push = expression onto stack
			VMWriter.writePop(THATseg, 0); // pop the = expression into THAT 0
			letArray = false;	// reset letArray flag
		}
		else 
		{
			VMWriter.writePop(VMWriter.currentSegment, symbolTable.IndexOf(symbolTable.currentLet));
		}
		
		writeSymbol(J->tokenString[tokenCount]);				// <symbol> ;
		++tokenCount;
		outputFileStream << "</letStatement>" << endl;			// </letStatement>
	}
	void CompileWhile()
	{
		
		labelCountVector.push_back(whileCount);
		++whileCount;
		VMWriter.writeLabel("WHILE_EXP" + to_string(labelCountVector.back()));	// WHILE label

		outputFileStream << "<whileStatement>" << endl;			// <whileStatement>
		writeKeyword(J->tokenString[tokenCount]);				// <keyword> while
		++tokenCount;
		writeSymbol(J->tokenString[tokenCount]);				// <symbol> (
		++tokenCount;
		CompileExpression();									// print the expression
		VMWriter.writeArithmetic(NOT);	// Code for NOT (exp)
		VMWriter.writeIf("WHILE_END" + to_string(labelCountVector.back()));
		writeSymbol(J->tokenString[tokenCount]);				// <symbol> )
		++tokenCount;
		writeSymbol(J->tokenString[tokenCount]);				// <symbol> {
		++tokenCount;
		CompileStatements();									// print the statements
		writeSymbol(J->tokenString[tokenCount]);				// <symbol> }
		++tokenCount;
		outputFileStream << "</whileStatement>" << endl;		// </whileStatements>

		VMWriter.writeGoto("WHILE_EXP" + to_string(labelCountVector.back()));		// WHILE Restart condition
		VMWriter.writeLabel("WHILE_END" + to_string(labelCountVector.back()));		// WHILE End condition
		labelCountVector.pop_back();
		;
	}
	void CompileReturn()
	{
		outputFileStream << "<returnStatement>" << endl;		// <returnStatement>
		writeKeyword(J->tokenString[tokenCount]);				// <keyword> return
		++tokenCount;
		if (J->tokenString[tokenCount] == ";")					// if no return expression, do nothing
		{
			VMWriter.writePush(CONSTseg, 0);
		}
		else 
		{
			CompileExpression();								// otherwise, print the expression
		}
		writeSymbol(J->tokenString[tokenCount]);				// <symbol> ;
		++tokenCount;
		VMWriter.writeReturn();
		outputFileStream << "</returnStatement>" << endl;		// </returnStatement>
	}
	void CompileIf()
	{
		
		labelCountVector.push_back(ifCount);
		++ifCount;
		outputFileStream << "<ifStatement>" << endl;			// <ifStatement>
		writeKeyword(J->tokenString[tokenCount]);				// <keyword> if
		++tokenCount;
		writeSymbol(J->tokenString[tokenCount]);				// <symbol> (
		++tokenCount;

		CompileExpression();									// print the expression

		VMWriter.writeIf("IF_TRUE" + to_string(labelCountVector.back()));	// VM code for: if-goto L1
		VMWriter.writeGoto("IF_FALSE" + to_string(labelCountVector.back()));	// goto L2
		VMWriter.writeLabel("IF_TRUE" + to_string(labelCountVector.back()));  // L1// print the statements
		writeSymbol(J->tokenString[tokenCount]);				// <symbol> )
		++tokenCount;
		writeSymbol(J->tokenString[tokenCount]);				// <symbol> {
		++tokenCount;
		CompileStatements();			


		writeSymbol(J->tokenString[tokenCount]);				// <symbol> }
		++tokenCount;
		if (J->tokenString[tokenCount] == "else")				// if there is an else block, print it
		{
			VMWriter.writeGoto("IF_END" + to_string(labelCountVector.back())); // L2
			VMWriter.writeLabel("IF_FALSE" + to_string(labelCountVector.back()));  // L1// print the statements
			writeKeyword(J->tokenString[tokenCount]);			// <keyword> else
			++tokenCount;
			writeSymbol(J->tokenString[tokenCount]);			// <symbol> {
			++tokenCount;
			CompileStatements();								// print the statements
			writeSymbol(J->tokenString[tokenCount]);			// <symbol> }
			++tokenCount;
		}
		else;
		VMWriter.writeLabel("IF_END" + to_string(labelCountVector.back()));  // L1// print the statements
		labelCountVector.pop_back();
		outputFileStream << "</ifStatement>" << endl;			// </ifStatement>
	}
	void CompileExpression()
	{
		outputFileStream << "<expression>" << endl;				// <expression>
		while ((J->tokenString[tokenCount] != ")") && (J->tokenString[tokenCount] != ";") && (J->tokenString[tokenCount] != "]"))
		{
			CompileTerm();	// Print the term
			while ((J->tokenString[tokenCount] == "-") || (J->tokenString[tokenCount] == "+") || (J->tokenString[tokenCount] == "*") || (J->tokenString[tokenCount] == "/") || (J->tokenString[tokenCount] == "&") || (J->tokenString[tokenCount] == "|") || (J->tokenString[tokenCount] == "<") || (J->tokenString[tokenCount] == ">") || (J->tokenString[tokenCount] == "="))
			{
				opType = J->tokenString[tokenCount];
				writeSymbol(J->tokenString[tokenCount]);			// <symbol> op symbol
				++tokenCount;
				expressionOPVector.push_back(opType);
				CompileTerm();
			}
			outputFileStream << "</expression>" << endl;
			if (J->tokenString[tokenCount] == ",")
			{
				break;
			}
		}
		ExecuteExpression();
	}
	void ExecuteExpression()
	{
		if (expressionOPVector.size() != 0)
		{
			string currentOp = expressionOPVector.back();
			if (currentOp == "+")
			{
				VMWriter.writeArithmetic(ADD);
			}
			else if (currentOp == "-")
			{
				VMWriter.writeArithmetic(SUB);
			}
			else if (currentOp == "*")
			{
				VMWriter.writeCall("Math.multiply", 2);
			}
			else if (currentOp == "/")
			{
				VMWriter.writeCall("Math.divide", 2);
			}
			else if (currentOp == "&")
			{
				VMWriter.writeArithmetic(AND);
			}
			else if (currentOp == "|")
			{
				VMWriter.writeArithmetic(OR);
			}
			else if (currentOp == "<")
			{
				VMWriter.writeArithmetic(LT);
			}
			else if (currentOp == ">")
			{
				VMWriter.writeArithmetic(GT);
			}
			else if (currentOp == "=")
			{
				VMWriter.writeArithmetic(EQ);
			}
			else if (currentOp == "neg")
			{
				VMWriter.writeArithmetic(NEG);
			}
			else if (currentOp == "~")
			{
				VMWriter.writeArithmetic(NOT);
			}
			else if (currentOp == "[")
			{
				VMWriter.writePop(POINTERseg,1);
				VMWriter.writePush(THATseg, 0);
			}
			else;
			expressionOPVector.pop_back();
		}
			
	}
	void CompileTerm()
	{
		outputFileStream << "<term>" << endl;
			if (J->tokenList[tokenCount] == INT_CONST)				// integer constant case
			{
				VMWriter.currentSegment = CONSTseg;
				int temp = stoi(J->tokenString[tokenCount]);
				VMWriter.writePush(VMWriter.currentSegment, temp); // push constant value
				writeIntConst(J->tokenString[tokenCount]);			// <integerConst> 
				++tokenCount;
			}
			else if (J->tokenList[tokenCount] == STRING_CONST)		// string constant case
			{
				VMWriter.writePush(CONSTseg, J->tokenString[tokenCount].length()-2);
				VMWriter.writeCall("String.new", 1);
				for (unsigned i = 1; i < J->tokenString[tokenCount].length()-1; i++)
				{
					char& character = J->tokenString[tokenCount].at(i);
					int ascii = (int)character;
					VMWriter.writePush(CONSTseg, ascii);
					VMWriter.writeCall("String.appendChar", 2);
				}
				writeStringConst(J->tokenString[tokenCount]);			// <stringConst> 
				++tokenCount;
			}
			else if (J->tokenList[tokenCount] == KEYWORD)			// keyword constant case
			{
				writeKeyword(J->tokenString[tokenCount]);			// <keyword>
				if (J->tokenString[tokenCount] == "this")
				{
					VMWriter.currentSegment = POINTERseg;
					VMWriter.writePush(VMWriter.currentSegment, 0); // push this
				}
				else if (J->tokenString[tokenCount] == "that")
				{
					VMWriter.currentSegment = THATseg;
					VMWriter.writePush(VMWriter.currentSegment, 0); // push that
				}
				else if (J->tokenString[tokenCount] == "true")
				{
					VMWriter.currentSegment = CONSTseg;
					VMWriter.writePush(VMWriter.currentSegment, 0);
					VMWriter.writeArithmetic(NOT);
				}
				else if (J->tokenString[tokenCount] == "false")
				{
					VMWriter.currentSegment = CONSTseg;
					VMWriter.writePush(VMWriter.currentSegment, 0);
				}
				else
				{

				}
				++tokenCount;
			}
			else if (J->tokenList[tokenCount] == SYMBOL)			// Unary op case/expression
			{
				if (J->tokenString[tokenCount] == "(")				// expression
				{
					//negFlag = false;
					//notFlag = false;
					writeSymbol(J->tokenString[tokenCount]);			// <symbol> (
					++tokenCount;
					CompileExpression();								// print expression
					writeSymbol(J->tokenString[tokenCount]);			// <symbol> )
					++tokenCount;
				}
				else if ((J->tokenString[tokenCount] == "-")|| (J->tokenString[tokenCount] == "~"))				// "-" or "~" unary op
				{
					if (J->tokenString[tokenCount] == "-")
						
					{
						//negFlag = true;
						opType = "neg";
						expressionOPVector.push_back(opType);
					}
					else if (J->tokenString[tokenCount] == "~")
					{
						//notFlag = true;
						opType = "~";
						expressionOPVector.push_back(opType);
					}
					writeSymbol(J->tokenString[tokenCount]);			// <symbol> "unary op"
					++tokenCount;
					CompileTerm();										// unary op term
				}
				else
				{
					return;
				}
			}
			else
			{
				writeIdentifier(J->tokenString[tokenCount]);			// <identifier> varName 
				// Symbol table state definition
				symbolTable.checkDefined(J->tokenString[tokenCount]);
				symbolTable.currentVarName = J->tokenString[tokenCount];
				if (symbolTable.beingDefined)
				{
					outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintClassOrSub(CLASS) << " , " << "No Index" << " , " << "CLASS/SUB-notdefined" << ")" << endl;
				}
				else
				{
					outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintCategory(J->tokenString[tokenCount]) << " , " << symbolTable.IndexOf(J->tokenString[tokenCount]) << " , " << symbolTable.printbeingDefined << ")" << endl;
				}
				// End symbol table state definition
				++tokenCount;
				if (J->tokenString[tokenCount] == "(")					// subroutine call
				{
					writeIdentifier(J->tokenString[tokenCount]);		// <identifer> subroutineName/className/varName
					// Symbol table state definition
					outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintClassOrSub(CLASS) << " , " << "No Index" << " , " << "CLASS/SUB-notdefined" << ")" << endl;
					
					// End symbol table state definition
					++tokenCount;
					if (J->tokenString[tokenCount] == "(")
					{
						writeSymbol(J->tokenString[tokenCount]);			// <symbol> (
						++tokenCount;
						CompileExpressionList();							// calls expression list
						VMWriter.writeCall(symbolTable.currentSubName, argCount); //need to fix for nArgs
						writeSymbol(J->tokenString[tokenCount]);			// <symbol> )
						++tokenCount;
					}
					else
					{
						writeSymbol(J->tokenString[tokenCount]);			// <symbol> .
						++tokenCount;
						writeIdentifier(J->tokenString[tokenCount]);		// <identifer> subroutineName
				
																			// Symbol table state definition
						outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintClassOrSub(SUBROUTINE) << " , " << "No Index" << " , " << "CLASS/SUB-notdefined" << ")" << endl;
						symbolTable.currentSubName = symbolTable.currentSubName + "." + J->tokenString[tokenCount];

						// End symbol table state definition
						++tokenCount;
						writeSymbol(J->tokenString[tokenCount]);			// <symbol> (
						++tokenCount;
						CompileExpressionList();							// calls expression list
						VMWriter.writeCall(symbolTable.currentSubName, argCount); // need fix for nArgs
						writeSymbol(J->tokenString[tokenCount]);			// <symbol> )
						++tokenCount;
					}
					
				}
				else if (J->tokenString[tokenCount] == ".")
				{
					writeSymbol(J->tokenString[tokenCount]);			// <symbol> .
					++tokenCount;
					writeIdentifier(J->tokenString[tokenCount]);		// <identifer> subroutineName
					// Symbol table state definition
					outputFileStream << "(" << J->tokenString[tokenCount] << " , " << symbolTable.PrintClassOrSub(SUBROUTINE) << " , " << "No Index" << " , " << "CLASS/SUB-notdefined" << ")" << endl;
					symbolTable.currentSubName = symbolTable.currentVarName + "." + J->tokenString[tokenCount];

					// End symbol table state definition
					++tokenCount;
					writeSymbol(J->tokenString[tokenCount]);			// <symbol> (
					++tokenCount;
					CompileExpressionList();							// calls expression list
					VMWriter.writeCall(symbolTable.currentSubName, argCount); // need fix for nArgs
					writeSymbol(J->tokenString[tokenCount]);			// <symbol> )
					++tokenCount;
				}
				else if (J->tokenString[tokenCount] == "[")				// array
				{
					//writeSymbol(J->tokenString[tokenCount]);		// <symbol> [
					//++tokenCount;
					//CompileExpression();							// print expression
					//writeSymbol(J->tokenString[tokenCount]);		// <symbol> ]
					//++tokenCount;
					
					string arrayName = J->tokenString[tokenCount - 1];
					writeSymbol(J->tokenString[tokenCount]);			// <symbol> [
					++tokenCount;
					opType = "";
					expressionOPVector.push_back(opType); // blank operation necessary here to delay the evaluation of the expression
					CompileExpression();								// print the expression
					VMWriter.findSegment(symbolTable.KindOf(arrayName));
					VMWriter.writePush(VMWriter.currentSegment, symbolTable.IndexOf(arrayName)); // need to push base adress of array
					VMWriter.writeArithmetic(ADD); // add base array to evaluated expression
					opType = "[";
					expressionOPVector.push_back(opType); // array operation necessary here
					writeSymbol(J->tokenString[tokenCount]);			// <symbol> ]
					ExecuteExpression(); // need to execute the array operation here?
					++tokenCount;
				}
				else 
				{
					//symbolTable.currentKind = symbolTable.KindOf(J->tokenString[tokenCount - 1]));
					VMWriter.findSegment(symbolTable.KindOf(symbolTable.currentVarName));
					VMWriter.writePush(VMWriter.currentSegment, symbolTable.IndexOf(symbolTable.currentVarName)); // needs to push
				}
			}
			outputFileStream << "</term>" << endl;
	}
	void CompileExpressionList()
	{
		outputFileStream << "<expressionList>" << endl;
		if (J->tokenString[tokenCount] == ")")
		{

		}
		else
		{
			++argCount;
			CompileExpression();
			//ExecuteExpression();
			while (J->tokenString[tokenCount] == ",")
			{
				termCounter = 0; // Keep track of number of terms in the expression (maybe not needed)...
				writeSymbol(J->tokenString[tokenCount]);		// <symbol> ,
				++tokenCount;
				++argCount; // Keeps track of number of arguments in a function -- if separated by commas, we can increment by 1
				CompileExpression();
				//ExecuteExpression();

			}
			// ++argCount; // By default, if the expression list has at least 1 argument, we increment by 1.
		}
		outputFileStream << "</expressionList>" << endl;
	}
	void writeKeyword(string input)
	{
		outputFileStream << "<keyword> " << input << " </keyword>" << endl;
		return;
	}
	void writeSymbol(string input)
	{
		if (input == "\"") { input = "&quot;"; }
		else if (input == "<") { input = "&lt;"; }
		else if (input == ">") { input = "&gt;"; }
		else if (input == "&") { input = "&amp;"; }
		else;
		outputFileStream << "<symbol> " << input << " </symbol>" << endl;
		return;
	}
	void writeIdentifier(string input)
	{
		outputFileStream << "<identifier> " << input << " </identifier>" << endl;
		return;
	}
	void writeStringConst(string input)
	{
		size_t inputSize = input.size();
		string temp = input.substr(1, inputSize - 2);
		outputFileStream << "<stringConstant> " << temp << " </stringConstant>" << endl;
		return;
	}
	void writeIntConst(string input)
	{
		outputFileStream << "<integerConstant> " << input << " </integerConstant>" << endl;
		return;
	}
	~CompilationEngine()
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
	string filename(argv[1]);
	string inputFileName, asmFileName;
	string fileLocation;
	vector<string> fileList;
	_finddata_t data;
	size_t last_slash_idx = filename.find_last_of("\\/");
	if (filename.find(".jack") != string::npos)
	{
		const size_t backslashIndex = filename.rfind('\\');
		fileLocation = filename;
		fileLocation.erase(backslashIndex + 1);
		bool directoryCheck = false;
		filename.erase(0, last_slash_idx + 1);
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
	string fileNameCurrent;
	for (int i = 0; i < fileList.size(); i++) {
		fileNameCurrent = fileLocation + fileList[i];
		TokenWriter TokenWriter(fileLocation, fileList[i]);

		JackTokenizer JackTokenizer(fileNameCurrent);
		while (JackTokenizer.hasMoreCommands())
		{
			JackTokenizer.advance();
		}
		for (int j = 0; j < JackTokenizer.tokenString.size(); j++)
		{
			tokenType currentTokenType = JackTokenizer.tokenType(j);
			cout << JackTokenizer.tokenString[j] << "\t\t\t\t" << currentTokenType << endl;
			if (currentTokenType != TOKEN_NULL)
			{
				if (currentTokenType == KEYWORD)
				{
					TokenWriter.writeKeyword(JackTokenizer.tokenString[j]);
					JackTokenizer.tokenList.push_back(currentTokenType);
				}
				else if (currentTokenType == SYMBOL)
				{
					TokenWriter.writeSymbol(JackTokenizer.tokenString[j]);
					JackTokenizer.tokenList.push_back(currentTokenType);
				}
				else if (currentTokenType == IDENTIFIER)
				{
					TokenWriter.writeIdentifier(JackTokenizer.tokenString[j]);
					JackTokenizer.tokenList.push_back(currentTokenType);
				}
				else if (currentTokenType == STRING_CONST)
				{
					TokenWriter.writeStringConst(JackTokenizer.tokenString[j]);
					JackTokenizer.tokenList.push_back(currentTokenType);
				}
				else if (currentTokenType == INT_CONST)
				{
					TokenWriter.writeIntConst(JackTokenizer.tokenString[j]);
					JackTokenizer.tokenList.push_back(currentTokenType);
				}
				else;
			}
		}
		TokenWriter.writeFooter();
		TokenWriter.Close();
		CompilationEngine CompilationEngine(fileLocation, fileList[i], JackTokenizer);
		CompilationEngine.tokenCount = 0;
		CompilationEngine.CompileClass();
		CompilationEngine.Close();
	}
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