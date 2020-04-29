#ifndef SRC_PROFESSION_H_
#define SRC_PROFESSION_H_

#include <vector>

#include "competency.h"

/**
 * A profession is a job sought by a student.
 * 
 * It has a set a prerequisted competencies for the student to perform it correctly.
 */
class Profession
{
    private:
        std::vector<Competency> prerequisites;

        // Static
        static int S_professionCounter;
    public:
        Profession();
};

#endif // SRC_PROFESSION_H_