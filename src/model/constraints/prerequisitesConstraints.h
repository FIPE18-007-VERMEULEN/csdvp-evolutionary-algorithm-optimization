#ifndef SRC_MODEL_CONSTRAINTS_PREREQUISITES_CONSTRAINTS_H_
#define SRC_MODEL_CONSTRAINTS_PREREQUISITES_CONSTRAINTS_H_

#include <vector>
#include <utility>

#include <eo>

#include "model/problem.h"
#include "model/profession.h"
#include "model/ea/cursus.h"

/**
 * This class is used to verify constraints regarding the prerequisites of each courses of an individu (indiv).
 * Each prerequisites of each courses must be respected. Note that the position of the courses in the indiv is important,
 * since it represents how the courses are dispatched in the timeframes.
 * Logically, a course see in TF t DO NOT CONTRIBUTE in the prerequisite for the courses in x < t
 */

class ConstraintsPrerequisites
{
    private:
        CSDVP _pb;
        Profession _job;

        /**
         * Checks the prerequisites prereqs in the competencies cInTF (generally, the competencies from the previous TF).
         * It returns a std::pair, where the first element indicates how many prerequisites HAS NOT BEEN FOUND.
         * The second elements indicates how many prerequisites has not enought mastery (BUT EXISTS in cInTF!)
         */ 
        std::pair<int,int> _prereqsInPreviousTF(std::vector<Competency> cInTF, std::vector<Competency> prereqs);
    public:
        ConstraintsPrerequisites(const CSDVP & csdvp, const Profession & job)
            : _pb(csdvp), _job(job) {}
        
        /** Integrity check is used to investigate wheteher or not one indiv respects the constraints represented by THIS.
         * Returns a std::pair. First is a boolean set to true when the indiv passes the test and therefore is compilant with the constraint, false otherwise. Second is the associated metric, mostly usable during fitness calcul.
         * @todo Decay competency magnitude
         */
        std::pair<bool, double> integrityCheck(Cursus indiv); 
};

#endif // SRC_MODEL_CONSTRAINTS_PREREQUISITES_CONSTRAINTS_H_