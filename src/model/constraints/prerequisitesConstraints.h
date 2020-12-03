#ifndef SRC_MODEL_CONSTRAINTS_PREREQUISITES_CONSTRAINTS_H_
#define SRC_MODEL_CONSTRAINTS_PREREQUISITES_CONSTRAINTS_H_

#include <vector>
#include <utility>
#include <tuple>

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
         * It returns a std::tuple, where the first element indicates how many prerequisites HAS NOT BEEN FOUND.
         * The second elements indicates how many prerequisites has not enought mastery (BUT EXISTS in cInTF!)
         * The third element is the continous metric calculated accordingly the difference of mag between cours prereqs and comp mag
         * The fourth element is to be used to divide the third one (how many elements contributed to establish the mag diff) 
         */ 
        std::tuple<int,int, double, int> _prereqsInPreviousTF(std::vector<Competency> cInTF, std::vector<Competency> prereqs);
    public:
        static int DISCRETE_METRIC;
        static int INTEGRITY_CHECK;
        static int OVERFLOW_PERCENT;

        ConstraintsPrerequisites(const CSDVP & csdvp, const Profession & job)
            : _pb(csdvp), _job(job) {}
        
        /** Integrity check is used to investigate wheteher or not one indiv respects the constraints represented by THIS.
         * Returns a std::pair. First is a boolean set to true when the indiv passes the test and therefore is compilant with the constraint, false otherwise. Second is the associated metric, mostly usable during fitness calcul.
         * @deprecated
         * @todo Decay competency magnitude
         */
        std::pair<bool, double> old_integrityCheck(Cursus indiv);

        /**
         * 2nd version of integrity check. Supposed to be more reliable and faster
         */
        std::pair<bool, double> integrityCheck(Cursus indiv);

        /**
         * Displayer debug purpose. Display decayed array and non decayed array
         * This is mostly a duplicata of integrityCheck without the prerequisite check and display
         */ 
        void _displayDecayedArrays(Cursus indiv);
};

#endif // SRC_MODEL_CONSTRAINTS_PREREQUISITES_CONSTRAINTS_H_