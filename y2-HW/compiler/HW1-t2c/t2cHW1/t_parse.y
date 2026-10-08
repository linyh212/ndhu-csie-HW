%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "t2c.h"
    #include "t_parse.h"
%}

%token lWRITE lREAD lIF lASSIGN
%token lRETURN lBEGIN lEND
%token lEQU lNEQ lGT lLT lGE lLE
%token lADD lMINUS
%token lTIMES lDIVIDE
%token lLP lRP
%token lINT lREAL lSTRING
%token lELSE
%token lMAIN
%token lSEMI lCOMMA
%token lID lINUM lRNUM lQSTR
%token lAND lOR lNOT

%left  lOR
%left  lAND
%left  lEQU lNEQ lGT lLT lGE lLE
%left  lADD lMINUS
%left  lTIMES lDIVIDE
%right UMINUS
%right lNOT

%expect 1

%%

prog	:	mthdcls
        { printf("Program -> MethodDecls\nParsed OK!\n"); }
    | 
        { printf("****** Parsing failed!\n"); }	
    ;

mthdcls	:	mthdcl mthdcls
        { printf("MethodDecls -> MethodDecl MethodDecls\n"); }	
    |	mthdcl
        { printf("MethodDecls -> MethodDecl\n"); }	
    ;

type	:	lINT
        { printf("Type -> INT\n"); }	
    |	lREAL
        { printf("Type -> REAL\n"); }	
    ;

mthdcl	:	type lMAIN lID lLP formals lRP block
        { printf("MethodDecl -> Type MAIN ID LP Formals RP Block\n"); }	
    |	type lID lLP formals lRP block
        { printf("MethodDecl -> Type ID LP Formals RP Block\n"); }	
    ;

formals	:	formal oformal
        { printf("Formals -> Formal OtherFormals\n"); }	
    | 
        { printf("Formals -> (empty)\n"); }	
    ;

formal	:	type lID
        { printf("Formal -> Type ID\n"); }	
    ;

oformal	:	lCOMMA formal oformal
        { printf("OtherFormals -> COMMA Formal OtherFormals\n"); }	
    | 
        { printf("OtherFormals -> (empty)\n"); }	
    ;

block : lBEGIN statements lEND
        { printf("Block -> BEGIN Statements END\n"); }
    ;

statements : statement statements
        { printf("Statements -> Statement Statements\n"); }
    | 
        { printf("Statements -> (empty)\n"); }
    ;

statement : block
        { printf("Processing Statement: Block -> BEGIN Statements END\n"); }
    | localVarDecl
        { printf("Processing Statement: LocalVarDecl -> Type ID SEMI or Type AssignStmt\n"); }
    | assignStmt
        { printf("Processing Statement: AssignStmt -> ID ASSIGN Expression SEMI\n"); }
    | returnStmt
        { printf("Processing Statement: ReturnStmt -> RETURN Expression SEMI\n"); }
    | ifStmt
        { printf("Processing Statement: IfStmt -> IF LP BoolExpr RP Statement [ELSE Statement]\n"); }
    | writeStmt
        { printf("Processing Statement: WriteStmt -> WRITE LP Expression COMMA QString RP SEMI\n"); }
    | readStmt
        { printf("Processing Statement: ReadStmt -> READ LP ID COMMA QString RP SEMI\n"); }
    | error lSEMI
        { yyerrok; printf("Recovered from error in statement.\n"); }
    ;

localVarDecl : type lID lSEMI
        { printf("Processing LocalVarDecl: LocalVarDecl -> Type ID SEMI\n"); }
    | type assignStmt
        { printf("Processing LocalVarDecl: LocalVarDecl -> Type AssignStmt\n"); }
    ;

assignStmt : lID lASSIGN expression lSEMI
        { printf("Processing AssignStmt: AssignStmt -> ID ASSIGN Expression SEMI\n"); }
    ;

returnStmt : lRETURN expression lSEMI
        { printf("Processing ReturnStmt: ReturnStmt -> RETURN Expression SEMI\n"); }
    ;

ifStmt : lIF lLP boolExpr lRP statement
        { printf("Processing IfStmt: IfStmt -> IF LP BoolExpr RP Statement\n"); }
    | lIF lLP boolExpr lRP statement lELSE statement
        { printf("Processing IfStmt: IfStmt -> IF LP BoolExpr RP Statement ELSE Statement\n"); }
    ;

