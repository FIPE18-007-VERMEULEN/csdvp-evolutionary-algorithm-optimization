#ifndef DEF_SCALE
#define DEF_SCALE

#include <scaleValue.h>

/** Represent a competency scale used by academic to assert competency level of a student.
 * 
 * Use a scale to discretize the magnitude 
 */
class Scale
{
    private:

    public:
        void getRange(ScaleValue &, double & inf, double & max);
};

#endif DEF_SCALE