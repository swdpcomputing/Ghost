#include "CoinPair.h"
#include "E.h"
#include "F.h"
#include "Indicators.h"

CoinPair::CoinPair (const String& s) : symbol (s) {}

void CoinPair::updateCrossesData (const var& data)
{
    for (int i = 0; i < data.size(); ++i)
    {
        crossesOpens.insert (0, data [i] [1]);
        crossesHighs.insert (0, data [i] [2]);
        crossesLows.insert (0, data [i] [3]);
        crossesCloses.insert (0, data [i] [4]);
        crossesTimes.insert (0, static_cast <int> ((data [i] [0].operator int64() / 1000)));
    };
    
    crossesDataRetrieved = true;
}

void CoinPair::updateLongData (const var& data)
{
    for (int i = 0; i < data.size(); ++i)
    {
        longHighs.insert (0, data [i] [1]);
        longOpens.insert (0, data [i] [2]);
        longLows.insert (0, data [i] [3]);
        longCloses.insert (0, data [i] [4]);
        longTimes.insert (0, static_cast <int> ((data [i] [0].operator int64() / 1000)));
    };
    
    longDataRetrieved = true;
}

    //-----GETTERS

const String& CoinPair::getSymbol() const
{
    return symbol;
}

const Array <double>& CoinPair::getCrossesValues (const String& s) const
{
    if      (s == "opens") return crossesOpens;
    else if (s == "highs") return crossesHighs;
    else if (s == "lows") return crossesLows;
    else if (s == "closes") return crossesCloses;
    throw;
}

const double& CoinPair::getCrossesValue (const String& s, const int i) const
{
    if      (s == "opens") return crossesOpens.getReference (i);
    else if (s == "highs") return crossesHighs.getReference (i);
    else if (s == "lows") return crossesLows.getReference (i);
    else if (s == "closes") return crossesCloses.getReference (i);
    throw;
}

const Array <int>& CoinPair::getCrossesTimes() const
{
    return crossesTimes;
}

const int CoinPair::getCrossesTime (const int i) const
{
    return crossesTimes.getUnchecked (i);
}

const bool CoinPair::getCrossesDataRetrieved() const
{
    return crossesDataRetrieved;
}

const Array <double>& CoinPair::getLongValues (const String& s) const
{
    if      (s == "opens") return longOpens;
    else if (s == "highs") return longHighs;
    else if (s == "lows") return longLows;
    else if (s == "closes") return longCloses;
    throw;
}

const double& CoinPair::getLongValue (const String& s, const int i) const
{
    if      (s == "opens") return longOpens.getReference (i);
    else if (s == "highs") return longHighs.getReference (i);
    else if (s == "lows") return longLows.getReference (i);
    else if (s == "closes") return longCloses.getReference (i);
    throw;
}

const Array <int>& CoinPair::getLongTimes() const
{
    return longTimes;
}

const int CoinPair::getLongTime (const int i) const
{
    return longTimes.getUnchecked (i);
}

const bool CoinPair::getLongDataRetrieved() const
{
    return longDataRetrieved;
}

const bool CoinPair::getAllDataRetrieved() const
{
    if (crossesDataRetrieved == true && longDataRetrieved == true) return true;
    else return false;
}