#ifndef SRC_COMPETENCY_H_
#define SRC_COMPETENCY_H_

#include <string>

#include "magnitude.h"

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
        Magnitude & _m;
        std::string _name;
        int _id;

        //Constructor
        Competency(int, Magnitude, std::string);

        //STATIC
        static int COMPETENCY_COUNTER;
        static int assignID();
    public:
        static Competency build(Magnitude &, std::string);
        static Competency build(double, std::string);

        // === FUNCTION
            /** evolveTowards allows the competency to increase or decrease according to the value passed in parameter (either in Magnitude._value or double). It represents the competency evolving towards a specfic state, here represented by this->_m.value() + m.value(). 
             * Use it instead of manipuling directly a Magnitude, since it'll handle MagnitudeException
             * 
             * @throw CompetencyEvolvingException
             */ 
            void evolveTowards(Magnitude & m);
            /** evolveTowards allows the competency to increase or decrease according to the value passed in parameter (either in Magnitude._value or double). It represents the competency evolving towards a specfic state, here represented by this->_m.value() + d.
             * Use it instead of manipuling directly a Magnitude, since it'll handle MagnitudeException
             * 
             * @throw CompetencyEvolvingException
             */ 
            void evolveTowards(double d);

        // === GETTER
            ///Retrieves the magnitude/mastery value of the competency
        const double competencyValue() const;
            ///Retrieves the magnitude of the compentecy
        const Magnitude c_magnitude() const {return this->_m;}
        Magnitude & magnitude(){return this->_m;}
            ///Retrieves the name of the competency
        const std::string c_name() const {return this->_name;}
        std::string & name() {return this->_name;}

        const int id() const {return this->_id;}

        // === SETTER
        void setMagnitude(Magnitude & m){this->_m = m;};
};
int Competency::COMPETENCY_COUNTER = 0;

#endif // SRC_COMPETENCY_H_