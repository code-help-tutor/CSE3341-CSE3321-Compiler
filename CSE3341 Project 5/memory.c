https://tutorcs.com
WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "tree.h"
#include "core.h"
#include "memory.h"
#include "executor.h"

/*
*
* Data Structs
*
*/

// function lookup?

static char **fLookup;
static struct nodeFunc **fValues;
static int fLen;

static struct frame** frames;
static int top;

int **objects;
int *oArrayLen;
int *oRefCount;
int oLen;

/*
*
* Helper functions
*
*/

// If iden is an integer, return the index. Otherwise, return -1
static int searchInteger(char* iden) {
	int location = -1;
	for (int i=0; i<frames[top]->iLen; i++) {
		if (strcmp(frames[top]->iLookup[i], iden)==0) {
			location = i;
		}
	}
	return location;
}

// If iden is a record, return the index. Otherwise, return -1
static int searchRecord(char* iden) {
	int location = -1;
	for (int i=0; i<frames[top]->rLen; i++) {
//        printf("[top: %d](len: %d)iden: %s ",top, frames[top]->rLen, iden);
//        printf("cmpr to: %s\n", frames[top]->rLookup[i]);
        if (strcmp(frames[top]->rLookup[i], iden)==0) {
			location = i;
		}
	}
	return location;
}

// If iden is a function, return the index. Otherwise, return -1
//static struct nodeFunc* searchFunction(struct nodeFuncSeq* fs, char* iden) {
//    if(strcmp(fs->f->name, iden)==0){
//        return fs->f;
//    }
//    if(fs->type == 0){
//        return NULL;
//    }else if(fs->type == 1){
//        return searchFunction(fs->fs, iden);
//    }
//}

static int searchFunction(char* iden) {
    int location = -1;
    for (int i=0; i<fLen; i++) {
        if (strcmp(fLookup[i], iden)==0) {
            location = i;
        }
    }
    return location;
}


/*
*
* Memory functions
*
*/

// Initialize data structures
// Don't worry about freeing memory, will do that in project 5
void memory_init() {
    frames = (struct frame**)calloc(20, sizeof(struct frame*));
    top = 0;
    frames[top] = (struct frame*) calloc(20, sizeof(struct frame));
    frame_init(frames[top]);
//    fs = (struct nodeFuncSeq*)calloc(1, sizeof(struct nodeFuncSeq));
    objects = (int **)calloc(20, sizeof(int*));
    oArrayLen = (int *)calloc(20, sizeof(int));
    oRefCount = (int *)calloc(20, sizeof(int));
    oLen = 0;
    fs_init();
}

void fs_init(){
    fLookup = (char **)calloc(20, sizeof(char*));
    fValues = (struct nodeFunc**)calloc(20, sizeof(struct nodeFunc*));
    fLen = 0;
//    fs = funcSeq;
}


void frame_init(struct frame* f) {
    f->iLookup = (char **) calloc(20, sizeof(char *));
    f->iValues = (int *) calloc(20, sizeof(int));
    f->iLen = 0;
    f->rLookup = (char **) calloc(20, sizeof(char *));
    f->rValues = (int **) calloc(20, sizeof(int *));
    f->rArrayLens = (int *) calloc(20, sizeof(int *));
    f->rCount = (int **) calloc(20, sizeof(int *));
    f->rLen = 0;
}

void declare_func(char* iden, struct nodeFunc* f){
    fLookup[fLen] = (char*) calloc(20, sizeof(char));
    if(searchFunction(iden)!=-1){
        printf("ERROR: declare function with the same name");
        exit(0);
    }
    strcpy(fLookup[fLen], iden);
    fValues[fLen] = f;
    ++fLen;
}

// Handle an integer or record declaration
void declare(char* iden, int type) {
    struct frame* f = frames[top];
    int iLen = f->iLen;
    int rLen = f->rLen;
    if(type == INTEGER){
        // frame[top]->iLookup[iLen]
        f->iLookup[iLen] = (char*) calloc(20, sizeof(char));
        strcpy(f->iLookup[iLen], iden);
        f->iValues[iLen] = 0;
        ++(f->iLen);
    }else if(type == RECORD){
        f->rLookup[rLen] = (char*) calloc(20, sizeof(char));
        strcpy(f->rLookup[rLen], iden);
        f->rValues[rLen] = NULL;
        f->rCount[rLen] = NULL;
//        rValues[rLen] = (int*) calloc(20, sizeof(int));
        ++(f->rLen);
    }
}


