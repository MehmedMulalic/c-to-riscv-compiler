%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *input_filename = NULL;
char *output_filename = "a.c";

int yylex();
void yyerror();
%}

%code requires {
	#include "intermediate.h"
}

%union {
	char *strval;
	ASTNode *node;
}

%type <node> statement_list statement labeled_statement compound_statement
%type <node> expression_statement selection_statement iteration_statement jump_statement
%type <node> expression assignment_expression conditional_expression logical_or_expression
%type <node> logical_and_expression inclusive_or_expression exclusive_or_expression
%type <node> and_expression equality_expression relational_expression shift_expression
%type <node> additive_expression multiplicative_expression cast_expression unary_expression
%type <node> postfix_expression primary_expression

%token <strval> IDENTIFIER CONSTANT STRING_LITERAL SIZEOF
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token TYPEDEF EXTERN STATIC AUTO REGISTER
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%start translation_unit
%%

primary_expression
	: IDENTIFIER { $$ = make_identifier($1, lookup($1)); }
	| CONSTANT { $$ = make_constant($1); }
	| STRING_LITERAL { printf("DEBUG STLIT: %s\n", $1); }
	| '(' expression ')' { printf("DEBUG expression parser\n"); }
	;

postfix_expression
	: primary_expression
	| postfix_expression '[' expression ']'
	| postfix_expression '(' ')'
	| postfix_expression '(' argument_expression_list ')'
	| postfix_expression '.' IDENTIFIER
	| postfix_expression PTR_OP IDENTIFIER
	| postfix_expression INC_OP
	| postfix_expression DEC_OP
	;

argument_expression_list
	: assignment_expression
	| argument_expression_list ',' assignment_expression
	;

unary_expression
	: postfix_expression
	| INC_OP unary_expression { $$ = $2; }
	| DEC_OP unary_expression { $$ = $2; }
	| unary_operator cast_expression { $$ = $2; }
	| SIZEOF unary_expression { $$ = $2; }
	| SIZEOF '(' type_name ')'{ $$ = NULL; }
	;

unary_operator
	: '&'
	| '*'
	| '+'
	| '-'
	| '~'
	| '!'
	;

cast_expression
	: unary_expression
	| '(' type_name ')' cast_expression { $$ = $4; }
	;

multiplicative_expression
	: cast_expression
	| multiplicative_expression '*' cast_expression
	| multiplicative_expression '/' cast_expression
	| multiplicative_expression '%' cast_expression
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression
		{ $$ = make_binop("+", $1, $3); }
	| additive_expression '-' multiplicative_expression
	;

shift_expression
	: additive_expression
	| shift_expression LEFT_OP additive_expression
	| shift_expression RIGHT_OP additive_expression
	;

relational_expression
	: shift_expression
	| relational_expression '<' shift_expression
	| relational_expression '>' shift_expression
	| relational_expression LE_OP shift_expression
	| relational_expression GE_OP shift_expression
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression
	| equality_expression NE_OP relational_expression
	;

and_expression
	: equality_expression
	| and_expression '&' equality_expression
	;

exclusive_or_expression
	: and_expression
	| exclusive_or_expression '^' and_expression
	;

inclusive_or_expression
	: exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression
	;

logical_and_expression
	: inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression
	;

conditional_expression
	: logical_or_expression
	| logical_or_expression '?' expression ':' conditional_expression
	;

assignment_expression
	: conditional_expression
	| unary_expression assignment_operator assignment_expression
		{ $$ = make_binop("=", $1, $3); }
	;

assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;

expression
	: assignment_expression
	| expression ',' assignment_expression
		{ printf("DEBUGEXPRESSION LIST\n"); }
	;

constant_expression
	: conditional_expression
	;

declaration
	: declaration_specifiers ';'
	| declaration_specifiers init_declarator_list ';'
	;

declaration_specifiers
	: storage_class_specifier
	| storage_class_specifier declaration_specifiers
	| type_specifier
	| type_specifier declaration_specifiers
	| type_qualifier
	| type_qualifier declaration_specifiers
	;

init_declarator_list
	: init_declarator
	| init_declarator_list ',' init_declarator
	;

init_declarator
	: declarator /* int a; */
	| declarator '=' initializer /* int a = 5; */
	;

storage_class_specifier
	: TYPEDEF
	| EXTERN
	| STATIC
	| AUTO
	| REGISTER
	;

type_specifier
	: VOID { current_type = VOID; }
	| CHAR { current_type = CHAR; }
	| SHORT { current_type = SHORT; }
	| INT { current_type = INT; }
	| LONG { current_type = LONG; }
	| FLOAT { current_type = FLOAT; }
	| DOUBLE { current_type = DOUBLE; }
	| SIGNED { current_type = SIGNED; }
	| UNSIGNED { current_type = UNSIGNED; }
	| struct_or_union_specifier
	| enum_specifier
	| TYPE_NAME { current_type = TYPE_NAME; }
	;

struct_or_union_specifier
	: struct_or_union IDENTIFIER '{' struct_declaration_list '}'
	| struct_or_union '{' struct_declaration_list '}'
	| struct_or_union IDENTIFIER
	;

struct_or_union
	: STRUCT
	| UNION
	;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';'
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list
	| type_specifier
	| type_qualifier specifier_qualifier_list
	| type_qualifier
	;

struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	;

struct_declarator
	: declarator
	| ':' constant_expression
	| declarator ':' constant_expression
	;

