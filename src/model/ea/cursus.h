#ifndef SRC_MODEL_EA_CURSUS_H_
#define SRC_MODEL_EA_CURSUS_H_

#include <eoInt.h>
#include <eoScalarFitness.h>


/**
 * Represents a cursus of a student. It is equivalent to an individu in a EA.
 * Here, it is defined as an eoVector of int: eoInt with a "eoMinimizingFitness" template. Each int represents a course.
 * In version 1.1, the representation is based on the course's position in the vector. Older version dealing with the course's ID **IS** deprecated
 * @version 1
 */ 

typedef eoInt<eoMinimizingFitness> Cursus;
#endif // SRC_MODEL_EA_CURSUS_H_