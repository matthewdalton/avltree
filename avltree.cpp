#include <assert.h>
#include <queue>
#include <iostream>

#include "avltree.h"

using namespace std;

/*
 * Case 1: A node balance factor changes from 0 -> +1 / -1. No changes required.
 * Case 2: A node bf changes from -1 to 0, or +1 to 0. No changes required.
 * Case 3: A node bf changes from -1 to -2, or +1 to +2. Changes required:
 *    subcase A: (P goes from -1 to -2)
 *               P's left subnode bf is -1.
 *               => Rotate P right
 *               (P goes from +1 to +2)
 *               P's right subnode bf is +1
 *               => Rotate P left
 *    subcase B: P is -2 or +2, and the subnode is +1 or -1 (ie. opposite sign)
 *               double rotation required
 */

void AVLTree::insertInto(AVLNode *tree, AVLNode *item, bool &fixBalance)
{
    if (item->value() < tree->value())
    {
        if (tree->left() == NULL)
        {
            tree->setLeft(item);
            item->setParent(tree);
            fixBalance = true;
        }
        else
        {
            insertInto(tree->left(), item, fixBalance);
        }

        assert(tree->balanceFactor() >= -1 && tree->balanceFactor() <= 1);
        if (fixBalance)
        {
            switch (tree->balanceFactor())
            {
            case -1: // will be -2
                // Fix left tree
                tree->dec();
                fixLeftTree(tree, fixBalance);
                break;
            case 0: // will be -1
                tree->dec();
                fixBalance = true;
                break;
            case 1: // will be 0
                tree->dec();
                fixBalance = false;
                break;
            }
        }
    }
    else
    {
        if (tree->right() == NULL)
        {
            tree->setRight(item);
            item->setParent(tree);
            fixBalance = true;
        }
        else
        {
            insertInto(tree->right(), item, fixBalance);
        }

        assert(tree->balanceFactor() >= -1 && tree->balanceFactor() <= 1);
        if (fixBalance)
        {
            switch (tree->balanceFactor())
            {
            case 1: // will be 2
                tree->inc();
                fixRightTree(tree, fixBalance);
                break;
            case 0: // will be 1
                tree->inc();
                fixBalance = true;
                break;
            case -1: // will be 0;
                tree->inc();
                fixBalance = false;
            }
        }
    }
}

/*
 *        oA                   oC
 *       / \                  / \
 *      oB  oC               oA  oE
 *         / \      =>      / \   \
 *        oD  oE           oB  oD  oF
 *             \
 *              oF
 *
 * Balance factor changes:
 *   A: +2 -> 0  (-1 if D is NULL)
 *   C: +1 -> 0
 *
 *        oA                   oC
 *         \                  / \
 *          oC      =>       oA  oE
 *           \
 *            oE
 *
 * Balance factor changes:
 *   A: +2 -> 0
 *   C: +1 -> 0
 */
void AVLTree::leftRotation(AVLNode *C)
{
    AVLNode *A = C->parent();
    if (A->parent() != NULL)
    {
        if (A->parent()->left() == A)
        {
            A->parent()->setLeft(C);
        }
        else
        {
            A->parent()->setRight(C);
        }
    }

    AVLNode *D = C->left();
    C->setLeft(A);
    A->setRight(D);
    if (D != NULL)
        D->setParent(A);
    C->setParent(A->parent());
    A->setParent(C);

    assert(A->balanceFactor() == 2);
    // A->setBalanceFactor( ((D != NULL) ? 1 : 0) +
    //                   ((A->left() != NULL) ? -1 : 0) );
    A->setBalanceFactor(0);
    assert(C->balanceFactor() == 1);
    C->setBalanceFactor(0);
}

/*
 *            oA               oB
 *           / \              / \
 *          oB  oC           oD  oA
 *         / \       =>     /   / \
 *        oD  oE           oF  oE  oC
 *       /
 *      oF
 *
 * Balance factor changes:
 *   A: -2 -> 0  (+1 if E is NULL)
 *   B: -1 -> 0
 *
 *            oA             oB
 *           /              / \
 *          oB       =>    oD  oA
 *         /
 *        oD
 *
 * Balance factor changes:
 *   A: -2 -> 0
 *   B: -1 -> 0
 */
void AVLTree::rightRotation(AVLNode *B)
{
    AVLNode *A = B->parent();
    if (A->parent() != NULL)
    {
        if (A->parent()->left() == A)
        {
            A->parent()->setLeft(B);
        }
        else
        {
            A->parent()->setRight(B);
        }
    }

    AVLNode *E = B->right();
    B->setRight(A);
    A->setLeft(E);
    if (E != NULL)
        E->setParent(A);
    B->setParent(A->parent());
    A->setParent(B);

    assert(A->balanceFactor() == -2);
    // A->setBalanceFactor( ((E != NULL) ? -1 : 0) +
    //                   ((A->right() != NULL) ? 1 : 0) );
    A->setBalanceFactor(0);
    assert(B->balanceFactor() == -1);
    B->setBalanceFactor(0);
}

