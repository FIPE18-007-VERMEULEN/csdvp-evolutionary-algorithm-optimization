#ifndef SRC_MODEL_EA_EVALUATOR_H_
#define SRC_MODEL_EA_EVALUATOR_H_

#include <eoEvalFunc.h>

#include "cursus.h"
#include <model/constraints/ectsConstraints.h>
#include <model/constraints/repetitionConstraints.h>
#include <model/constraints/professionConstraints.h>
#include <model/constraints/prerequisitesConstraints.h>

class CursusEval : public eoEvalFunc<Cursus>
{
public:

 CursusEval(ConstraintsPrerequisites& _cpr, ConstraintsRepetition& _cr, ConstraintsProfession& _cp, ConstraintsECTS& _ce):cpr(_cpr), cr(_cr), cp(_cp), ce(_ce){}
  
    void operator()(Cursus & _cursus);


 private:
    ConstraintsPrerequisites cpr;
    ConstraintsRepetition cr;
    ConstraintsProfession cp;
    ConstraintsECTS ce;
};

#endif // SRC_MODEL_EA_EVALUATOR_H_
