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
	
	//	size_t lastCharacter;

public:
	vector<string> tokenString;
	vector<tokenType> tokenList;
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
			// currentCommand = currentCommand.substr(0, currentCommand.find("/*"));


			size_t lastCharacter = currentCommand.find_last_not_of(" \t\r\n");
			size_t firstCharacter = currentCommand.find_first_not_of(" \t\r\n");

			if (string::npos != firstCharacter) {
				// currentCommand.substr(firstCharacter, lastCharacter - firstCharacter);
				size_t range = lastCharacter - firstCharacter + 1;
				currentCommand = currentCommand.substr(firstCharacter, range);
			}
			else return;
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
			//tokens = { istream_iterator<string>{iss},istream_iterator<string>{} };

		}
		else
			cout << "Reached end of file.." << endl;
		return;
	}


	tokenType tokenType(int j)
	{
		//firstCharacter = currentCommand.find_first_not_of(" \t\r\n");
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

	//
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
		// outputFileStreamT << "<?xml version=\"1.0\"?>" << endl;
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

class CompilationEngine : public JackTokenizer
{

	string outputFileName;
	ofstream outputFileStream;

	//
public:
	int tokenCount = 0;
	JackTokenizer* J;
	CompilationEngine(string directory, string filename, JackTokenizer &Jack)
	{
		J = &Jack;
		filename = (filename.substr(0, filename.find("."))); // remove .jack if it is there
		outputFileName = directory + filename + ".xml";
		setFileName(outputFileName);
		// writeBootstrapper();
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
		outputFileStream << "<class>" << endl;
		writeKeyword(J->tokenString[tokenCount]);
		++tokenCount;
		writeIdentifier(J->tokenString[tokenCount]);
		++tokenCount;
		writeSymbol(J->tokenString[tokenCount]);
		++tokenCount;
		while ((J->tokenString[tokenCount] == "static") || (J->tokenString[tokenCount] == "field"))
		{
			CompileClassVarDec();
		}
		while ((J->tokenString[tokenCount] == "function") || (J->tokenString[tokenCount] == "method") || (J->tokenString[tokenCount] == "constructor"))
		{
			CompileSubroutine();
		}
		++tokenCount;
		outputFileStream << "</class>" << endl;
	}

	void CompileClassVarDec()
	{
		outputFileStream << "<classVarDec>" << endl;
		writeKeyword(J->tokenString[tokenCount]);
		++tokenCount;
		if (J->tokenList[tokenCount] == KEYWORD)
		{
			writeKeyword(J->tokenString[tokenCount]);
		}
		else
		{
			writeIdentifier(J->tokenString[tokenCount]);
		}
		++tokenCount;
		writeIdentifier(J->tokenString[tokenCount]);
		++tokenCount;
		while (J->tokenString[tokenCount] == ",")
		{
			writeIdentifier(J->tokenString[tokenCount]);
			++tokenCount;
		}
		writeSymbol(J->tokenString[tokenCount]);
		++tokenCount;
		outputFileStream << "</classVarDec>" << endl;
	}
	void CompileSubroutine()
	{
		outputFileStream << "<subroutineDec>" << endl;
		writeKeyword(J->tokenString[tokenCount]);
		++tokenCount;
		if (J->tokenList[tokenCount] == KEYWORD)
		{
			writeKeyword(J->tokenString[tokenCount]);
		}
		else
		{
			writeIdentifier(J->tokenString[tokenCount]);
		}
		++tokenCount;
		writeIdentifier(J->tokenString[tokenCount]);
		++tokenCount;
		writeSymbol(J->tokenString[tokenCount]);
		++tokenCount;
		CompileParameterList();
		writeSymbol(J->tokenString[tokenCount]);
		++tokenCount;
		outputFileStream << "<subroutineBody>" << endl;
		writeSymbol(J->tokenString[tokenCount]);
		++tokenCount;
		while (J->tokenString[tokenCount] == "var")
		{
			CompileVarDec();
			++tokenCount;
		}
		CompileStatements();
		++tokenCount;
		outputFileStream << "</subroutineDec>" << endl;
	}
	void CompileParameterList()
	{
		outputFileStream << "<parameterList>" << endl;
		if (J->tokenString[tokenCount] == ")")
		{
			outputFileStream << "</parameterList>" << endl;
		}
		else
		{
			writeIdentifier(J->tokenString[tokenCount]);
			++tokenCount;
			writeIdentifier(J->tokenString[tokenCount]);
			++tokenCount;
			while (J->tokenString[tokenCount] == ",")
			{
				writeIdentifier(J->tokenString[tokenCount]);
				++tokenCount;
			}
		}
		outputFileStream << "</parameterList>" << endl;
	}
	void CompileVarDec()
	{
		outputFileStream << "<varDec>" << endl;
		writeKeyword(J->tokenString[tokenCount]);
		++tokenCount;
		writeIdentifier(J->tokenString[tokenCount]);
		++tokenCount;
		writeIdentifier(J->tokenString[tokenCount]);
		++tokenCount;
		while (J->tokenString[tokenCount] == ",")
		{
			writeIdentifier(J->tokenString[tokenCount]);
			++tokenCount;
		}
		writeSymbol(J->tokenString[tokenCount]);
		++tokenCount;
		outputFileStream << "</varDec>" << endl;
	}
	void CompileStatements()
	{
		outputFileStream << "<statements>" << endl;
		while ((J->tokenString[tokenCount] == "let") || (J->tokenString[tokenCount] == "if") || (J->tokenString[tokenCount] == "while") || (J->tokenString[tokenCount] == "do")||(J->tokenString[tokenCount] == "return"))
		{
			if (J->tokenString[tokenCount] == "let")
			{
				CompileLet();
				++tokenCount;
			}
			else if (J->tokenString[tokenCount] == "if")
			{
				CompileIf();
				++tokenCount;
			}
			else if (J->tokenString[tokenCount] == "while")
			{
				CompileWhile();
				++tokenCount;
			}
			else if (J->tokenString[tokenCount] == "do")
			{
				CompileDo();
				++tokenCount;
			}
			else if (J->tokenString[tokenCount] == "return")
			{
				CompileReturn();
				++tokenCount;
			}
			else {}
		}
		outputFileStream << "</statements>" << endl;
	}
	void CompileDo()
	{
		outputFileStream << "<doStatement>" << endl;
		writeKeyword(J->tokenString[tokenCount]);
		++tokenCount;
		writeIdentifier(J->tokenString[tokenCount]);
		++tokenCount;
		CompileExpressionList();
		writeSymbol(J->tokenString[tokenCount]);
		++tokenCount;
		writeSymbol(J->tokenString[tokenCount]);
		++tokenCount;
		outputFileStream << "</doStatement>" << endl;
	}
	void CompileLet()
	{
		outputFileStream << "<letStatement>" << endl;
		writeKeyword(J->tokenString[tokenCount]);
		++tokenCount;
		writeIdentifier(J->tokenString[tokenCount]);
		++tokenCount;
		if (J->tokenString[tokenCount] == "[")
		{
			writeSymbol(J->tokenString[tokenCount]);
			++tokenCount;
			CompileExpression();
			writeSymbol(J->tokenString[tokenCount]);
			++tokenCount;
		}
		else;
		writeSymbol(J->tokenString[tokenCount]);
		++tokenCount;
		CompileExpression();
		writeSymbol(J->tokenString[tokenCount]);
		++tokenCount;
		outputFileStream << "</letStatement>" << endl;
	}
	void CompileWhile()
	{
		outputFileStream << "<whileStatement>" << endl;
		writeKeyword(J->tokenString[tokenCount]);
		++tokenCount;
		writeSymbol(J->tokenString[tokenCount]);
		++tokenCount;
		CompileExpression();
		writeSymbol(J->tokenString[tokenCount]);
		++tokenCount;
		writeSymbol(J->tokenString[tokenCount]);
		++tokenCount;
		CompileStatements();
		++tokenCount;
		writeSymbol(J->tokenString[tokenCount]);
		++tokenCount;
		outputFileStream << "</whileStatement>" << endl;
	}
	void CompileReturn()
	{
		outputFileStream << "<returnStatement>" << endl;
		writeKeyword(J->tokenString[tokenCount]);
		++tokenCount;
		if (J->tokenString[tokenCount] == ";")
		{

		}
		else 
		{
			CompileExpression();
			++tokenCount;
		}
		writeSymbol(J->tokenString[tokenCount]);
		++tokenCount;
		outputFileStream << "</returnStatement>" << endl;
	}
	void CompileIf()
	{
		outputFileStream << "<ifStatement>" << endl;
		writeKeyword(J->tokenString[tokenCount]);
		++tokenCount;
		writeSymbol(J->tokenString[tokenCount]);
		++tokenCount;
		CompileExpression();
		writeSymbol(J->tokenString[tokenCount]);
		++tokenCount;
		writeSymbol(J->tokenString[tokenCount]);
		++tokenCount;
		CompileStatements();
		++tokenCount;
		if (J->tokenString[tokenCount] == "else")
		{
			writeKeyword(J->tokenString[tokenCount]);
			++tokenCount;
			writeSymbol(J->tokenString[tokenCount]);
			++tokenCount;
			CompileStatements();
			writeSymbol(J->tokenString[tokenCount]);
			++tokenCount;
		}
		else;
		outputFileStream << "</ifStatement>" << endl;
	}
	void CompileExpression()
	{
		outputFileStream << "<expression>" << endl;
	}
	void CompileTerm()
	{
		outputFileStream << "<term>" << endl;
	}
	void CompileExpressionList()
	{
		outputFileStream << "<expressionlist>" << endl;
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

	
//	TokenWriter TokenWriter(fileLocation, asmFileName);
	string fileNameCurrent;
	for (int i = 0; i < fileList.size(); i++) {
		fileNameCurrent = fileLocation + fileList[i];
		TokenWriter TokenWriter(fileLocation, fileList[i]);

		JackTokenizer JackTokenizer(fileNameCurrent);
		while (JackTokenizer.hasMoreCommands())
		{
			//	currentCode = "";
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