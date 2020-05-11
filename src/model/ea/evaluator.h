#ifndef SRC_MODEL_EA_EVALUATOR_H_
#define SRC_MODEL_EA_EVALUATOR_H_

#include <eoEvalFunc.h>

#include "cursus.h"

class CursusEval : public eoEvalFunc<Cursus>
{
public:
    void operator()(Cursus & _cursus);
};

#endif // SRC_MODEL_EA_EVALUATOR_H_