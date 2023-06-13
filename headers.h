/*********************************************************************
 * Myke Leony dos Santos Amorim
 *
 *
 * Primeiro semestre de 2023
 *
 * Parte 1 do EP 2 de AED-II: árvores B sem índices
 *
 * Código de cabeçalho alterado: função declarada delete() não existe em implementation.c
 * Outras funções não declaradas foram adicionadas.
 *********************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// 1 , AFG , DIST , 93 , 18
struct rec {
	int key;
	// rest
	char country[5];
	char grate[10];
	int score;
	int rate;
};
typedef struct rec recordNode;

struct bTreeNode {
	bool isLeaf;
	int pos;
	int noOfRecs;
	recordNode* recordArr[2 * t - 1];
	int children[2 * t];
};
typedef struct bTreeNode bTreeNode;

struct tree {
	char fileName[20];
	FILE* fp;
	int root;
	int nextPos;
};
typedef struct tree bTree;

bTree* createTree(char* fileName,bool mode);
bTreeNode* nodeInit(bTreeNode* node,bool isLeaf,bTree* tree);
void insert(bTree* tree,recordNode* record);
void traverse(bTree* tree, int root);
void hardPrint(bTree* tree);
void doublePrint(bTree* tree);
void dispNode(bTreeNode* node);
void writeFile(bTree* ptr_tree, bTreeNode* p, int pos);
void readFile(bTree* ptr_tree, bTreeNode* p, int pos);

void splitChild(bTree* tree, bTreeNode* x, int i, bTreeNode* y);
void insertNonFull(bTree* tree, bTreeNode* x, recordNode* record);
void enterData(recordNode* record, int id_num, char country[], char Grate[], int Score, int Rate);
recordNode* getData(char *filepath, int len);
recordNode* search(bTree* tree, int key);
recordNode* searchRecursive(bTree* tree, int key, bTreeNode* root);
bool removeFromTree(bTree* tree, int key);
bTreeNode* merge(bTree* tree, bTreeNode *node, int idx);
void borrowFromNext(bTree* tree, bTreeNode *node, int idx);
void borrowFromPrev(bTree* tree, bTreeNode *node, int idx);
void fill(bTree* tree, bTreeNode *node, int idx);
recordNode *getSucc(bTree* tree, bTreeNode *node, int idx);
recordNode *getPred(bTree* tree, bTreeNode *node, int idx);
void removeFromNonLeaf(bTree* tree, bTreeNode *node, int idx);
void removeFromLeaf (bTree* tree, bTreeNode *node, int idx);
void removeNode(bTree* tree, bTreeNode* node, int k);
int findKey(bTreeNode* node, int k);
