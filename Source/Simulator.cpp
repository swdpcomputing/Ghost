#include "Simulator.h"
#include "ThreadPoolJobs.h"
#include "E.h"
#include "F.h"

Simulator::Simulator()  :   Thread ("Simulator", 0)
{
}

Simulator::~Simulator()
{
    stopThread (2000);
}

void Simulator::runAction (const String& s)
{
    action = s;
    startThread();
}

void Simulator::run()
{
    if      (action == "runSimulation") simulateAll();
    else if (action == "b") DBG ("b");
    
    signalThreadShouldExit();
}

void Simulator::simulateAll()
{
    OwnedArray <TradeValueSet> tradeValueSets;
    populateTradeValueSets (tradeValueSets);
    OwnedArray <CoinPair> coinPairs;
        
    if (getAllCandleData ("4h",  "1d", coinPairs) == true)
    {
        for (auto& tvs : tradeValueSets)
        {
            runSimulations (coinPairs, *tvs);
        };
            
        tradeValueSets.sort (tvsc, -1);
            
        for (auto& tvs : tradeValueSets)
        {
            tvs->printParameters();
        };
        
        analyseTVSs (tradeValueSets);
    }
      
    else DBG ("Candle Data Request Failed");
};

void Simulator::populateTradeValueSets (OwnedArray <TradeValueSet>& tradeValueSets)
{
    //COPY OVER!!!
    Array <double> obLevels = {86.0, 85.0};
    Array <double> osLevels = {31.0, 32.0, 33.0, 34.0};
    Array <double> lengths = {10.0};
    Array <int> zlemaLengths = {4};
    Array <int> tradeTypes = {2};
    Array <double> tradeTypeVar1s = {1.4, 1.7, 2.0, 2.3};
    Array <double> tradeTypeVar2s = {0.09, 0.1, 0.11, 0.12};
    Array <double> tradeTypeVar3s = {6.0, 7.0, 8.0};
    
    for (const auto& o1 : obLevels)
    {
        for (const auto& o2 : osLevels)
        {
            for (const auto& o3 : lengths)
            {
                for (const auto& o4 : zlemaLengths)
                {
                    for (const auto& o5 : tradeTypes)
                    {   
                        for (const auto& o6 : tradeTypeVar1s)
                        {
                            for (const auto& o7 : tradeTypeVar2s)
                            {
                                for (const auto& o8 : tradeTypeVar3s)
                                {
                                    tradeValueSets.add (new TradeValueSet (o1, o2, o3, o4, o5, o6, o7, o8));                           
                                };
                            };    
                        };
                    };
                };
            };
        };
    };
    
    //TradeValueSets.add (new TradeValueSet (87.0, 30.0, 10.0, 4, 2, 2.30, 0.10, 7.0)); 
}

const bool Simulator::getAllCandleData  (   const String& crossesTimeFrameID,
                                            const String& longTimeFrameID,
                                            OwnedArray <CoinPair>& coinPairs
                                        )
{
    StringArray coinPairIDs;
    
    if (getCoinPairIDs (coinPairIDs) == true)
    {
        for (const auto& o : coinPairIDs) coinPairs.add (new CoinPair (o));
    
        getCandleData (crossesTimeFrameID, longTimeFrameID, coinPairs);
        
        return true;
    }
    
    else return false;
}

const bool Simulator::getCoinPairIDs (StringArray& coinPairIDs)
{
    WebInputStream requestData (URL (E::getBinanceExchangeInformationURL()), false);
      
    if (requestData.getStatusCode() == 200) 
    {
        var json = JSON::parse (requestData);
        
        //for (int i = 1; i < json ["symbols"].size(); ++i)
        for (int i = 1; i < 20; ++i)
        {
            var v = json ["symbols"] [i];

            if (v ["quoteAsset"].toString() != "BNB" && v ["quoteAsset"].toString() != "USDT")
            {
                coinPairIDs.add (v ["symbol"].toString());
            };  
        };
        
        coinPairIDs.removeString ("ETHBTC");
        coinPairIDs.removeString ("BTCETH"); 
        
        return true;
    }
    
    else return false;
}

void Simulator::getCandleData   (   const String& crossesTimeFrameID,  
                                    const String& longTimeFrameID,  
                                    OwnedArray <CoinPair>& coinPairs
                                )
{
    for (const auto& o : coinPairs)
    {
        auto* cdr = new CrossesDataRequest (crossesTimeFrameID, 1000, *o);
        E::pool->addJob (cdr, true);
    };
    
    while (E::pool->getNumJobs() > 0) we.wait (50);
    
    for (const auto& o : coinPairs)
    {
        auto* ldr = new LongDataRequest (longTimeFrameID, 300, *o);
        E::pool->addJob (ldr, true);
    };
    
    while (E::pool->getNumJobs() > 0) we.wait (50);
}

