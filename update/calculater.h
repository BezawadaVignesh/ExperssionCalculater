#ifndef CALCULATER_H
#define CALCULATER_H

#include"matrix.h"

#define PI 3.14159265358

#define NO_OF_FUNC 2

char my_func[NO_OF_FUNC][10] = {"det", "echelon"};

typedef enum {
	PLUS=0,
	MINUS,
	TAN,
	SIN,
	COS,
	MAT,
	INT,
	POW,
	FUNC,
	FLOAT,
	OPEN_BRACES,
	OPEN_PAREN,
	
	MULT,
	DIVIDE,
	MODULES,
		
	
	CLOSE_BRACES,
	
	CLOSE_PAREN,
	COMMA,
	EQUAL,
	
	VAR,
	UNKNOWN,
	NONE,
	
} TokenType;

typedef struct {
	char *value;
	TokenType type;
	unsigned int pos;
	//size_t len;
} Token;

typedef struct{
	TokenType tag;
	union{
		double num;
		Matrix* m;
	};
} stack_item;

typedef struct Exp{
	enum {TOKEN, BINOP, UNARYOP, ASS, MATOP, FUNCOP} tag;
	union{
		Token* token;
		struct {
			Token* op;
			struct Exp* left;
			struct Exp* right;
		} BinOp;
		struct {
			Token* sign;
			struct Exp* right;
		}UnaryOp;
		struct{
			char* var_name;
			struct Exp* right;
		} AssOp;
		
		struct{
			char* func_name;
			struct Exp* args;
		}Func;
	}op;
}ast;


typedef stack_item* (*func_type)(stack_item*);

ast* expr();
ast* term();
ast* factor();

double intptr(ast*);

#endif

