#include "Language.h"

bool Writting (Branch *Node, FILE *Output_File)
{

    if (Node->Elem->Name != nullptr){
        if (NTYPE == MODE_FUNCTION)
            fprintf(Output_File, "{$%s", Node->Elem->Name);
        else
            fprintf(Output_File, "{%s", Node->Elem->Name);
    }
    
    if (NTYPE == MODE_SYSTEM_OP)
        switch (Node->Elem->ElemData){
            case While: fprintf(Output_File, "{while");
                        break;
            case If: fprintf(Output_File, "{if");
                        break;
            case if_else: fprintf(Output_File, "{if-else");
                        break;
            case ret: fprintf(Output_File, "{ret");
                        break;
            case print: fprintf(Output_File, "{put");
                        break;
            case scan: fprintf(Output_File, "{get");
                        break;
            case oper: fprintf(Output_File, "{op");
                        break;
            case '=':  fprintf(Output_File, "{=");
                        break;
    }

    if (NTYPE == MODE_NUMBER)
        fprintf(Output_File, "{%d", Node->Elem->ElemData);

    if (NTYPE == MODE_CONNECTIONS || NTYPE == MODE_MATH_OP || NTYPE == MODE_NIL)
        fprintf(Output_File, "{%c", Node->Elem->ElemData);


   if (Node->Left != nullptr)
       Writting(Node->Left, Output_File);

   if (Node->Left == nullptr && Node->Right == nullptr)
       return true;

   if (Node->Right != nullptr){
       fprintf(Output_File, "}");
       Writting(Node->Right, Output_File);
   }


   fprintf(Output_File, "}");

   return true;
}
