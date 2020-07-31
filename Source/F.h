#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class F
{
    public:
        static void p2 (const var a, const var b)
        {
            DBG (a.toString() << " : " << b.toString());
        }
        
        static void p3 (const var a, const var b, const var c)
        {
            DBG (a.toString() << " : " << b.toString() << " : " << c.toString());
        }
        
        static void printVarArray (const var a)
        {
            for (int i = 0; i < a.size(); ++i) DBG ("Var at index " << i << " : " << a [i].toString());
        }
        
        static void printStringArray (const StringArray& a)
        {
            for (const auto& o : a) DBG ("String at index " << a.indexOf (o) << " " << o);
        }
        
        template <class T>
        static void printNumArray (const Array <T>& a)
        {
            int i = 0;
            
            for (const auto& o : a) 
            {
                DBG ("    " << i << ": " << o);
                ++i;
            };
        }
        
        template <class T, class T2>
        static void printDualNumArray (const Array <T>& a, const Array <T2>& b)
        {
            int i = 0;
            
            for (const auto& o : a) 
            {
                DBG ("    " << i << ": " << o << " : " << b [i]);
                ++i;
            };
        }
        
        static void printBoolArray (const Array <bool>& a)
        {
            for (const auto& o : a) 
            {
                DBG ("    " << a.indexOf (o) << ": " << b2s (o));
            };
        }
    
        static void printStringPairArray (const StringPairArray& a)
        {
            for (const auto& o : a.getAllKeys())
            {   
                DBG (o << " : " << a.getValue (o, "No Return Value"));
            };
        }
        
        static void printPropertyIDArray (const StringArray& a)
        {
            DBG ("     Properties");
            for (const auto& o : a) DBG (" - " << o);
        }
        
        static void getAllPropertyIds (ValueTree a, StringArray& sa)
        {
            for (int i = 0; i < a.getNumProperties(); ++i) sa.add (a.getPropertyName (i).toString());
        }
        
        static void getAllChildIds (ValueTree a, StringArray& sa)
        {
            for (int i = 0; i < a.getNumChildren(); ++i) sa.add (a.getChild (i).getType().toString());
        }
        
        static void printAllChildIds (ValueTree a)
        {
            StringArray sa;
            getAllChildIds (a, sa);
            printStringArray (sa);
        }
        
        static void printAllProperties (ValueTree a)
        {
            StringArray sa;
            getAllPropertyIds (a, sa);
            for (auto& o : sa)
            {
                DBG (o << ": " << a.getProperty (o).toString());
            };
        }
        
        static void printTree (ValueTree a)
        {
            DBG ("----" << a.getType().toString());
            if (a.getNumProperties() > 0) printAllProperties (a);
                        
            if (a.getNumChildren() > 0)
            {
                StringArray sa;
                getAllChildIds (a, sa);
                for (auto& o : sa) printTree (a.getChildWithName (o));
            };
            DBG ("");
        }    
        
        static inline const char* const b2s (const bool b)
        {
            return b ? "true" : "false";
        }
        
        template <class T>
        static void arrNumToVar (const Array <T>& aN, var& v)
        {
            for (const auto& o : aN) v.append (o);
        }
        
        static void arrVarToInt (const var& v, Array <int>& aI)
        {
            for (int i = 0; i < v.size(); ++i) aI.add (v [i].operator int());
        }
        
        static void arrVarToDouble (const var& v, Array <double>& aD)
        {
            for (int i = 0; i < v.size(); ++i) aD.add (v [i].operator double());
        }
        
        static const int roundDownToNearestMultiple (const int number, const int round)
        {
            return round * (std::floor (number / round));
        }
        
        static void printBreak()
        {
            DBG ("");
            DBG ("----------");          
            DBG ("");
        }
        
        static void printTime()
        {
            DBG (Time::getCurrentTime().toString (true, true, true, true));
        }   
        
        static void printTimeFromSecs (const int a)
        {
            DBG (Time (a * 1000).toString (true, true, true, true));
        }
        
        static String getTimeFromSecs (const int a)
        {
            return Time (static_cast <int64> (a) * 1000).toString (true, true, true, true);
        }
        
        static void printTimeFromFile (const File& a)
        {
            printTimeFromSecs (a.getFileName().getIntValue());
        }
        
        template <class T>
        static void calcArrayDirection (Array <T>& arr, Array <int>& directions)
        {
            // 0: Equal, 1: Down, 2: Up
            for (int i = 0; i < arr.size() - 1; ++i)
            {
                if      (arr.getUnchecked (i) == arr.getUnchecked (i + 1)) directions.add (0);
                else if (arr.getUnchecked (i) > arr.getUnchecked (i + 1))  directions.add (1);
                else                                                       directions.add (2);
            };
        } 
        
        inline static const int getVTIDInt (const ValueTree& vt)
        {
            return vt.getType().toString().getIntValue();
        }
        
        static const int getLastChildID (const ValueTree& coinVT, const int defaultReturn)
        {
            if (coinVT.getNumChildren() == 0) return defaultReturn;
            return getVTIDInt (coinVT.getChild (coinVT.getNumChildren())); // Need to minus 1?
        }
        
        static const int getFirstChildID (const ValueTree& coinVT, const int defaultReturn)
        {
            if (coinVT.getNumChildren() == 0) return defaultReturn;
            return getVTIDInt (coinVT.getChild (0));
        }
        
        
        static const int getTimeUnitInSecs (const String& timeUnit)
        {
            if      (timeUnit == "minute") return 60;
            else if (timeUnit == "hour") return 3600;
            else if (timeUnit == "day") return 86400;
            throw ("Should Never Happen");
        }
        
        static const int getTimeSecs()
        {
            return static_cast <int> (Time::currentTimeMillis() / 1000);
        }       
        
        template <class T>
        static void factorNumArray (Array <T>& arr, const T& mult)
        {
            for (auto& o : arr) o *= mult;
        }
        
        template <class T>
        static bool isBetween (const T& a, const T& b, const T& c)
        {
            if (a >= b && a < c) return true;
            else return false;
        }
        
        static const String genAN (const int a)
        {
            String all = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
            String result = "";
            for (int i = 0; i < a; ++i)
            {
                int randomInt = Random::getSystemRandom().nextInt (all.length());
                result += all.substring (randomInt, randomInt + 1);
            }
            return result;
        };
        
        static const double percChange (const double& oldValue, const double& newValue)
        {
            return ((newValue - oldValue) / oldValue) * 100.0;
        }
        
        template <class T>
        static const bool calcIfInclineForX (const Array <T> values, const int numSteps)
        {
            if (numSteps > values.size() - 1) throw;
            
            for (int i = 0; i < numSteps; ++i)
            {
                if (values.getUnchecked (i) <= values.getUnchecked (i + 1))
                {
                    return false;
                };
            };
        
            return true;
        }
};