/*
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<math.h>
#include<assert.h>

#include"calculater.h"

int is_op(const char ch){
	switch(ch){
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '(':
		case ')':
		case '=':
		case ',':
			return 1;
		default:
			return 0;
	}
}

char* get_num(const char *text, unsigned int* pos, int* not_float){
	char* num=(char*)malloc(16*sizeof(char));
	unsigned int i;
	*not_float=1;
	for(i=0;isdigit(text[*pos]) || (text[*pos]=='.' && (*not_float));i++){
		if(text[*pos]=='.')
			(*not_float) = 0;
		*(num+i) = text[*pos];
		(*pos)++;
	}
	num = (char*)realloc(num, i+1); 
	*(num+i)='\0';
	return num;
}

char* get_var(const char *text, unsigned int* pos){
	char* str=(char*)malloc(16*sizeof(char));
	unsigned int i;
	str[0]=text[*(pos)]; // first char is char
	(*pos)++;
	for(i=1;isalpha(text[*pos])||isdigit(text[*pos])||text[*pos]=='_';i++){
		*(str+i) = text[*pos];
		(*pos)++;
	}
	str =(char*)realloc(str, i+1);
	*(str+i)='\0';
	return str;
}
 

Token* lexer(const char *text,unsigned int* pos){
	size_t l=0;
	char *ch='\0';
	
	Token* token= (Token*)malloc(sizeof(Token));
	
	while(text[*pos] != '\0'){
		if(isspace(text[*pos])){
			(*pos)++;
			continue;
		}
		if(isdigit(text[*pos])){
			(*token).pos = *pos;
			int not_float = 1;
			(*token).value = get_num(text,pos, &not_float);
			if(not_float)
				(*token).type = INT;
			else{
				(*token).type = FLOAT;
			}
			return token;
		}
		if(isalpha(text[*pos])||text[*pos]=='_'){
			(*token).pos = *pos;
			(*token).value = get_var(text,pos);
			//printf("%s %d %d\n ", (*token).value, strcmp((*token).value, "tan"),strcmp((*token).value, "tan\0"));
			if(!strcmp((*token).value, "tan\0"))
				token->type = TAN;
			else if(!strcmp(token->value, "sin\0"))
				token->type = SIN;
			else if(!strcmp(token->value, "cos\0"))
				token->type = COS;
			else if(!strcmp(token->value, "mat\0"))
				token->type = MAT;
			else
				(*token).type = VAR;
			return (token);
		}
		if(is_op(text[*pos])){
			ch=(char*)malloc(2*sizeof(char));
			ch[0] = text[*pos];
			ch[1] = '\0';			//adding a null char at the end
			token->value=ch;
			token->pos=*pos;
			switch(text[*pos]){
				case '+':
					token->type = PLUS;
					break;
				case '-':
					token->type = MINUS;
					break;
				case '*':
					token->type = MULT;
					break;
				case '/':
					token->type = DIVIDE;
					break;
				case '%':
					token->type = MODULES;
					break;
				case '(':
					token->type = OPEN_PAREN;
					break;
				case ')':
					token->type = CLOSE_PAREN;
					break;
				case '=':
					token->type = EQUAL;
					break;
				case ',':
					token->type=COMMA;
					break;
				default:
					printf("Unknown op.");
					assert(0);
			}
			
			(*pos)++; //for present
			return (token);
		}
		printf("Unknown token.");
		assert(0);
	}
	(*token).value = '\0';
	(*token).pos;
	(*token).type =NONE;
	return (token);
}





#define MAX_STACK 50
stack_item stack[MAX_STACK];
char* vars[MAX_STACK];
int stack_size=0;

ast* parse(char * str){
	//below  functions are declared in parse function
	// so that only parser can access them
	auto ast* expr();
	auto ast* term();
	auto ast* factor();
	auto TokenType tpeek();
	unsigned int i=0;
	
	Token* current_token;
	current_token = lexer(str, &i);
	
	
	if(current_token->type >= PLUS && current_token->type <= OPEN_PAREN){	
		return expr();
	}else if(current_token->type == VAR){
		return expr();
	}
	else{
		return NULL;
	}
	
	TokenType tpeek(){
		int j = i;
		Token* ttoken = lexer(str, &j);
		TokenType ttype = ttoken->type;
		free(ttoken->value);
		free(ttoken);
		return ttype;
	}
	
	void eat(TokenType type){
		if(type==current_token->type){
		current_token = lexer(str, &i);
	}
		else{
			printf("eat error expected %d got %d", type, current_token->type);
			exit(0);
		}
	}
	Token* mat_token(int *row, int *col){
		Token* mat = (Token*)malloc(20*sizeof(Token));
		int k,j;
		for(j=0;current_token->type != CLOSE_PAREN;j++){
			for(k=0;current_token->type != COMMA && current_token->type != CLOSE_PAREN;k++){
				mat[k]=*current_token;
				if(current_token->type==INT)
					eat(INT);
				else if(current_token->type==FLOAT)
					eat(FLOAT);
				else if(current_token->type==VAR)
					eat(VAR);
				else{
					printf("Expected a int or float or var but got '%s'", current_token->value);
					assert(0);
				}
			}
			if(current_token->type == CLOSE_PAREN) break;
			eat(COMMA);
		}
		mat = (Token*)realloc(mat,k*j*sizeof(Token*));
		*row = j;
		*col = k;
		return mat;
	}
	ast* expr(){
		ast* node, *temp;
		node = term();
		Token* token = current_token;
		while(current_token->type == PLUS || current_token->type == MINUS){
			token = current_token;
			if(current_token->type == PLUS){
				eat(PLUS);
			}
			else if(current_token->type == MINUS){
				eat(MINUS);
			}
			temp = (ast*)malloc(sizeof(ast));
			temp->tag = BINOP;
			temp->op.BinOp.left=node;
			temp->op.BinOp.op=token;
			temp->op.BinOp.right=term();
			node=temp;
		}
		return node;
	}
	
	ast* term(){
		ast* node, *temp;
		node = factor();
		
		Token* token = current_token;
		if(token->type==NONE){
			return node;
		}
		while(current_token->type == MULT || current_token->type == DIVIDE){
			token = current_token;
			if(current_token->type == MULT){
				eat(MULT);
			}
			else if(current_token->type == DIVIDE){
				eat(DIVIDE);
			}
			temp = (ast*)malloc(sizeof(ast));
			temp->tag = BINOP;
			temp->op.BinOp.left=node;
			temp->op.BinOp.op=token;
			temp->op.BinOp.right=factor();
			node=temp;
		}
		return node;
	}
	
	ast* factor(){
		Token* token = current_token;
		ast* node=(ast*)malloc(sizeof(ast));
		if(token->type == INT){
			node->tag = TOKEN;
			node->op.token = token;
			eat(INT);
			return node;
		}
		else if(token->type == FLOAT){
			node->tag = TOKEN;
			node->op.token = token;
			eat(FLOAT);
			return node;
		}
		else if(token->type == VAR){
			if(tpeek()==EQUAL){
				node->tag = ASS;
				node->op.AssOp.var_name = token->value;
				eat(VAR);
				eat(EQUAL);
				node->op.AssOp.right = expr();
			}
			else{
				node->tag = TOKEN;
				node->op.token = token;
				eat(VAR);
			}
			
			return node;
		}
		else if(token->type == OPEN_PAREN){
			eat(OPEN_PAREN);
			node = expr();
			eat(CLOSE_PAREN);
			return node;
		}else if(token->type == PLUS){
			node->tag = UNARYOP;
			node->op.UnaryOp.sign = token;
			eat(PLUS);
			node->op.UnaryOp.right = factor();
			
			return node;
		}
		else if(token->type == MINUS){
			node->tag = UNARYOP;
			node->op.UnaryOp.sign = token;
			eat(MINUS);
			node->op.UnaryOp.right = factor();
			
			return node;
		}
		else if(token->type == TAN){
			node->tag = UNARYOP;
			node->op.UnaryOp.sign = token;
			eat(TAN);
			node->op.UnaryOp.right = expr();
			
			return node;
		}
		else if(token->type == SIN){
			node->tag = UNARYOP;
			node->op.UnaryOp.sign = token;
			eat(SIN);
			node->op.UnaryOp.right = expr();
			
			return node;
		}
		else if(token->type == COS){
			node->tag = UNARYOP;
			node->op.UnaryOp.sign = token;
			eat(COS);
			node->op.UnaryOp.right = expr();
			
			return node;
		}
		else if(token->type == MAT){
			node->tag = MATOP;
			eat(MAT);
			eat(OPEN_PAREN);
			node->op.MatOp.mat_t = mat_token(&(node->op.MatOp.row), &(node->op.MatOp.col));
			eat(CLOSE_PAREN);
			return node;
		}
		printf("end of factor current token value '%s'", token->value);
		assert(0);
		
	}
	
	
	
	
	
}

int j;
void print_ast(ast* a, int n){
	
	if(a->tag==TOKEN){
		printf("(%d", n);
		for(j=0;j<n;j++)printf(" ");
		printf("%s",a->op.token->value);
		printf(")");
	}
	else{
		
		print_ast(a->op.BinOp.left, n+1);
		printf("%d",a->op.token->type);
		
		print_ast(a->op.BinOp.right, n+1);
	}
}

void free_ast(ast* a){
	
	if(a->tag==TOKEN){ 
		free(a->op.token->value);
		free(a->op.token);
		}
	else if(a->tag == BINOP)
	{
		free_ast(a->op.BinOp.left);
		free_ast(a->op.BinOp.right);
		free(a->op.token);
	}
}

double temp;
int i;
stack_item* intptr(ast* a){
	if(a->tag==TOKEN){
		stack_item* smat = (stack_item*)malloc(sizeof(stack_item));
		smat->tag=NUM;
		
		temp=0;
		//converting char* to int
		if(a->op.token->type == INT){
			for(i=0;i<strlen(a->op.token->value);i++)
				temp = temp*10 + a->op.token->value[i] - '0';
			smat->op.num = temp;
			return smat;
		}
		else if(a->op.token->type == FLOAT){
			for(i=0;isdigit(a->op.token->value[i]);i++){
				temp = temp*10 + a->op.token->value[i] - '0';
				
			}
			int j=1;
			i+=1;
			for( ;isdigit(a->op.token->value[i]);i++,j*=10){
				temp = temp*10 + a->op.token->value[i] - '0';
			}
			temp /= j;
			smat->op.num = temp;
			return  smat;
		}
		else if(a->op.token->type == VAR){
			for(i=0;i<stack_size;i++){
				if(!strcmp(vars[i], a->op.token->value)){
					if(stack[i].tag==NUM){
						smat->op.num = stack[i].op.num;
						return smat;
					}else if(stack[i].tag == TMAT){
						smat->tag=TMAT;
						smat->op.m = stack[i].op.m;
						return smat;
					}
					else
						assert(0); // to be chainged
				}
			}
			printf("No variable with name %s\n", a->op.token->value);
			return 0; // for now
		}
			
		
		return smat;
	}/*else if(a->tag == BINOP){
		stack_item* smat = (stack_item*)malloc(sizeof(stack_item));
		if(a->op.token->type==PLUS)
			return add(intptr(a->op.BinOp.left) , intptr(a->op.BinOp.right));
		else if(a->op.token->type==MINUS)
			return sub(intptr(a->op.BinOp.left) , intptr(a->op.BinOp.right));
		else if(a->op.token->type==MULT)
			return mul(intptr(a->op.BinOp.left) , intptr(a->op.BinOp.right));
		else if(a->op.token->type==DIVIDE)
			return div(intptr(a->op.BinOp.left) , intptr(a->op.BinOp.right));
		else{
			assert(0);
			printf("Unknown binop token type");
		}
	}/*
	else if(a->tag == UNARYOP){
		if(a->op.UnaryOp.sign->type == PLUS)
			return (intptr(a->op.UnaryOp.right));
		else if(a->op.UnaryOp.sign->type == MINUS)
			return -(intptr(a->op.UnaryOp.right));
		else if(a->op.UnaryOp.sign->type == TAN)
			return tan(intptr(a->op.UnaryOp.right));
		else if(a->op.UnaryOp.sign->type == SIN)
			return cos(intptr(a->op.UnaryOp.right));
		else if(a->op.UnaryOp.sign->type == COS)
			return cos(intptr(a->op.UnaryOp.right));
		else{
			assert(0);
			printf("Unknown Unaryop token type");
		}
	}else if(a->tag == ASS){
		vars[stack_size]=a->op.AssOp.var_name;
		stack[stack_size] = *intptr(a->op.AssOp.right);
		return &stack[stack_size++];
	}
	else if(a->tag == MATOP){
		int siz=(a->op.MatOp.row) * (a->op.MatOp.col, j);
		stack_item* smat = (stack_item*)malloc(sizeof(stack_item));
		smat->tag=TMAT;
		smat->op.m = (matrix*)malloc(sizeof(matrix));
		smat->op.m->row = a->op.MatOp.row;
		smat->op.m->col = a->op.MatOp.col;
		smat->op.m->mat = (double*)malloc(siz*sizeof(double));
		for(j=0;j<siz;j++){
			smat->op.m->mat[i]=intptr(a->op.MatOp.mat_t[i]);
		}
		return smat;
	}
	printf("End of intptr 'unknown token tag'");
	assert(0);
	
}


int main(){
	int run=1;
	char str[50];
	while(run){
		gets(str);
		if(!strcmp(str,"q")) break;
		
		ast* a = parse(str);
		printf("Parsing completed.\n");
		if(a != NULL){
			printf("Starting inptr.\n");
			stack_item* itm = intptr(a);
			if(itm->tag == NUM){
				printf("%lf\n", itm->op.num);
			}else{
				
			}
			free_ast(a);
		}
		
	}
	return 0;
}
*/
