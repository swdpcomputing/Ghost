#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "CoinPair.h"

class CrossesDataRequest  :   public ThreadPoolJob
{
    public:
        CrossesDataRequest (const String&, const int, CoinPair&);
        ~CrossesDataRequest() {};

    private:
        ThreadPoolJob::JobStatus runJob() override;
        
        static const String binanceCandleDataURL;
        const String& timeFrameID;
        const int periods;   
        CoinPair& coinPair;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CrossesDataRequest)
};

class LongDataRequest  :   public ThreadPoolJob
{
    public:
        LongDataRequest (const String&, const int, CoinPair&);
        ~LongDataRequest() {};

    private:
        ThreadPoolJob::JobStatus runJob() override;
        
        static const String binanceCandleDataURL;
        const String& timeFrameID;
        const int periods;   
        CoinPair& coinPair;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LongDataRequest)
};

#include "TradeValueSet.h"

class Simulation    :   public ThreadPoolJob
{
    public:
        Simulation (const CoinPair&, TradeValueSet&);
        ~Simulation() {};
    
    private:
        ThreadPoolJob::JobStatus runJob() override;
        void calcCrosses (const Array <double>&, const Array <double>&, Array <int>&);
        void runCrosses (const Array <int>&);
        const bool validateLong (const int);
        void runTrade (int&, const Array <int>&);
        
        void runTrailingStopTrade (int&, const Array <int>&);
        
        const String crossTime (const int);
         
        const CoinPair& coinPair;
        TradeValueSet& svs;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Simulation)
};