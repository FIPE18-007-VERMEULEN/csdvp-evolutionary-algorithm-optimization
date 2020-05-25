#ifndef SRC_MODEL_DECAY_H_
#define SRC_MODEL_DECAY_H_

#include <cmath>

class DecayEngine
{
    private:

    public:
        /** Expresses the decay over the time x. 
         */
        static double defaultDecay(int t)
        {
            if(t == 0)
                return 0;
            return (exp(t / 1.25) + 5)/100;
        }
};

#endif //SRC_MODEL_DECAY_H_