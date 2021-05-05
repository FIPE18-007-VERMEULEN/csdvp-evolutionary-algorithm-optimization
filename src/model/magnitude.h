#ifndef SRC_MAGNITUDE_H_
#define SRC_MAGNITUDE_H_

#include <string>

/** Magnitude represents the mastery of a competency.
 * 
 * It should be used in accordance with a scale
 */
class Magnitude
{
    private:
        double _value = 0;

        //Constructor
        Magnitude(double);

        // function
        static int _inRange(double);

    public:
        static Magnitude build(double); //factory

        Magnitude() = default;
        /* Magnitude(const Magnitude & m); */

        double rebase(); /// if value is out of range, assign it the nearest value. Usefull when a MagnitudeException is caught

        // Operator
        Magnitude &  operator+=(const Magnitude & m);
        Magnitude &  operator+=(double const d);
        bool operator>(const Magnitude & m) const;

        //GETTER
        double value() const;
        
        //SETTER
        bool set(double v);

        //EXPORT
        std::string exportMe() const;
};

Magnitude operator+(const Magnitude & m1, const Magnitude & m2);
Magnitude operator+(const Magnitude & m1, double d);

#endif // SRC_MAGNITUDE_H_