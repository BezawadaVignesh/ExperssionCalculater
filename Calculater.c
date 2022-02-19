#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

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
			return 1;
		default:
			return 0;
	}
}

char* get_num(const char *text, unsigned int* pos){
	char* num=(char*)malloc(20*sizeof(char));
	unsigned int i;
	for(i=0;isdigit(text[*pos]);i++){
		*(num+i) = text[*pos];
		(*pos)++;
	}
	*(num+i)='\0';
	return num;
}

char* get_var(const char *text, unsigned int* pos){
	char* str=(char*)malloc(20*sizeof(char));
	unsigned int i;
	for(i=0;isalpha(text[*pos]);i++){
		*(str+i) = text[*pos];
		*(pos)++;
	}
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
			(*token).value = get_num(text,pos);
			(*token).type = INT;
			return (token);
		}/*
		if(isalpha(text[*pos])){
			(*token).pos = *pos;
			(*token).value = get_var(text,pos);
			(*token).type = VAR;
			return (*token).value;//return (token);
		}*/
		if(is_op(text[*pos])){
			ch=(char*)malloc(2*sizeof(char));
			ch[0] = text[*pos];
			ch[1] = '\0';			//adding a null char at the end
			(*token).value=ch;
			(*token).pos=*pos;
			switch(text[*pos]){
				case '+':
					(*token).type= PLUS;
					break;
				case '-':
					(*token).type= MINUS;
					break;
				case '*':
					(*token).type= MULT;
					break;
				case '/':
					(*token).type= DIVIDE;
					break;
				case '%':
					(*token).type= MODULES;
					break;
				case '(':
					(*token).type= OPEN_PAREN;
					break;
				case ')':
					(*token).type= CLOSE_PAREN;
					break;
			}
			(*pos)++; //for present
			return (token);
		}
		
	}
	(*token).value = '\0';
	(*token).pos;
	(*token).type =NONE;
	return (token);
}


unsigned int i=0;
char str[50];
Token* current_token;


void eat(TokenType type){
	if(type==current_token->type){
	current_token = lexer(str, &i);
}
	else{
		printf("eat error expected %d got %d", type, current_token->type);
		exit(0);
	}
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
	else if(token->type == OPEN_PAREN){
		eat(OPEN_PAREN);
		node = expr();
		eat(CLOSE_PAREN);
		return node;
	}
	printf("end of factor current token value '%s'", token->value);
	exit(0);
	
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
	
	if(a->tag==TOKEN) free(a->op.token);
	else
	{
		free_ast(a->op.BinOp.left);
		free_ast(a->op.BinOp.right);
		free(a->op.token);
	}
}

int temp;

int intptr(ast* a){
	if(a->tag==TOKEN){
		temp=0;
		//converting char* to int
		for(i=0;i<strlen(a->op.token->value);i++)
			temp = temp*10 + a->op.token->value[i] - '0';
		
		return temp;
	}else{
		
		if(a->op.token->type==PLUS)
			return intptr(a->op.BinOp.left) + intptr(a->op.BinOp.right);
		else if(a->op.token->type==MINUS)
			return intptr(a->op.BinOp.left) - intptr(a->op.BinOp.right);
		else if(a->op.token->type==MULT)
			return intptr(a->op.BinOp.left) * intptr(a->op.BinOp.right);
		else if(a->op.token->type==DIVIDE)
			return intptr(a->op.BinOp.left) / intptr(a->op.BinOp.right);
	}
}


int main(){
	int run=1;
	while(run){
		gets(str);
		i=0;
		if(!strcmp(str,"q")) break;
		current_token = lexer(str, &i);
		ast* a = expr();
		
		printf("%d\n", intptr(a));
		free_ast(a);
	}
	return 0;
}

