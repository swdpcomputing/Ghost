#include "SetExternal.h"
#include "E.h"

ScopedPointer <ThreadPool> E::pool = new ThreadPool();
//const String E::tradesPathL = "/Users/louise/Dropbox/code/juce/Jolokia/data/trades/";
//const String E::tradesPathS = "/Users/stephenpowell/Dropbox/code/juce/Jolokia/data/trades/";
//const String E::strategyResultsPathL = "/Users/louise/Dropbox/code/juce/Jolokia/data/strategyResults/";
//const String E::strategyResultsPathS = "/Users/stephenpowell/Dropbox/code/juce/Jolokia/data/strategyResults/";

const String E::binanceSecretKey = "";
const String E::binanceAPIKey = "";
const String E::binanceApiHeader = "\r\nX-MBX-APIKEY: " + E::getBinanceAPIKey();

const String E::binanceExchangeInformationURL = "https://api.binance.com/api/v1/exchangeInfo";
const String E::binanceBuyURL = "https://api.binance.com/api/v3/order?";
const String E::binanceBuyTestURL = "https://api.binance.com/api/v3/order/test?";
const String E::binanceAccountInformationURL = "https://api.binance.com/api/v3/account?";
const String E::binancePriceTickerURL = "https://api.binance.com//api/v3/ticker/price?symbol=";

const String E::primaryString = "primary";
const String E::longString = "long";

SetExternal::~SetExternal()
{
    E::pool->removeAllJobs (true, 1000, nullptr);
}
