#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(player1);
    addAndMakeVisible(player2);
    addAndMakeVisible(mixVol1);
    addAndMakeVisible(mixVol2);
    addAndMakeVisible(mixToggle);
    addAndMakeVisible(mixLabel);

    mixVol1.setRange(0.0, 1.0, 0.01);
    mixVol1.setValue(0.7);
    mixVol2.setRange(0.0, 1.0, 0.01);
    mixVol2.setValue(0.7);

    mixToggle.setButtonText("Mix Mode: OFF");
    mixToggle.onClick = [this] {
        mixingEnabled = !mixingEnabled;
        mixToggle.setButtonText(mixingEnabled ? "Mix Mode: ON" : "Mix Mode: OFF");


        if (mixingEnabled)
        {
            if (player2.playlist.empty() && !player1.playlist.empty())
            {
                for (auto& file : player1.playlist)
                {
                    player2.playlist.push_back(file);
                }
                player2.currentTrackIndex = 0;
                player2.loadPlaylistFile(player2.playlist[0]);
            }
            if (!player2.playlist.empty())
            {
                if (player2.currentTrackIndex < 0)
                {
                    player2.currentTrackIndex = 0;
                    player2.loadPlaylistFile(player2.playlist[0]);
                }
                player2.play();
            }
        }
        };

    mixLabel.setText("Mixer - Player1 Volume | Player2 Volume", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);

    setSize(1100, 900);
    setAudioChannels(0, 2);
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
    bufferToFill.clearActiveBufferRegion();

    if (mixingEnabled)
    {
        mixTracks(bufferToFill);
    }
    else
    {
        if (player1.currentTrackIndex >= 0) {
            player1.getNextAudioBlock(bufferToFill);
        }
        else if (player2.currentTrackIndex >= 0) {
            player2.getNextAudioBlock(bufferToFill);
        }
    }
}

void MainComponent::mixTracks(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();

    float vol1 = (float)mixVol1.getValue();
    float vol2 = (float)mixVol2.getValue();

    if (player1.currentTrackIndex >= 0)
    {
        juce::AudioBuffer<float> tempBuffer1(bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);
        juce::AudioSourceChannelInfo tempInfo1(&tempBuffer1, 0, bufferToFill.numSamples);

        player1.getNextAudioBlock(tempInfo1);

        for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
        {
            bufferToFill.buffer->addFrom(channel, bufferToFill.startSample,
                tempBuffer1, channel, 0, bufferToFill.numSamples, vol1);
        }
    }

    if (player2.currentTrackIndex >= 0)
    {
        juce::AudioBuffer<float> tempBuffer2(bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);
        juce::AudioSourceChannelInfo tempInfo2(&tempBuffer2, 0, bufferToFill.numSamples);

        player2.getNextAudioBlock(tempInfo2);


        for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
        {
            bufferToFill.buffer->addFrom(channel, bufferToFill.startSample,
                tempBuffer2, channel, 0, bufferToFill.numSamples, vol2);
        }
    }
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
}

void MainComponent::resized()
{
    auto area = getLocalBounds();


    int mixerHeight = 80;
    int playerHeight = (getHeight() - mixerHeight) / 2;

    player1.setBounds(area.removeFromTop(playerHeight));

    auto mixerArea = area.removeFromTop(mixerHeight);
    mixLabel.setBounds(mixerArea.removeFromTop(25));

    int sliderWidth = 120;
    int spacing = 20;
    int startX = (mixerArea.getWidth() - (2 * sliderWidth + spacing + 100)) / 2;

    mixVol1.setBounds(startX, mixerArea.getY() + 10, sliderWidth, 35);
    mixToggle.setBounds(startX + sliderWidth + 10, mixerArea.getY() + 15, 100, 25);
    mixVol2.setBounds(startX + sliderWidth + 120, mixerArea.getY() + 10, sliderWidth, 35);

    player2.setBounds(area);

    // إعادة رسم المكونات الفرعية
    player1.resized();
    player2.resized();
}
