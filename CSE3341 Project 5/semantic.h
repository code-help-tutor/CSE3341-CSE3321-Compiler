https://tutorcs.com
WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "tree.h"


void semanticProcedure(struct nodeProcedure* p);

void semanticDeclSeq(struct nodeDeclSeq* ds);

void semanticDecl(struct nodeDecl* d);

void semanticDeclInt(struct nodeDeclInt* di);

void semanticDeclRec(struct nodeDeclRec* dr);

void semanticStmtSeq(struct nodeStmtSeq* ss);

void semanticStmt(struct nodeStmt* s);

void semanticAssign(struct nodeAssign* a);

void semanticIndex(struct nodeIndex* index);

void semanticOut(struct nodeOut* out);

void semanticIf(struct nodeIf* ifStmt);

void semanticLoop(struct nodeLoop* loop);

void semanticCond(struct nodeCond* cond);

void semanticCmpr(struct nodeCmpr* cmpr);

void semanticExpr(struct nodeExpr* expr);

void semanticTerm(struct nodeTerm* term);

void semanticFactor(struct nodeFactor* factor);

#endif