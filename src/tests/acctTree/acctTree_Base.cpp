/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "acctTree.h"

//-----------------------------------------------------------------------------
acctTree_Node::acctTree_Node(void) : index(0) {
}

//-----------------------------------------------------------------------------
acctTree_Node::~acctTree_Node(void) {
}

//-----------------------------------------------------------------------------
acctTree_Node* acctTree_Node::newBranch(
    const SFString& _k1,
    const SFString& _v1,
    const SFString& _k2,
    const SFString& _v2) {

    unsigned prefix = commonPrefix(_k1, _k2);

    acctTree_Node* ret;
    if (_k1.length() == prefix) {
        if (verbose == 2) cerr << "k1 matches up to " << prefix << endl;
        ret = new acctTree_Branch(_k2[prefix], new acctTree_Leaf(_k2.substr(prefix+1), _v2), _v1);

    } else if (_k2.length() == prefix) {
        if (verbose == 2) cerr << "k2 matches up to " << prefix << endl;
        ret = new acctTree_Branch(_k1[prefix], new acctTree_Leaf(_k1.substr(prefix+1), _v1), _v2);

    } else {
        // both continue after split
        if (verbose == 2) cerr << "both keys continue past prefix " << prefix << endl;
        ret = new acctTree_Branch(_k1[prefix],
                                    new acctTree_Leaf(_k1.substr(prefix+1), _v1),
                                    _k2[prefix],
                                    new acctTree_Leaf(_k2.substr(prefix+1), _v2));
    }

    if (prefix) {
        // have shared prefix - split.
        if (verbose == 2) cerr << "shared prefix " << prefix << endl;
        ret = new acctTree_Infix(_k1.substr(0, prefix), ret);
    }

    return ret;
}
