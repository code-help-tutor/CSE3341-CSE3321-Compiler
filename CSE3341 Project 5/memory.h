https://tutorcs.com
WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
#ifndef MEMORY_H
#define MEMORY_H

struct frame{
    char** iLookup;
    int* iValues;
    int iLen;
    char** rLookup;
    int **rValues;
    int rLen;
    int** rCount;
    int* rArrayLens;
};


void memory_init();

void fs_init();

void frame_init(struct frame* f);

void declare_func(char* iden, struct nodeFunc* f);

void memory_free();

void declare(char* iden, int type);

void call(struct nodeCall* c);

void deref(int pNum);

void store(char* iden, int value);

int recall(char* iden);

void storeRec(char* iden, int index, int value);

int recallRec(char* iden, int index);

void record(char* lhs, char* rhs);

void allocateRecord(char* iden, int size);

void printGc();

#endif