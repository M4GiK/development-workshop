#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__
class BinaryTree;

#include "BinaryTreeNode.hh"
#include "EventNode.hh"

#define DES_MAX_LIST   4

class BinaryTree {
    public:
        BinaryTree();
        virtual ~BinaryTree();

        void pushEvent(EventNode* node);
        EventNode* popFront();

    protected:
        BinaryTreeNode* prepare(BinaryTreeNode* btnode, EventNode* node,
                                bool divData = true);
        BinaryTreeNode* insert (double clock_value, int priority = 0);
        BinaryTreeNode* lookup (double clock_value, int priority = 0);

        void deleteNode (BinaryTreeNode* node);
        BinaryTreeNode* removeFirstNode();

        BinaryTreeNode* root;
        BinaryTreeNode* first;
        BinaryTreeNode* last;
        int nnode;
};
#endif
