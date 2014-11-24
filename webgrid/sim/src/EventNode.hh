#ifndef __EVENT_NODE_H__
#define __EVENT_NODE_H__
class EventNode;

/*-----------------------------------------------------------------------*/
//#include "EventListener.hh" /* Breaks the deadlock with EventListener  */
class EventListener;
/* In order to break the deadlock with EventListener, when compiling     */
/* BinaryTree.cc.This is because BinaryTree.hh includes BinaryTreeNode.hh*/
/* ,BinaryTreeNode.hh includes EventNode.hh, EventNode.hh includes       */
/* EventListener.hh , EventListener.hh includes EventHandler.hh and      */
/* EventHandler.hh includes BinaryTree.hh (since it is a subclass).      */
/* This cicle ends with the (#ifndef __BINARY_TREE__ ) instruction, and  */
/* therefore BinaryTree is not a defined type for EventHandler (but it   */
/* needs te be defined to allow EventHandler.hh inherit from).           */
/* Consequently BinaryTree.cc can't be compiled. The solution consists in*/
/* breaking the cicle, as told, in this class, removing the dependence   */
/* with EventListener.hh.  This can be done because this class only has  */
/* pointers to EventListener objects, and because it doesn't access      */
/* EventListener objects methods. However, a forward declaration is      */
/* needed... (C++ *.hh :S :S)                                            */
/*-----------------------------------------------------------------------*/
#include <vector>
using namespace std;

class EventNode {
    public:
        EventNode(EventListener* src, EventListener* dest, double time,
                  int eventType, int priority, vector<double> attrib,
                  void* data = NULL);
        EventNode(EventListener* src, EventListener* dest,
                  double time, int eventType, int priority = 0,
                  void* data = NULL);
        virtual ~EventNode();

        EventListener* src;
        EventListener* dest;
        double time;
        int eventType;
        int priority;
        vector<double> attrib;
        void* data;
};
#endif
