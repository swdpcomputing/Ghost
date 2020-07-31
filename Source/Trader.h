#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "CoinPair.h"
#include "TradeValueSet.h"

class Trader    :   public Thread
{
    public:
        Trader();
        ~Trader();
        
        void runAction (const String&);

    private:
        void run() override;
        const bool testIsReadyToBuy();
        void setBalances (const var&, double&, double&);
        
        
        void makeMarketTrade (const String&, const double&);
        
        OwnedArray <TradeValueSet> tradeValueSets;
        String action = "";
        WaitableEvent we;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Trader)
};