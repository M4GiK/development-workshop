#ifndef __ALLOC_POLICY_FACTORY_H__
#define __ALLOC_POLICY_FACTORY_H__
class AllocPolicyFactory;

#include "AllocPolicy.hh"
#include "UtilsPackage.hh"
#include <vector>
#include <string>
using namespace std;

class AllocPolicyFactory {
    public:
        AllocPolicyFactory ();
        virtual ~AllocPolicyFactory();
        virtual AllocPolicy*  CreateAllocPolicy (string st);
};
#endif
