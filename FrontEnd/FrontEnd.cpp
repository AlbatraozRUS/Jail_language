#include "Language.h"


void FrontEnd()
{
    Branch *Root = CreateBranch();

    char *buffer = Reading();

    Root = GetG(buffer,Root);

    tree_dot(Root);

    NILISE(Root);

    Backend(Root);

    FILE * OutputFile = fopen (Output, "wb");

    Writting(Root, OutputFile);

    fclose(OutputFile);

    DestBranch(Root);
}
