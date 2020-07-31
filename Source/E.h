#pragma once
#include "hmac.h"
#include "sha256.h"

class E
{
    public:
        //-----PATHS
        
        static const String& getTradesPath()
        {
            if (juce::SystemStats::getComputerName() == "Stephens-MacBook-Pro")
            {
                return tradesPathS;
            }
        
            else return tradesPathL;
        };
        
        static const String& getStrategyResultsPath()
        {
            if (juce::SystemStats::getComputerName() == "Stephens-MacBook-Pro")
            {
                return strategyResultsPathS;
            }
        
            else return strategyResultsPathL;
        };
        
        //-----BINANCE
        
        static const String createSignature (const String& postData)
        {
            return String (hmac <SHA256B> (postData.toStdString(), binanceSecretKey.toStdString()));
        }
        
        static const String& getBinanceSecretKey()
        {
            return binanceSecretKey;
        }
        
        static const String& getBinanceAPIKey()
        {
            return binanceAPIKey;
        }
        
        static const String& getBinanceExchangeInformationURL()
        {
            return binanceExchangeInformationURL;
        }
        
        static const String& getBinanceBuyURL()
        {
            return binanceBuyURL;
        }
        
        static const String& getBinanceBuyTestURL()
        {
            return binanceBuyTestURL;
        };
        
        static const String& getBinanceAccountInformationURL()
        {
            return binanceAccountInformationURL;
        }
        
        static const String& getBinancePriceTickerURL()
        {
            return binancePriceTickerURL;
        }
        
        static const String& getBinanceApiHeader()
        {
            return binanceApiHeader;
        }

        static ScopedPointer <ThreadPool> pool;
        
        // MISC
        
        static const String& getPrimaryString()
        {
            return primaryString;
        }
        
        static const String& getLongString()
        {
            return longString;
        }
    
    private:   
        static const String tradesPathL;
        static const String tradesPathS;
        static const String strategyResultsPathL;
        static const String strategyResultsPathS;
        
        static const String binanceSecretKey;
        static const String binanceAPIKey;
        static const String binanceBuyURL;
        static const String binanceExchangeInformationURL;
        static const String binanceBuyTestURL;
        static const String binanceAccountInformationURL;
        static const String binancePriceTickerURL;
        static const String binanceApiHeader;  
        static const String primaryString;
        static const String longString;
};