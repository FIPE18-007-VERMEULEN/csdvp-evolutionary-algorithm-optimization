#ifndef SRC_MODEL_CONSTRAINTS_PROFESSION_CONSTRAINTS_H_
#define SRC_MODEL_CONSTRAINTS_PROFESSION_CONSTRAINTS_H_

#include <vector>
#include <utility>

#include <eo>

#include "model/problem.h"
#include "model/profession.h"
#include "model/ea/cursus.h"

/**
 * This class is used to verify the constraints regarding the profession expected by the student 
 * It mostly consist in *version 1* to check if all the prerequisites have been answered.
 */

class ConstraintsProfession
{
    private:
        CSDVP _pb;
        Profession _job;

    public:
        ConstraintsProfession(const CSDVP & csdvp, const Profession & job)
            : _pb(csdvp), _job(job) {}
        
        /** Integrity check is used to investigate wheteher or not one indiv respects the constraints represented by THIS.
         * Returns a std::pair. First is a boolean set to true when the indiv passes the test and therefore is compilant with the constraint, false otherwise. Second is the associated metric, mostly usable during fitness calcul.
         * @todo Decay competency magnitude
         */
        std::pair<bool, double> integrityCheck(Cursus indiv);

};

#endif // SRC_MODEL_CONSTRAINTS_PROFESSION_CONSTRAINTS_H_