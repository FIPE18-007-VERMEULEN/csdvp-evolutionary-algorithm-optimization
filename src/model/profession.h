#ifndef SRC_PROFESSION_H_
#define SRC_PROFESSION_H_

#include <vector>

#include "competency.h"
#include "problem.h"

/**
 * A profession is a job sought by a student.
 * 
 * It has a set a prerequisted competencies for the student to perform it correctly.
 */
class Profession
{
    private:
        // ---------- CONFIGURATION ATTRIBUTES ---------
        std::string _name;

        int _requiredECTS = -1;
        int _minimalPrerequisites = -1;
        int _maximalPrerequisites = -1;

        Magnitude _minimalMagnitude;
        Magnitude _maximalMagnitude;
        // ---------- END CONFIGURATIONS ATTRIBUTES ----------
        
        // ---------- PROFESSION SPECIFIC ATTRIBUTES ----------
        std::vector<Competency> _prerequisites;
        // ---------- END PROFESSION SPECIFIC ATTRIBUTES

        int _id;
        int _seed;

        /// Use to determine if Profession has been config by the user.
        bool _isConfig;
        bool _isECTSRandom =false;

        // === FUNC
        /** _duplicataProtection returns true if the value (2nd param) searched into (1st param) is found*/
        bool _duplicataProtection(std::vector<Competency> *, Competency);

        static void _pickWithHLWeighting(int nbToPick, Profession &, CSDVP &);

        // Static
        static int PROFESSION_COUNTER;
        static int assignID();
        static void _randomlyGenerate(Profession & job, CSDVP & pb);

    public:
        static unsigned int JOB_SELECTION_TYPE;
        static unsigned int JOB_EVAL_DISCRETE;

        enum GenerationType
        {
            RANDOM
            //PRESET
        };
        
        Profession();
        Profession(std::string name);
        Profession(std::vector<Competency> & p, std::string n = "");
        
        /** Generates the profession configuration. ECTS still needs to be filled before calling this function.
         * Nonetheless, progession _prerequisites are taken randomly in the problem
         */ 
        static void generateProfession(Profession & job, Profession::GenerationType type, CSDVP & csdvp, int seed = 0);

        /**Checks the configuration of a profession, mostly before performing a generation.*/
        bool checkConfig();

        // === GETTER
        int id() const{return this->_id;}
        int seed() const{return this->_seed;}
        int requiredECTS() const{return this->_requiredECTS;}
        const std::string name() const{return this->_name;}
        const std::vector<Competency> & prerequisites() const{return this->_prerequisites;}
        /// return a modifiable reference to _prerequisite;
        std::vector<Competency> & unlocked_prerequisites(){return this->_prerequisites;}

        int cfg_minimalPrerequisites() const{return this->_minimalPrerequisites;}
        int cfg_maximalPrerequisites() const{return this->_maximalPrerequisites;}
        const Magnitude cfg_minimalMagnitude() const{return this->_minimalMagnitude;}
        const Magnitude cfg_maximalMagnitude() const{return this->_maximalMagnitude;}

        // === MUTATOR
            // SETTER
            /// Set name of the profession. If name empty, creates a default name based on ID
            void setName(std::string name);
            /// Set the prerequisites of a profession. The old prereq is returned.
            std::vector<Competency> & setPrerequisites(std::vector<Competency> & v);
            void setRequiredECTS(int ects);
            void setRequiredECTS(Profession::GenerationType type);
            // ADDER
            bool addPrerequisite(Competency &);

            void set_cfg_minimalPrerequisites(int nb);
            void set_cfg_maximalPrerequisites(int nb);
            void set_cfg_minimalMagnitude(double mag);
            void set_cfg_maximalMagnitude(double mag);

};

// === OPERATOR
std::ostream & operator<<(std::ostream & Stream, const Profession & p);

#endif // SRC_PROFESSION_H_