#include "Indicators.h"
#include "F.h"

    //-----MOVING AVERAGES
    
void Indicators::calcZLEMA (const Array <double>& values, const int length, Array <double>& mas)
{   
    Array <double> emas1;
    Array <double> emas2;
    calcEMA (values, length, emas1);
    calcEMA (emas1, length, emas2);
    
    for (int i = 0; i < emas2.size(); ++i)
    {
        mas.add (emas1.getReference (i) - emas2.getUnchecked (i));
        mas.set (i, mas.getReference (i) += emas1.getUnchecked (i));
    };
}
    
void Indicators::calcEMA (const Array <double>& values, const int length, Array <double>& mas)
{
    double k = 2.0 / static_cast <double> (length + 1);
    calcXMA (values, length, k, mas);
}
    
void Indicators::calcRMA (const Array <double>& values, const int length, Array <double>& mas)
{
    double k = 1.0 / static_cast <double> (length);
    calcXMA (values, length, k, mas);
}

void Indicators::calcXMA (const Array <double>& values, const int length, const double& k, Array <double>& mas)
{
    mas.add (0.0);   
    calcInitialSMA (values, length, mas.getReference (0));
    
    for (int i = (values.size() - length) - 1; i > -1; --i)
    {
        mas.insert (0, ((values [i] - mas [0]) * k) + mas [0]); // The MA at index 0 will always be the 'last' MA
    };
}

void Indicators::calcInitialSMA (const Array <double>& values, const int length, double& initialSMA)
{
    int j = length;
    for (int i = values.size() - 1; j > -1; --i)
    {
        initialSMA += values [i];
        --j;
    };
    
    initialSMA /= length;
}

void Indicators::calcSMA (const Array <double>& values, const int length, Array <double>& mas)
{
    double sma = 0.0;
        
    for (int i = 0; i < length; ++i) sma += values.getUnchecked (i);
    
    mas.add (sma / static_cast <double> (length));
    
    for (int i = 0; i < values.size() - length; ++i)
    {
        sma -= values.getUnchecked (i);
        sma += values.getUnchecked (i + length);
        mas.add (sma / static_cast <double> (length));
    };
}

    //-----OSCILLATORS

void Indicators::calcChandeMO (const Array <double>& prices, double& x)
{
    double diff = 0.0;    
    double su = 0.0;
    double sd = 0.0;
    
    for (int i = 0; i < 20; ++i)
    {
        diff = prices.getUnchecked (i) - prices.getUnchecked (i + 1);
        
        if      (diff > 0.0) su += diff;
        else if (diff < 0.0) sd += fabs (diff);
    };
    
    x = 100 * ((su - sd) / (su + sd));
}

    //-----ARRAY UTILITIES
    
void Indicators::calcProximity (Array <double>& a, Array <double>& b, double& proximity)
{
    proximity = ((a.getUnchecked (0) - b.getUnchecked (0)) / b.getUnchecked (0)) * 100.0;
}

void Indicators::calcGradient (const Array <double>& arr, double& gradient)
{
    gradient = ((arr.getUnchecked (0) - arr.getUnchecked (arr.size() + 1)) / arr.getUnchecked (arr.size() + 1) * 100.0);
}

void Indicators::calcDivergence (   const Array <double>& a, 
                                    const Array <double>& b, 
                                    Array <double>& divergenceValues
                                )
{
    for (int i = 0; i < a.size(); ++i)
    {
        divergenceValues.add (a [i] - b [i]);
    };
}

    //-----RSI Midline

void Indicators::calcRSIMidline (const Array <double>& closes, const double& obLevel, const double& osLevel, const double& length, Array <double>& rsiMidline)
{
    const int ep = 2 * static_cast <int> (length) - 1;
    
    Array <double> aucs;
    Array <double> adcs;
    calcAXCs (closes, ep, aucs, adcs);
    
    Array <double> x1s;
    Array <double> x2s;
    calcXs (aucs, adcs, length, obLevel, osLevel, x1s, x2s);
    
    Array <double> ubs;
    Array <double> lbs;
    calcBands (closes, x1s, x2s, obLevel, osLevel, ubs, lbs);

    Array <double> bandAverages;
    for (int i = 0; i < ubs.size(); ++i) bandAverages.add ((ubs.getUnchecked (i) + lbs.getUnchecked (i)) / 2.0);
    
    calcEMA (bandAverages, 20, rsiMidline);  
}

void Indicators::calcAXCs (const Array <double>& closes, const int ep, Array <double>& aucs, Array <double>& adcs)
{
    Array <double> aucSources;
    Array <double> adcSources;
    
    for (int i = 0; i < closes.size() - 1; ++i)
    {
        aucSources.add (jmax ((closes.getUnchecked (i) - closes.getUnchecked (i + 1)), 0.0));
        adcSources.add (jmax ((closes.getUnchecked (i + 1) - closes.getUnchecked (i)), 0.0));
    };
    
    calcEMA (aucSources, ep, aucs);
    calcEMA (adcSources, ep, adcs);
}

void Indicators::calcXs (const Array <double>& aucs, const Array <double>& adcs, const double& length, const double& obLevel, const double& osLevel, Array <double>& x1s, Array <double>& x2s)
{
    const double lm1 = length - 1.00;
    const double oppOb = 100.0 - obLevel;
    const double oppOs = 100.0 - osLevel;
    
    for (int i = 0; i < adcs.size(); ++i)
    {
        x1s.add (lm1 * (adcs.getUnchecked (i) * obLevel / oppOb - aucs.getUnchecked (i)));
        x2s.add (lm1 * (adcs.getUnchecked (i) * osLevel / oppOs - aucs.getUnchecked (i)));
    };
}

void Indicators::calcBands (const Array <double>& closes, const Array <double>& x1s, const Array <double>& x2s, const double& obLevel, const double& osLevel, Array <double>& ubs, Array <double>& lbs)
{
    const double obForm = (100.0 - obLevel) / obLevel;
    const double osForm = (100.0 - osLevel) / osLevel;
    
    for (int i = 0; i < x1s.size(); ++i)
    {
        if (x1s.getUnchecked (i) >= 0.0) ubs.add (closes.getUnchecked (i) + x1s.getUnchecked (i));
        else ubs.add (closes.getUnchecked (i) + x1s.getUnchecked (i) * obForm);
        
        if (x2s.getUnchecked (i) >= 0.0) lbs.add (closes.getUnchecked (i) + x2s.getUnchecked (i));
        else lbs.add (closes.getUnchecked (i) + x2s.getUnchecked (i) * osForm);
    };
}