#include "ThreadPoolJobs.h"
#include "E.h"
#include "F.h"

    //-----CANDLE DATA REQUEST

const String CrossesDataRequest::binanceCandleDataURL = "https://api.binance.com/api/v1/klines?symbol=";

CrossesDataRequest::CrossesDataRequest  (   const String& tfid,
                                            const int p,
                                            CoinPair& cp                                
                                        ) : ThreadPoolJob ("CrossesDataRequest"),
                                            timeFrameID (tfid),
                                            periods (p),
                                            coinPair (cp)
{
}

ThreadPoolJob::JobStatus CrossesDataRequest::runJob()
{
    String url = binanceCandleDataURL + coinPair.getSymbol() + "&interval=" + timeFrameID + "&limit=" + String (periods);
    WebInputStream wis (URL (url), false);
    
    if  (wis.getStatusCode() == 200) 
    {
        const var data = JSON::parse (wis);
        if (data.size() == periods) coinPair.updateCrossesData (data);
    };
    
    signalJobShouldExit();
    return jobHasFinished; 
}

const String LongDataRequest::binanceCandleDataURL = "https://api.binance.com/api/v1/klines?symbol=";

LongDataRequest::LongDataRequest    (   const String& tfid,
                                        const int p,
                                        CoinPair& cp                                
                                    ) : ThreadPoolJob ("LongDataRequest"),
                                        timeFrameID (tfid),
                                        periods (p),
                                        coinPair (cp)
{
}

ThreadPoolJob::JobStatus LongDataRequest::runJob()
{
    String url = binanceCandleDataURL + coinPair.getSymbol() + "&interval=" + timeFrameID + "&limit=" + String (periods);
    WebInputStream wis (URL (url), false);
    
    if  (wis.getStatusCode() == 200) 
    {
        const var data = JSON::parse (wis);
        if (data.size() == periods) coinPair.updateLongData (data);
    };
    
    signalJobShouldExit();
    return jobHasFinished; 
}

    //-----SIMULATION
    
#include "Indicators.h"
    
Simulation::Simulation  (   const CoinPair& cp, 
                            TradeValueSet& s
                        )  :    ThreadPoolJob ("Simulation"),
                                coinPair (cp),
                                svs (s)
{
}

ThreadPoolJob::JobStatus Simulation::runJob()
{
    Array <double> zlemas;
    Indicators::calcZLEMA (coinPair.getCrossesValues ("highs"), svs.getZlemaLength(), zlemas);
    
    Array <double> rsiMidline;
    Indicators::calcRSIMidline (coinPair.getCrossesValues ("lows"), svs.getOBLevel(), svs.getOSLevel(), svs.getRSILength(), rsiMidline);
    rsiMidline.removeLast (40); // To compensate for value synchronisation
    
    Array <int> crosses;
    calcCrosses (zlemas, rsiMidline, crosses);
    runCrosses (crosses);
    
    signalJobShouldExit();
    return jobHasFinished;
}

void Simulation::calcCrosses (const Array <double>& zlemas, const Array <double>& rsiMidline, Array <int>& crosses)
{
    int endIndex = jmin (zlemas.size(), rsiMidline.size());

    for (int i = 0; i < endIndex - 1; ++i)
    {
        //DBG (crossTime (i) << " : " << zlemas.getUnchecked (i) << " : " << rsiMidline.getUnchecked (i));
        
        if ((zlemas.getUnchecked (i) > rsiMidline.getUnchecked (i)) && (zlemas.getUnchecked (i + 1) <= rsiMidline.getUnchecked (i + 1)))
        {
            crosses.add (1);
        }
        
        else if ((zlemas.getUnchecked (i) < rsiMidline.getUnchecked (i)) && (zlemas.getUnchecked (i + 1) >= rsiMidline.getUnchecked (i + 1)))
        {
            crosses.add (2);
        }
        
        else crosses.add (0);
    };
}

void Simulation::runCrosses (const Array <int>& crosses)
{   
    for (int i = crosses.size() - 1; i >= 0; --i)
    {
        //DBG (crossTime (i) << " : " << crosses.getUnchecked (i));
        
        if (crosses.getUnchecked (i) == 1 && validateLong (i)) runTrade (i, crosses);
    };
}

const bool Simulation::validateLong (const int index)
{
    const int indexTime = coinPair.getCrossesTime (index);
    Array <double> longValues = coinPair.getLongValues ("closes");

    for (const auto& o : coinPair.getLongTimes())
    {
        if (o >= indexTime) longValues.remove (0);
        else break;
    };
    
    longValues.insert (0, coinPair.getCrossesValue ("closes", index));
    
    Array <double> longEMAS;
    Indicators::calcEMA (longValues, 10, longEMAS);
    
    if (longEMAS.getUnchecked (0) > longEMAS.getUnchecked (1))
    {
        return true;
    }
    
    else return false;
}

void Simulation::runTrade (int& startIndex, const Array <int>& crosses)
{
    if (svs.getTradeType() == 2) runTrailingStopTrade (startIndex, crosses);
}

void Simulation::runTrailingStopTrade (int& startIndex, const Array <int>& crosses)
{
    // With anything but closes, use up to date price as last value
    
    //DBG (coinPair.getSymbol());
    const double trailStartPerc = svs.getTradeTypeVar1();
    const double startSellPerc = svs.getTradeTypeVar2();
    const double range = svs.getTradeTypeVar3(); 
    const double startPrice = coinPair.getCrossesValue ("closes", startIndex);
    double highestPrice = startPrice;
    const double trailStartPrice = highestPrice * (1.0 + (trailStartPerc / 100.0));
    double sellPrice = 0.0;
    
    const Array <double>& values = coinPair.getCrossesValues ("closes");
    bool trailReached = false;

    //DBG ("Trade Start: " << crossTime (startIndex)  << " : " << startPrice);

    for (int i = startIndex; i >= 0; --i)
    {
        if (values.getUnchecked (i) <= sellPrice || crosses.getUnchecked (i) == 2)
        {
            svs.addToTradesTotal (F::percChange (startPrice, values.getUnchecked (i)), startIndex - i);
            //DBG ("Trade End:   " << crossTime (i) << " : " << F::percChange (startPrice, values.getUnchecked (i)));
            startIndex = i - 1;
            return;
        };
        
        if (values.getUnchecked (i) >= trailStartPrice)
        {
            trailReached = true;
        };
        
        if (trailReached == true && values.getUnchecked (i) > highestPrice)
        {
            highestPrice = values.getUnchecked (i);
            const double percChange = F::percChange (startPrice, highestPrice);
            const double sellPriceMult = ((percChange - trailStartPerc) / range) + startSellPerc;
            sellPrice = ((highestPrice - startPrice) * sellPriceMult) + startPrice;
            //DBG (crossTime (i) << " : " << percChange << " : " << sellPriceMult);
        };
    };
    
    //DBG ("");
}

const String Simulation::crossTime (const int i)
{
    return F::getTimeFromSecs (coinPair.getCrossesTime (i));
}
