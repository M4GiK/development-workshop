#ifndef __BINARY_TREE_NODE_H__
#define __BINARY_TREE_NODE_H__
class BinaryTreeNode;

#include "EventNode.hh"
#include <list>
using namespace std;

class BinaryTreeNode {
    friend class BinaryTree;
    friend class EventHandler;
    public:
        BinaryTreeNode(double clock_value, int priority = 0);
        ~BinaryTreeNode();

    protected:
        BinaryTreeNode* parent;
        BinaryTreeNode* leftBranch;
        BinaryTreeNode* rightBranch;
        BinaryTreeNode* nextValue;

        list<EventNode*> llist;
        int listLength;
        double clock_value;
        double priority;
};
#endif
