#include "Recursive descent.h"

const char *Symbol = "";
        
Branch *GetG (const char *Code, Branch *Root)
{
    assert(Code);

    Symbol = Code;

    GetCode(Root);

    return Root;
}

void GetCode(Branch *Root)
{
    SkipTrash();
    Branch *Node = Root;

    while(true){

        switch (CheckForSystemOP()){

        case 0:                    {if (Root->Left != nullptr && Root->Right == nullptr)
                                        Node = Root;
                                    Symbol += 30;
                                    Branch *SubTree = GetFunc();
                                    Branch *Connection = NewBranch(MODE_CONNECTIONS, nullptr, ';');
                                    Node->Right = Connection;
                                    Connection->Parent = Node;
                                    Node = Connection;
                                    Node->Left = SubTree;
                                    SubTree->Parent = Node;
            
                                    break;}

        case It_Is_Not_A_Function: {if (Root->Right != nullptr && Root->Left == nullptr)
                                        Node = Root;
                                    Branch *SubTree = GetWV();
                                    Branch *Connection = NewBranch(MODE_CONNECTIONS, nullptr, ',');
                                    Node->Left = Connection;
                                    Connection->Parent = Node;
                                    Node = Connection;
                                    Node->Right = SubTree;
                                    SubTree->Parent = Node;
                                    break;}

        case 2:                    {return;}

        default: {fprintf(stderr, "Something went wrong in GetCode\nType - %d", CheckForSystemOP()); abort();}

        }
    }
}

Branch *GetFunc()
{
    char *NameForFunction = ScanVar();

    SkipTrash();
    assert(*Symbol == '(');
    Symbol++;

    Branch *Params = nullptr;

    if (*Symbol != ')')
        Params = GetPar();
    else
        Symbol++;


    SkipTrash();

    assert(*Symbol == '{');
    Symbol++;

    Branch *Connection = nullptr;
    Branch *Beginning  = nullptr;
    SkipTrash();
    int count = 0;

    while(*Symbol != '}'){
        if (CheckForSystemOP() != It_Is_Not_A_Function){
              count++;
              Branch *ConnectionTemp = NewBranch(MODE_SYSTEM_OP, nullptr, oper);
              Branch *Body = GetE();
              ConnectionTemp->Left = Body;
              Body->Parent = ConnectionTemp;
              SkipTrash();
              if (count == 1){
                  Beginning = ConnectionTemp;
                  Connection = ConnectionTemp;
                  continue;
              }
              Connection->Right = ConnectionTemp;
              ConnectionTemp->Parent = Connection;
              Connection = ConnectionTemp;
        }
        else {
            count++;
              Branch *ConnectionTemp = NewBranch(MODE_SYSTEM_OP, nullptr, oper);
              Branch *Body = GetWV();
              ConnectionTemp->Left = Body;
              Body->Parent = ConnectionTemp;
              SkipTrash();
              if (count == 1){
                  Beginning = ConnectionTemp;
                  Connection = ConnectionTemp;
                  continue;
              }
              Connection->Right = ConnectionTemp;
              ConnectionTemp->Parent = Connection;
              Connection = ConnectionTemp;
        }
    }
    
    Symbol++;

    Branch *Function = (Branch*) calloc(1, sizeof(Branch));
    Data   *Elem     = (Data*)   calloc(1, sizeof(Data));

    Elem->Type           = MODE_FUNCTION;
    Function->Elem       = Elem;
    Function->Elem->Name = strdup(NameForFunction);
    free(NameForFunction);

    Function->Left = Params;
    if (Params != nullptr)
        Params->Parent = Function;
    Function->Right = Beginning;
    Beginning->Parent = Function;


    return Function;
}

