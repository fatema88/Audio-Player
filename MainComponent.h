#pragma once
#include "JuceHeader.h"
#include "PlayerGUI.h"

class MainComponent : public juce::AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void resized() override;
    bool isMixing() const { return mixingEnabled; }

private:
    PlayerGUI player1;
    PlayerGUI player2;

    juce::Slider mixVol1, mixVol2;
    juce::ToggleButton mixToggle;
    juce::Label mixLabel;
    bool mixingEnabled = false;

    void mixTracks(const juce::AudioSourceChannelInfo& bufferToFill);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
