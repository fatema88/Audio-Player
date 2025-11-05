#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::ComboBox::Listener,
    public juce::ListBoxModel,
    public juce::Timer
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void resized() override;
    void paint(juce::Graphics& g) override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    void updateUI();

    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g,
        int width, int height, bool rowIsSelected) override;
    void listBoxItemClicked(int row, const juce::MouseEvent&) override;
    void listBoxItemDoubleClicked(int row, const juce::MouseEvent&) override;
    juce::Component* refreshComponentForRow(int rowNumber, bool isRowSelected,
        juce::Component* existingComponentToUpdate) override;

    void timerCallback() override;
    std::vector<juce::File> playlist;
    int currentTrackIndex = -1;
    void loadTrack(int trackIndex);
    void setGain(float gain) { playerAudio.setGain(gain); }
    void play() { playerAudio.play(); }
    void stop() { playerAudio.stop(); }
    bool hasTracks() const { return !playlist.empty(); }

private:
    PlayerAudio playerAudio;

    double aPoint = 0.0;
    double bPoint = 0.0;

    juce::Slider positionSlider;
    juce::Label posLabel{ "Position", "00:00" };
    juce::Label durationLabel{ "Duration", "00:00" };

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
    juce::TextButton clearABButton{ "Clear AB" };
    juce::ToggleButton reverbToggle{ "Reverb" };
    juce::Slider reverbSlider;
    juce::Slider speedSlider;
    juce::Label reverbLabel{ "Reverb Level", "Reverb:" };
    juce::Label speedLabel{ "Speed", "Speed:" };

    juce::TextEditor metadataDisplay;

    juce::ListBox playlistBox;
    juce::TextButton nextButton{ "Next >" };
    juce::TextButton prevButton{ "< Prev" };
    juce::TextButton removeButton{ "Remove" };
    juce::TextButton clearButton{ "Clear All" };

    bool isMuted = false;
    float previousVol = 0.5f;
    juce::Slider volumeSlider;
    std::unique_ptr<juce::FileChooser> fileChooser;
    bool isLooping = false;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    void pauseButtonClicked();
    void goToStartButtonClicked();
    void goToEndButtonClicked();
    void displayMetadata();
    void loadPlaylistFile(const juce::File& file);
    void updatePlaylistDisplay();
    void removeSelectedTrack();
    void clearPlaylist();
    void updatePositionDisplay();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