/*
 *          oA                oD
 *         / \               / \
 *        oB  oC            oA  oC
 *           / \    =>     / \   \
 *          oD  oE        oB  oF  oE
 *         /
 *        oF
 *
 * Balance factor changes:
 *   A: +2 ->  0
 *   C: -1 -> +1  (0 if E is NULL)
 *   D: -1 ->  0
 *
 *          oA                oD
 *           \               / \
 *            oC    =>      oA  oC
 *           /
 *          oD
 *
 * Balance factor changes:
 *   A: +2 -> 0
 *   C: -1 -> 0
 *   D:  0 -> 0
 *
 *            oA                        oD
 *         /     \                   /      \
 *        oB      oC                oA       oC
 *         \     / \               / \      / \
 *          o   oD  oE   =>       oB  oF   oX  oE
 *             / \   \             \      /     \
 *            oF  oX  o             o    oY      o
 *               /
 *              oY
 *                                              .
 * Balance factor changes:
 *   A: +2 -> -1
 *   C: -1 -> 0
 *   D: +1 -> 0
 */
void AVLTree::doubleLeftRotation(AVLNode *C)
{
    AVLNode *A = C->parent();
    AVLNode *D = C->left();

    if (A->parent() != NULL)
    {
        if (A->parent()->left() == A)
        {
            A->parent()->setLeft(D);
        }
        else
        {
            A->parent()->setRight(D);
        }
    }

    AVLNode *F = D->left();
    D->setLeft(A);
    D->setParent(A->parent());
    A->setParent(D);
    A->setRight(F);
    if (F != NULL)
        F->setParent(A);
    if (D->right() != NULL)
        D->right()->setParent(C);
    C->setLeft(D->right());
    D->setRight(C);
    C->setParent(D);

    assert(A->balanceFactor() == 2);
    assert(C->balanceFactor() == -1);

    if (D->balanceFactor() == -1)
    {
        A->setBalanceFactor(0);
        C->setBalanceFactor(1);
    }
    else if (D->balanceFactor() == 1)
    {
        A->setBalanceFactor(-1);
        C->setBalanceFactor(0);
    }
    else
    {
        A->setBalanceFactor(0);
        C->setBalanceFactor(0);
    }
    D->setBalanceFactor(0);
}

/*
 *            oA              oE
 *           / \             / \
 *          oB  oC          oB  oA
 *         / \      =>     /   / \
 *        oD  oE          oD  oF  oC
 *             \
 *              oF
 *
 * Balance factor changes:
 *   A: -2 ->  0
 *   B: +1 -> -1  (0 if D is NULL)
 *   E: +1 ->  0
 *
 *            oA              oE
 *           /               / \
 *          oB      =>      oB  oA
 *           \
 *            oE
 *
 * Balance factor changes:
 *   A: -2 -> 0
 *   B: +1 -> 0
 *   E:  0 -> 0
 *
 *              oA                     oE
 *           /     \                 /    \
 *         oB         oC           oB       oA
 *       /   \       / \          / \      / \
 *      oD     oE   o   o   =>   oD  oX  oF   oC
 *     / \    / \               / \     /    / \
 *    o   o  oX  oF            o   o   o    o   o
 *              /
 *             o
 *
 * Balance factor changes:
 *  A: -2 -> 0
 *  B: +1 -> depends
 *  E: ?  -> 0
 */
void AVLTree::doubleRightRotation(AVLNode *B)
{
    AVLNode *A = B->parent();
    AVLNode *E = B->right();

    if (A->parent() != NULL)
    {
        if (A->parent()->left() == A)
        {
            A->parent()->setLeft(E);
        }
        else
        {
            A->parent()->setRight(E);
        }
    }

    AVLNode *F = E->right();
    E->setRight(A);
    E->setParent(A->parent());
    A->setParent(E);
    A->setLeft(F);
    if (F != NULL)
        F->setParent(A);
    if (E->left() != NULL)
        E->left()->setParent(B);
    B->setRight(E->left());
    E->setLeft(B);
    B->setParent(E);

    assert(A->balanceFactor() == -2);
    assert(B->balanceFactor() == 1);

    if (E->balanceFactor() == 1)
    {
        A->setBalanceFactor(0);
        B->setBalanceFactor(-1);
    }
    else if (E->balanceFactor() == -1)
    {
        A->setBalanceFactor(1);
        B->setBalanceFactor(0);
    }
    else
    {
        A->setBalanceFactor(0);
        B->setBalanceFactor(0);
    }
    E->setBalanceFactor(0);
}

