#ifndef DEF_PROFESSION
#define DEF_PROFESSION

#include <vector>

#include <competency.h>

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

#endif