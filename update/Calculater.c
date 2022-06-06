#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<math.h>
#include<assert.h>

#include"calculater.h"
#include"matrix.h"
#include"matrix.c"
#include"matop.c"

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
		case '^':
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
	str[0]=text[*(pos)]; // first char should be char
	(*pos)++;
	for(i=1;isalpha(text[*pos])||isdigit(text[*pos])||text[*pos]=='_';i++){
		*(str+i) = text[*pos];
		(*pos)++;
	}
	str =(char*)realloc(str, i+1);
	*(str+i)='\0';
	return str;
}

char* get_mat(const char *text, unsigned int* pos){
	char* str=(char*)malloc(50*sizeof(char));
	unsigned int i;
	//str[0]=text[*(pos)]; // first char is char
	(*pos)++;
	for(i=0;text[*pos]!=')';i++){
		*(str+i) = text[*pos];
		(*pos)++;
	}
	(*pos)++;
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
			else if(!strcmp(token->value, "mat\0")){
				token->type = MAT;
				token->value = get_mat(text,pos);
			}
			else if(!strcmp(token->value, "pow\0"))
				token->type = POW;
			for(int i=0;i<NO_OF_FUNC;i++)
				if(!strcmp(token->value, my_func[i])){
					token->type = FUNC;
					return token;
				}
			
			/*
			else if(!strcmp(token->value, "det\0"))
				token->type = FUNC;
			else if(!strcmp(token->value, "echelon\0"))
				token->type = FUNC;
			*/
			else
				(*token).type = VAR;
			return token;
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
				case '^':
					token->type = POW;
					break;
				default:
					printf("Unknown op.");
					token->type = UNKNOWN;
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
		while(current_token->type == MULT || current_token->type == DIVIDE||current_token->type == POW){
			token = current_token;
			if(current_token->type == MULT){
				eat(MULT);
			}
			else if(current_token->type == DIVIDE){
				eat(DIVIDE);
			}
			else if(current_token->type == POW){
				eat(POW);
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
		if(current_token->type == UNKNOWN) return NULL;
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
		else if(token->type == MAT){
			node->tag = TOKEN;
			node->op.token = token;
			eat(MAT);
			return node;
		}
		else if(token->type == FUNC){
			node->tag = FUNCOP;
			node->op.Func.func_name = token->value;
			eat(FUNC);
			eat(OPEN_PAREN);
			node->op.Func.args = expr();
			eat(CLOSE_PAREN);
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
		}/*
		else if(token->type == MAT){
			node->tag = MATOP;
			node->op.MatOp.matstr = 
		}*/
		
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
	
	if(a->tag==TOKEN || a->tag == MATOP){ 
		free(a->op.token->value);
		free(a->op.token);
	}
	else if(a->tag == BINOP){
		free_ast(a->op.BinOp.left);
		free_ast(a->op.BinOp.right);
		free(a->op.token);
	}else if(a->tag == UNARYOP){
		free_ast(a->op.UnaryOp.right);
		free(a->op.UnaryOp.sign->value);
		free(a->op.UnaryOp.sign);
	}else if(a->tag == FUNCOP){
		free_ast(a->op.Func.args);
		free(a->op.Func.func_name);
	}else if(a->tag == ASS){
		free_ast(a->op.AssOp.right);
		//free(a->op.AssOp.var_name); 
	}
	else{
		printf("Deletion not implemented.");
	}
}

int c=0,d=0;

stack_item* create_stack_item(TokenType type){
	stack_item* s = (stack_item*)malloc(sizeof(stack_item));
	s->tag = type;
	c++;
	return s;
}

void stack_item_free(stack_item* itm){
	
	if(itm == NULL) return;
	d++;
	if(itm->tag==MAT)
		matrix_free(itm->m);
	free(itm);
}

stack_item* ipadd(stack_item* a, stack_item* b){
	stack_item* s=NULL;
	if(a->tag==INT){
		
		if(b->tag==INT){
			s= create_stack_item(INT);
			s->num = a->num + b->num;			
		}else if(b->tag==MAT){
			s= create_stack_item(MAT);
			Matrix* temp = matrix_unit(b->m->rows, b->m->cols, a->num);
			s->m = add(b->m, temp);
			matrix_free(temp);	
		}
		
	}else if(a->tag==MAT){
		s= create_stack_item(MAT);
		if(b->tag==INT){
			Matrix* tmm = matrix_unit(a->m->rows, a->m->cols, b->num);
			 
			s->m = add(a->m, tmm);
			matrix_free(tmm);
		}else if(b->tag==MAT){
			s->m = add(a->m, b->m);
		}
		else{
			printf("It should never get hear.");
			assert(0);
		}
	}else{
		printf("It should never get hear.");
		assert(0);
	}
	stack_item_free(a);
	stack_item_free(b);
	return s;
	
}
stack_item* ipsub(stack_item* a, stack_item* b){
	stack_item* s=NULL;
	if(a->tag==INT){
		
		if(b->tag==INT){
			s= create_stack_item(INT);
			s->num = a->num - b->num;			
		}else if(b->tag==MAT){
			s= create_stack_item(MAT);
			Matrix* temp = matrix_unit(b->m->rows, b->m->cols, a->num);
			s->m = subtract(b->m, temp);
			matrix_free(temp);	
		}
		
	}else if(a->tag==MAT){
		s= create_stack_item(MAT);
		if(b->tag==INT){
			Matrix* tmm = matrix_unit(a->m->rows, a->m->cols, b->num);
			 
			s->m = subtract(a->m, tmm);
			matrix_free(tmm);
		}else if(b->tag==MAT){
			s->m = subtract(a->m, b->m);
		}
		else{
			printf("It should never get hear.");
			assert(0);
		}
	}else{
		printf("It should never get hear.");
		assert(0);
	}
	stack_item_free(a);
	stack_item_free(b);
	return s;
}

stack_item* ipmul(stack_item* a, stack_item* b){
	stack_item* s = NULL;
	if(a->tag==INT){
		if(b->tag==INT){
			s= create_stack_item(INT);
			s->num = a->num * b->num;
			return s;
			
		}else if(b->tag==MAT){
			s= create_stack_item(MAT);
			num_multiply(b->m, a->num);
			s->m = matrix_copy(b->m);
			return s;
		}
		else{
			printf("It should never get hear.");
			assert(0);
		}
		
	}else if(a->tag==MAT){
		s= create_stack_item(MAT);
		if(b->tag==INT){
			num_multiply(a->m, b->num);
			s->m = matrix_copy(a->m);
		}else if(b->tag==MAT){
			s->m = dot(a->m, b->m);
		}
		else{
			printf("It should never get hear.");
			assert(0);
		}
		
	}
	else{
		printf("It should never get hear.");
		assert(0);
	}
	stack_item_free(a);
	stack_item_free(b);
	return s;
}
stack_item* ipdiv(stack_item* a, stack_item* b){
	stack_item* s=NULL;
	if(a->tag==INT){
		if(b->tag==INT){
			s= create_stack_item(INT);
			if(b->num != 0)
				s->num = a->num / b->num;
			else{
				printf("Divide by zero.");
				s->num=0;
			}
			
		}else if(b->tag==MAT){
			s= create_stack_item(MAT);
			num_multiply(b->m, a->num);
			s->m = mat_inverse(b->m);
		}
		else{
			printf("It should never get hear.");
			assert(0);
		}
		
	}else if(a->tag==MAT){
		s= create_stack_item(MAT);
		if(b->tag==INT){
			num_multiply(a->m, 1/(b->num));			 
			s->m = matrix_copy(a->m);
		}else if(b->tag==MAT){
			Matrix* temp = mat_inverse(b->m);
			if(temp == NULL) return NULL;
			s->m = dot(a->m, temp);
			matrix_free(temp);
		}
		else{
			printf("It should never get hear.");
			assert(0);
		}
		
	}
	else{
		printf("It should never get hear.");
		assert(0);
	}
	stack_item_free(a);
	stack_item_free(b);
	return s;	
}
stack_item* ippow(stack_item* a, stack_item* b){
	stack_item* s=NULL;
	if(a->tag==INT){
		
		if(b->tag==INT){
			s= create_stack_item(INT);
			s->num = pow(a->num , b->num);			
		}
		else{
			printf("[ERROR] number ^ matrix not allowed.");
			assert(0);
		}
		
	}else if(a->tag==MAT){
		
		if(b->tag==INT){
			if(b->num == 0){
				s= create_stack_item(INT);
				return s;
			}
			s= create_stack_item(MAT);
			Matrix* tmm = matrix_copy(a->m);
			if(b->num < 0){	
				tmm = mat_inverse(a->m);
				if(tmm == NULL)return NULL;
				matrix_free(a->m);
				a->m = matrix_copy(tmm);
				b->num = b->num * -1;
			}
			s->m = tmm;
			for(int v=1;v<b->num;v++){
				s->m = dot(tmm, a->m);
				matrix_free(tmm);
				tmm = s->m;
				//matrix_print(a->m);
			}
			//matrix_free(tmm);
		}
		else{
			printf("[ERROR] matrix ^ matrix not allowed.");
			assert(0);
		}
	}else{
		printf("It should never get hear.");
		assert(0);
	}
	stack_item_free(a);
	stack_item_free(b);
	return s;
}

stack_item* mat_det(stack_item* s){
	if(s==NULL) return NULL;
	if(s->tag == INT) {
		printf("Determent takes only mat");
		return NULL;
	}
	else if(s->tag == MAT){
		stack_item* itm = create_stack_item(INT);
		itm->num = determinant(s->m->entries, s->m->rows);
		stack_item_free(s);
		return itm;
	}
	else{
		assert(0);
	}
}

stack_item* mat_echelon(stack_item* s){
	if(s==NULL) return NULL;
	if(s->tag == INT) {
		printf("Echelon takes only mat");
		return NULL;
	}
	else if(s->tag == MAT){
		stack_item* itm = create_stack_item(MAT);
		itm->m = echelon_form_of(s->m);
		stack_item_free(s);
		return itm;
	}
	else{
		assert(0);
	}
}

stack_item* not_implemented(stack_item* s){
	printf("Function not implemented.");
	assert(0);
}

func_type find_func(char* func_name){
	if(!strcmp(func_name, "det\0")){
		return mat_det;
	}
	else if(!strcmp(func_name, "echelon\0")) return mat_echelon;
	else{
		return not_implemented;
	}
}


double temp;
int i;
stack_item* num_intptr(ast* a){
	if(a == NULL) return NULL;
	if(a->tag==TOKEN){
		temp=0;
		//converting char* to int
		if(a->op.token->type == INT){
			for(i=0;i<strlen(a->op.token->value);i++)
				temp = temp*10 + a->op.token->value[i] - '0';
			stack_item* s= create_stack_item(INT);
			s->num = temp;
			return s;
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
			stack_item* s= create_stack_item(INT);
			s->num = temp;
			return s;
		}
		else if(a->op.token->type == MAT){
			stack_item* s= create_stack_item(MAT);
			s->m = matrix_load(a->op.token->value);
			return s;
		}
		else if(a->op.token->type == VAR){
			for(i=0;i<stack_size;i++){
				if(!strcmp(vars[i], a->op.token->value)){
					if(stack[i].tag==INT){
						stack_item* s= create_stack_item(INT);
						s->num = stack[i].num;
						return s;
					}else if(stack[i].tag==MAT){
						stack_item* s= create_stack_item(MAT);
						s->m=matrix_copy(stack[i].m);
						return s;
					}
					else
						return NULL; // to be chainged
				}
			}
			printf("No variable with name %s\n", a->op.token->value);
			return NULL; // for now
		}
		else{
			printf("Unknown token type");
			assert(0);
		}
	}else if(a->tag == BINOP){
		
		if(a->op.token->type==PLUS){
			return ipadd(num_intptr(a->op.BinOp.left) , num_intptr(a->op.BinOp.right));
		}
		else if(a->op.token->type==MINUS){
			return ipsub(num_intptr(a->op.BinOp.left) , num_intptr(a->op.BinOp.right));
		}
		else if(a->op.token->type==MULT){
			return ipmul(num_intptr(a->op.BinOp.left) , num_intptr(a->op.BinOp.right));
		}
		else if(a->op.token->type==DIVIDE){
			return ipdiv(num_intptr(a->op.BinOp.left) , num_intptr(a->op.BinOp.right));
		}
		else if(a->op.token->type==POW){
			return ippow(num_intptr(a->op.BinOp.left) , num_intptr(a->op.BinOp.right));
		}
		else{
			printf("Unknown binop token type");
			assert(0);
		}
	}
	else if(a->tag == UNARYOP){
		if(a->op.UnaryOp.sign->type == PLUS)
			return (num_intptr(a->op.UnaryOp.right));
		else if(a->op.UnaryOp.sign->type == MINUS){
			stack_item* itm = create_stack_item(INT);
			itm->num = -1.0;
			return ipmul(itm , num_intptr(a->op.UnaryOp.right));
		}
		else if(a->op.UnaryOp.sign->type == TAN){
			stack_item* itm = num_intptr(a->op.UnaryOp.right);
			if(itm->tag != INT){
				printf("tan takes only int values.");
				return NULL;
			}
			itm->num = tan(itm->num * PI / 180);
			return itm;
		}
		else if(a->op.UnaryOp.sign->type == SIN){
			stack_item* itm = num_intptr(a->op.UnaryOp.right);
			if(itm->tag != INT){
				printf("sin takes only int values.");
				return NULL;
			}
			itm->num = sin(itm->num * PI / 180);
			return itm;
		}
		else if(a->op.UnaryOp.sign->type == COS){
			stack_item* itm = num_intptr(a->op.UnaryOp.right);
			if(itm->tag != INT){
				printf("cos takes only int values.");
				return NULL;
			}
			itm->num = cos(itm->num * PI / 180);
			return itm;
		}
		else{
			printf("Unknown Unaryop token type");
			assert(0);
		}
	}else if(a->tag == ASS){
		vars[stack_size]=a->op.AssOp.var_name;
		stack[stack_size] = *(num_intptr(a->op.AssOp.right));
		stack_item* s=NULL;
		if(stack[stack_size].tag == INT){
			s= create_stack_item(INT);
			s->num=stack[stack_size].num;
		}else if(stack[stack_size].tag == MAT){
			s = create_stack_item(MAT);
			s->m = matrix_copy(stack[stack_size].m);
		}
		stack_size++;
		return s;
	}
	else if(a->tag == FUNCOP){
		func_type func = find_func(a->op.Func.func_name);
		return func(num_intptr(a->op.Func.args));
	}
	printf("End of intptr 'unknown token tag'");
	assert(0);
	
} 

//Matrix* mat_intptr(ast*)
void print(stack_item* s){
	if(s==NULL) return;
	printf("\n");
	if(s->tag == INT){
		printf("%.3lf", s->num);
	}else if(s->tag == MAT){
		int i,j;
		if(s->m != NULL)
			matrix_print(s->m);
	}
	printf("\n");
}
int main(){
	int run=1;
	char str[50];
	while(run){
		printf(">>>");
		gets(str);
		if(!strcmp(str,"q")) break;
		if(!strcmp(str,"cls")) {
			system("cls");
			continue;
		}
		ast* a = parse(str);
		stack_item* itm;
		//printf("Parsing completed.\n");
		if(a != NULL){
			//printf("Starting inptr.\n");
			itm = num_intptr(a);
			print(itm);
			stack_item_free(itm);
			//printf("No. of stack items created :%d\nNo. of stack items Destroyed : %d",c,d);
			c=0;
			d=0;
			free_ast(a);
		}
		
	}
	return 0;
}

