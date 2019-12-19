#include "Language.h"

int main()
{
    printf("\nAh shit, here go again...\nI mean welcome to prototype of Blyat_Language\nLet`s begin!\n\n");

    FrontEnd();

    Branch *Root =FrontBack();

    tree_dot(Root->Left);

    printf("Finished successfully\n");

    return 0;
}
