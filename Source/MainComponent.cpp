#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize (600, 400);
    
    addComponents();
    addListeners();
    setComponentText();
}

MainComponent::~MainComponent()
{
}

void MainComponent::addComponents()
{
    addAndMakeVisible (runSimulationButton);
    addAndMakeVisible (startTraderButton);
    addAndMakeVisible (cButton);
    addAndMakeVisible (dButton);
    addAndMakeVisible (eButton);
    addAndMakeVisible (fButton);
}

void MainComponent::addListeners()
{
    runSimulationButton.addListener (this);
    startTraderButton.addListener (this);
    cButton.addListener (this);
    dButton.addListener (this);
    eButton.addListener (this);
    fButton.addListener (this);
}

void MainComponent::setComponentText()
{
    runSimulationButton.setButtonText ("Run Simulation");
    startTraderButton.setButtonText ("Start Trader");
    cButton.setButtonText ("c");
    dButton.setButtonText ("d");
    eButton.setButtonText ("e");
    fButton.setButtonText ("f");
}

void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    runSimulationButton.setBounds (5, 5, 150, 25);
    startTraderButton.setBounds (5, 35, 150, 25);
    cButton.setBounds (5, 65, 150, 25);
    dButton.setBounds (5, 95, 150, 25);
    eButton.setBounds (5, 125, 150, 25);
    fButton.setBounds (5, 155, 150, 25);
}

void MainComponent::buttonClicked (Button* button)
{
    if      (button == &runSimulationButton) simulator.runAction ("runSimulation");
    else if (button == &startTraderButton) trader.runAction ("startTrader");
    else if (button == &cButton) DBG ("c");
    else if (button == &dButton) DBG ("d");
    else if (button == &eButton) DBG ("e");
    else if (button == &fButton) DBG ("f");
}