Branch *GetPar()
{
    SkipTrash();

    Branch *Comma = NewBranch(MODE_CONNECTIONS, nullptr, ',');
    Branch *BeginOfParams = Comma;

    char *Param = ScanVar();

    SkipTrash();

    Branch *ParamNode = NewBranch(MODE_VARIABLE, Param, POISON);
    Comma->Right = ParamNode;
    ParamNode->Parent = Comma;

    while(true){
        
        if (*Symbol == ')'){
            Symbol++;
            break;
        }

        Param = ScanVar();

        SkipTrash();

        ParamNode = NewBranch(MODE_VARIABLE, Param, POISON);

        Branch *NextComma  = NewBranch(MODE_CONNECTIONS, nullptr, ',');
        NextComma->Right = ParamNode;
        ParamNode->Parent = NextComma;
        Comma->Left = NextComma;
        NextComma->Parent = Comma;
        Comma = NextComma;
    }

    free(Param);

    return BeginOfParams;
}

Branch *GetWV()
{
    SkipTrash();

    char *Name_Variable = ScanVar();

    Branch *Variable = NewBranch(MODE_VARIABLE, Name_Variable, POISON);

    SkipTrash();
    assert(*Symbol == '=');
    Symbol++;
    Branch *Equal = NewBranch(MODE_SYSTEM_OP, nullptr, '=');

    Branch *Expression = GetE();
    
    Equal->Left = Variable;
    Variable->Parent = Equal;
    Equal->Right = Expression;
    Expression->Parent = Equal;

    return Equal;
}

Branch *GetIf()
{
    SkipTrash();
    Symbol += 37;
    SkipTrash();

    assert(*Symbol == '(');
    Symbol++;

    Branch *LeftExp = GetE();
    SkipTrash();

    int CompareOP = *Symbol;
    Symbol++;
    SkipTrash();

    Branch *RightExp = GetE();
    SkipTrash();

    Branch *Condition = NewBranch(MODE_MATH_OP, nullptr, CompareOP);
    Condition->Left   = LeftExp;
    LeftExp->Parent = Condition;
    Condition->Right  = RightExp;
    RightExp->Parent = Condition;

    assert(*Symbol == ')');
    Symbol++;
    SkipTrash();

    while (*Symbol != ':')
        Symbol++;
    Symbol++;
    SkipTrash();

    Branch *Connection1 = NewBranch(MODE_SYSTEM_OP, nullptr, oper);
    Branch *Beginning1  = Connection1;
    SkipTrash();

    ConnectSubtrees(Connection1);

    SkipTrash();

    assert(*Symbol == '}');
    Symbol++;

    while (*Symbol != ':')
        Symbol++;
    Symbol++;
    SkipTrash();

    Branch *Connection2 = NewBranch(MODE_SYSTEM_OP, nullptr, oper);
    Branch *Beginning2  = Connection2;
    SkipTrash();

    ConnectSubtrees(Connection2);

    SkipTrash();
    assert(*Symbol == '}');
    Symbol++;

    Branch *IF_ELSE = NewBranch(MODE_SYSTEM_OP, nullptr, if_else);
    Branch *main_If = NewBranch(MODE_SYSTEM_OP, nullptr, If);

    main_If->Left     = Condition;
    Condition->Parent = main_If;
    main_If->Right    = IF_ELSE;
    IF_ELSE->Parent   = main_If;

    IF_ELSE->Left      = Beginning1;
    Beginning1->Parent = IF_ELSE;
    IF_ELSE->Right     = Beginning2;
    Beginning2->Parent = IF_ELSE;
    IF_ELSE->Parent    = main_If;

    return main_If;
}

Branch *GetWh()
{
    SkipTrash();
    Symbol += 16;
    SkipTrash();

    assert(*Symbol == '(');
    Symbol++;

    Branch *LeftExp = GetE();
    SkipTrash();

    int CompareOP = *Symbol;
    Symbol++;
    SkipTrash();

    Branch *RightExp = GetE();
    SkipTrash();

    Branch *Condition = NewBranch(MODE_MATH_OP, nullptr, CompareOP);
    Condition->Left   = LeftExp;
    LeftExp->Parent   = Condition;
    Condition->Right  = RightExp;
    RightExp->Parent  = Condition;

    assert(*Symbol == ')');
    Symbol++;
    SkipTrash();

     assert(*Symbol == '{');
    Symbol++;

    Branch *Connection = NewBranch(MODE_SYSTEM_OP, nullptr, oper);
    Branch *Beginning  = Connection;
    SkipTrash();

    ConnectSubtrees(Connection);

    SkipTrash();
    assert(*Symbol == '}');
    Symbol++;

    Branch *main_While = NewBranch(MODE_SYSTEM_OP, nullptr, While);

    main_While->Left   = Condition;
    Condition->Parent  = main_While;
    main_While->Right  = Beginning;
    Beginning->Parent  = main_While;

    return main_While;
}

