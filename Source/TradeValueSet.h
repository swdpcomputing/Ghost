#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class TradeValueSet
{
    public:
        TradeValueSet (const double&, const double&, const double&, const int&, const int&, const double&, const double&, const double&);
        ~TradeValueSet() {};
        
        const double& getOBLevel() const;
        const double& getOSLevel() const;     
        const double& getRSILength() const;
        const int getZlemaLength() const;
        const int getTradeType() const;
        const double& getTradeTypeVar1() const;
        const double& getTradeTypeVar2() const;
        const double& getTradeTypeVar3() const;
        const double& getTradesTotalAverage() const;
        const double& getProfitPerPeriod() const;
        
        void addToTradesTotal (const double&, const int);
        void setTradesTotalAverage();
        
        void printParameters();
        
    private:
        const double obLevel;
        const double osLevel;     
        const double rsiLength;
        const int zlemaLength;
        const int tradeType;
        const double tradeTypeVar1;
        const double tradeTypeVar2;
        const double tradeTypeVar3;
        
        double tradesTotal = 0.0;
        double tradesTotalAverage = 0.0;
        int totalPeriods = 0;
        double totalPeriodsAverage = 0.0;
        int numTrades = 0;
        double profitPerPeriod = 0.0;
};

class TradeValueSetComparator
{
    public:
        int compareElements (TradeValueSet* a, TradeValueSet* b)
        {
            if (a->getProfitPerPeriod() > b->getProfitPerPeriod()) return -1;
            else return 1;
        };
};