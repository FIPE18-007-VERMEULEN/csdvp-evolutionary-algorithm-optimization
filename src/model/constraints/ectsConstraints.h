#ifndef SRC_MODEL_CONSTRAINTS_ECTS_CONSTRAINTS_H_
#define SRC_MODEL_CONSTRAINTS_ECTS_CONSTRAINTS_H_

#include <vector>
#include <utility>

#include <eo>

#include "model/problem.h"
#include "model/profession.h"
#include "model/ea/cursus.h"

/**
 * This class is used to verify the constraints regarding ECTS 
 */ 
class ConstraintsECTS
{
    private:
        CSDVP _pb; //Description of the pb
        Profession _job;
    
    public:
        ConstraintsECTS(const CSDVP& csdvp, const Profession & p)
            : _pb(csdvp), _job(p) {}
        
        /** integrityCheck is used to investigate whether or not one individu (indiv) respects the constraints stated.
         * Returns a std::pair. First is a boolean set to true when the individu passe the test and is ok with the constraints, false otherwise. Second is a metric inherent to the class caller which can be used during the fitness calcul.
         * Metric here is : currentIndivECTS / expectedECTS
         */

        std::pair<bool, double> integrityCheck(Cursus indiv);
};

#endif // SRC_MODEL_CONSTRAINTS_ECTS_CONSTRAINTS_H_