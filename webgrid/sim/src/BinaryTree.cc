#include "BinaryTree.hh"

BinaryTree::BinaryTree() {
    root = NULL;
    first = last = NULL;
    nnode = 0;
    insert(-1,0);
}

BinaryTree::~BinaryTree() {
    deleteNode(root);
}

void BinaryTree::pushEvent (EventNode* node) {
    BinaryTreeNode* cur;
    list<EventNode*>::iterator it;
    double time;
    int priority;

    time = node->time;
    priority = node->priority;

    cur = lookup(time, priority);
    cur = prepare(cur, node);

    for(it=cur->llist.begin(); it!=(cur->llist.end()); it++) {
        double ltime = 0.0;
        int lprio = 0;
        ltime = (*it)->time;
        lprio = (*it)->priority;
        if (time < ltime || (time == ltime && priority <= lprio)) {
            break;
        }
    }
    cur->llist.insert(it, node);
    cur->listLength++;
    nnode++;
}

EventNode* BinaryTree::popFront() {
    EventNode* del;

    if (!first) {
        return NULL;
    }
    if (!first->listLength) {
        if (removeFirstNode() == NULL) {
            return NULL;
        }
        return popFront();
    }

    del = first->llist.front();
    first->llist.pop_front();
    first->listLength--;
    nnode--;

    return del;
}

BinaryTreeNode* BinaryTree::prepare ( BinaryTreeNode* btnode,
                                      EventNode* node, bool divData){
    BinaryTreeNode* newBinaryNode;
    list<EventNode*>::iterator it;
    double time;
    int priority;

    time = 0.0;
    priority = 0;
    time = node->time;
    priority = node->priority;

    if (btnode == NULL) {
        btnode = insert(time,priority);
    }
    if (btnode->listLength == DES_MAX_LIST) {
        if (divData) {
            it = btnode->llist.begin();
            for(int i=0; i < DES_MAX_LIST/2; i++) {
                it++;
            }

            EventNode* fnode = (*it);
            double ntime = fnode->time;
            int npriority = fnode->priority;
            newBinaryNode = insert(ntime, npriority);

            BinaryTreeNode* parent = newBinaryNode->parent;
            double ptime = (parent->llist.back())->time;
            int ppriority = (parent->llist.back())->priority;

           if (parent!=btnode && parent && parent->clock_value == ntime &&
                (ptime>ntime || (ptime==ntime && ppriority > npriority))) {

                newBinaryNode->llist.splice ( newBinaryNode->llist.begin(),
                                              parent->llist,
                                              parent->llist.begin(),
                                              parent->llist.end() );
                parent->llist.erase ( parent->llist.begin(),
                                      parent->llist.end() );
                parent->llist.splice ( parent->llist.begin(),btnode->llist,
                                       it,btnode->llist.end() );
                btnode->llist.erase ( it, btnode->llist.end() );
                parent->listLength = parent->llist.size();
            } else {
                newBinaryNode->llist.splice(newBinaryNode->llist.begin(),
                                            btnode->llist, it,
                                            btnode->llist.end() );
            }
        } else {
            newBinaryNode = insert(time,priority);
        }
        newBinaryNode->listLength = newBinaryNode->llist.size();
        btnode->listLength = btnode->llist.size();

        if (time >= newBinaryNode->clock_value) {
            btnode = newBinaryNode;
        }
    }

    return btnode;
}

BinaryTreeNode* BinaryTree::insert (double clock_value, int priority) {
    BinaryTreeNode* newNode;
    BinaryTreeNode* splitNode;
    BinaryTreeNode* cur;
    int leftCount, rightCount;

    if (!root) {
        newNode = new BinaryTreeNode(clock_value);
        root = newNode;
        first = last = root;
        return newNode;
    }

    splitNode = NULL;
    cur = root;
    leftCount = rightCount = 0;
    while (cur) {
        if (clock_value == cur->clock_value &&
                (!cur->nextValue?
                    1:
                    clock_value<((cur->nextValue)->clock_value)) &&
                priority >= cur->priority &&
                (!cur->nextValue?
                 1:
                 priority<(cur->nextValue)->priority) && !splitNode) {
            splitNode = cur;
        } else if (clock_value >= cur->clock_value &&
                (!cur->nextValue?
                 1:
                 clock_value<(cur->nextValue)->clock_value) && !splitNode){
            splitNode = cur;
        }
        if (clock_value < cur->clock_value) {
            leftCount++;
            cur = cur->leftBranch;
        } else {
            rightCount++;
            cur = cur->rightBranch;
        }
    }
    if (splitNode == NULL) {
        splitNode = root;
    }

    newNode = new BinaryTreeNode(clock_value);

    if (leftCount > rightCount || !splitNode->nextValue) {
        newNode->rightBranch = splitNode->rightBranch;
        if (splitNode->rightBranch) {
            splitNode->rightBranch->parent = newNode;
        }
        splitNode->rightBranch = newNode;
        newNode->parent = splitNode;
        if (splitNode == last) {
            last = newNode;
        }
    } else {
        newNode->leftBranch = (splitNode->nextValue)->leftBranch;
        (splitNode->nextValue)->leftBranch = newNode;
        newNode->parent = splitNode->nextValue;
        if (newNode->leftBranch) {
            newNode->leftBranch->parent = newNode;
        }
        if (splitNode->nextValue == first) {
            first = newNode;
        }
    }
    newNode->nextValue = splitNode->nextValue;
    splitNode->nextValue = newNode;

    return newNode;
}

BinaryTreeNode* BinaryTree::lookup(double clock_value,int priority) {

    BinaryTreeNode* cur = root;

    while (cur) {
        if (clock_value == cur->clock_value &&
                (!cur->nextValue?
                    1:
                    clock_value<((cur->nextValue)->clock_value)) &&
                priority >= cur->priority &&
                (!cur->nextValue?
                 1:
                 priority<(cur->nextValue)->priority)) {
            return cur;
        } else if (clock_value >= cur->clock_value &&
                (!cur->nextValue?
                 1:
                 clock_value<(cur->nextValue)->clock_value)) {
            return cur;
        } else if (cur->clock_value > clock_value && !cur->leftBranch) {
            return cur;
        } else if (clock_value < cur->clock_value) {
            cur = cur->leftBranch;
        } else {
            cur = cur->rightBranch;
        }
    }
    return NULL;
}

BinaryTreeNode* BinaryTree::removeFirstNode() {

    BinaryTreeNode* del;

    del = first;
    if (first->parent) {
        first->parent->leftBranch = first->rightBranch;
        if (first->rightBranch) {
            first->rightBranch->parent = first->parent;
            first = first->rightBranch;
        } else {
            first = first->parent;
        }
    } else {
        first = root = first->rightBranch;
        if (root) {
            root->parent = NULL;
        }
    }
    if (first) {
        while(first->leftBranch) {
            first = first->leftBranch;
        }
    }

    nnode -= del->listLength;
    delete del;
    return first;
}

void BinaryTree::deleteNode (BinaryTreeNode* node) {
    if (!node) {
        return;
    }
    if (node->leftBranch) {
        deleteNode(node->leftBranch);
    }
    if (node->rightBranch) {
        deleteNode(node->rightBranch);
    }
    delete node;
}
