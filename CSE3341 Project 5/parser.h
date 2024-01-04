https://tutorcs.com
WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
#ifndef PARSER_H
#define PARSER_H

#include "tree.h"

void parseProcedure(struct nodeProcedure* p);

void parseFuncSeq(struct nodeFuncSeq* fs);

void parseFunc(struct nodeFunc* f);

void parseParam(struct nodeParam* p);

void parseDeclSeq(struct nodeDeclSeq* ds);

void parseDecl(struct nodeDecl* ds);

void parseDeclInt(struct nodeDeclInt* di);

void parseDeclRec(struct nodeDeclRec* dr);

void parseStmtSeq(struct nodeStmtSeq* ss);

void parseStmt(struct nodeStmt* s);

void parseCall(struct nodeCall* c);

void parseAssign(struct nodeAssign* a);

void parseIndex(struct nodeIndex* index);

void parseOut(struct nodeOut* out);

void parseIf(struct nodeIf* ifStmt);

void parseLoop(struct nodeLoop* loop);

void parseCond(struct nodeCond* cond);

void parseCmpr(struct nodeCmpr* cmpr);

void parseExpr(struct nodeExpr* expr);

void parseTerm(struct nodeTerm* term);

void parseFactor(struct nodeFactor* factor);

#endif