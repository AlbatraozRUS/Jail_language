#include "Backend.h"

void Backend (Branch *Root)
{
    FILE *ASM_Ouput = fopen(OutNameForASM, "wb");
    assert(ASM_Ouput);

    Main = (Table_Name*) calloc(AmountVar, sizeof(Table_Name));
    for (int index = 0; index < AmountVar; index++)
        Main[index].Name_Variable = (char *) calloc(SSize, sizeof(char));

    Explore_Tree(Root, ASM_Ouput);

    fprintf(ASM_Ouput, "End\n");

    fclose(ASM_Ouput);

    free(Main);


}

void Explore_Tree(Branch *Node, FILE *ASM_Out)
{
    assert(Node);

    if (NTYPE == MODE_SYSTEM_OP){

        ASM_SYSTEM_OP(Node, ASM_Out);

        if (NTYPE != MODE_CONNECTIONS &&
            NTYPE != MODE_NIL &&
            Node->Elem->ElemData != oper)

            return;
    }
    if (Node->Elem->Name != nullptr){
        if (strcmp(Node->Elem->Name, "корешок") == 0){
            Explore_Tree(Node->Left, ASM_Out);
            ASM_MATH_FUNC(Node, ASM_Out);
            return;
        }
    }

    if (Node->Left != nullptr)
        Explore_Tree(Node->Left, ASM_Out);

    if (FINAL){
        My_Switch(Node, ASM_Out);
        return;
    }

    if (Node->Right != nullptr)
        Explore_Tree(Node->Right, ASM_Out);

    My_Switch(Node, ASM_Out);

}

bool My_Switch (Branch *Node, FILE *ASM_Out)
{
    assert(Node);

    switch (NTYPE){

    case MODE_MATH_OP:     {ASM_MATH_OP(Node, ASM_Out);                              break;}

    case MODE_SYSTEM_OP:   {ASM_SYSTEM_OP(Node, ASM_Out);                            break;}

    case MODE_CONNECTIONS: {                                                         break;}

    case MODE_NUMBER:      {fprintf(ASM_Out, "Push %d\n", NDATA);                    break;}

    case MODE_VARIABLE:    {ASM_VARIABLES(Node, ASM_Out);                            break;}

    case MODE_MATH_FUNC:   {ASM_MATH_FUNC(Node, ASM_Out);                            break;}

    case MODE_NIL:         {                                                         break;}

    case MODE_FUNCTION:    {ASM_FUNCTION(Node, ASM_Out);                             break;}


    default: {fprintf(stderr, "Problem occurred in My_Switch, please check input.\n"
                              "Type - %d\nData - %d", NTYPE, NDATA); abort();}

    }

    return true;
}

bool ASM_VARIABLES (Branch *Node, FILE * ASM_Out)
{
    int Var_Index_Table = -1;

    for (int index = 0; index < AmountVar; index++){

        if (strcmp(Node->Elem->Name, Main[index].Name_Variable) == 0){

            Var_Index_Table = index;

            if (Node->Parent->Left == Node){

                if ((Node->Parent->Elem->Type == MODE_SYSTEM_OP && (
                        Node->Parent->Elem->ElemData == print ||
                        Node->Parent->Elem->ElemData == scan)) ||
                        Node->Parent->Elem->Type == MODE_MATH_FUNC){
                    
                fprintf(ASM_Out, "Pop [%d]\n", Main[Var_Index_Table].index);
                break;
                }

                if (Node->Parent->Elem->Type == MODE_SYSTEM_OP ||
                        (Node->Parent->Elem->Type == MODE_CONNECTIONS &&
                        Node->Parent->Elem->ElemData  == ';')){

                My_Switch(Node->Parent->Right, ASM_Out);

                fprintf(ASM_Out, "Push [%d]\n", Main[Var_Index_Table].index);

                break;
                }

                if (Node->Parent->Elem->Type == MODE_MATH_OP ||
                   (Node->Parent->Elem->Type == MODE_CONNECTIONS &&
                    Node->Parent->Elem->ElemData  == ',')){

                    fprintf(ASM_Out, "Pop [%d]\n", Main[Var_Index_Table].index);
                    
                    break;
                }


            }
            else {

                fprintf(ASM_Out, "Pop [%d]\n", Main[Var_Index_Table].index);

                break;
            }
        }
    }

    if (Var_Index_Table == -1){

        Main[TABLE_FREE_SPACE].Name_Variable = strdup(Node->Elem->Name);
        Main[TABLE_FREE_SPACE++].index = RAM_FREE_SPACE;

        Explore_Tree(Node->Parent->Right, ASM_Out);

        fprintf(ASM_Out, "Push [%d]\n", RAM_FREE_SPACE++);

    }

    return true;
}

