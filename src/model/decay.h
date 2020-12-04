#ifndef SRC_MODEL_DECAY_H_
#define SRC_MODEL_DECAY_H_

#include <cmath>
#include <iostream>

class DecayEngine
{
    private:

    public:
        static int IS_DECAY_DEACTIVATED;
        /** Expresses the decay over the time x. 
         */
        static double defaultDecay(int t)
        {
            if(IS_DECAY_DEACTIVATED)
                return 0;
            if(t == 0)
                return 0;
            // seems to be quitte similar to ebbinghaus : r = e^{-t/s} with s memory stability and t time
            //return (exp(t / 1.25) + 5)/100; 
            return (exp(t/2.125) + 10)/100;
        }
};

#endif //SRC_MODEL_DECAY_H_