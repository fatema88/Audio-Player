
#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void resized() override;
    void paint(juce::Graphics& g) override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    

private:
    PlayerAudio playerAudio;

    double aPoint = 0.0;
    double bPoint = 0.0;

    juce::Slider positionSlider;
    juce::Label posLabel{
        "Position","00:00"
    };
    
    juce::TextButton loadButton{ "Load File" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton loopButton{ "Loop" };
    juce::TextButton pauseButton{ "Pause" };
    juce::TextButton playButton{ "Play" };
    juce::TextButton goToStartButton{ "<<" };
    juce::TextButton goToEndButton{ ">>" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton setAButton{ "Set A" };
    juce::TextButton setBButton{ "Set B" };
    juce::TextButton clearABButton{ "clear AB" };


    bool isMuted = false;
    float previousVol = 0.5f;



    juce::Slider volumeSlider;
    std::unique_ptr<juce::FileChooser> fileChooser;
    bool isLooping = false;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void pauseButtonClicked();
    void goToStartButtonClicked();
    void goToEndButtonClicked();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
