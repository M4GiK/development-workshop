#include "EventHandler.hh"

EventHandler::EventHandler(): BinaryTree() {
    simTime = 0;
}

EventHandler::~EventHandler() {
}

void EventHandler::reset() {
    deleteNode(root);
    root = first = last = NULL;
}

EventNode* EventHandler::nextEvent() {
    EventNode* node;

    node = popFront();
    if (node) {
        if (node->time < simTime) {
            error ("EventHandler.cc : Back to the past!!");
        }
        simTime = node->time;
    }
    return node;
}

void EventHandler::scheduleEvent(EventNode* newEntry) {
    pushEvent(newEntry);
}

int EventHandler::deleteEvent (int eventType, vector<double> attrib,
                               bool allFlag) {
    BinaryTreeNode* btnode;
    list<EventNode*>::iterator it;
    int count, i;

    count = 0;
    btnode = first;
    while (btnode) {
        for (i=0, it=btnode->llist.begin(); it!=btnode->llist.end() &&
             i < btnode->listLength; ) {
            if ((*it)->eventType == eventType && (attrib.size() == 0 ||
                (*it)->attrib == attrib)) {
                delete (*it);
                it = btnode->llist.erase(it);
                i--;
                btnode->listLength--;
                count++;
                if (!allFlag) {
                    return count;
                }
            } else {
                it++;
            }
            i++;
        }
        btnode = btnode->nextValue;
    }
    return count;
}

int EventHandler::deleteEvent(int eventType, bool allFlag) {
    BinaryTreeNode* btnode;
    list<EventNode*>::iterator it;
    int count, i;

    count = 0;
    btnode = first;
    while (btnode) {
        for(i = 0, it=btnode->llist.begin(); it!=btnode->llist.end() &&
            i < btnode->listLength; ) {
            if ((*it)->eventType == eventType) {
                delete (*it);
                it = btnode->llist.erase(it);
                i--;
                btnode->listLength--;
                count++;
                if (!allFlag) {
                    return count;
                }
            } else {
                it++;
            }
            i++;
        }
        btnode = btnode->nextValue;
    }
    return count;
}

double EventHandler::getSimTime() {
    return simTime;
}
