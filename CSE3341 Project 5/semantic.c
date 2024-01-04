https://tutorcs.com
WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "core.h"
#include "tree.h"
#include "semantic.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "core.h"
#include "tree.h"
#include "semantic.h"

/*
*
* data structures, helper functions
*
*/

static char** integers;
static int iLen;
static char** records;
static int rLen;

// Return 0 if not declared, 1 if integer, 2 if record
static int findVariable(char* var) {
	//printf("Hello = %s\n", var);
	int type = 0;
	for (int i=0; i<iLen && !type; i++) {
		//printf("Hello1: %s\n", integers[i]);
		type = (strcmp(integers[i], var)==0) ? 1 : 0;
	}
	for (int i=0; i<rLen && !type; i++) {
		//printf("Hello2: %s\n", records[i]);
		type = (strcmp(records[i], var)==0) ? 2 : 0;
	}
	return type;
}

/*
*
* Semantic check functions
*
*/

void semanticProcedure(struct nodeProcedure* p) {
	integers = malloc(0);
	iLen = 0;
	records = malloc(0);
	rLen = 0;
	semanticDeclSeq(p->ds);
	semanticStmtSeq(p->ss);
	free(integers);
	free(records);
}

void semanticDeclSeq(struct nodeDeclSeq* ds) {
	semanticDecl(ds->d);
	if (ds->type == 1) {
		semanticDeclSeq(ds->ds);
	}
}

void semanticDecl(struct nodeDecl* d) {
	if (d->type == 0) {
		semanticDeclInt(d->di);
	} else {
		semanticDeclRec(d->dr);
	}
}

void semanticDeclInt(struct nodeDeclInt* di) {
	if (findVariable(di->name) == 0) {
		iLen++;
		integers = realloc(integers, iLen*sizeof(char*));
		integers[iLen-1] = di->name;
	} else {
		printf("Semantic error: variable %s is already declared!\n", di->name);
		exit(0);
	}	
}

void semanticDeclRec(struct nodeDeclRec* dr) {
	if (findVariable(dr->name) == 0) {
		rLen++;
		records = realloc(records, rLen*sizeof(char*));
		records[rLen-1] = dr->name;
	} else {
		printf("Semantic error: variable %s is already declared!\n", dr->name);
		exit(0);
	}
}



void semanticStmtSeq(struct nodeStmtSeq* ss) {
    semanticStmt(ss->s);
	if (ss->more == 1) {
		semanticStmtSeq(ss->ss);
	}
}

void semanticStmt(struct nodeStmt* s) {
	if (s->type == 0) {
		semanticAssign(s->assign);
	} else if (s->type == 1) {
		semanticIf(s->ifStmt);
	} else if (s->type == 2) {
		semanticLoop(s->loop);
	} else {
		semanticOut(s->out);
	}
}

void semanticAssign(struct nodeAssign* a) {
	int lhsType = findVariable(a->lhs);
	if (lhsType == 0) {
		printf("Semantnic error: lhs assignment variable %s has not been declared!\n", a->lhs);
		exit(0);
	}
	if (a->type == 1) {
		if (lhsType != 2) {
			printf("Semantic error: lhs assignment variable %s has not been declared as record!\n", a->lhs);
			exit(0);
		}
		semanticIndex(a->index);
	}
	if (a->type <= 1) {
		semanticExpr(a->expr);
	} else if (a->type == 2 && lhsType != 2) {
		printf("Semantic error: lhs assignment variable %s is not of type record!\n", a->lhs);
		exit(0);
	} else if (a->type == 3 && (lhsType != 2 || findVariable(a->rhs) != 2)){
		printf("Semantic error: assignment variables %s and %s are not both of type record!\n", a->lhs, a->rhs);
		exit(0);
	}
}


void semanticIndex(struct nodeIndex* index) {
	semanticExpr(index->expr);
}

void semanticOut(struct nodeOut* out) {
	semanticExpr(out->expr);
}

void semanticIf(struct nodeIf* ifStmt) {
	semanticCond(ifStmt->cond);
	semanticStmtSeq(ifStmt->ss1);
	if (ifStmt->type == 1) {
		semanticStmtSeq(ifStmt->ss2);
	}
}

void semanticLoop(struct nodeLoop* loop) {
	semanticCond(loop->cond);
	semanticStmtSeq(loop->ss);
}

void semanticCond(struct nodeCond* cond) {
	if (cond->type == 0) {
		semanticCmpr(cond->cmpr);
	} else if (cond->type == 1) {
		semanticCond(cond->cond);
	} else {
		semanticCmpr(cond->cmpr);
		semanticCond(cond->cond);
	}
}

void semanticCmpr(struct nodeCmpr* cmpr) {
	semanticExpr(cmpr->expr1);
	semanticExpr(cmpr->expr2);
}

void semanticExpr(struct nodeExpr* expr) {
	semanticTerm(expr->term);
	if (expr->type > 0) {
		semanticExpr(expr->expr);
	}
}

void semanticTerm(struct nodeTerm* term) {
	semanticFactor(term->factor);
	if (term->type > 0) {
		semanticTerm(term->term);
	}
}

void semanticFactor(struct nodeFactor* factor) {
	if (factor->type == 0 && findVariable(factor->id) == 0) {
		printf("Semantic error: variable %s has not been declared!\n", factor->id);
		exit(0);
	} else if (factor->type == 1) {
		if (findVariable(factor->id) != 2) {
			printf("Semantic error: variable %s is not delcared as type record!\n", factor->id);
			exit(0);
		}
		semanticExpr(factor->expr);
	} else if (factor->type == 3) {
		semanticExpr(factor->expr);
	}
}
