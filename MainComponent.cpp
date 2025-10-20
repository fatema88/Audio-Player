#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(player1);
    setSize(500, 250);
    setAudioChannels(0, 2);

    // 👇 السطر الجديد ده بيخلي JUCE يحدث الصوت باستمرار لو في loop
    startTimerHz(30); // 30 مرة في الثانية لتحديث الحالة
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    player1.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
}

void MainComponent::resized()
{
    player1.setBounds(getLocalBounds());
}


void MainComponent::timerCallback()
{
    
    player1.getNextAudioBlock(juce::AudioSourceChannelInfo());
}
