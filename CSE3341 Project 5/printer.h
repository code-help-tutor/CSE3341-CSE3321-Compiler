https://tutorcs.com
WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
#ifndef PRINTER_H
#define PRINTER_H

#include "tree.h"

void printProcedure(struct nodeProcedure* p);

void printFuncSeq(struct nodeFuncSeq* fs);

void printFunc(struct nodeFunc* f);

void printParam(struct nodeParam* p);

void printDeclSeq(struct nodeDeclSeq* ds, int indent);

void printDecl(struct nodeDecl* d, int indent);

void printDeclInt(struct nodeDeclInt* di, int index);
	
void printDeclRec(struct nodeDeclRec* di, int index);

void printStmtSeq(struct nodeStmtSeq* ss, int indent);

void printStmt(struct nodeStmt* s, int indent);

void printCall(struct nodeCall* c);

void printAssign(struct nodeAssign* a, int indent);

void printIndex(struct nodeIndex* index);

void printOut(struct nodeOut* out, int indent);

void printIf(struct nodeIf* ifStmt, int indent);

void printLoop(struct nodeLoop* loop, int indent);

void printCond(struct nodeCond* cond);

void printCmpr(struct nodeCmpr* cmpr);

void printExpr(struct nodeExpr* expr);

void printTerm(struct nodeTerm* term);

void printFactor(struct nodeFactor* factor);



#endif