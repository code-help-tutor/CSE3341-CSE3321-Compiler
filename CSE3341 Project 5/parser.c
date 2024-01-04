https://tutorcs.com
WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "scanner.h"
#include "core.h"
#include "tree.h"
#include "parser.h"

/*
*
* Helper functions
*
*/

// Converts token value to a string
static void tokenString(char* str, int current) {
	switch (current) {
	  case AND : strcpy(str, "AND"); break;
	  case BEGIN : strcpy(str, "BEGIN"); break;
	  case DO : strcpy(str, "DO"); break;
	  case ELSE : strcpy(str, "ELSE"); break;
	  case END : strcpy(str, "END"); break;
	  case IF : strcpy(str, "IF"); break;
	  case IN : strcpy(str, "IN"); break;
	  case INTEGER : strcpy(str, "INTEGER"); break;
	  case IS : strcpy(str, "IS"); break;
	  case NEW : strcpy(str, "NEW"); break;
	  case NOT : strcpy(str, "NOT"); break;
	  case OR : strcpy(str, "OR"); break;
	  case OUT : strcpy(str, "OUT"); break;
	  case PROCEDURE : strcpy(str, "PROCEDURE"); break;
	  case RECORD : strcpy(str, "RECORD"); break;
	  case THEN : strcpy(str, "THEN"); break;
	  case WHILE : strcpy(str, "WHILE"); break;
	  case ADD : strcpy(str, "ADD"); break;
	  case SUBTRACT : strcpy(str, "SUBTRACT"); break;
	  case MULTIPLY : strcpy(str, "MULTIPLY"); break;
	  case DIVIDE : strcpy(str, "DIVIDE"); break;
	  case ASSIGN : strcpy(str, "ASSIGN"); break;
	  case EQUAL : strcpy(str, "EQUAL"); break;
	  case LESS : strcpy(str, "LESS"); break;
	  case COLON : strcpy(str, "COLON"); break;
	  case SEMICOLON : strcpy(str, "SEMICOLON"); break;
	  case PERIOD : strcpy(str, "PERIOD"); break;
	  case COMMA : strcpy(str, "COMMA"); break;
	  case LPAREN : strcpy(str, "LPAREN"); break;
	  case RPAREN : strcpy(str, "RPAREN"); break;
	  case LBRACE : strcpy(str, "LBRACE"); break;
	  case RBRACE : strcpy(str, "RBRACE"); break;
	  case CONST : strcpy(str, "CONST"); break;
	  case ID : strcpy(str, "ID"); break;
	  case EOS : strcpy(str, "EOS"); break;
	}
}

// If the current token is something other than expected,
// prints a meaningful error message and halts the program
static void expectedToken(int expected) {
	int actual = currentToken();
	
    if (actual != expected) {
		char actualStr[20];
		char expectedStr[20];
		
		tokenString(actualStr, actual);
		tokenString(expectedStr, expected);
		
        printf("Error: expected %s but recieved %s", expectedStr, actualStr);
        exit(0);
    }
}


/*
*
* Parse functions
*
*/


void parseProcedure(struct nodeProcedure* p) {
    expectedToken(PROCEDURE);
    nextToken();
    p->type = 0;
    expectedToken(ID);
    p->name = calloc(20, sizeof(char));
    getId(p->name);
    nextToken();
    if(currentToken() == AND){
        nextToken();
        p->type = 1;
        p->fs = (struct nodeFuncSeq*)calloc(1, sizeof(struct nodeFuncSeq));
        parseFuncSeq(p->fs);
    }
    expectedToken(IS);
    nextToken();
    p->ds = (struct nodeDeclSeq*) calloc(1, sizeof(struct nodeDeclSeq));
    parseDeclSeq(p->ds);
    expectedToken(BEGIN);
    nextToken();
    p->ss = (struct nodeStmtSeq*) calloc(1, sizeof(struct nodeStmtSeq));
    parseStmtSeq(p->ss);
    expectedToken(END);
    nextToken();
    expectedToken(EOS);
}

