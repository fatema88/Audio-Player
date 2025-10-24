#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    for (auto* btn : { &loadButton, &restartButton, &stopButton, &loopButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    for (auto* btn : { &playButton, &pauseButton, &goToStartButton, &goToEndButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    muteButton.addListener(this);
    addAndMakeVisible(muteButton);

    loopButton.setButtonText("Loop: OFF");
    muteButton.setButtonText("Mute");

    isMuted = false;
    isLooping = false;
    previousVol = 0.5f;

    setSize(600, 250);
}

PlayerGUI::~PlayerGUI() {}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(20, y, 100, 40);
    playButton.setBounds(140, y, 80, 40);
    pauseButton.setBounds(240, y, 80, 40);
    stopButton.setBounds(340, y, 80, 40);
    restartButton.setBounds(440, y, 80, 40);
    loopButton.setBounds(540, y, 100, 40);

    int y2 = y + 60;
    goToStartButton.setBounds(20, y2, 100, 40);
    goToEndButton.setBounds(140, y2, 100, 40);

    volumeSlider.setBounds(20, 150, getWidth() - 40, 30);
    muteButton.setBounds(20, 200, 100, 40);
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            ".wav;.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    playerAudio.loadFile(file);
                    playerAudio.play();
                }
            });
    }

    if (button == &restartButton)
        playerAudio.play();

    if (button == &playButton)
        playerAudio.play();

    if (button == &pauseButton)
        pauseButtonClicked();

    if (button == &stopButton)
        playerAudio.stop();

    if (button == &goToStartButton)
        goToStartButtonClicked();

    if (button == &goToEndButton)
        goToEndButtonClicked();

    if (button == &loopButton)
    {
        isLooping = !isLooping;
        loopButton.setButtonText(isLooping ? "Loop: ON" : "Loop: OFF");
        playerAudio.setLooping(isLooping);
    }

    if (button == &muteButton)
    {
        if (!isMuted)
        {
            previousVol = volumeSlider.getValue();
            playerAudio.setGain(0.0f);
            volumeSlider.setValue(0.0);
            muteButton.setButtonText("Unmute");
            isMuted = true;
        }
        else
        {
            playerAudio.setGain(previousVol);
            volumeSlider.setValue(previousVol);
            muteButton.setButtonText("Mute");
            isMuted = false;
        }
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());
}

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
    playerAudio.updateLoop();
}

void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}

void PlayerGUI::pauseButtonClicked()
{
    playerAudio.pause();
}

void PlayerGUI::goToStartButtonClicked()
{
    playerAudio.goToStart();
}

void PlayerGUI::goToEndButtonClicked()
{
    playerAudio.goToEnd();
}