Branch *GetPr()
{
    SkipTrash();
    Symbol += 22;
    SkipTrash();

    assert(*Symbol == '(');
    Symbol++;

    SkipTrash();

    char *Name_Variable = ScanVar();
    SkipTrash();

    assert(*Symbol == ')');
    Symbol++;

    Branch *Variable = NewBranch(MODE_VARIABLE, Name_Variable, POISON);

    Branch *Print    = NewBranch(MODE_SYSTEM_OP, nullptr, print);

    Print->Left = Variable;
    Variable->Parent = Print;

    return Print;
}

Branch *GetSc()
{
    SkipTrash();
    Symbol += 28;
    SkipTrash();

    assert(*Symbol == '(');
    Symbol++;

    SkipTrash();

    char *Name_Variable = ScanVar();

    assert(*Symbol == ')');
    Symbol++;

    Branch *Variable = NewBranch(MODE_VARIABLE, Name_Variable, POISON);

    Branch *Scan     = NewBranch(MODE_SYSTEM_OP, nullptr, scan);

    Scan->Left = Variable;
    Variable->Parent = Scan;

    return Scan;

}


Branch* GetE()
{
    SkipTrash();

    switch (CheckForSystemOP()){

    case 9: { return GetRet();}

    case 4: { return GetIf();}

    case 7: { return GetPr();}

    case 6: { return GetWh();}

    case 8: { return GetSc();}

    case 0: { return GetCall();}

    default: {break;}
    }

    SkipTrash();

    Branch *Value1 = GetT();
    SkipTrash();
    Branch *Value3 = Value1;
    while (*Symbol == '+' || *Symbol == '-')
    {
        char Operator = *Symbol;
        Symbol++;
        SkipTrash();
        Branch *Value2 = GetT();
        SkipTrash();
        if (Operator == '+')
            Value3 = NewBranch(MODE_MATH_OP, nullptr, '+');
        else
            Value3 = NewBranch(MODE_MATH_OP, nullptr, '-');
        Value3->Left  = Value1;
        Value3->Right = Value2;

        Value1->Parent = Value3;
        Value2->Parent = Value3;
    }

    return Value3;
}

Branch* GetT()
{
    Branch* val1 = GetPow();
    SkipTrash();

    while(*Symbol == '*' || *Symbol == '/') {

        SkipTrash();
        char sign = (*Symbol);
        Symbol++;

        SkipTrash();
        Branch* val2 = GetPow();

        Branch* val3 = (Branch*) calloc(1, sizeof(Branch));
        Data  *Elem  = (Data*)   calloc(1, sizeof(Data));
        val3->Elem = Elem;

        val3->Elem->Type     = MODE_MATH_OP;
        val3->Elem->ElemData = sign;

        Branch* tmp = val1;
        val1 = val3;
        val3 = tmp;

        val1->Right = val2;
        val2->Parent = val1;

        val1->Left = val3;
        val3->Parent = val1;
    }

    return val1;
}

Branch *GetPow()
{
    Branch *Value1 = GetP();
    SkipTrash();
    if (*Symbol == '^'){
        Symbol++;
        Branch *Value2 = NewBranch(MODE_MATH_OP, nullptr, '^');

        Value2->Left   = Value1;
        Value2->Right  = GetE();
        
        Value1->Parent = Value2;
        Value2->Right->Parent = Value2;

        return Value2;
    }

    return Value1;
}

Branch* GetP ()
{
    Branch* Value = nullptr;
    if (*Symbol == '('){

        Symbol++;
        Value = GetE();
        SkipTrash();
        Symbol++;

        return Value;
    }
    
    if (isdigit(*Symbol))
        return GetN();

    Branch* Value2 = GetSM();

    return Value2;
}

