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
#include "executor.h"
#include "scanner.h"
#include "memory.h"
/*
*
* Helper functions
*
*/

int nextInput(){
    if(currentToken() == EOS){
        printf("ERROR: reach the end of file, no enough values");
        exit(0);
    }
    int val = getConst();
    nextToken();
    return val;
}

/*
*
* execute functions
*
*/

void executeProcedure(struct nodeProcedure* p) {
    //// mem init here
    memory_init();
    if(p->type == 1){
        executeFuncSeq(p->fs);
    }
	executeDeclSeq(p->ds);
	executeStmtSeq(p->ss);
    deref(0);
}

void executeFuncSeq(struct nodeFuncSeq* fs){
    executeFunc(fs->f);
    if(fs->type == 1){
        executeFuncSeq(fs->fs);
    }
}

void executeFunc(struct nodeFunc* f){
    declare_func(f->name, f);
}


void executeDeclSeq(struct nodeDeclSeq* ds) {
	executeDecl(ds->d);
	if (ds->type == 1) {
		executeDeclSeq(ds->ds);
	}
}

void executeDecl(struct nodeDecl* d) {
	if (d->type == 0) {
		executeDeclInt(d->di);
	} else {
		executeDeclRec(d->dr);
	}
}

void executeDeclInt(struct nodeDeclInt* di) {
    declare(di->name, INTEGER);
}

void executeDeclRec(struct nodeDeclRec* dr) {
    declare(dr->name, RECORD);
}

void executeStmtSeq(struct nodeStmtSeq* ss) {
    executeStmt(ss->s);
	if (ss->more == 1) {
		executeStmtSeq(ss->ss);
	}
}

void executeStmt(struct nodeStmt* s) {
	if (s->type == 0) {
		executeAssign(s->assign);
		
	} else if (s->type == 1) {
		executeIf(s->ifStmt);
		
	} else if (s->type == 2) {
		executeLoop(s->loop);
		
	} else if (s->type == 3){
		executeOut(s->out);
	} else if (s->type == 4){
        executeCall(s->call);
    }
}

void executeCall(struct nodeCall* c){
    call(c);
}


void executeAssign(struct nodeAssign* a) {
    int val;
    int pos;
    if(a->type == 0){
        val = executeExpr(a->expr);
//        printf("val: %d\na->lhs: %s\n", val, a->lhs);
        store(a->lhs, val);
    }else if (a->type == 1) {
        val = executeExpr(a->expr);
        pos = executeIndex(a->index);
        storeRec(a->lhs, pos, val);
	}else if(a->type == 2){
        val = executeExpr(a->expr);
        allocateRecord(a->lhs, val);
    }else if(a->type == 3){
        record(a->lhs, a->rhs);
    }
}

int executeIndex(struct nodeIndex* index) {
	return executeExpr(index->expr);
}

void executeOut(struct nodeOut* out) {
	
	
	int val = executeExpr(out->expr);
	printf("%d\n", val);
	
}

void executeIf(struct nodeIf* ifStmt) {
	if(executeCond(ifStmt->cond)){
        executeStmtSeq(ifStmt->ss1);
    }else{
        if (ifStmt->type == 1) {
            executeStmtSeq(ifStmt->ss2);
        }
	}
}

void executeLoop(struct nodeLoop* loop) {
	while(executeCond(loop->cond)){
        executeStmtSeq(loop->ss);
    }
}

int executeCond(struct nodeCond* cond) {
    int res;
    int vcmpr;
    int vcond;

	if (cond->type == 0) {
		vcmpr = executeCmpr(cond->cmpr);
        res = vcmpr;
	} else if (cond->type == 1) {
        vcond = executeCond(cond->cond);
        res = !vcond;
    } else if (cond->type == 2) {
		vcond = executeCond(cond->cond);
        res = vcmpr || vcond;
	} else if (cond->type == 3) {
        vcond = executeCond(cond->cond);
        res = vcmpr && vcond;
	}
    return res;
}

int executeCmpr(struct nodeCmpr* cmpr) {
	int l = executeExpr(cmpr->expr1);
    int r = executeExpr(cmpr->expr2);
    int res;
    if (cmpr->type == 0) {
        res = l == r;
	} else {
		res = l < r;
	}
    return res;
}

int executeExpr(struct nodeExpr* expr) {
	int val = executeTerm(expr->term);
	if (expr->type == 1) {
        val += executeExpr(expr->expr);
	} else if (expr->type == 2) {
		val -= executeExpr(expr->expr);
	}
    return val;
}

int executeTerm(struct nodeTerm* term) {
    int val = executeFactor(term->factor);
	if (term->type == 1) {
        val *= executeTerm(term->term);
	} else if (term->type == 2) {
        int val2 = executeTerm(term->term);
        if(val2 != 0){
            val /= val2;
        }else{
            // deal with 0 cases
            printf("ERROR: Divide by zero\n");
            exit(0);
        }
    }
    return val;
}

int executeFactor(struct nodeFactor* factor) {
    int val = 0;
    int pos;
	if (factor->type == 0){
        val = recall(factor->id);
    } else if (factor->type == 1) {
        pos = executeExpr(factor->expr);
        val = recallRec(factor->id, pos);
    } else if (factor->type == 2) {
		val = factor->constant;
	} else if (factor->type == 3) {
		val = executeExpr(factor->expr);
	} else {
		val = nextInput();
	}
    return val;
}