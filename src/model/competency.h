#ifndef DEF_COMPETENCY
#define DEF_COMPETNECY

#include <string>
#include <magnitude.h>

/**
 * Represents the notion of a competency (a.k.a. "non operationalisable" skill).
 *
 * A compentecy C always exists, at any moment, with a magnitude (or mastery) m in [0;1].
 * @author alexis.lebis
 * @version 1 
 */
class Competency
{
    private:
        Magnitude m;
        std::string name;
        int id;

        //STATIC
        static int COMPETENCY_COUNTER;
    public:
        Competency();
};

#endif