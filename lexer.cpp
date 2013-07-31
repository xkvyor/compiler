#include "lexer.h"

bool Lexer::MatchEOL()
{
	char c = in.peek();
	return (c == '\n' || c == EOF);
}

bool Lexer::MatchIdFirst()
{
	char c = in.peek();
	return ((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| c == '_');
}

bool Lexer::MatchId()
{
	char c = in.peek();
	return ((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9')
		|| c == '_');
}

bool Lexer::MatchNumber()
{
	char c = in.peek();
	return (c >= '0' && c <= '9');
}

bool Lexer::MatchString()
{
	char c = in.peek();
	return (c == '\'' || c == '\"');
}

bool Lexer::MatchOperation()
{
	char c = in.peek();
	return (c == '+' || c == '-' || c == '%'
		|| c == '*' || c == '/' || c == '.'
		|| c == '>' || c == '<');
}

bool Lexer::MatchLogicalOperation()
{
	char c = in.peek();
	return (c == '&' || c == '|' || c == '!'
		|| c == '^' || c == '=' || c == '~');
}

bool Lexer::MatchSymbol()
{
	char c = in.peek();
	return (c == '(' || c == ')' || c == '['
		|| c == ']' || c == '{' || c == '}'
		|| c == ',' || c == ':');
}

bool Lexer::MatchBlank()
{
	char c = in.peek();
	return (c == '\r' || c == ' '
		|| c == '\t' || c == '\f');
}

bool Lexer::MatchComment()
{
	char c = in.peek();
	return (c == '#');
}

void Lexer::Consume()
{
	while(!in.eof() && MatchBlank())
		in.get();
}

void Lexer::ConsumeComment()
{
	while(!in.eof() && !MatchEOL())
		in.get();
	in.get();
	++line;
}

void Lexer::AddNumber()
{
	string buf;
	TokenType type = TOKEN_INT;
	while(!in.eof() && MatchNumber())
		buf.push_back(in.get());
	if (in.peek() == '.')
	{
		type = TOKEN_FLOAT;
		do
		{
			buf.push_back(in.get());
		} while(!in.eof() && MatchNumber());
	}
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddString()
{
	string buf;
	TokenType type = TOKEN_STRING;
	char ter = in.get();
	while(!in.eof() && in.peek() != ter)
		buf.push_back(in.get());
	in.get();
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddId()
{
	string buf;
	TokenType type = TOKEN_ID;
	while(!in.eof() && MatchId())
		buf.push_back(in.get());
	if (buf == "for")
	{
		type = TOKEN_FOR;
	}
	else if (buf == "while")
	{
		type = TOKEN_WHILE;
	}
	else if (buf == "if")
	{
		type = TOKEN_IF;
	}
	else if (buf == "else")
	{
		type = TOKEN_ELSE;
	}
	else if (buf == "elif")
	{
		type = TOKEN_ELIF;
	}
	else if (buf == "end")
	{
		type = TOKEN_END;
	}
	else if (buf == "def")
	{
		type = TOKEN_DEF;
	}
	else if (buf == "break")
	{
		type = TOKEN_BREAK;
	}
	else if (buf == "return")
	{
		type = TOKEN_RETURN;
	}
	else if (buf == "continue")
	{
		type = TOKEN_CONTINUE;
	}
	else if (buf == "in")
	{
		type = TOKEN_IN;
	}
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddLBrackets()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_LBRACKETS;
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddRBrackets()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_RBRACKETS;
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddLMBrackets()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_LMBRACKETS;
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddRMBrackets()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_RMBRACKETS;
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddLBBrackets()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_LBBRACKETS;
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddRBBrackets()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_LBBRACKETS;
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddComma()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_COMMA;
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddColon()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_COLON;
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddPlus()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_PLUS;
	if (in.peek() == '+')
	{
		buf[1] = in.get();
		type = TOKEN_INC;
	}
	else if (in.peek() == '=')
	{
		buf[1] = in.get();
		type = TOKEN_PLUS_ASSIGN;
	}
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddMinus()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_MINUS;
	if (in.peek() == '-')
	{
		buf[1] = in.get();
		type = TOKEN_DEC;
	}
	else if (in.peek() == '=')
	{
		buf[1] = in.get();
		type = TOKEN_MINUS_ASSIGN;
	}
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddMulti()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_MULTI;
	if (in.peek() == '=')
	{
		buf[1] = in.get();
		type = TOKEN_MULTI_ASSIGN;
	}
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddDiv()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_DIV;
	if (in.peek() == '=')
	{
		buf[1] = in.get();
		type = TOKEN_DIV_ASSIGN;
	}
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddMod()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_MOD;
	if (in.peek() == '=')
	{
		buf[1] = in.get();
		type = TOKEN_MOD_ASSIGN;
	}
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddAnd()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_BIT_AND;
	if (in.peek() == '=')
	{
		buf[1] = in.get();
		type = TOKEN_BIT_AND_ASSIGN;
	}
	else if (in.peek() == '&')
	{
		buf[1] = in.get();
		type = TOKEN_AND;
	}
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddOr()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_BIT_OR;
	if (in.peek() == '=')
	{
		buf[1] = in.get();
		type = TOKEN_BIT_OR_ASSIGN;
	}
	else if (in.peek() == '|')
	{
		buf[1] = in.get();
		type = TOKEN_OR;
	}
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddXor()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_BIT_XOR;
	if (in.peek() == '=')
	{
		buf[1] = in.get();
		type = TOKEN_BIT_XOR_ASSIGN;
	}
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddNot()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_NOT;
	if (in.peek() == '=')
	{
		buf[1] = in.get();
		type = TOKEN_NOT_EQUAL;
	}
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddMore()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_MORE;
	if (in.peek() == '=')
	{
		buf[1] = in.get();
		type = TOKEN_GE;
	}
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddLess()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_LESS;
	if (in.peek() == '=')
	{
		buf[1] = in.get();
		type = TOKEN_LE;
	}
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddBitNot()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_BIT_NOT;
	if (in.peek() == '=')
	{
		buf[1] = in.get();
		type = TOKEN_BIT_NOT_ASSIGN;
	}
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddAssign()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_ASSIGN;
	if (in.peek() == '=')
	{
		buf[1] = in.get();
		type = TOKEN_EQUAL;
	}
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddInvoke()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_INVOKE;
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
}

void Lexer::AddEOL()
{
	char buf[4] = {0};
	buf[0] = in.get();
	TokenType type = TOKEN_EOL;
	Token* t = new Token(buf, type, line);
	token_list.push_back(t);
	++line;
}

void Lexer::ProcessUnknownToken()
{
	fail = true;
	cerr << "[Error] Unknown Token \'" << in.get()
		<< "\' is found at line: " << line << endl;
}

Lexer::Lexer(istream& is) : in(is)
{
	line = 1;
	fail = false;

	while(!in.eof())
	{
		char c = in.peek();
		if (MatchBlank())
		{
			Consume();
		}
		else if (MatchComment())
		{
			ConsumeComment();
		}
		else if (MatchNumber())
		{
			AddNumber();
		}
		else if (MatchString())
		{
			AddString();
		}
		else if (MatchIdFirst())
		{
			AddId();
		}
		else if (MatchSymbol())
		{
			switch(c)
			{
			case '(':
				AddLBrackets();
				break;
			case ')':
				AddRBrackets();
				break;
			case '[':
				AddLMBrackets();
				break;
			case ']':
				AddRMBrackets();
				break;
			case '{':
				AddLBBrackets();
				break;
			case '}':
				AddRBBrackets();
				break;
			case ',':
				AddComma();
				break;
			case ':':
				AddColon();
				break;
			}
		}
		else if (MatchOperation())
		{
			switch(c)
			{
			case '+':
				AddPlus();
				break; 
			case '-':
				AddMinus();
				break; 
			case '*':
				AddMulti();
				break; 
			case '/':
				AddDiv();
				break; 
			case '%':
				AddMod();
				break;
			case '.':
				AddInvoke();
				break;
			case '>':
				AddMore();
				break;
			case '<':
				AddLess();
				break;
			}
		}
		else if (MatchLogicalOperation())
		{
			switch(c)
			{
			case '&':
				AddAnd();
				break;
			case '|':
				AddOr();
				break;
			case '^':
				AddXor();
				break;
			case '!':
				AddNot();
				break;
			case '=':
				AddAssign();
				break;
			case '~':
				AddBitNot();
				break;
			}
		}
		else if (MatchEOL())
		{
			AddEOL();
		}
		else
		{
			ProcessUnknownToken();
		}
	}
}

Lexer::~Lexer()
{
	for (list<Token*>::iterator it = token_list.begin();
		it != token_list.end(); ++it)
	{
		if (*it)
		{
			delete *it;
		}
	}
}
