#include "lexer.h"
#include "parser.h"

ProgramNode::~ProgramNode()
{
	for (ParamList::iterator it = statements.begin();
		it != statements.end(); ++it)
	{
		delete *it;
	}
}

Parser::Parser(Lexer& lex) : lexer(lex), root(NULL)
{}

Parser::~Parser()
{
	if (root)
	{
		delete root;
		root = NULL;
	}
}

bool Parser::MatchToken(TokenType type)
{
	if (lexer.IsEnd())
	{
		ostringstream oss;
		oss << "[Error] Can not found token: " << type
			<< endl;
		throw ParseException(oss.str());
	}
	return (lexer.Peek()->GetType() == type);
}

bool Parser::MatchTokenMultiLine(TokenType type)
{
	while(MatchToken(TokenType::TOKEN_EOL))
		lexer.Next();
	return (lexer.Peek()->GetType() == type);
}

void Parser::MustMatch(TokenType type)
{
	Token* token = lexer.Get();
	if (token->GetType() != type)
	{
		int line = token->GetLine();
		ostringstream oss;
		oss << "[Error] Expect " << type << " but "
			<< token->GetType() << " at line " << line
			<< endl;
		throw ParseException(oss.str());
	}
}

void Parser::Parse()
{
	root = new ProgramNode();
	lexer.StartIterate();
	while(!lexer.End())
	{
		if (MatchToken(TokenType::EOL))
			continue;

		ASTNode* stat = statement();
		root->AddStatement(stat);
	}
}

ASTNode* Parser::statement()
{
	ASTNode* tmp = NULL;

	if (MatchToken(TOKEN_IF))
	{
		tmp = ifstat();
	}
	else if (MatchToken(TOKEN_DEF))
	{
		tmp = functiondef();
	}
	else if (MatchToken(TOKEN_FOR))
	{
		tmp = forloop();
	}
	else if (MatchToken(TOKEN_WHILE))
	{
		tmp = whileloop();
	}
	else if (MatchToken(TOKEN_RETURN))
	{
		tmp = returnstat();
	}
	else if (MatchToken(TOKEN_BREAK))
	{
		tmp = breakstat();
	}
	else if (MatchToken(TOKEN_CONTINUE))
	{
		tmp = continuestat();
	}
	else
	{
		tmp = assign();
	}

	return tmp;
}

ASTNode* Parser::ifstat()
{
	int line = lexer.Peek()->GetLine();

	MustMatch(TOKEN_IF);

	IfNode* tmp = new IfNode(line);
	
	ExprNode* ep = expr();
	tmp->SetCondition(ep);

	MustMatch(TOKEN_EOL);

	while(MatchToken(TOKEN_END) == false)
	{
		if (MatchToken(TOKEN_EOL))
		{
			lexer.Next();
			continue;
		}
		ASTNode* stat = statment();
		tmp->AddStatement(stat);
	}

	MustMatch(TOKEN_END);
	MustMatch(TOKEN_EOL);

	return tmp;
}

ASTNode* Parser::functiondef()
{
	int line = lexer.Peek()->GetLine();

	MustMatch(TOKEN_DEF);

	FunctionNode* tmp = new FunctionNode(line);
	
	if (MatchToken(TOKEN_ID))
	{
		IdNode* id = new IdNode(lexer.Peek()->GetLine(),
								lexer.Peek()->GetContent());
		lexer.Next();
		tmp->SetFunction(id);
	}
	else
	{
		ostringstream oss;
		oss << "[Error] Expect function name but "
			<< lexer.Peek()->GetType() << " at line " << line
			<< endl;
		throw ParseException(oss.str());
	}

	MustMatch(TOKEN_LBRACKETS);

	ElementsNode* elm = elements();
	tmp->SetParam(elm);

	MustMatch(TOKEN_RBRACKETS);

	MustMatch(TOKEN_EOL);

	while(MatchToken(TOKEN_END) == false)
	{
		if (MatchToken(TOKEN_EOL))
		{
			lexer.Next();
			continue;
		}
		ASTNode* stat = statment();
		tmp->AddStatement(stat);
	}

	MustMatch(TOKEN_END);
	MustMatch(TOKEN_EOL);

	return tmp;
}

