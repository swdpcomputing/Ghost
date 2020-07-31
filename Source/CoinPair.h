#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class CoinPair
{
    public:
        CoinPair (const String&);
        ~CoinPair() {};

        void updateCrossesData (const var&);
        void updateLongData (const var&);
        
        const String& getSymbol() const;
        
        const Array <double>& getCrossesValues (const String&) const;
        const double& getCrossesValue (const String&, const int) const;       
        const Array <int>& getCrossesTimes() const; 
        const int getCrossesTime (const int) const;
        const bool getCrossesDataRetrieved() const;
        
        const Array <double>& getLongValues (const String&) const;
        const double& getLongValue (const String&, const int) const;
        const Array <int>& getLongTimes() const;
        const int getLongTime (const int) const;
        const bool getLongDataRetrieved() const;
        
        const bool getAllDataRetrieved() const;

    private:
        const String symbol;           
        
        Array <double> crossesOpens;
        Array <double> crossesHighs;
        Array <double> crossesLows;
        Array <double> crossesCloses;
        Array <int> crossesTimes;
        bool  crossesDataRetrieved = false;
        
        Array <double> longOpens;
        Array <double> longHighs;
        Array <double> longLows;
        Array <double> longCloses;
        Array <int> longTimes;
        bool  longDataRetrieved = false;
       
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CoinPair)
};