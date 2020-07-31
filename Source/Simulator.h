#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "CoinPair.h"
#include "TradeValueSet.h"

class Simulator :   public Thread
{
    public:
        Simulator();
        ~Simulator();
        
        void runAction (const String&);

    private:
        void run() override;
        void simulateAll();
        void populateTradeValueSets (OwnedArray <TradeValueSet>&);
        const bool getAllCandleData (const String&, const String&, OwnedArray <CoinPair>&);
        const bool getCoinPairIDs (StringArray&);
        void getCandleData (const String&, const String&, OwnedArray <CoinPair>&);
        void runSimulations (const OwnedArray <CoinPair>&, TradeValueSet&);
        
        void analyseTVSs (const OwnedArray <TradeValueSet>&);
        void analyseOBLevels (const OwnedArray <TradeValueSet>&, const Array <double>&);
        void analyseOSLevels (const OwnedArray <TradeValueSet>&, const Array <double>&);
        void analyseLengths (const OwnedArray <TradeValueSet>&, const Array <double>&);
        void analyseZlemaLengths (const OwnedArray <TradeValueSet>&, const Array <int>&);
        void analyseTrailPerc (const OwnedArray <TradeValueSet>&, const Array <double>&);
        void analyseInitialStartSellPerc (const OwnedArray <TradeValueSet>&, const Array <double>&);
        void analyseRange (const OwnedArray <TradeValueSet>&, const Array <double>&);

        WaitableEvent we;
        
        String action = "";   
        
        TradeValueSetComparator tvsc;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Simulator)
};
