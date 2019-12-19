#ifndef LANGUAGE_LANGUAGE_H
#define LANGUAGE_LANGUAGE_H

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <string>

#define NTYPE Node->Elem->Type
#define NL Node->Left
#define NR Node->Right
#define POSITION *(buffer + *Letter)
#define NNAME Node->Elem->Name

const char Input_File[] = "/Users/albatraozrus/CLionProjects/Language/Code.txt";
const char Output[]     = "/Users/albatraozrus/CLionProjects/Language/Output.txt";
const char Trans[]      = "/Users/albatraozrus/CLionProjects/Language/Translator.txt";
const char DOT[]        = "/Users/albatraozrus/Desktop/DOT.dot";
const int POISON        = -606;
const char Lang_Back[]       = "/Users/albatraozrus/Desktop/Language.txt";
const int It_Is_Not_System_OP = -1;



struct Data {
  int Type     = POISON;
  int ElemData = POISON;
  char *Name   = nullptr;
};

struct Branch {
  Branch *Parent = nullptr;
  Branch *Left   = nullptr;
  Branch *Right  = nullptr;

  Data   *Elem   = nullptr;
};

enum Types {
  MODE_NUMBER = 201,
          MODE_VARIABLE = 211,
                  MODE_SYSTEM_OP = 200,
                          MODE_MATH_OP = 202,
                                  MODE_MATH_FUNC = 203,
                                          MODE_CONNECTIONS = 204,
                                                  MODE_FUNCTION = 205,
                                                          MODE_NIL = 206,
                                                                  MODE_SYSTEM_OP_BACK = 207,


};

enum System_Operators{
  While = 251,
          If = 252,
                  if_else = 253,
                          ret = 254,
                              print = 256,
                                    scan = 257,
                                         oper = 258,


};

char *Reading();
Branch *NewBranch (unsigned Type, char *String, int Number);
Branch *GetG(const char *Code, Branch *Root);
Branch *CreateBranch ();
void tree_dot(Branch* Root);
bool Writting (Branch *Node, FILE *Output_File);
void FrontEnd();
void NILISE (Branch *Node);
void Backend (Branch *Root);
bool DestBranch (Branch *Node);
char *Reading_Back();
bool FillTree (Branch *Node, char *buffer, unsigned int *Letter);
int CheckForSystemOP_Back(char *buffer, unsigned *Letter);
bool CheckForMathOP(char *Name);
Branch *Variable_Back (char *buffer, unsigned *Letter);
Branch *FrontBack();
char *ScanName (char *buffer, unsigned *Letter);
void PrintSystemOP(Branch *Node, FILE *OUT);
void TreeToLang(Branch *Node, FILE *OUT);

#endif //LANGUAGE_LANGUAGE_H
