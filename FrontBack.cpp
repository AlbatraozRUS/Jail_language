#include "Language.h"

int Space = -1;

Branch *Analyze (char *buffer, unsigned *Letter);
void Make_Space(FILE *OUT);

Branch *FrontBack()
{
    FILE *Out = fopen(Trans, "wb");
    assert(Out);

    char *buffer = Reading_Back();

    Branch *Root = (Branch*) calloc(1, sizeof(Branch));
    Data   *Elem = (Data*)   calloc(1, sizeof(Data));
    Root->Elem = Elem;

    unsigned Letter = 0;

    FillTree(Root, buffer, &Letter);

    FILE *Lang = fopen (Lang_Back, "wb");

    TreeToLang(Root->Left, Lang);

    fclose(Out);
    fclose(Lang);

    return Root;
}

void TreeToLang(Branch *Node, FILE *OUT)
{
    if (Node == nullptr)
        return;


    switch(Node->Elem->Type){

    case MODE_CONNECTIONS:    {TreeToLang(NL, OUT); TreeToLang(NR, OUT); break;}

    case MODE_NUMBER:         {fprintf(OUT, "%d", Node->Elem->ElemData); break;}

    case MODE_VARIABLE:       {fprintf(OUT, "%s", Node->Elem->Name);     break;}

    case MODE_MATH_FUNC:      {fprintf(OUT, "%s", Node->Elem->Name);
                               Space++;
                               Make_Space(OUT);
                               TreeToLang(NL, OUT);
                               Space--;                                  break;}

    case MODE_FUNCTION:       {fprintf(OUT, "Залететь на зону %s ( ", Node->Elem->Name);

                               TreeToLang(NL, OUT);

                               fprintf(OUT, " )");

                               if (Node->Parent->Elem->Type == MODE_CONNECTIONS){

                                   fprintf(OUT, "\n{\n");

                                   Space++;
                                   Make_Space(OUT);

                                   TreeToLang(NR, OUT);

                                   Space--;

                                   fprintf(OUT, "}\n\n");
                                   }
                                                                         break;}

    case MODE_NIL:                                                       break;

    case MODE_MATH_OP:        {TreeToLang(NL, OUT);
                               fprintf(OUT, " %c ", Node->Elem->ElemData);
                               TreeToLang(NR, OUT);
                                                                         break;}

    case MODE_SYSTEM_OP_BACK: {PrintSystemOP(Node, OUT);                 break;}
    }


}

