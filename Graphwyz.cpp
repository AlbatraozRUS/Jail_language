#include "Language.h"

void print_mode(Branch* Node, FILE* dot_out) {
    assert(Node);
    assert(dot_out);

    switch (Node->Elem->Type) {
        case MODE_MATH_FUNC:     fprintf(dot_out, "[label = \"%s\", shape = \"box\", "
                                           "color=\"#ffe4c4\", style = \"filled\", "
                                           "fillcolor = \"#ffe4c4\"]", Node->Elem->Name);
                          break;

        case MODE_VARIABLE:    fprintf(dot_out, "[label = \"%s\", shape = \"egg\", "
                                           "color=\"#556b2f\", style = \"filled\", "
                                           "fillcolor = \"#556b2f\"]", Node->Elem->Name);
                          break;

        case MODE_NUMBER: fprintf(dot_out, "[label = \"%d\", shape = \"egg\", "
                                           "color=\"#8fbc8f\", style = \"filled\" ,"
                                           "fillcolor = \"#8fbc8f\"]", Node->Elem->ElemData);
                          break;

        case MODE_SYSTEM_OP:{

                          switch (Node->Elem->ElemData){

                          case ret: {fprintf(dot_out,  "[label = \"return\", shape = \"septagon\", color=\"#ffffff\", "
                                                       "style=\"filled\", fillcolor = \"#F0E68C\"]");
                          break;}

                          case While: {fprintf(dot_out,  "[label = \"while\", shape = \"pentagon\", color=\"#ffffff\", "
                                                         "style=\"filled\", fillcolor = \"#F0E68C\"]");
                          break;}

                          case If: {fprintf(dot_out,  "[label = \"if\", shape = \"hexagon\", color=\"#ffffff\", "
                                                      "style=\"filled\", fillcolor = \"#F0E68C\"]");
                          break;}

                          case if_else: {fprintf(dot_out,  "[label = \"IF-ELSE\", shape = \"hexagon\", color=\"#ffffff\","
                                                           " style=\"filled\", fillcolor = \"#F0E68C\"]");
                          break;}

                          case print: {fprintf(dot_out, "[label = \"put\", shape = \"circle\", color=\"#ffffff\", "
                                                        "style=\"filled\", fillcolor = \"#F0E68C\"]");
                          break;}

                          case scan: {fprintf(dot_out,  "[label = \"get\", shape = \"circle\", color=\"#ffffff\","
                                                        " style=\"filled\", fillcolor = \"#F0E68C\"]");
                          break;}

                          case oper: {fprintf(dot_out,  "[label = \"OP\", shape = \"circle\", color=\"#ffffff\", "
                                                        "style=\"filled\", fillcolor = \"#F0E68C\"]");
                          break;}


                          case '=': {fprintf(dot_out, "[label = \"=\", shape = \"parallelogram\","
                                           "color=\"#D3D3D3\", style = \"filled\","
                                            "fillcolor = \"#D3D3D3\"]");
                           break;}

                          }
                          break;
        }

        case MODE_SYSTEM_OP_BACK: {fprintf(dot_out, "[label = \"%s\", shape = \"circle\", color=\"#ffffff\", "
                                                        "style=\"filled\", fillcolor = \"#F0E68C\"]", Node->Elem->Name);
                          break;}

        case MODE_FUNCTION: {fprintf(dot_out, "[label = \"$%s\", shape = \"box\", color=\"#ffe4c4\","
                                              " style=\"filled\", fillcolor = \"#32CD32\"]", Node->Elem->Name);
                          break;}

        case MODE_MATH_OP: {

            switch(Node->Elem->ElemData){

                case '+': {fprintf(dot_out, "[label = \"+\", shape = \"diamond\", "
                                           "color=\"#FFD700\", style=\"filled\","
                                            "fillcolor = \"#F0E68C\"]");
                          break;}

                case '-': {fprintf(dot_out, "[label = \"-\", shape = \"diamond\","
                                            " color=\"#da70d6\", style=\"filled\","
                                            "fillcolor = \"#F0E68C\"]");
                          break;}

                case '*': {fprintf(dot_out, "[label = \"*\", shape = \"diamond\", "
                                           "color=\"#FFC0CB\", style=\"filled\","
                                            "fillcolor = \"#FFC0CB\"]");
                          break;}

                case '/': {fprintf(dot_out, "[label = \"/\", shape = \"diamond\","
                                           "color=\"#ffdab9\", style = \"filled\","
                                           "fillcolor = \"#FFDAB9\"]");
                          break;}

                case '^': {fprintf(dot_out, "[label = \"^\", shape = \"diamond\","
                                           "color=\"#FFFFF0\", style = \"filled\","
                                            "fillcolor = \"#FFFFF0\"]");
                          break;}

                case '>': {fprintf(dot_out, "[label = \">\", shape = \"parallelogram\","
                                           "color=\"#D3D3D3\", style = \"filled\","
                                            "fillcolor = \"#D3D3D3\"]");
                           break;}

                case '<': {fprintf(dot_out, "[label = \"<\", shape = \"parallelogram\","
                                           "color=\"#D3D3D3\", style = \"filled\","
                                            "fillcolor = \"#D3D3D3\"]");
                           break;}

                case '=': {fprintf(dot_out, "[label = \"==\", shape = \"parallelogram\","
                                           "color=\"#D3D3D3\", style = \"filled\","
                                            "fillcolor = \"#D3D3D3\"]");
                            break;}

                case '!': {fprintf(dot_out, "[label = \"!=\", shape = \"parallelogram\","
                                           "color=\"#D3D3D3\", style = \"filled\","
                                            "fillcolor = \"#D3D3D3\"]");
                            break;}
            }
                          break;}
                          
        case MODE_CONNECTIONS: {fprintf(dot_out, "[label = \"%c\", shape = \"circle\", color=\"#ffffff\","
                                                 " style=\"filled\", fillcolor = \"#F0E68C\"]", Node->Elem->ElemData);
                          break;}

        case MODE_NIL: {fprintf(dot_out, "[label = \"NIL\", shape = \"trapezium\", color=\"#A0522D\","
                                                 " style=\"filled\", fillcolor = \"#A0522D\"]");
                          break;}

        default:          fprintf(dot_out, "\"error\"");
                          break;
    }                                                                                          

    assert(Node);
    assert(dot_out);
}


