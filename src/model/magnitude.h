#ifndef DEF_MAGNITUDE
#define DEF_MAGNITUDE

/** Magnitude represents the mastery of a competency.
 * 
 * It should be used in accordance with a scale
 */
class Magnitude
{
    private:
        double value = 0;

    public:
    Magnitude();

    //GETTER
    const double value();
    
    //SETTER
    bool set(double v);
};

#endif