void parseFuncSeq(struct nodeFuncSeq* fs){
    fs->type = 0;
    fs->f = (struct nodeFunc*)calloc(1, sizeof(struct nodeFunc));
    parseFunc(fs->f);
    if(currentToken() != IS){
        fs->type = 1;
        fs->fs = (struct nodeFuncSeq*)calloc(1, sizeof(struct nodeFuncSeq));
        parseFuncSeq(fs->fs);
    }
}

void parseFunc(struct nodeFunc* f){
    expectedToken(PROCEDURE);
    nextToken();
    expectedToken(ID);
    f->name = calloc(20, sizeof(char));
    getId(f->name);
    nextToken();
    expectedToken(LPAREN);
    nextToken();
    f->p = (struct nodeParam*)calloc(1, sizeof(struct nodeParam));
    parseParam(f->p);
    expectedToken(RPAREN);
    nextToken();
    expectedToken(IS);
    nextToken();
    f->ds = (struct nodeDeclSeq*)calloc(1, sizeof(struct nodeDeclSeq));
    parseDeclSeq(f->ds);
    expectedToken(BEGIN);
    nextToken();
    if(currentToken()!= END){
        f->ss = (struct nodeStmtSeq*)calloc(1, sizeof(struct nodeStmtSeq));
        parseStmtSeq(f->ss);
    }else{
        printf("ERROR: function body missing");
        exit(0);
    }
    expectedToken(END);
    nextToken();
}

void parseParam(struct nodeParam* p){
    p->type = 0;
    expectedToken(ID);
    p->p1 = calloc(20, sizeof(char));
    getId(p->p1);
    nextToken();
    if(currentToken() == COMMA){
        nextToken();
        p->type = 1;
        expectedToken(ID);
        p->p2 = calloc(20, sizeof(char));
        getId(p->p2);
        nextToken();
    }
}

void parseDeclSeq(struct nodeDeclSeq* ds) {
    ds->type = 0;
    ds->d = (struct nodeDecl*) calloc(1, sizeof(struct nodeDecl));
    parseDecl(ds->d);
    if (currentToken() != BEGIN) {
		ds->type = 1;
        ds->ds = (struct nodeDeclSeq*) calloc(1, sizeof(struct nodeDeclSeq));
        parseDeclSeq(ds->ds);
    }
}

void parseDecl(struct nodeDecl* d) {
    if (currentToken() == INTEGER) {
		d->type = 0;
        d->di = (struct nodeDeclInt*) calloc(1, sizeof(struct nodeDeclInt));
        parseDeclInt(d->di);
    } else if (currentToken() == RECORD) {
		d->type = 1;
        d->dr = (struct nodeDeclRec*) calloc(1, sizeof(struct nodeDeclRec));
        parseDeclRec(d->dr);
    }
}

void parseDeclInt(struct nodeDeclInt* di) {
    expectedToken(INTEGER);
    nextToken();
    di->name = calloc(20, sizeof(char));
    getId(di->name);
    nextToken();
    expectedToken(SEMICOLON);
    nextToken();
}

void parseDeclRec(struct nodeDeclRec* dr) {
    expectedToken(RECORD);
    nextToken();
    dr->name = calloc(20, sizeof(char));
    getId(dr->name);
    nextToken();
    expectedToken(SEMICOLON);
    nextToken();
}

void parseStmtSeq(struct nodeStmtSeq* ss) {
    char s[20];
    tokenString(s, currentToken());
	ss->more = 0;
    ss->s = (struct nodeStmt*) calloc(1, sizeof(struct nodeStmt));
    parseStmt(ss->s);
    if (currentToken() != END && currentToken() != ELSE) {
		ss->more = 1;
        ss->ss = (struct nodeStmtSeq*) calloc(1, sizeof(struct nodeStmtSeq));
        parseStmtSeq(ss->ss);
    }
}

