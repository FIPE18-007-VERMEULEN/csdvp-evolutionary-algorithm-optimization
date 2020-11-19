#ifndef SRC_COMPETENCY_H_
#define SRC_COMPETENCY_H_

#include <string>

#include "magnitude.h"
#include "decay.h"


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
        int _id;
        Magnitude _m;
        Magnitude _undecayedMag; //Decay related
        std::string _name;
        int _howLongDecaying; //Decay related
        bool _isDecaying; //Decay related
        

        //Constructor
        Competency(int, Magnitude, std::string);

        //STATIC
        static int COMPETENCY_COUNTER;
        static int COMPETENCY_TMP_COUNTER;
        
        static int assignID();
        static int assignID4TMP();//Called by the tmp builder
    public:
        static Competency build(Magnitude &, std::string s = "");
        static Competency build(double, std::string = "");
        ///This builder should be used for TMP element
        static Competency buildTMP(double, std::string);

        Competency() = default;
        // === FUNCTION
            /** evolveTowards allows the competency to increase or decrease according to the value passed in parameter (either in Magnitude._value or double). It represents the competency evolving towards a specfic state, here represented by this->_m.value() + m.value(). 
             * Use it instead of manipuling directly a Magnitude, since it'll handle MagnitudeException
             * 
             * @throw CompetencyEvolvingException Exception containing this
             */ 
            void evolveTowards(Magnitude & m);
            /** evolveTowards allows the competency to increase or decrease according to the value passed in parameter (either in Magnitude._value or double). It represents the competency evolving towards a specfic state, here represented by this->_m.value() + d.
             * Use it instead of manipuling directly a Magnitude, since it'll handle MagnitudeException
             * 
             * @throw CompetencyEvolvingException Exception containing this
             */ 
            void evolveTowards(double d);

        // === GETTER
            ///Retrieves the magnitude/mastery value of the competency
        double competencyValue() const;
            ///Retrieves the magnitude of the compentecy
        const Magnitude c_magnitude() const {return this->_m;}
        Magnitude & magnitude(){return this->_m;}
            ///Retrieves the name of the competency
        const std::string c_name() const {return this->_name;}
        std::string & name() {return this->_name;}

        int id() const {return this->_id;}

        // === SETTER
        void setMagnitude(Magnitude & m){this->_m = m;}
        void setName(std::string s){this->_name = s;}

        // === DECAY
        bool isDecaying() const {return this->_isDecaying;}
        void decayState(bool state) {this->_isDecaying = state;}
        int increaseDecay(){this->_isDecaying = true; this->_howLongDecaying++; return this->_howLongDecaying;}
        void resetDecay(){this->_isDecaying = false; this->_howLongDecaying = 0;}
        void setTimeDecay(unsigned int time){this->_howLongDecaying = time;}
        Magnitude getUndecayedMagnitude(){return this->_undecayedMag;}
        /**Returns the new magnitude of this affected by the current competency's decay.
         * Note that the decay **IS NOT** saved in the magnitude. Use saveDecay instead.
        */
        double decay();
        /** Save the new magnitude value according to the decay of this. The function reset the decay of this by setting it to FALSE. Therefore, the decay restart to 0.
         */ 
        void saveDecay();

        // === OPERATOR
        /// A competency is equal to another iff their id are the same, or their name
        bool operator==(const Competency & c) const;
        
};
// === OPERATOR
std::ostream & operator<<(std::ostream& Stream, const Competency & c);

#endif // SRC_COMPETENCY_H_