Branch* GetN()
{
    int Value = 0;
    while (*Symbol >= '0' && *Symbol <= '9'){
        Value = Value * 10 + *Symbol - '0';
        Symbol++;
    }

    return NewBranch(MODE_NUMBER, nullptr, Value);
    }

Branch* GetSM()
{
    char *Name =  ScanVar();

    Branch *Value  = (Branch*) calloc(1, sizeof(Branch));
    Data *Info     = (Data*)   calloc(1, sizeof(Data));

    Value->Elem    = Info;

    if (CheckForMathOP(Name)){
        Value->Elem->Type = MODE_MATH_FUNC;
        Value->Elem->Name = strdup(Name);
        Branch *Expression = GetE();
        Expression->Parent = Value;
        Value->Left = Expression;
    }

    else {
        Info->Type = MODE_VARIABLE;
        Info->Name = strdup(Name);
    }


    return Value;
}

Branch *GetCall ()
{
    SkipTrash();
    Symbol += 30;
    SkipTrash();

    char *Name_Function = ScanVar();
    SkipTrash();

    assert(*Symbol == '(');
    Symbol++;
    Branch *Param = GetPar();

    Branch *CallFunc = NewBranch(MODE_FUNCTION, Name_Function, POISON);
    CallFunc->Left = Param;
    Param->Parent = CallFunc;

    return CallFunc;
}

Branch *GetRet()
{
    SkipTrash();
    Symbol += 18;
    SkipTrash();

    assert(*Symbol == '(');
    Symbol++;

    Branch *Expression = GetE();
    Branch *Return = NewBranch(MODE_SYSTEM_OP, nullptr, ret);

    if (strchr(Expression->Elem->Name, ')') == nullptr){
        Return->Left = Expression;
        Expression->Parent = Return;
    }
    else
        {
        Symbol--;
        while (*Symbol == '\t' || *Symbol == ' ' || *Symbol == '\n')
        Symbol--;
        }

    assert(*Symbol == ')');
    Symbol++;

    return Return;
}

void SkipTrash()
{
    while (*Symbol == '\t' || *Symbol == ' ' || *Symbol == '\n')
        Symbol++;

    if (*Symbol == '%'){
        while (*Symbol != '\n')
            Symbol++;
        Symbol++;
    }
}

bool CheckForMathOP(char *Name)
{
    SkipTrash();

    #define MATH
    #define OP_MATH(name, length) {\
            if (strncmp(#name, Name, length) == 0)\
                return true;}

    #include "OPes.h"

    #undef MATH

    return false;
}

bool ConnectSubtrees(Branch *Connection)
{
     while(*Symbol != '}'){

        if (CheckForSystemOP() != It_Is_Not_A_Function){
            Branch *SubTree = GetE();
            SkipTrash();
            Connection->Left = SubTree;
            SubTree->Parent  = Connection;
            Branch *ConnectionTemp = NewBranch(MODE_SYSTEM_OP, nullptr, oper);
            Connection->Right = ConnectionTemp;
            ConnectionTemp->Parent = Connection;
            Connection = ConnectionTemp;
        }

        else {
            Branch *SubTree = GetWV();
            SkipTrash();
            Connection->Left = SubTree;
            SubTree->Parent = Connection;
            Branch *ConnectionTemp = NewBranch(MODE_SYSTEM_OP, nullptr, oper);
            Connection->Right = ConnectionTemp;
            ConnectionTemp->Parent = Connection;
            Connection = ConnectionTemp;
        }
    }

    return true;
}

int CheckForSystemOP()
{
    #define SYSTEM
    #define SYSTEM_OP(name, number, length) {\
            SkipTrash();\
            if (strncmp(#name, Symbol, length) == 0){\
                return number;}}

    #include "OPes.h"

    #undef SYSTEM

    return It_Is_Not_A_Function;
}

char *ScanVar ()
{
    char *TempForVariable   = (char*) calloc(SSize, sizeof(char));
    unsigned ScannedLetters = 0;
    sscanf(Symbol, "%s%n", TempForVariable, &ScannedLetters);
    Symbol += ScannedLetters;

    char *Name_Variable = strdup(TempForVariable);
    free(TempForVariable);

    SkipTrash();

    return Name_Variable;
}
