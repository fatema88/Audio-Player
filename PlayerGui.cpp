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

    positionSlider.setRange(0.0, 1.0);
    positionSlider.addListener(this);
    addAndMakeVisible(positionSlider);

    muteButton.addListener(this);
    addAndMakeVisible(muteButton);

    loopButton.setButtonText("Loop: OFF");
    muteButton.setButtonText("Mute");

    addAndMakeVisible(setAButton);
    addAndMakeVisible(setBButton);
    addAndMakeVisible(clearABButton);
    setAButton.onClick = [this]() {
        aPoint = playerAudio.getPosition();
        };
    setBButton.onClick = [this]() {
        bPoint = playerAudio.getPosition();
        if (aPoint != bPoint) {
            playerAudio.setLoopPoints(aPoint, bPoint);
            playerAudio.setPosition(aPoint);
        }
        };
    clearABButton.onClick = [this]() {
        playerAudio.clearLoopPoints();
        aPoint = 0.0;
        bPoint = 0.0;
        };

    isMuted = false;
    isLooping = false;
    previousVol = 0.5f;

    setSize(600, 250);
}

PlayerGUI::~PlayerGUI() {}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void PlayerGUI::resized()
{
    int y = 20;

    loadButton.setBounds(20, y, 100, 40);
    playButton.setBounds(130, y, 80, 40);
    pauseButton.setBounds(220, y, 80, 40);
    stopButton.setBounds(310, y, 80, 40);
    restartButton.setBounds(400, y, 80, 40);
    loopButton.setBounds(490, y, 100, 40);
    setAButton.setBounds(20, 240, 80, 30);
    setBButton.setBounds(120, 240, 80, 30);
    clearABButton.setBounds(220, 240, 100, 30);

    y += 60;
    positionSlider.setBounds(20, y, getWidth() - 40, 30);

    y += 50;
    goToStartButton.setBounds(20, y, 100, 40);
    goToEndButton.setBounds(130, y, 100, 40);

    y += 60;
    volumeSlider.setBounds(20, y, getWidth() - 150, 30);
    muteButton.setBounds(getWidth() - 120, y, 100, 40);
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
    if (slider == &positionSlider) {
        double newPos = slider->getValue() * playerAudio.getLength();
        playerAudio.setPosition(newPos);
    }
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