bool ASM_SYSTEM_OP (Branch *Node, FILE *ASM_Out)
{
    switch (NDATA){

    case if_else: {                                     return true;}

    case oper:    {                                     return true;}

    case '=':     {Explore_Tree(Node->Left, ASM_Out);   return true;}

    case ret:     {fprintf(ASM_Out, "Push ax\n");
                   fprintf(ASM_Out, "Ret\n");
                                                        return true;}

    case scan:    {fprintf(ASM_Out, "In\n");
                   Explore_Tree(Node->Left, ASM_Out);
                                                        return true;}

    case print:   {Explore_Tree(Node->Left, ASM_Out);
                   fprintf(ASM_Out, "Out\n");
                                                        return true;}

    case If:      {
                   int num = NumIf++;
                   My_Switch(Node->Left->Left, ASM_Out);
                   My_Switch(Node->Left->Right, ASM_Out);
                   ASM_MATH_OP(Node->Left, ASM_Out);

                   fprintf(ASM_Out, "Else_if_%d\n", num);

                   Explore_Tree(Node->Right->Left, ASM_Out);

                   fprintf(ASM_Out, "Jp End_if_%d\n", num);
                   fprintf(ASM_Out, "Else_if_%d:\n", num);

                   Explore_Tree(Node->Right->Right, ASM_Out);

                   fprintf(ASM_Out, "End_if_%d:\n", num);

                   return true;
    }

    case While:   {
                    int num = NumWhile++;
                    My_Switch(Node->Left, ASM_Out);

                    fprintf(ASM_Out, "End_while_%d\n", num);

                    My_Switch(Node->Right, ASM_Out);

                    fprintf(ASM_Out, "End_while_%d:\n", num);

                    return true;}


    default: {fprintf(stderr, "Unknown System operator\n%d\n", NDATA); abort();}

    }
}

bool ASM_MATH_OP (Branch *Node, FILE *ASM_Out)
{
    switch (NDATA){

    case '+': {fprintf(ASM_Out, "Add\n"); break;}

    case '-': {fprintf(ASM_Out, "Sub\n"); break;}

    case '/': {fprintf(ASM_Out, "Div\n"); break;}

    case '*': {fprintf(ASM_Out, "Mul\n"); break;}

    case '>': {fprintf(ASM_Out, "Ja ");   break;}

    case '=': {fprintf(ASM_Out, "Neq ");   break;}

    case '<': {fprintf(ASM_Out, "Jb ");   break;}


    default: {fprintf(stderr, "Unknown Math operator\n[%d] %c\n", NDATA, NDATA); abort();}

    }

    return true;
}


bool ASM_FUNCTION (Branch *Node, FILE * ASM_Out)
{
    if (Node->Parent->Parent->Parent == nullptr)
        return true;

    if (Node->Parent->Elem->Type == MODE_CONNECTIONS &&
        Node->Parent->Elem->ElemData == ';'){
        fprintf(ASM_Out, "%s:\n", Node->Elem->Name);
        Explore_Tree(Node->Left, ASM_Out);
        Explore_Tree(Node->Right, ASM_Out);
        return true;
    }
    Explore_Tree(Node->Left, ASM_Out);
    fprintf(ASM_Out, "Call %s\n", Node->Elem->Name);

    return true;
}

bool ASM_MATH_FUNC (Branch *Node, FILE * ASM_Out)
{


    switch (NDATA){

    case sqrt: {fprintf(ASM_Out, "Sqrt\n"); break;}

    case sin:  {fprintf(ASM_Out, "Sin\n");  break;}

    case cos:  {fprintf(ASM_Out, "Cos\n");  break;}

    default: {fprintf(stderr, "Unknown math function\n"); abort();}
    }

    return true;
}
