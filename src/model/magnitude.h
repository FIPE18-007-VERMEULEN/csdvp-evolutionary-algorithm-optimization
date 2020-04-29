#ifndef SRC_MAGNITUDE_H_
#define SRC_MAGNITUDE_H_

/** Magnitude represents the mastery of a competency.
 * 
 * It should be used in accordance with a scale
 */
class Magnitude
{
    private:
        double _value = 0;

        //Constructor
        Magnitude();
        Magnitude(double);

        // function
        static const int _inRange(double);

    public:
        static Magnitude* build(double); //factory

    //GETTER
    const double value();
    
    //SETTER
    bool set(double v);
};

#endif // SRC_MAGNITUDE_H_