#include <queen.h>
#include <eoEvalFunc.h>

class queenEval : public eoEvalFunc<QUEEN>
{
public:
    void operator()(QUEEN & _queen);
};
