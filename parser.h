#ifndef _PARSER_H_
#define _PARSER_H_

#include <string>
#include <list>
#include <exception>
using namespace std;

class ParseException: public exception
{
private:
	string err_msg;

public:
	ParseException(const char* msg) : err_msg(msg)
	{}
	ParseException(const string& msg) : err_msg(msg)
	{}

	virtual const string& what() const throw()
	{
		return err_msg;
	}
};

class Lexer;
class ASTNode;

typedef list<ASTNode*> ParamList;

enum ASTType = {
	AST_STATEMENT, AST_ASSIGN, AST_IF,
	AST_FUNCTION, AST_FOR, AST_LOOP,
	AST_WHILE, AST_RETURN, AST_BREAK,
	AST_CONTINUE, AST_PASS,
	AST_EXPR, AST_BOOL, AST_LOGIC, AST_CMP,
	AST_ADD, AST_UNARY, AST_POST,
	AST_UNARYOP, AST_PRIMARY, AST_VAR,
	AST_LIST, AST_DICT, AST_ELEMENTS, AST_MAPPING,
	AST_VALUE, AST_ID, AST_PROGRAM
};

class ASTNode
{
private:
	ASTType type;
	int line;

public:
	ASTNode(ASTType t, int l) : type(t), line(l) {};
	virtual ~ASTNode() {};

	bool MatchToken(Token& token, TokenType type);
	ASTType GetType() { return type; };
	int GetLine() { return line; };
};

class ProgramNode: public ASTNode
{
private:
	ParamList statements;

public:
	ProgramNode() : ASTNode(ASTType::AST_PROGRAM, 0)
	{}
	~ProgramNode();

	void AddStatement(ASTNode* node) { statements.push_back(node); };
};

class StateNode: public ASTNode
{
private:
	ParamList statements;

public:
	StateNode(int l) : ASTNode(ASTType::AST_STATEMENT, l)
	{}
	~ProgramNode();

	
};

class Parser
{
private:
	Lexer& lexer;
	ASTNode* root;

	bool MatchToken(TokenType type);
	bool MatchTokenMultiLine(TokenType type);

	ASTNode* statement();
	ASTNode* assign();
	ASTNode* ifstat();
	ASTNode* functionstat();
	ASTNode* forloop();
	ASTNode* whileloop();
	ASTNode* returnstat();
	ASTNode* breakstat();
	ASTNode* continuestat();
	ASTNode* passstat();

	ASTNode* expr();
	ASTNode* boolexpr();
	ASTNode* logicexpr();
	ASTNode* cmpexpr();
	ASTNode* addexpr();
	ASTNode* unaryexpr();
	ASTNode* postexpr();

	ASTNode* unaryop();
	ASTNode* priexpr();
	ASTNode* var();
	ASTNode* list();
	ASTNode* dict();
	ASTNode* elements();
	ASTNode* mapping();
	
public:
	Parser(Lexer& lex);
	~Parser();
};

#endif