enum_specifier
	: ENUM '{' enumerator_list '}'
	| ENUM IDENTIFIER '{' enumerator_list '}'
	| ENUM IDENTIFIER
	;

enumerator_list
	: enumerator
	| enumerator_list ',' enumerator
	;

enumerator
	: IDENTIFIER
	| IDENTIFIER '=' constant_expression
	;

type_qualifier
	: CONST
	| VOLATILE
	;

declarator
	: pointer direct_declarator { printf("DEBUG pointer declarator\n"); }
	| direct_declarator
	;

direct_declarator
	: IDENTIFIER { insert($1); }
	| '(' declarator ')' { printf("DEBUG (DECLARATOR)\n"); }
	| direct_declarator '[' constant_expression ']'
	| direct_declarator '[' ']'
	| direct_declarator '(' parameter_type_list ')'
	| direct_declarator '(' identifier_list ')'
	| direct_declarator '(' ')'
	;

pointer
	: '*'
	| '*' type_qualifier_list
	| '*' pointer
	| '*' type_qualifier_list pointer
	;

type_qualifier_list
	: type_qualifier
	| type_qualifier_list type_qualifier
	;


parameter_type_list
	: parameter_list
	| parameter_list ',' ELLIPSIS
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration
	: declaration_specifiers declarator
	| declaration_specifiers abstract_declarator
	| declaration_specifiers
	;

identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;

type_name
	: specifier_qualifier_list
	| specifier_qualifier_list abstract_declarator
	;

abstract_declarator
	: pointer
	| direct_abstract_declarator
	| pointer direct_abstract_declarator
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'
	| '[' ']'
	| '[' constant_expression ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' constant_expression ']'
	| '(' ')'
	| '(' parameter_type_list ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_type_list ')'
	;

initializer
	: assignment_expression
	| '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	;

initializer_list
	: initializer
	| initializer_list ',' initializer
	;

statement
	: labeled_statement { $$ = $1; }
	| compound_statement { $$ = $1; }
	| expression_statement { $$ = $1; }
	| selection_statement { $$ = $1; }
	| iteration_statement { $$ = $1; }
	| jump_statement { $$ = $1; }
	;

labeled_statement
	: IDENTIFIER ':' statement { $$ = $3; }
	| CASE constant_expression ':' statement { $$ = $4; }
	| DEFAULT ':' statement { $$ = $3; }
	;

compound_statement
	: '{' '}' { $$ = NULL; }
	| '{' statement_list '}' { root_node = $2; }
	| '{' declaration_list '}' { $$ = NULL; }
	| '{' declaration_list statement_list '}' { root_node = $3; }
	;

declaration_list
	: declaration
	| declaration_list declaration
	;

statement_list
	: statement
	| statement_list statement { $$ = make_statement_list($1, $2); }
	;

expression_statement
	: ';' { $$ = NULL; }
	| expression ';' { $$ = $1; }
	;

selection_statement
	: IF '(' expression ')' statement { $$ = NULL; }
	| IF '(' expression ')' statement ELSE statement { $$ = NULL; }
	| SWITCH '(' expression ')' statement { $$ = NULL; }
	;

iteration_statement
	: WHILE '(' expression ')' statement { $$ = NULL; }
	| DO statement WHILE '(' expression ')' ';' { $$ = NULL; }
	| FOR '(' expression_statement expression_statement ')' statement
		{ $$ = NULL; }
	| FOR '(' expression_statement expression_statement expression ')' statement
		{ $$ = NULL; }
	;

jump_statement
	: GOTO IDENTIFIER ';' { $$ = NULL; }
	| CONTINUE ';' { $$ = NULL; }
	| BREAK ';' { $$ = NULL; }
	| RETURN ';' { $$ = NULL; }
	| RETURN expression ';' { $$ = $2; }
	;

translation_unit
	: external_declaration
	| translation_unit external_declaration
	;

external_declaration
	: function_definition
	| declaration
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement
	| declaration_specifiers declarator compound_statement
	| declarator declaration_list compound_statement
	| declarator compound_statement
	;

%%
#include <stdio.h>

extern char yytext[];
extern int column;

void yyerror(s)
char *s;
{
	fflush(stdout);
	printf("\n%*s\n%*s\n", column, "^", column, s);
}

int main(int argc, char **argv)
{
 int i;

 for(i=1;i<argc;i++) {
    if (*argv[i]=='-') {
       switch(*(argv[i]+1)) {
       /* output option */
       case 'o':
          output_filename=argv[i]+2;
          break;

       /********************************/
       /* specify your own option here */
       /********************************/

       default:
          fprintf(stderr,"%s: unknown argument option\n",argv[0]);
          exit(1);
       }
    } else {
      if (input_filename != NULL) {
         fprintf(stderr,"%s: only one input file allowed\n",argv[0]);
         exit(1);
      }
      input_filename = argv[i];
    }
 }

 if (input_filename != NULL) {
    if ((freopen(input_filename, "r",stdin))==NULL) {
      fprintf(stderr,"%s: cannot open input file %s\n",argv[0],input_filename);

      exit(1);
    }
 }

 i=yyparse();

 if (root_node != NULL) print_dot("AST.dot", root_node);

 if (i==0)
   fprintf(stderr,"\nNo errors detected.\n");
 else
   fprintf(stderr,"\nErrors detected.\n");

 exit(i);
}