writeStmt : lWRITE lLP expression lCOMMA lQSTR lRP lSEMI
        { printf("Processing WriteStmt: WriteStmt -> WRITE LP Expression COMMA QString RP SEMI\n"); }
    ;

readStmt : lREAD lLP lID lCOMMA lQSTR lRP lSEMI
        { printf("Processing ReadStmt: ReadStmt -> READ LP ID COMMA QString RP SEMI\n"); }
    ;

expression : multiplicativeExpr
        { printf("Processing Expression: Expression -> MultiplicativeExpr\n"); }
    | expression lADD multiplicativeExpr
        { printf("Processing Expression: Expression -> Expression ADD MultiplicativeExpr\n"); }
    | expression lMINUS multiplicativeExpr
        { printf("Processing Expression: Expression -> Expression MINUS MultiplicativeExpr\n"); }
    ;

multiplicativeExpr : unaryExpr
        { printf("Processing MultiplicativeExpr: MultiplicativeExpr -> UnaryExpr\n"); }
    | multiplicativeExpr lTIMES unaryExpr
        { printf("Processing MultiplicativeExpr: MultiplicativeExpr -> MultiplicativeExpr TIMES UnaryExpr\n"); }
    | multiplicativeExpr lDIVIDE unaryExpr
        { printf("Processing MultiplicativeExpr: MultiplicativeExpr -> MultiplicativeExpr DIVIDE UnaryExpr\n"); }
    ;

unaryExpr : primaryExpr
        { printf("Processing UnaryExpr: UnaryExpr -> PrimaryExpr\n"); }
    | lMINUS unaryExpr %prec UMINUS
        { printf("Processing UnaryExpr: UnaryExpr -> MINUS UnaryExpr\n"); }
    ;

primaryExpr : lINUM
        { printf("Processing PrimaryExpr: PrimaryExpr -> INUM\n"); }
    | lRNUM
        { printf("Processing PrimaryExpr: PrimaryExpr -> RNUM\n"); }
    | lID
        { printf("Processing PrimaryExpr: PrimaryExpr -> ID\n"); }
    | lLP expression lRP
        { printf("Processing PrimaryExpr: PrimaryExpr -> LP Expression RP\n"); }
    | lID lLP actualParams lRP
        { printf("Processing PrimaryExpr: PrimaryExpr -> ID LP ActualParams RP\n"); }
    | lQSTR
        { printf("Processing PrimaryExpr: PrimaryExpr -> QSTR\n"); }
    ;

boolExpr : expression lEQU expression
        { printf("Processing BoolExpr: BoolExpr -> Expression EQU Expression\n"); }
    | expression lNEQ expression
        { printf("Processing BoolExpr: BoolExpr -> Expression NEQ Expression\n"); }
    | expression lGT expression
        { printf("Processing BoolExpr: BoolExpr -> Expression GT Expression\n"); }
    | expression lGE expression
        { printf("Processing BoolExpr: BoolExpr -> Expression GE Expression\n"); }
    | expression lLT expression
        { printf("Processing BoolExpr: BoolExpr -> Expression LT Expression\n"); }
    | expression lLE expression
        { printf("Processing BoolExpr: BoolExpr -> Expression LE Expression\n"); }
    | boolExpr lAND boolExpr
        { printf("Processing BoolExpr: BoolExpr -> BoolExpr AND BoolExpr\n"); }
    | boolExpr lOR boolExpr
        { printf("Processing BoolExpr: BoolExpr -> BoolExpr OR BoolExpr\n"); }
    | lNOT boolExpr
        { printf("Processing BoolExpr: BoolExpr -> NOT BoolExpr\n"); }
    ;

actualParams : expression
        { printf("Processing ActualParams: ActualParams -> Expression\n"); }
    | actualParams lCOMMA expression
        { printf("Processing ActualParams: ActualParams -> ActualParams COMMA Expression\n"); }
    | 
        { printf("Processing ActualParams: ActualParams -> (empty)\n"); }
    ;
%%

int yyerror(char *s)
{
    printf("Syntax Error: %s\n", s);
    return 1;
}
