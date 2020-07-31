#include "Trader.h"
#include "ThreadPoolJobs.h"
#include "E.h"
#include "F.h"

Trader::Trader()  :   Thread ("Trader", 0)
{
}

Trader::~Trader()
{
    stopThread (2000);
}

void Trader::runAction (const String& s)
{
    action = s;
    startThread();
}

void Trader::run()
{
    testIsReadyToBuy();
    // 0.001 = 5GBP to BTC
    
    //tradeValueSets.add (new TradeValueSet (87.0, 30.0, 10.0, 4, 2, 2.30, 0.10, 7.0)); 
}

const bool Trader::testIsReadyToBuy()
{
    String parameters = "timestamp=" + String (Time::currentTimeMillis());
	parameters += "&signature=" + E::createSignature (parameters); // Don't seperate!
		
    URL url (E::getBinanceAccountInformationURL() + parameters);
    auto* wis = dynamic_cast <WebInputStream*> (url.createInputStream (false, nullptr, nullptr, E::getBinanceApiHeader(), 0, nullptr, nullptr, 5, ""));
    
    double btcBalance = 0.0;
    double ethBalance = 0.0;
    
    if (wis->getStatusCode() == 200)
    {
        const var data = JSON::parse (*wis);
        setBalances (data, btcBalance, ethBalance);
    };
    
    DBG ("BTC: " << btcBalance << " " << "ETH: " << ethBalance);

    return true;
}

void Trader::setBalances (const var& data, double& btcBalance, double& ethBalance)
{
    bool ethSet = false;
    bool btcSet = false;
    
    for (int i = 0; i < data ["balances"].size(); ++i)
    {
        if (data ["balances"] [i] ["asset"].toString() == "BTC")
        {
            btcBalance = data ["balances"] [i] ["free"].operator double();
            if (ethSet == true) return;
            btcSet = true;
        }
        
        else if (data ["balances"] [i] ["asset"].toString() == "ETH")
        {
            ethBalance = data ["balances"] [i] ["free"].operator double();
            if (btcSet == true) return;
            ethSet = true;
        };
    };
}

void Trader::makeMarketTrade (const String& pairSymbol, const double& quantity)
{
	String postData = "symbol=";
	postData += pairSymbol;
	postData += "&side=BUY&type=MARKET&quantity=";
	postData += String (quantity); // undo hard value
    postData += "&recvWindow=5000&timestamp=";
	postData += String (Time::currentTimeMillis());
    postData += "&signature=" + E::createSignature (postData); // Don't seperate!
	
    URL url (E::getBinanceBuyTestURL());
	url = url.withPOSTData (postData);
	
    auto* wis = dynamic_cast <WebInputStream*> (url.createInputStream (true, nullptr, nullptr, E::getBinanceApiHeader(), 0, nullptr, nullptr, 5, ""));
      
    if (wis->getStatusCode() == 200)
    {
        DBG ("Market Trade Successful");
    }
    
    else DBG ("Market Trade Unsuccessful");
}