#include "BinaryTreeNode.hh"

BinaryTreeNode::BinaryTreeNode(double clock_value, int priority) {
    parent = leftBranch = rightBranch = nextValue = NULL;
    llist.clear();
    listLength = 0;
    this->clock_value = clock_value;
    this->priority = priority;
}

BinaryTreeNode::~BinaryTreeNode()  {
    for(list<EventNode*>::iterator it=llist.begin();it!=llist.end(); it++){
        delete (*it);
    }
    listLength = 0;
}
