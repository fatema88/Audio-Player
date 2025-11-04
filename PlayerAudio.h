#pragma once

#include <JuceHeader.h>

class PlayerAudio
{
public:
    PlayerAudio();
    ~PlayerAudio();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    void setLooping(bool shouldLoop);
    void updateLoop();
    void setLoopPoints(double start, double end);
    void clearLoopPoints();
    double loopStart = 0.0;
    double loopEnd = 0.0;
    bool ABloop = false;
    double aPoint = 0.0;
    double bPoint = 0.0;
    void setSpeed(double speed);
    bool loadFile(const juce::File& file);
    void play();
    void stop();
    void setGain(float gain);
    void setPosition(double pos);
    double getPosition() const;
    double getLength() const;
    void pause();
    void goToStart();
    void goToEnd();
    bool isPlaying() const;

    juce::StringArray getMetadata() const;
    juce::String getTitle() const;
    juce::String getArtist() const;
    juce::String getAlbum() const;
    juce::String getFormattedDuration() const;

    void setReverbLevel(float level);
    void setReverbEnabled(bool enabled);

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::File currentFile;
    bool isPaused = false;

    juce::Reverb reverb;
    bool reverbEnabled = false;
    float reverbLevel = 0.0f;
    juce::String extractArtistFromFilename(const juce::String& filename) const;
};


