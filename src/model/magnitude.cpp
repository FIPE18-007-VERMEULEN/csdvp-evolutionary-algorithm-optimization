#include "magnitude.h"
#include "exception/magnitudeException.h"

// === FACTORY
Magnitude Magnitude::build(double value = 0)
{
    bool isInRange = !(_inRange(value));

    if( !isInRange )
    {
        Magnitude mag(value);
        throw MagnitudeException(mag);
    }
    else
        return Magnitude(value);
}

// === CONSTRUCTOR
Magnitude::Magnitude()
{

}

Magnitude::Magnitude(double d)
{
    this->_value=d;
}

Magnitude::Magnitude(const Magnitude & m)
{
    this->_value = m._value;
}

// === OPERATOR

/**
 * @throw MagnitudeException Throw a MagnitudeException if the new value is > 1 or < 0. Note that the this->_value IS set (thus you can use this->rebase() to put _value in good range)
 */ 
Magnitude & Magnitude::operator+=(const double d)
{
    this->_value+=d;
    if( Magnitude::_inRange(this->_value) )
        throw MagnitudeException(*this);
    return *this;
}

/**
 * @throw MagnitudeException Throw a MagnitudeException if the new value is > 1 or < 0. Note that the this->_value IS set (thus you can use this->rebase() to put _value in good range)
 */ 
Magnitude & Magnitude::operator+=(const Magnitude &  m)
{
    this->_value += m._value;
    if( Magnitude::_inRange(this->_value) )
        throw MagnitudeException(*this);
    return *this;
}

/**
 * @throw MagnitudeException Throw a MagnitudeException if the new value is > 1 or < 0. You can catch the new magnitude causing the exception with magnitudeException.getMagnitude() (and thus perform the desired action, like a rebase)
 */ 
Magnitude operator+(const Magnitude & m, const Magnitude & n)
{
    Magnitude mag(m);
    mag+= n.value();
    return mag;
}

/**
 * @throw MagnitudeException Throw a MagnitudeException if the new value is > 1 or < 0. You can catch the new magnitude causing the exception with magnitudeException.getMagnitude() (and thus perform the desired action, like a rebase)
 */
Magnitude operator+(const Magnitude & m, const double d)
{
    Magnitude mag(m);
    mag += d;
    return mag;
}

// === FUNCTION
/** Checks if the value v is in [0;1]. If yes, return 0. Else, if v < 0 return -1, 1 otherwise
 */ 
int Magnitude::_inRange(double v)
{
    return v < 0 ? -1 : v > 1 ? 1 : 0;
}

double Magnitude::value() const {return this->_value;}

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

double Magnitude::rebase()
{
    if(this->_value < 0)
        this->_value = 0;
    else if(this->_value > 1)
        this->_value = 1;
    return this->_value;        
}