void Simulator::runSimulations (const OwnedArray <CoinPair>& coinPairs, TradeValueSet& TradeValueSet)
{
    for (const auto& o : coinPairs)
    {
        if (o->getAllDataRetrieved() == true)
        {
            //DBG (o->getSymbol());
            
            auto* simulation = new Simulation (*o, TradeValueSet);
            E::pool->addJob (simulation, true);
        };
    };

    while (E::pool->getNumJobs() > 0) we.wait (50);

    TradeValueSet.setTradesTotalAverage();
}

void Simulator::analyseTVSs (const OwnedArray <TradeValueSet>& tvss)
{
    //COPY OVER!!!
    Array <double> obLevels = {86.0, 85.0};
    Array <double> osLevels = {31.0, 32.0, 33.0, 34.0};
    Array <double> lengths = {10.0};
    Array <int> zlemaLengths = {4};
    Array <int> tradeTypes = {2};
    Array <double> tradeTypeVar1s = {1.4, 1.7, 2.0, 2.3};
    Array <double> tradeTypeVar2s = {0.09, 0.1, 0.11, 0.12};
    Array <double> tradeTypeVar3s = {6.0, 7.0, 8.0};
    
    analyseOBLevels (tvss, obLevels);
    analyseOSLevels (tvss, osLevels);
    analyseLengths (tvss, lengths);
    analyseZlemaLengths (tvss, zlemaLengths);
    analyseTrailPerc (tvss, tradeTypeVar1s);
    analyseInitialStartSellPerc (tvss, tradeTypeVar2s);
    analyseRange (tvss, tradeTypeVar3s);
}

void Simulator::analyseOBLevels (const OwnedArray <TradeValueSet>& tvss, const Array <double>& parameter)
{
    DBG ("OBLevels");
    for (const auto& o : parameter)
    { 
        double profitPerPeriodTotal = 0.0;
        for (const auto& p : tvss) if (p->getOBLevel() == o) profitPerPeriodTotal += p->getProfitPerPeriod();
        DBG ("    " << o << " : " << profitPerPeriodTotal);
    };
}

void Simulator::analyseOSLevels (const OwnedArray <TradeValueSet>& tvss, const Array <double>& parameter)
{
    DBG ("OSLevels");
    for (const auto& o : parameter)
    { 
        double profitPerPeriodTotal = 0.0;
        for (const auto& p : tvss) if (p->getOSLevel() == o) profitPerPeriodTotal += p->getProfitPerPeriod();
        DBG ("    " << o << " : " << profitPerPeriodTotal);
    };
}

void Simulator::analyseLengths (const OwnedArray <TradeValueSet>& tvss, const Array <double>& parameter)
{
    DBG ("Lengths");
    for (const auto& o : parameter)
    { 
        double profitPerPeriodTotal = 0.0;
        for (const auto& p : tvss) if (p->getRSILength() == o) profitPerPeriodTotal += p->getProfitPerPeriod();
        DBG ("    " << o << " : " << profitPerPeriodTotal);
    };
}

void Simulator::analyseZlemaLengths (const OwnedArray <TradeValueSet>& tvss, const Array <int>& parameter)
{
    DBG ("ZlemaLengths");
    for (const auto& o : parameter)
    { 
        double profitPerPeriodTotal = 0.0;
        for (const auto& p : tvss) if (p->getZlemaLength() == o) profitPerPeriodTotal += p->getProfitPerPeriod();
        DBG ("    " << o << " : " << profitPerPeriodTotal);
    };
}

void Simulator::analyseTrailPerc (const OwnedArray <TradeValueSet>& tvss, const Array <double>& parameter)
{
    DBG ("TrailPerc");
    for (const auto& o : parameter)
    { 
        double profitPerPeriodTotal = 0.0;
        for (const auto& p : tvss) if (p->getTradeTypeVar1() == o) profitPerPeriodTotal += p->getProfitPerPeriod();
        DBG ("    " << o << " : " << profitPerPeriodTotal);
    };
}

void Simulator::analyseInitialStartSellPerc (const OwnedArray <TradeValueSet>& tvss, const Array <double>& parameter)
{
    DBG ("StartSellPerc");
    for (const auto& o : parameter)
    { 
        double profitPerPeriodTotal = 0.0;
        for (const auto& p : tvss) if (p->getTradeTypeVar2() == o) profitPerPeriodTotal += p->getProfitPerPeriod();
        DBG ("    " << o << " : " << profitPerPeriodTotal);
    };
}

void Simulator::analyseRange (const OwnedArray <TradeValueSet>& tvss, const Array <double>& parameter)
{
    DBG ("Range");
    for (const auto& o : parameter)
    { 
        double profitPerPeriodTotal = 0.0;
        for (const auto& p : tvss) if (p->getTradeTypeVar3() == o) profitPerPeriodTotal += p->getProfitPerPeriod();
        DBG ("    " << o << " : " << profitPerPeriodTotal);
    };
}
