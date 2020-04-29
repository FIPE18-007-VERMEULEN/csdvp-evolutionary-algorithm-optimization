#include "magnitude.h"
#include "exception/magnitudeException.h"

// === FACTORY
Magnitude* Magnitude::build(double value = 0)
{
    bool isInRange = !(_inRange(value));

    if( !isInRange )
        throw MagnitudeException(value);
    else
        return new Magnitude(value);
}

// === CONSTRUCTOR
Magnitude::Magnitude()
{

}

Magnitude::Magnitude(double d)
{
    this->_value=d;
}

// === FUNCTION
/** Checks if the value v is in [0;1]. If yes, return 0. Else, if v < 0 return -1, 1 otherwise
 */ 
const int Magnitude::_inRange(double v)
{
    return v < 0 ? -1 : v > 1 ? 1 : 0;
}

const double Magnitude::value(){return this->_value;}

/** Set the magnitude value of a competency. Indicates whether or not there is an overflow in
 * the given v value (and that it has been automatically corrected)
 */
bool Magnitude::set(double v)
{
    int isInRange = _inRange(v);

    switch (isInRange)
    {
    case -1:
        this->_value = 0;
        break;
    case 1:
        this->_value = 1;
        break;
    default:
        this->_value = v;
        break;
    }

    return !isInRange;

}