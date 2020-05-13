#ifndef SRC_MODEL_CONSTRAINTS_REPETITION_CONSTRAINTS_H_
#define SRC_MODEL_CONSTRAINTS_REPETITION_CONSTRAINTS_H_

#include <eo>
#include <utility>

#include "model/problem.h"
#include "model/profession.h"
#include "model/ea/cursus.h"

/** This class is used to verify the integrity of an individu regarding repetition of courses in the sol.
 */ 

class ConstraintsRepetition
{
    private:
        CSDVP _pb;
        Profession _job;
    
    public:
        ConstraintsRepetition(const CSDVP & csdvp, const Profession & job)
            : _pb(csdvp), _job(job) {}

        /** Integrity check is used to investigate wheteher or not one indiv respects the constraints represented by THIS.
         * Returns a std::pair. First is a boolean set to true when the indiv passes the test and therefore is compilant with the constraint, false otherwise. Second is the associated metric, mostly usable during fitness calcul.
         */
        std::pair<bool, double> integrityCheck(Cursus indiv);
};

#endif //SRC_MODEL_CONSTRAINTS_REPETITION_CONSTRAINTS_H_