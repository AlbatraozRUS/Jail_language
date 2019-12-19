#include "Recursive descent.h"

Branch *CreateBranch ()
{

    Branch *Root = (Branch*) calloc(1, sizeof(Branch));
    Data *Elem   = (Data*)   calloc(1, sizeof(Data));

    Root->Elem = Elem;

    Elem->Type = MODE_CONNECTIONS;
    Elem->ElemData = ';';

    return Root;

}

Branch *NewBranch (unsigned Type, char *String, int Number)
{
    Branch *Node    = (Branch*) calloc(1, sizeof(Branch));
    Data   *Elem    = (Data*)   calloc(1, sizeof(Data));

    Node->Elem = Elem;

    switch (Type)
    {
        case MODE_NUMBER:         {Node->Elem->ElemData = Number;      Node->Elem->Type = MODE_NUMBER;      break;}

        case MODE_VARIABLE:       {Node->Elem->Name = strdup(String);  Node->Elem->Type = MODE_VARIABLE;    break;}

        case MODE_MATH_OP:        {Node->Elem->ElemData = Number;      Node->Elem->Type = MODE_MATH_OP;     break;}

        case MODE_MATH_FUNC:      {Node->Elem->Name = strdup(String);  Node->Elem->Type = MODE_MATH_FUNC;   break;}

        case MODE_CONNECTIONS:    {Node->Elem->ElemData = Number;      Node->Elem->Type = MODE_CONNECTIONS; break;}

        case MODE_SYSTEM_OP:      {Node->Elem->ElemData = Number;      Node->Elem->Type = MODE_SYSTEM_OP;   break;}

        case MODE_SYSTEM_OP_BACK: {Node->Elem->Name = strdup(String);  Node->Elem->Type = MODE_SYSTEM_OP_BACK;
                                                                                                            break;}

        case  MODE_NIL:           {Node->Elem->ElemData = '@';         Node->Elem->Type = MODE_NIL;         break;}

        case MODE_FUNCTION:       {Node->Elem->Name = strdup(String);  Node->Elem->Type = MODE_FUNCTION;    break;}

        default: {printf("Something went wrong in NewBranch\nType-%d\nString-%s\nNumber-%d", Type, String, Number);
                                                                                                           abort();}
    }


    return Node;
}

bool DestBranch (Branch *Node)
{
    free(Node->Elem);

    if (Node->Left != nullptr)
        DestBranch(Node->Left);
    if (Node->Right != nullptr)
        DestBranch(Node->Right);

    free(Node);

    return true;
}
