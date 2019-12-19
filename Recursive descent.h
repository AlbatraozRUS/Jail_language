#ifndef LANGUAGE_RECURSIVE_DESCENT_H
#define LANGUAGE_RECURSIVE_DESCENT_H

#include "Language.h"


const unsigned SSize           = 100;
const int It_Is_Not_A_Function = -1;


Branch *GetG(const char *Code);
void GetCode(Branch *Node);
char *ScanVar ();
Branch *GetFunc();
Branch *GetPar ();
Branch *GetWV  ();
Branch *GetIf  ();
Branch *GetWh  ();
Branch *GetPr  ();
Branch *GetSc  ();
Branch *GetRet ();
Branch *GetE   ();
Branch *GetT   ();
Branch *GetCall();
Branch *GetPow ();
Branch *GetP   ();
Branch *GetN   ();
Branch *GetSM  ();
void SkipTrash ();
int CheckForSystemOP();
bool ConnectSubtrees(Branch *Connection);

#endif //LANGUAGE_RECURSIVE_DESCENT_H
