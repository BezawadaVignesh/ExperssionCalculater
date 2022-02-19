#ifndef CALCULATER_H
#define CALCULATER_H
typedef enum {
	PLUS,
	MINUS,
	MULT,
	DIVIDE,
	MODULES,
		
	OPEN_BRACES,
	CLOSE_BRACES,
	OPEN_PAREN,
	CLOSE_PAREN,
	
	INT,
	FLOAT,
	
	VAR,
	NONE,
	
} TokenType;

typedef struct {
	char *value;
	TokenType type;
	unsigned int pos;
	//size_t len;
} Token;

typedef struct Exp{
	enum {TOKEN, BINOP}tag;
	union{
		Token* token;
		struct {
			Token* op;
			struct Exp* left;
			struct Exp* right;
		} BinOp;
	}op;
}ast;

ast* expr();
ast* term();
ast* factor();

int intptr(ast*);

#endif
