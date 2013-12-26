#include <iostream>
#include <cmath>
#include <cstdlib>
#include "avltree.h"

using namespace std;

int getHeight(AVLNode *subtree)
{
    if (subtree == NULL)
	return 0;

    int left_h = getHeight(subtree->left());
    int right_h = getHeight(subtree->right());

    if (subtree->balanceFactor() != right_h - left_h)
    {
	cout << "Incorrect balance factor at node [" << subtree->value()
	     << "] - should be " << (right_h - left_h)
	     << ", is " << subtree->balanceFactor() << "." << endl;
    }

    return 1 + (left_h > right_h ? left_h : right_h);
}

void balanceCheck(AVLTree *tree, int numvalues)
{
    int height = getHeight(tree->root());
    cout << "Tree is height " << height << endl;

    // floor(log (numvalues)) = height - 1
    //          2
    int bestHeight = 1 + floor(log(numvalues) / log(2));
    //if (bestHeight != height)
	cout << "Theoretical best height is " << bestHeight << endl;
}

int main()
{
    AVLTree tree;
    // Does left and double left rotation
    //int values[] = {7, 8, 9, 10, 11, 12};
    // Does right and double right rotation
    //int values[] = {12, 11, 10, 9, 8, 7};

    //int values[] = {80, 30, 100, 20, 50, 90, 120, 10, 40, 60, 55};
    //int values[] = {80, 30, 100, 20, 50, 90, 120, 85, 95, 125, 93};

#if 0
    int values[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130,
                     140, 150, 160, 170, 180, 190, 200, 195, 185, 175, 165,
                     155, 145, 135, 125, 115, 105, 95, 85, 75, 65, 55, 45,
                     35, 25, 15, 5, 270, 560, 430, 230, 510, 700 };
    int numvalues = sizeof(values) / sizeof(int);
#else
    srand(time(NULL));
    //srand(57);
    int numvalues = (rand() % 100001) + 100000;
    int *values = (int *)malloc(sizeof(int) * numvalues);
    int minv = INT_MAX, maxv = -1;
    for (int i = 0; i < numvalues; i++)
    {
        values[i] = (rand() % 500000) + 1;
	if (values[i] > maxv)
	    maxv = values[i];
	if (values[i] < minv)
	    minv = values[i];
    }
#endif

    int i = 0;

    for (i = 0; i < numvalues; i++)
    {
        tree.insert(values[i]);
        //cout << "Inserted " << values[i] << endl;
        //tree.dumpTree();
	//balanceCheck(&tree, i+1);
    }

    cout << "Num values: " << numvalues << endl;
    balanceCheck(&tree, numvalues);
    cout << "Min value is " << tree.min()->value() << ", should be " << minv << endl;
    cout << "Max value is " << tree.max()->value() << ", should be " << maxv << endl;

    return 0;
}
