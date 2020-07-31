#include "TradeValueSet.h"

TradeValueSet::TradeValueSet    (   const double& a,
                                    const double& b,
                                    const double& c,
                                    const int& d,
                                    const int& e,
                                    const double& f,
                                    const double& g,
                                    const double& h
                                )   :   obLevel (a),
                                        osLevel (b),
                                        rsiLength (c),
                                        zlemaLength (d),
                                        tradeType (e),
                                        tradeTypeVar1 (f),
                                        tradeTypeVar2 (g),
                                        tradeTypeVar3 (h)
{
}

const double& TradeValueSet::getOBLevel() const
{
    return obLevel;    
}

const double& TradeValueSet::getOSLevel() const
{
    return osLevel;    
}

const double& TradeValueSet::getRSILength() const
{
    return rsiLength;
}

const int TradeValueSet::getZlemaLength() const
{
    return zlemaLength;;
}

const int TradeValueSet::getTradeType() const
{
    return tradeType;
}

const double& TradeValueSet::getTradeTypeVar1() const
{
    return tradeTypeVar1;
}

const double& TradeValueSet::getTradeTypeVar2() const
{
    return tradeTypeVar2;
}

const double& TradeValueSet::getTradeTypeVar3() const
{
    return tradeTypeVar3;
}

const double& TradeValueSet::getTradesTotalAverage() const
{
    return tradesTotalAverage;
}

const double& TradeValueSet::getProfitPerPeriod() const
{
    return profitPerPeriod;
}

void TradeValueSet::addToTradesTotal (const double& tradeTotal, const int tradePeriods)
{
    //DBG (tradeTotal << " : " << tradeHours);
    tradesTotal += tradeTotal;
    totalPeriods += tradePeriods;
    ++numTrades;
}

void TradeValueSet::setTradesTotalAverage()
{
    if (numTrades != 0)
    {
        tradesTotalAverage = tradesTotal / static_cast <double> (numTrades);
        totalPeriodsAverage = static_cast <double> (totalPeriods) / static_cast <double> (numTrades);
        profitPerPeriod = tradesTotalAverage / totalPeriodsAverage;
    }
    
    else 
    {
        tradesTotalAverage = 0.0;
        totalPeriods = 0;
        profitPerPeriod = 0.0;
    };
}

void TradeValueSet::printParameters()
{
    //DBG (tradesTotal << " : " << numTrades);
    
    DBG (   obLevel << " : " <<
            osLevel << " : " <<
            rsiLength << " : " <<
            zlemaLength << " : " <<
            tradeType << " : " <<
            tradeTypeVar1 << " : " <<
            tradeTypeVar2 << " : " <<
            tradeTypeVar3 << " : " <<
            numTrades << " : " <<
            tradesTotalAverage << " : " <<
            totalPeriodsAverage << " : " <<
            profitPerPeriod
         );
         
    //DBG ("");
}
