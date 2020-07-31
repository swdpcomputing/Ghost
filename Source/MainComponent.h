#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SetExternal.h"
#include "Trader.h"
#include "Simulator.h"

class MainComponent   : public Component,
                        public Button::Listener
{
    public:
        MainComponent();
        ~MainComponent();

        void paint (Graphics&) override;
        void resized() override;

    private:
        void addComponents();
        void addListeners();
        void setComponentText();
        void buttonClicked (Button*) override;
    
        SetExternal setExternal;

        TextButton runSimulationButton;
        TextButton startTraderButton;
        TextButton cButton;
        TextButton dButton;
        TextButton eButton;
        TextButton fButton;

        Trader trader;
        Simulator simulator;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
}; 