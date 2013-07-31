#ifndef _LEXER_H_
#define _LEXER_H_

#include <iostream>
#include <fstream>
#include <list>
#include <string>
using namespace std;

enum TokenType {
	TOKEN_INT = 0, TOKEN_FLOAT, TOKEN_STRING, TOKEN_ID,
	TOKEN_FOR, TOKEN_WHILE, TOKEN_IF, TOKEN_ELSE, TOKEN_ELIF,
	TOKEN_END, TOKEN_RETURN, TOKEN_BREAK, TOKEN_CONTINUE,
	TOKEN_DEF, TOKEN_IN,
	TOKEN_LBRACKETS, TOKEN_RBRACKETS, TOKEN_LMBRACKETS,
	TOKEN_RMBRACKETS, TOKEN_LBBRACKETS, TOKEN_RBBRACKETS,
	TOKEN_COMMA, TOKEN_COLON,
	TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULTI, TOKEN_DIV, TOKEN_MOD,
	TOKEN_PLUS_ASSIGN, TOKEN_MINUS_ASSIGN, TOKEN_MULTI_ASSIGN,
	TOKEN_DIV_ASSIGN, TOKEN_MOD_ASSIGN,
	TOKEN_INC, TOKEN_DEC,
	TOKEN_BIT_AND, TOKEN_BIT_OR, TOKEN_BIT_XOR, TOKEN_BIT_NOT
	TOKEN_BIT_AND_ASSIGN, TOKEN_BIT_OR_ASSIGN,
	TOKEN_BIT_XOR_ASSIGN, TOKEN_BIT_NOT_ASSIGN,
	TOKEN_ASSIGN, TOKEN_INVOKE,
	TOKEN_EQUAL, TOKEN_NOT_EQUAL, TOKEN_MORE, TOKEN_LESS, TOKEN_GE, TOKEN_LE,
	TOKEN_AND, TOKEN_OR, TOKEN_NOT,
	TOKEN_EOL, TOKEN_UNKNOWN
};

class Token
{
private:
	int line_num;
	string content;
	TokenType type;

public:
	Token(const char* ctt, TokenType t, int ln) : content(ctt), type(t), line_num(ln) {};
	Token(const string& ctt, TokenType t, int ln) : content(ctt), type(t), line_num(ln) {};

	string& GetContent() { return content; };
	TokenType GetType() { return type; };
	int GetLine() { return line_num; };
};

typedef list<Token*> TokenList;
typedef list<Token*>::iterator TokenIterator;

class Lexer
{
private:
	istream& in;
	TokenList token_list;
	bool fail;
	TokenIterator it;
	int line;

private:
	bool MatchNumber();
	bool MatchString();
	bool MatchIdFirst();
	bool MatchId();
	bool MatchSymbol();
	bool MatchOperation();
	bool MatchLogicalOperation();
	bool MatchEOL();
	bool MatchComment();
	bool MatchBlank();

	void Consume();
	void ConsumeComment();

	void AddNumber();
	void AddString();
	void AddId();

	void AddLBrackets();
	void AddRBrackets();
	void AddLMBrackets();
	void AddRMBrackets();
	void AddLBBrackets();
	void AddRBBrackets();
	void AddComma();
	void AddColon();

	void AddPlus();
	void AddMinus();
	void AddMulti();
	void AddDiv();
	void AddMod();

	void AddAnd();
	void AddOr();
	void AddXor();
	void AddNot();
	void AddAssign();
	void AddInvoke();

	void AddMore();
	void AddLess();

	void AddEOL();

	void ProcessUnknownToken();

public:
	Lexer(istream& is);
	~Lexer();

	bool IsFail() { return fail; };

	TokenIterator StartIterate() { it = token_list.begin(); return it; };
	TokenIterator RestartIterate() { it = token_list.begin(); return it; };
	void SetPosition(TokenIterator i) { it = i; };
	void Next() { ++it; };
	void Prev() { --it; };
	TokenIterator Peek() { return it; };
	TokenIterator Get() { return it++; };
	bool IsBegin() { return (it == token_list.begin()); };
	bool IsEnd() { return (it == token_list.end()); };
};

#endif