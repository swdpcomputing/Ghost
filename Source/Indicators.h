#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Indicators
{
    public:
    
        // MOVING AVERAGES
        static void calcEMA (const Array <double>&, const int, Array <double>&);
        static void calcZLEMA (const Array <double>&, const int, Array <double>&);
        static void calcRMA (const Array <double>&, const int, Array <double>&);
        static void calcXMA (const Array <double>&, const int, const double&, Array <double>&);
        static void calcInitialSMA (const Array <double>&, const int, double&);
        static void calcSMA (const Array <double>&, const int, Array <double>&);
        
        // OSCILLATORS
        static void calcChandeMO (const Array <double>&, double&);

        // ARRAY UTILITIES
        static void calcProximity (Array <double>&, Array <double>&, double&);
        static void calcGradient (const Array <double>&, double&);
        static void calcDivergence (const Array <double>&, const Array <double>&, Array <double>&);
        
        // RSI MIDLINE
        static void calcRSIMidline (const Array <double>&, const double&, const double&, const double&, Array <double>&);
        static void calcAXCs (const Array <double>&, const int, Array <double>&, Array <double>&);
        static void calcXs (const Array <double>&, const Array <double>&, const double&, const double&, const double&, Array <double>&, Array <double>&);
        static void calcBands (const Array <double>&, const Array <double>&, const Array <double>&, const double&, const double&, Array <double>&, Array <double>&);
};