ASTNode* Parser::forloop()
{
	int line = lexer.Peek()->GetLine();

	MustMatch(TOKEN_FOR);

	ForNode* tmp = new FunctionNode(line);
	
	if (MatchToken(TOKEN_ID))
	{
		IdNode* id = new IdNode(lexer.Peek()->GetLine(),
								lexer.Peek()->GetContent());
		tmp->SetIterator(id);
		lexer.Next();
	}
	else
	{
		ostringstream oss;
		oss << "[Error] Expect function name but "
			<< lexer.Peek()->GetType() << " at line " << line
			<< endl;
		throw ParseException(oss.str());
	}

	MustMatch(TOKEN_IN);

	ExprNode* ep = expr();
	tmp->SetIterList(ep);

	MustMatch(TOKEN_EOL);

	while(MatchToken(TOKEN_END) == false)
	{
		if (MatchToken(TOKEN_EOL))
		{
			lexer.Next();
			continue;
		}
		ASTNode* stat = statment();
		tmp->AddStatement(stat);
	}

	MustMatch(TOKEN_END);
	MustMatch(TOKEN_EOL);

	return tmp;
}

ASTNode* Parser::whileloop()
{
	int line = lexer.Peek()->GetLine();

	MustMatch(TOKEN_WHILE);

	WhileNode* tmp = new WhileNode(line);

	ExprNode* ep = expr();
	tmp->SetCondition(ep);

	MustMatch(TOKEN_EOL);

	while(MatchToken(TOKEN_END) == false)
	{
		if (MatchToken(TOKEN_EOL))
		{
			lexer.Next();
			continue;
		}
		ASTNode* stat = statment();
		tmp->AddStatement(stat);
	}

	MustMatch(TOKEN_END);
	MustMatch(TOKEN_EOL);

	return tmp;
}

ASTNode* Parser::breakstat()
{
	int line = lexer.Peek()->GetLine();

	MustMatch(TOKEN_BREAK);

	BreakNode* tmp = new BreakNode(line);

	MustMatch(TOKEN_EOL);

	return tmp;
}

ASTNode* Parser::continuestat()
{
	int line = lexer.Peek()->GetLine();

	MustMatch(TOKEN_CONTINUE);

	ContinueNode* tmp = new ContinueNode(line);

	MustMatch(TOKEN_EOL);

	return tmp;
}

ASTNode* Parser::returnstat()
{
	int line = lexer.Peek()->GetLine();

	MustMatch(TOKEN_RETURN);

	ReturnNode* tmp = new ReturnNode(line);

	ExprNode* ep = expr();
	tmp->SetReturn(ep);

	MustMatch(TOKEN_EOL);

	return tmp;
}

ASTNode* Parser::assign()
{
	int line = lexer.Peek()->GetLine();

	TokenIterator it = lexer.Peek();

	ASTNode* tmp;
	ASTNode* ue = unaryexpr();
	if (MatchToken(TokenType::TOKEN_PLUS_ASSIGN))
	{
		ASTNode* as = new AssignNode(lexer.Get()->GetType(), line);
		as->SetLeft(ue);
		ASTNode* rv = assign();
		as->SetRight(rv);
		tmp = as;
	}
	else
	{
		delete ue;
		lexer.SetPosition(it);
		ASTNode* ex = expr();
		tmp = ex;
	}

	MustMatch(TOKEN_EOL);
	return tmp;
}

ASTNode* Parser::expr()
{
	int line = lexer.Peek()->GetLine();
	ASTNode* tmp = boolexpr();

	if (MatchToken(TokenType::TOKEN_AND)
		|| MatchToken(TokenType::TOKEN_OR)
		|| MatchToken(TokenType::TOKEN_EQUAL)
		|| MatchToken(TokenType::TOKEN_NOT)
		|| MatchToken(TokenType::TOKEN_NOT_EQUAL))
	{
		BoolNode* bl = new BoolNode(lexer.Get()->GetType(), line);
		bl->SetLeft(tmp);
		tmp = expr();
		bl->SetRight(tmp);

		return bl;
	}

	return tmp;
}

ASTNode* Parser::boolexpr()
{
	int line = lexer.Peek()->GetLine();
	ASTNode* tmp = logicexpr();

	if (MatchToken(TokenType::TOKEN_BIT_AND)
		|| MatchToken(TokenType::TOKEN_BIT_OR)
		|| MatchToken(TokenType::TOKEN_BIT_XOR))
	{
		LogicNode* ln = new LogicNode(lexer.Get()->GetType(), line);
		ln->SetLeft(tmp);
		tmp = boolexpr();
		ln->SetRight(tmp);

		return ln;
	}

	return tmp;
}