void parseStmt(struct nodeStmt* s) {
    if (currentToken() == ID) {
		s->type = 0;
        s->assign = (struct nodeAssign*) calloc(1, sizeof(struct nodeAssign));
        parseAssign(s->assign);
    } else if (currentToken() == IF) {
		s->type = 1;
        s->ifStmt = (struct nodeIf*) calloc(1, sizeof(struct nodeIf));
        parseIf(s->ifStmt);
    } else if (currentToken() == WHILE) {
		s->type = 2;
        s->loop = (struct nodeLoop*) calloc(1, sizeof(struct nodeLoop));
        parseLoop(s->loop);
    } else if (currentToken() == OUT) {
		s->type = 3;
        s->out = (struct nodeOut*) calloc(1, sizeof(struct nodeOut));
        parseOut(s->out);
    } else if(currentToken() == BEGIN){
        s->type = 4;
        s->call = (struct nodeCall*) calloc(1, sizeof(struct nodeCall));
        parseCall(s->call);
    }
}

void parseCall(struct nodeCall* c){
    expectedToken(BEGIN);
    nextToken();
    c->name = calloc(20, sizeof(char));
    expectedToken(ID);
    getId(c->name);
    nextToken();
    expectedToken(LPAREN);
    nextToken();
    c->p = (struct nodeParam*) calloc(1, sizeof(struct nodeParam));
    parseParam(c->p);
    expectedToken(RPAREN);
    nextToken();
    expectedToken(SEMICOLON);
    nextToken();
}

void parseAssign(struct nodeAssign* a) {
	a->type = 0;
    expectedToken(ID);
    a->lhs = calloc(20, sizeof(char));
    getId(a->lhs);
    nextToken();
    if (currentToken() == LBRACE) {
		a->type = 1;
        a->index = (struct nodeIndex*) calloc(1, sizeof(struct nodeIndex));
        parseIndex(a->index);
    }
    expectedToken(ASSIGN);
    nextToken();
    if (currentToken() == NEW) {
        a->type = 2;
        nextToken();
        expectedToken(RECORD);
        nextToken();
        expectedToken(LBRACE);
        nextToken();
        a->expr = (struct nodeExpr*) calloc(1, sizeof(struct nodeExpr));
        parseExpr(a->expr);
        expectedToken(RBRACE);
        nextToken();
    } else if (currentToken() == RECORD) {
        a->type = 3;
        nextToken();
        expectedToken(ID);
        a->rhs = calloc(20, sizeof(char));
        getId(a->rhs);
        nextToken();
    } else  {
        a->expr = (struct nodeExpr*) calloc(1, sizeof(struct nodeExpr));
        parseExpr(a->expr);
    }
    expectedToken(SEMICOLON);
    nextToken();
}

void parseIndex(struct nodeIndex* index) {
    expectedToken(LBRACE);
    nextToken();
    index->expr = (struct nodeExpr*) calloc(1, sizeof(struct nodeExpr));
    parseExpr(index->expr);
    expectedToken(RBRACE);
    nextToken();
}

void parseOut(struct nodeOut* out) {
	expectedToken(OUT);
	nextToken();
	expectedToken(LPAREN);
	nextToken();
	out->expr = (struct nodeExpr*) calloc(1, sizeof(struct nodeExpr));
    parseExpr(out->expr);
    expectedToken(RPAREN);
    nextToken();
	expectedToken(SEMICOLON);
    nextToken();
}

void parseIf(struct nodeIf* ifStmt) {
	ifStmt->type = 0;
	expectedToken(IF);
	nextToken();
	ifStmt->cond = (struct nodeCond*) calloc(1, sizeof(struct nodeCond));
    parseCond(ifStmt->cond);
	expectedToken(THEN);
	nextToken();
	ifStmt->ss1 = (struct nodeStmtSeq*) calloc(1, sizeof(struct nodeStmtSeq));
    parseStmtSeq(ifStmt->ss1);
	if (currentToken() == ELSE) {
		ifStmt->type = 1;
		nextToken();
		ifStmt->ss2 = (struct nodeStmtSeq*) calloc(1, sizeof(struct nodeStmtSeq));
		parseStmtSeq(ifStmt->ss2);
	}
	expectedToken(END);
	nextToken();
}