int temp = 1;
void create_tree(Branch* Node, FILE* dot_out) {
    assert(dot_out);
    assert(Node);

    fprintf(dot_out, "\t\tLOLKEK%d", temp);
    print_mode(Node, dot_out);
    fprintf(dot_out, ";\n");

    int counter = temp;

    if (Node->Left != nullptr) {
        fprintf(dot_out, "\t\tLOLKEK%d", counter);

        fprintf(dot_out, "->");

        fprintf(dot_out, "LOLKEK%d;\n", ++temp);

        create_tree(Node->Left, dot_out);
    }

    if (Node->Right != nullptr) {
        fprintf(dot_out, "\t\tLOLKEK%d", counter);

        fprintf(dot_out, "->");

        fprintf(dot_out, "LOLKEK%d;\n", ++temp);

        create_tree(Node->Right, dot_out);
    }

    assert(Node);
    assert(dot_out);
}

void tree_dot(Branch* Root)
{

    FILE *file_dot = fopen(DOT, "wb");
    assert(file_dot);

    fprintf(file_dot, "digraph {\n");
    fprintf(file_dot, "\tnode[color = \"#000000\", fontsize = 20];\n"
                      "\tedge[color = \"#000000\", fontsize = 20];\n");

    create_tree(Root, file_dot);

    fprintf(file_dot, "}");

    assert(file_dot);
    fclose(file_dot);

	system("dot -Tjpg -o /Users/albatraozrus/Desktop/Tree.jpg /Users/albatraozrus/Desktop/DOT.dot");
}