ASTNode* Parser::logicexpr()
{
	int line = lexer.Peek()->GetLine();
	ASTNode* tmp = cmpexpr();

	if (MatchToken(TokenType::TOKEN_EQUAL)
		|| MatchToken(TokenType::TOKEN_NOT_EQUAL)
		|| MatchToken(TokenType::TOKEN_MORE)
		|| MatchToken(TokenType::TOKEN_GE)
		|| MatchToken(TokenType::TOKEN_LESS)
		|| MatchToken(TokenType::TOKEN_LE))
	{
		CompareNode* cn = new CompareNode(lexer.Get()->GetType(), line);
		cn->SetLeft(tmp);
		tmp = logicexpr();
		cn->SetRight(tmp);

		return cn;
	}

	return tmp;
}

ASTNode* Parser::cmpexpr()
{
	int line = lexer.Peek()->GetLine();
	ASTNode* tmp = addexpr();

	if (MatchToken(TokenType::TOKEN_PLUS)
		|| MatchToken(TokenType::TOKEN_MINUS))
	{
		AddNode* an = new AddNode(lexer.Get()->GetType(), line);
		an->SetLeft(tmp);
		tmp = cmpexpr();
		an->SetRight(tmp);

		return an;
	}

	return tmp;
}

ASTNode* Parser::addexpr()
{
	int line = lexer.Peek()->GetLine();
	ASTNode* tmp = unaryexpr();

	if (MatchToken(TokenType::TOKEN_MULTI)
		|| MatchToken(TokenType::TOKEN_DIV)
		|| MatchToken(TokenType::TOKEN_MOD))
	{
		MultiNode* mn = new MultiNode(lexer.Get()->GetType(), line);
		mn->SetLeft(tmp);
		tmp = addexpr();
		mn->SetRight(tmp);

		return mn;
	}

	return tmp;
}

ASTNode* Parser::unaryexpr()
{
	int line = lexer.Peek()->GetLine();
	
	ASTNode* tmp;

	if (MatchToken(TokenType::TOKEN_INC)
		|| MatchToken(TokenType::TOKEN_DEC)
		|| MatchToken(TokenType::TOKEN_PLUS)
		|| MatchToken(TokenType::TOKEN_MINUS)
		|| MatchToken(TokenType::TOKEN_NOT)
		|| MatchToken(TokenType::TOKEN_BIT_NOT))
	{
		tmp = new PreUnaryNode(lexer.Get()->GetType, line);
		ASTNode* ue = unaryexpr();
		tmp->SetParam(ue);
	}
	else
	{
		tmp = postexpr();
	}

	return tmp;
}

ASTNode* Parser::postexpr()
{
	int line = lexer.Peek()->GetLine();
	
	ASTNode* tmp = priexpr();

	while(true)
	{
		if (MatchToken(TokenType::TOKEN_INC)
			|| MatchToken(TokenType::TOKEN_DEC))
		{
			PostUnaryNode* pn = new PostUnaryNode(lexer.Get()->GetType, line);
			pn->SetParam(tmp);
			tmp = pn;
		}
		else if (MatchToken(TokenType::TOKEN_INVOKE))
		{
			InvokeNode* in = new InvokeNode(line);
			in->SetInstance(tmp);
			lexer.Next();
			if (MatchToken(TokenType::TOKEN_ID))
			{
				IdNode* id = new IdNode(lexer.Peek()->GetLine(),
								lexer.Peek()->GetContent());
				in->SetAttr(id);
				lexer.Next();
				tmp = in;
			}
			else
			{
				ostringstream oss;
				oss << "[Error] Expect attibute name but "
					<< lexer.Get()->GetType() << " at line " << line
					<< endl;
				throw ParseException(oss.str());
			}
		}
		else if (MatchToken(TOKEN_LBRACKETS))
		{
			ASTNode* cn = new CallNode(lexer.Get()->GetLine());
			cn->SetFunction(tmp);
			if (!MatchToken(TOKEN_RBRACKETS))
			{
				ASTNode* elm = elements();
				cn->SetParams(elm);
			}
			MustMatch(TOKEN_RBRACKETS);
			tmp = cn;
		}
		else if (MatchToken(TOKEN_LMBRACKETS))
		{
			ASTNode* in = new IndexNode(lexer.Get()->GetLine());
			in->SetSource(tmp);
			ASTNode* idx = expr();
			in->SetIndex(idx);
			MustMatch(TOKEN_RMBRACKETS);
			tmp = in;
		}
		else
		{
			break;
		}
	}

	return tmp;
}