void parseLoop(struct nodeLoop* loop) {
	expectedToken(WHILE);
	nextToken();
	loop->cond = (struct nodeCond*) calloc(1, sizeof(struct nodeCond));
    parseCond(loop->cond);
	expectedToken(DO);
	nextToken();
	loop->ss = (struct nodeStmtSeq*) calloc(1, sizeof(struct nodeStmtSeq));
	parseStmtSeq(loop->ss);
	expectedToken(END);
	nextToken();
}

void parseCond(struct nodeCond* cond) {
	cond->type = 0;
	if (currentToken() == NOT) {
		nextToken();
		cond->type = 1;
		cond->cond = (struct nodeCond*) calloc(1, sizeof(struct nodeCond));
		parseCond(cond->cond);
	} else {
		cond->cmpr = (struct nodeCmpr*) calloc(1, sizeof(struct nodeCmpr));
		parseCmpr(cond->cmpr);
		if (currentToken() == OR) {
			nextToken();
			cond->type = 2;
			cond->cond = (struct nodeCond*) calloc(1, sizeof(struct nodeCond));
			parseCond(cond->cond);
		} else if (currentToken() == AND) {
			nextToken();
			cond->type = 3;
			cond->cond = (struct nodeCond*) calloc(1, sizeof(struct nodeCond));
			parseCond(cond->cond);
		}
	}
}

void parseCmpr(struct nodeCmpr* cmpr) {
	cmpr->expr1 = (struct nodeExpr*) calloc(1, sizeof(struct nodeExpr));
	parseExpr(cmpr->expr1);
	if (currentToken() == EQUAL) {
		cmpr->type = 0;
	} else if (currentToken() == LESS) {
		cmpr->type = 1;
	}
	nextToken();
	cmpr->expr2 = (struct nodeExpr*) calloc(1, sizeof(struct nodeExpr));
	parseExpr(cmpr->expr2);
}

void parseExpr(struct nodeExpr* expr) {
	expr->type = 0;
	expr->term = (struct nodeTerm*) calloc(1, sizeof(struct nodeTerm));
	parseTerm(expr->term);
	if (currentToken() == ADD) {
		expr->type = 1;
	} else if (currentToken() == SUBTRACT) {
		expr->type = 2;
	}
	if (expr->type > 0) {
		nextToken();
		expr->expr = (struct nodeExpr*) calloc(1, sizeof(struct nodeExpr));
		parseExpr(expr->expr);
	}
}

void parseTerm(struct nodeTerm* term) {
	term->type = 0;
	term->factor = (struct nodeFactor*) calloc(1, sizeof(struct nodeFactor));
	parseFactor(term->factor);
	if (currentToken() == MULTIPLY) {
		term->type = 1;
	} else if (currentToken() == DIVIDE) {
		term->type = 2;
	}
	if (term->type > 0) {
		nextToken();
		term->term = (struct nodeTerm*) calloc(1, sizeof(struct nodeTerm));
		parseTerm((*term).term);
	}
}

void parseFactor(struct nodeFactor* factor) {
	if (currentToken() == ID) {
		factor->type = 0;
		factor->id = calloc(20, sizeof(char));
        getId(factor->id);
        nextToken();
		if (currentToken() == LBRACE) {
			factor->type = 1;
			nextToken();
			factor->expr = (struct nodeExpr*) calloc(1, sizeof(struct nodeExpr));
			parseExpr((*factor).expr);
			expectedToken(RBRACE);
			nextToken();
		}
	} else if (currentToken() == CONST) {
		factor->type = 2;
		factor->constant = getConst();
		nextToken();
	} else if (currentToken() == LPAREN) {
		factor->type = 3;
		nextToken();
		factor->expr = (struct nodeExpr*) calloc(1, sizeof(struct nodeExpr));
		parseExpr(factor->expr);
		expectedToken(RPAREN);
		nextToken();
	} else {
		factor->type = 4;
		expectedToken(IN);
		nextToken();
		expectedToken(LPAREN);
		nextToken();
		expectedToken(RPAREN);
		nextToken();
	}
}