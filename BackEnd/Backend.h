
#ifndef LANGUAGE_BACKEND_H
#define LANGUAGE_BACKEND_H

#include "Language.h"

const char OutNameForASM[] = "/Users/albatraozrus/CLionProjects/Language/ASM.txt";
const unsigned SSize = 50;
const unsigned AmountVar = 10;

#define FINAL Node->Left == nullptr && Node->Right == nullptr
#define NDATA Node->Elem->ElemData
#define NTYPE Node->Elem->Type

#define PIZDEC {printf("\n\nBEGIN TABLENAME\n\n");\
        for (int index = 0; index < AmountVar; index++)\
        printf("Table [%d] - %s\n",index, Main[index].Name_Variable);\
        printf("\n\nEND TABLENAME\n\n");};

int NumIf            = 0;
int NumWhile         = 0;
int RAM_FREE_SPACE   = 0;
int TABLE_FREE_SPACE = 0;

struct Table_Name {

  char *Name_Variable = nullptr;
  int index = -1;

};

enum MATH_FUNC {
  sqrt = 1,
          sin = 2,
                  cos = 3,
                          tg = 4,
                                  ctg = 5,
};

Table_Name *Main = nullptr;

bool ASM_SYSTEM_OP (Branch *Node, FILE *ASM_Out);
bool ASM_VARIABLES (Branch *Node, FILE * ASM_Out);
bool ASM_MATH_OP (Branch *Node, FILE *ASM_Out);
bool ASM_MATH_FUNC (Branch *Node, FILE *ASM_Out);
bool My_Switch (Branch *Node, FILE *ASM_Out);
void Explore_Tree(Branch *Node, FILE *ASM_Out);
bool ASM_FUNCTION (Branch *Node, FILE * ASM_Out);


#endif //LANGUAGE_BACKEND_H
