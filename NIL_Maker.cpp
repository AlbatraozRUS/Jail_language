#include "Language.h"

void NILISE (Branch *Node)
{
    if (Node->Left != nullptr)
        NILISE(Node->Left);

    if (Node->Left == nullptr  && Node->Right == nullptr)
        return;

    if (Node->Left == nullptr && Node->Right != nullptr)
        Node->Left = NewBranch(MODE_NIL, nullptr, '@');

    if (Node->Right != nullptr)
        NILISE(Node->Right);

    if (Node->Right == nullptr && Node->Left != nullptr)
        Node->Right = NewBranch(MODE_NIL, nullptr, '@');

}
