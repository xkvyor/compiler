#include "lexer.h"
#include "parser.h"

int main()
{
	ifstream ifs("code");
	Lexer *lexer = new Lexer(ifs);
	ifs.close();

	if (lexer->IsFail())
	{
		cerr << "Lexer parse failed!" << endl;
		delete lexer;
		return -1;
	}

	for (TokenIterator it = lexer->Begin();
		!lexer->End(); it = lexer->Next())
	{
		cout << "Token: " << (*it)->GetType() << " \""
			<< (*it)->GetContent() << "\"" << endl;
	}

	cin.get();

	Parser* parser = new Parser(*lexer);

	delete lexer;
	delete parser;

	return 0;
}