void call(struct nodeCall* c){
    struct nodeParam* p = c->p;
    struct frame* frm = (struct frame*)calloc(1, sizeof(struct frame));
    frame_init(frm);
    int pos = searchFunction(c->name);
    if(pos == -1){
        printf("ERROR:function that has not been declared");
        exit(0);
    }
    struct nodeFunc* f = fValues[pos];
    // save param
//    frames[top]->rCount[pos1] = frames[top]->rCount[pos2];
//    *(frames[top]->rCount[pos1]) += 1;

    int p1 = searchRecord(p->p1);
    int* v1 = frames[top]->rValues[p1];
    int l1 = frames[top]->rArrayLens[p1];
    int* c1 = frames[top]->rCount[p1];
    int p2, *v2, l2, *c2;
    if(p->type == 1){
        p2 = searchRecord(p->p2);
        v2 = frames[top]->rValues[p2];
        l2 = frames[top]->rArrayLens[p2];
        c2 = frames[top]->rCount[p2];
    }
    // new frame
    ++top;
    frames[top] = frm;
    // copy param
    frm->rLookup[frm->rLen] = (char*) calloc(20, sizeof(char));
    strcpy(frm->rLookup[frm->rLen], f->p->p1);
    frm->rValues[frm->rLen] = v1;
    frm->rArrayLens[frm->rLen] = l1;
    frm->rCount[frm->rLen] = c1;
    (*c1) += 1;
    ++(frm->rLen);
    if(p->type != f->p->type){
        printf("Error: number of arguments does not match number of formal parameters.");
        exit(0);
    }
    if(p->type == 1){
        frm->rLookup[frm->rLen] = (char*) calloc(20, sizeof(char));
        strcpy(frm->rLookup[frm->rLen], f->p->p2);
        frm->rValues[frm->rLen] = v2;
        frm->rArrayLens[frm->rLen] = l2;
        frm->rCount[frm->rLen] = c2;
        (*c2) += 1;
        ++(frm->rLen);
    }
    executeDeclSeq(f->ds);
    executeStmtSeq(f->ss);
    // exit fram
    deref(0);
    free(frames[top]);
    --top;
}

//void deref(struct frame* f, struct nodeParam* p){
//    int rLen = f->rLen;
//    // skip the params
//    for (int i=p->type+1; i<rLen; i++) {
//        f->rCount[i] -= 1;
//        printGc();
//    }
//}

void deref(int pNum){
    int rLen = frames[top]->rLen;
    // skip the params
    for (int i=pNum; i<rLen; i++) {
        if(frames[top]->rCount[i]){
            *(frames[top]->rCount[i]) -= 1;
            if(*(frames[top]->rCount[i]) <= 0){
                printGc();
            }
        }
    }
}


// Store a value to a variable. Remember, unindexed stores to a record go to index 0
void store(char* iden, int value) {
    int pos;
    if((pos = searchInteger(iden)) != -1){
        frames[top]->iValues[pos] = value;
    }else if((pos = searchRecord(iden)) != -1){
        if(frames[top]->rValues[pos] != NULL){
            frames[top]->rValues[pos][0] = value;
        }else{
            printf("ERROR: Assign to null record variable\n");
            exit(0);
        }
    }
//    printf("pos: %d\n", pos);
}

// Read a value from a variable. Remember, unindexed reads from a record read index 0
int recall(char* iden) {
    int pos;
    int res;
    if((pos = searchInteger(iden)) != -1){
        res = frames[top]->iValues[pos];
    }else if((pos = searchRecord(iden)) != -1){
        res = frames[top]->rValues[pos][0];
    }else{
        printf("ERROR: Int not declared\n");
        exit(0);
    }
}

// Store a value to a record variable, at the given index
void storeRec(char* iden, int index, int value) {
    int pos;
    if((pos = searchRecord(iden)) != -1){
        if(frames[top]->rValues[pos] != NULL){
            if(index < frames[top]->rArrayLens[pos]){
                frames[top]->rValues[pos][index] = value;
            }else{
                printf("ERROR: Access outside of array bounds\n");
                exit(0);
            }
        }else{
            printf("ERROR: Assign to null record variable\n");
            exit(0);
        }
    }else{
        printf("ERROR: Record not declared(store)\n");
        exit(0);
    }
}

// Read a value from a record variable, from the given index
int recallRec(char* iden, int index) {
    int res;
    int pos;
    if((pos = searchRecord(iden)) != -1){
        if(index < frames[top]->rArrayLens[pos]){
            res = frames[top]->rValues[pos][index];
        }else{
            printf("ERROR: Access outside of array bounds\n");
            exit(0);
        }
    }else{
        printf("ERROR: Record not declared(recall)\n");
        exit(0);
    }
    return res;
}

// Handle "id := record id" type assignment
void record(char* lhs, char* rhs) {
    int pos1, pos2;
    if((pos1 = searchRecord(lhs)) != -1 && (pos2 = searchRecord(rhs))!=-1){
        if(frames[top]->rValues[pos1] != NULL){
            (*(frames[top]->rCount[pos1])) -= 1;
            if(*(frames[top]->rCount[pos1]) <= 0){
                printGc();
            }
        }
        frames[top]->rValues[pos1] = frames[top]->rValues[pos2];
        frames[top]->rArrayLens[pos1] = frames[top]->rArrayLens[pos2];
        frames[top]->rCount[pos1] = frames[top]->rCount[pos2];
        if(frames[top]->rValues[pos1] != NULL){
            (*(frames[top]->rCount[pos1])) += 1;
        }
    }
}

// Handle "id := new record[<expr>]" type assignment
void allocateRecord(char* iden, int size) {
    int pos;
    if((pos = searchRecord(iden)) != -1){
        objects[oLen] = (int *)calloc(size, sizeof(int));
        oArrayLen[oLen] = size;
        oRefCount[oLen] = 1;
        frames[top]->rValues[pos] = objects[oLen];
        frames[top]->rArrayLens[pos] = oArrayLen[oLen];
        frames[top]->rCount[pos] = &(oRefCount[oLen]);
        oLen += 1;
        printGc();
    }else{
        printf("ERROR: Record not declared(alloc)\n");
        exit(0);
    }
}

void printGc(){
    int i;
    int gc = 0;
    for(i = 0; i < oLen; i++){
        if(oRefCount[i] > 0){
            gc += 1;
        }
    }
    printf("gc: %d\n", gc);
}