#ifndef _AVLTREE_H_
#define _AVLTREE_H_

class AVLNode
{
public:
    explicit AVLNode(int val, AVLNode *parent=NULL)
	: m_value(val), m_balanceFactor(0),
	  m_left(NULL), m_right(NULL), m_parent(parent) {}

    int value() const
    { return m_value; }

    int balanceFactor() const
    { return m_balanceFactor; }

    void setBalanceFactor(int bf)
    { m_balanceFactor = bf; }

#if 0
    // Side-effects: adjusts the parent node's balance factor to match
    void incBF();
    void decBF();

    // Side-effects. Call these versions during insert.
    void incBFi();
    void decBFi();
#endif

    // No side-effects
    void inc()
    { m_balanceFactor++; }
    void dec()
    { m_balanceFactor--; }

    AVLNode *left() const
    { return m_left; }

    void setLeft(AVLNode *node)
    { m_left = node; }

    AVLNode *right() const
    { return m_right; }

    void setRight(AVLNode *node)
    { m_right = node; }

    AVLNode *parent() const
    { return m_parent; }

    void setParent(AVLNode *node)
    { m_parent = node; }

private:
    int m_value;
    int m_balanceFactor;

    AVLNode *m_left;
    AVLNode *m_right;
    AVLNode *m_parent;
};



class AVLTree
{
public:
    AVLTree()
	: m_root(NULL) {}

    AVLNode *search(int val) const;
    void insert(int val);
    void remove(AVLNode *node);
    AVLNode *min() const;
    AVLNode *max() const;
    AVLNode *next(AVLNode *node) const;
    AVLNode *prev(AVLNode *node) const;

    AVLNode *root() const
    { return m_root; }

    void dumpTree() const;

private:
    void insertInto(AVLNode *tree, AVLNode *item, bool &fixBalance);

    void leftRotation(AVLNode *node);
    void rightRotation(AVLNode *node);
    void doubleLeftRotation(AVLNode *node);
    void doubleRightRotation(AVLNode *node);

    void fixLeftTree(AVLNode *subparent, bool &fixBalance);
    void fixRightTree(AVLNode *subparent, bool &fixBalance);

    AVLNode *min(AVLNode *subtree) const;
    AVLNode *max(AVLNode *subtree) const;

    void dumpTreeDFS(AVLNode *n, int level=0) const;
    void dumpTreeBFS() const;

    AVLNode *m_root;
};


#endif
