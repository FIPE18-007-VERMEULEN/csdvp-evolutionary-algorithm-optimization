#include <eoInt.h>
#include <eoScalarFitness.h>

//Here, we define a QUEEN as an eoVector of int: eoInt with a "eoMinimizingFitness" template. It allow to indicate we want to minimize the fitness (concept included in eoVector cf doc).
typedef eoInt<eoMinimizingFitness> QUEEN;