ASTNode* Parser::priexpr()
{
	int line = lexer.Peek()->GetLine();
	
	ASTNode* tmp;

	if (MatchToken(TokenType::TOKEN_LBRACKETS))
	{
		tmp = expr();
	}
	else
	{
		tmp = variable();
	}

	return tmp;
}

ASTNode* Parser::variable()
{
	int line = lexer.Peek()->GetLine();
	
	ASTNode* tmp;

	if (MatchToken(TokenType::TOKEN_ID))
	{
		tmp = new IdNode(lexer.Peek()->GetLine(),
						lexer.Peek()->GetContent());
		lexer.Next();
	}
	else if (MatchToken(TokenType::TOKEN_STRING))
	{
		tmp = new StringNode(lexer.Peek()->GetLine(),
						lexer.Peek()->GetContent());
		lexer.Next();
	}
	else if (MatchToken(TokenType::TOKEN_INT))
	{
		tmp = new IntNode(lexer.Peek()->GetLine(),
						lexer.Peek()->GetContent());
		lexer.Next();
	}
	else if (MatchToken(TokenType::TOKEN_FLOAT))
	{
		tmp = new FloatNode(lexer.Peek()->GetLine(),
						lexer.Peek()->GetContent());
		lexer.Next();
	}
	else if (MatchToken(TokenType::TOKEN_LMBRACKETS))
	{
		tmp = new ListNode(lexer.Get()->GetLine());
		if (!MatchToken(TokenType::TOKEN_RMBRACKETS))
		{
			ElementsNode* elm = elements();
			tmp->SetElements(elm);
		}
		MustMatch(TokenType::TOKEN_RMBRACKETS);
	}
	else if (MatchToken(TokenType::TOKEN_LLBRACKETS))
	{
		tmp = new DictNode(lexer.Get()->GetLine());
		if (!MatchToken(TokenType::TOKEN_RLBRACKETS))
		{
			MappingNode* mn = mapping();
			tmp->SetMapping(mn);
		}
		MustMatch(TokenType::TOKEN_RLBRACKETS);
	}
	else
	{
		ostringstream oss;
		oss << "[Error] Expect variable name but "
			<< lexer.Get()->GetType() << " at line " << line
			<< endl;
		throw ParseException(oss.str());
	}

	return tmp;
}

ASTNode* Parser::elements()
{
	int line = lexer.Peek()->GetLine();
	
	ASTNode* tmp = new ElementsNode(line);

	while(MatchToken(TokenType::TOKEN_EOL))
		lexer.Next();

	ASTNode* ep = expr();
	tmp->AddElement(ep);
	if (MatchTokenMultiLine(TokenType::TOKEN_COMMA))
		lexer.Next();

	while(!MatchTokenMultiLine(TokenType::TOKEN_RMBRACKETS))
	{
		ep = expr();
		tmp->AddElement(ep);
		if (MatchTokenMultiLine(TokenType::TOKEN_COMMA))
			lexer.Next();
	}

	return tmp;
}

ASTNode* Parser::mapping()
{
	int line = lexer.Peek()->GetLine();
	
	ASTNode* tmp = new MappingNode(line);

	while(MatchToken(TokenType::TOKEN_EOL))
		lexer.Next();

	ASTNode* key = expr();
	MustMatch(TokenType::TOKEN_COLON);
	ASTNode* val = expr();
	tmp->AddMapping(key, val);
	if (MatchTokenMultiLine(TokenType::TOKEN_COMMA))
		lexer.Next();

	while(!MatchTokenMultiLine(TokenType::TOKEN_RLBRACKETS))
	{
		key = expr();
		MustMatch(TokenType::TOKEN_COLON);
		val = expr();
		tmp->AddMapping(key, val);
		if (MatchTokenMultiLine(TokenType::TOKEN_COMMA))
			lexer.Next();
	}

	return tmp;
}