void PrintSystemOP(Branch *Node, FILE *OUT)
{
    int OP_CODE = -1;

    #define CLASSIC
    #define SYSTEM_OP(name, length, code) {\
                     if (strncmp(#name, NNAME, length) == 0)\
                           OP_CODE = code;}

    #include "OPes.h"

    #undef CLASSIC
    #undef SYSTEM_OP

    switch (OP_CODE){

    case 1: {TreeToLang(NL, OUT); TreeToLang(NR, OUT);  break;}

    case 2: {fprintf(OUT, "На какой стул сядешь (");
             TreeToLang(NL, OUT);
             fprintf(OUT," )\n");
             Space++;
             Make_Space(OUT);
             TreeToLang(NR, OUT);
             Space--;
                                                        break;}

    case 3: {fprintf(OUT, "Стул 1:\n");
             Space++;
             Make_Space(OUT);
             TreeToLang(NL, OUT);
             Space--;
             fprintf(OUT, "}\nСтул 2:\n");
             Space++;
             Make_Space(OUT);
             TreeToLang(NR, OUT);
             Space--;
             fprintf(OUT, "}\n");
                                                        break;}

    case 4: {fprintf(OUT, "Кто по жизни ( ");
             TreeToLang(NL, OUT);
             fprintf(OUT, " )\n");
                                                        break;}

    case 5: {fprintf(OUT, "По какой статье ( ");
             TreeToLang(NL, OUT);
             fprintf(OUT, " )\n");
                                                        break;}

    case 6: {fprintf(OUT, "Погонять ( ");
             TreeToLang(NL, OUT);
             fprintf(OUT, " )\n{");
             Space++;
             Make_Space(OUT);
             TreeToLang(NR, OUT);
             Space--;
             fprintf(OUT, "\n}\n");
                                                        break;}

    case 7: {fprintf(OUT, "Откинулся ( ");
             TreeToLang(NL, OUT);
             fprintf(OUT, " )\n");
                                                        break;}

    default: {fprintf(stderr, "Unrecognized function in PrintSystemOP\nType - %d\nOP - %s", OP_CODE, NNAME);
              abort();}
    }
}

bool FillTree (Branch *Node, char *buffer, unsigned int *Letter)
{
   assert(Node);

   for (;;)
   {
       if (POSITION == '{'){
           *Letter += 1;
           Branch *SubTree = Analyze(buffer, Letter);
           SubTree->Parent = Node;

           if (Node->Left == nullptr){
               Node->Left = SubTree;
               FillTree(Node->Left, buffer, Letter);
           }

       }


       while (buffer[*Letter] == '}'){
           Node = Node->Parent;
           *Letter += 1;
       }

       if (POSITION == '{' && Node->Left != nullptr){
               *Letter += 1;
               Branch *SubTree = Analyze(buffer, Letter);
               Node->Right = SubTree;
               SubTree->Parent = Node;
               FillTree(Node->Right, buffer, Letter);
           }

       break;
   }

   return true;
}


Branch *Analyze (char *buffer, unsigned *Letter)
{
    if (isdigit(POSITION) != 0){

        int Number = 0;
        unsigned ScannedLetters = 0;

        sscanf(buffer + *Letter, "%d%n", &Number, &ScannedLetters);
        *Letter += ScannedLetters;

        return NewBranch(MODE_NUMBER, nullptr, Number);
    }

    if (POSITION == '*' || POSITION == '/' || POSITION == '+' || POSITION == '-' || POSITION == '>' || POSITION == '<'){

        int sign = POSITION;
        *Letter += 1;

        return NewBranch(MODE_MATH_OP, nullptr, sign);
    }

    if (CheckForSystemOP_Back(buffer, Letter)){
        char *Name = ScanName(buffer, Letter);


        return NewBranch(MODE_SYSTEM_OP_BACK, Name, POISON);
    }

    if (CheckForMathOP(buffer + *Letter))
    {
        char *Name = ScanName(buffer, Letter);


        return NewBranch(MODE_MATH_FUNC, Name, POISON);
    }

    switch (POSITION){

    case ';': {int Connection = POSITION; *Letter += 1; return NewBranch(MODE_CONNECTIONS, nullptr, Connection);}

    case ',': {int Connection = POSITION; *Letter += 1; return NewBranch(MODE_CONNECTIONS, nullptr, Connection);}

    case '@': {*Letter += 1;                            return NewBranch(MODE_NIL, nullptr, '@');}

    case '=': {*Letter += 1;
               if (*(buffer + *Letter - 3) == 'f')
                                                        return NewBranch(MODE_MATH_OP, nullptr, '=');

                                                        return NewBranch(MODE_SYSTEM_OP, nullptr, '=');}

    case '$': {*Letter += 1;
               char *Name = ScanName(buffer, Letter);   return NewBranch(MODE_FUNCTION, Name, POISON);}
                                                            

    default: {break;}
    }

    return Variable_Back(buffer, Letter);

}

int CheckForSystemOP_Back(char *buffer, unsigned *Letter)
{
    #define CLASSIC
    #define SYSTEM_OP(name, length, number) {\
            if (strncmp(#name, buffer + *Letter, length) == 0){\
                return number;}}

    #include "OPes.h"

    #undef CLASSIC
    #undef SYSTEM_OP

    return 0;
}

Branch *Variable_Back (char *buffer, unsigned *Letter)
{
    char *Name = ScanName(buffer, Letter);

    return NewBranch(MODE_VARIABLE, Name, POISON);
}

char *ScanName (char *buffer, unsigned *Letter)
{
    char *TempName = (char*) calloc(40, sizeof(char));
    unsigned ScannedLetters = 0;

    sscanf(buffer + *Letter, "%[^}^{]%n", TempName, &ScannedLetters);
    *Letter += ScannedLetters;

    char *Name = strdup(TempName);
    free(TempName);

    return Name;
}

void Make_Space(FILE *OUT)
{
    for (int Iteration = 0; Iteration < Space; Iteration++)
        fprintf(OUT, " ");
}