void AVLTree::fixLeftTree(AVLNode *subparent, bool &fixBalance)
{
    assert(subparent->balanceFactor() == -2);

    if (subparent->left()->balanceFactor() == -1)
    {
        rightRotation(subparent->left());
        fixBalance = false;
    }
    else if (subparent->left()->balanceFactor() == 1)
    {
        doubleRightRotation(subparent->left());
        fixBalance = false;
    }
    if (subparent->parent()->parent() == NULL)
        m_root = subparent->parent();
}

void AVLTree::fixRightTree(AVLNode *subparent, bool &fixBalance)
{
    assert(subparent->balanceFactor() == 2);

    if (subparent->right()->balanceFactor() == 1)
    {
        leftRotation(subparent->right());
        fixBalance = false;
    }
    else if (subparent->right()->balanceFactor() == -1)
    {
        doubleLeftRotation(subparent->right());
        fixBalance = false;
    }
    if (subparent->parent()->parent() == NULL)
        m_root = subparent->parent();
}

void AVLTree::insert(int val)
{
    AVLNode *n = new AVLNode(val);

    if (m_root == NULL)
    {
        m_root = n;
        return;
    }

    bool fixBalance = false;
    /* Insert into binary tree */
    insertInto(m_root, n, fixBalance);
}

void AVLTree::dumpTree() const
{
    dumpTreeDFS(m_root);
}

void AVLTree::dumpTreeBFS() const
{
    /* do BFS */

    std::queue<AVLNode *> q;
    AVLNode *n;

    if (m_root == NULL)
    {
        cout << "Tree is empty" << endl;
        return;
    }

    q.push(m_root);
    while (q.size() > 0)
    {
        n = q.front();

        cout << "Processing node with value " << n->value() << endl;

        q.push(n->left());
        q.push(n->right());

        q.pop();
    }
}

void AVLTree::dumpTreeDFS(AVLNode *n, int level) const
{
    if (n == NULL)
        return;

    // pre-order traversal
    // L: level, P: parent, BF: balance factor
    cout << n->value() << "{L:" << level << "} [P:"
         << (n->parent() != NULL ? n->parent()->value() : 0) << "]"
         << " (BF: " << n->balanceFactor() << ")" << endl;
    dumpTreeDFS(n->left(), level + 1);
    dumpTreeDFS(n->right(), level + 1);
}

void AVLTree::remove(AVLNode *node)
{
}

AVLNode *AVLTree::min() const
{
    return min(m_root);
}

AVLNode *AVLTree::min(AVLNode *subtree) const
{
    if (subtree == NULL)
        return NULL;

    while (subtree->left() != NULL)
        subtree = subtree->left();

    return subtree;
}

AVLNode *AVLTree::max() const
{
    return max(m_root);
}

AVLNode *AVLTree::max(AVLNode *subtree) const
{
    if (subtree == NULL)
        return NULL;

    while (subtree->right() != NULL)
        subtree = subtree->right();

    return subtree;
}

// Returns NULL if no successor found
AVLNode *AVLTree::next(AVLNode *node) const
{
    if (node == NULL)
        return NULL;

    if (node->right() != NULL)
    {
        return min(node->right());
    }
    else
    {
        AVLNode *n;
        if (node->parent() == NULL)
            return NULL;

        n = node;
        while (n->parent()->left() != n)
        {
            n = n->parent();
            if (n->parent() == NULL)
                return NULL;
        }
        return n->parent();
    }

    return NULL;
}

AVLNode *AVLTree::prev(AVLNode *node) const
{
    if (node == NULL)
        return NULL;

    if (node->left() != NULL)
    {
        return max(node->left());
    }
    else
    {
        AVLNode *n;
        if (node->parent() == NULL)
            return NULL;

        n = node;
        while (n->parent()->right() != n)
        {
            n = n->parent();
            if (n->parent() == NULL)
                return NULL;
        }
        return n->parent();
    }

    return NULL;
}

static AVLNode *searchSubtree(AVLNode *sub, int val)
{
    if (sub->value() == val)
	return sub;

    if (val < sub->value())
    {
	if (sub->left() != NULL)
	    return searchSubtree(sub->left(), val);
    }
    else
    {
	if (sub->right() != NULL)
	    return searchSubtree(sub->right(), val);
    }
    return NULL;
}

AVLNode *AVLTree::search(int val) const
{
    return searchSubtree(m_root, val);
}
