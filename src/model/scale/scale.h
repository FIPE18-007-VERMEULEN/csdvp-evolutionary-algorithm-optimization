#ifndef SRC_SCALE_SCALE_H_
#define SRC_SCALE_SCALE_H_

#include "scaleValue.h"

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

#endif // SRC_SCALE_SCALE_H_