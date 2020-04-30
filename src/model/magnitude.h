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
        static int _inRange(double);

    public:
        static Magnitude build(double); //factory

        /* Magnitude(const Magnitude & m); */

        double rebase(); /// if value is out of range, assign it the nearest value. Usefull when a MagnitudeException is caught

        // Operator
        Magnitude &  operator+=(const Magnitude & m);
        Magnitude &  operator+=(double const d);

        //GETTER
        double value() const;
        
        //SETTER
        bool set(double v);
};

Magnitude operator+(const Magnitude & m1, const Magnitude & m2);
Magnitude operator+(const Magnitude & m1, double d);

#endif // SRC_MAGNITUDE_H_