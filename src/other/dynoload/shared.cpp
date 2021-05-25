#include "acctlib.h"

bool range(CTraverser* trav, void* data) {
    cout << "range" << endl;
    return true;
}

bool pre(CTraverser* trav, void* data) {
    cout << "pre" << endl;
    return true;
}

bool post(CTraverser* trav, void* data) {
    cout << "post" << endl;
    return true;
}

bool disp(CTraverser* trav, void* data) {
    cout << "display" << endl;
    return true;
}

bool data(CTraverser* trav, void* data) {
    cout << "data" << endl;
    return true;
}

class CTestTraverser : public CTraverser {
    public:
        CTestTraverser(void) : CTraverser(cout, "testing") {
            filterFunc = range;
            preFunc = pre;
            postFunc = post;
            displayFunc = disp;
            dataFunc = data;
        }
};

extern "C" CTraverser *makeTraverser(void) {
    return new CTestTraverser;
}
