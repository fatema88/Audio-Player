
#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(player1);
    addAndMakeVisible(player2);
    setSize(800, 400);
    setAudioChannels(0, 2);
    startTimer(30);
	


}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    player1.getNextAudioBlock(bufferToFill);
    player1.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player1.releaseResources();

}

void MainComponent::resized()
{
    auto area = getLocalBounds();
    player1.setBounds(area.removeFromTop(getHeight()/2));
    player2.setBounds(area);

}


void MainComponent::timerCallback()
{


}
