
#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio()
{
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    transportSource.getNextAudioBlock(bufferToFill);
}

void PlayerAudio::releaseResources()
{
    transportSource.releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& file)
{
    if (auto* reader = formatManager.createReaderFor(file))
    {
        transportSource.stop();
        transportSource.setSource(nullptr);
        readerSource.reset();

        readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
        readerSource->setLooping(false); 

        transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);
        transportSource.setLooping(true);
        
        return true;
    }
    juce::Logger::writeToLog("failed to load the file");
    return false;
}

void PlayerAudio::play()
{
    transportSource.start();
}

void PlayerAudio::stop()
{
    transportSource.stop();
    transportSource.setPosition(0.0);
}

void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(gain);
}
void PlayerAudio::setLooping(bool shouldLoop)
{
    if (readerSource != nullptr)
        readerSource->setLooping(shouldLoop);

    transportSource.setLooping(shouldLoop);
}
void PlayerAudio::updateLoop()
{
    if (transportSource.hasStreamFinished())
    {
        if (readerSource != nullptr && readerSource->isLooping())
        {
            transportSource.setPosition(0.0);
            transportSource.start();
        }
    }
}
void PlayerAudio::pause()
{
    if (transportSource.isPlaying())
    {
        transportSource.stop();
        isPaused = true;
    }
}

void PlayerAudio::goToStart()
{
    transportSource.setPosition(0.0);
}

void PlayerAudio::goToEnd()
{
    double length = getLength();
    if (length > 0.0)
        transportSource.setPosition(length);
}

bool PlayerAudio::isPlaying() const
{
    return transportSource.isPlaying();
}


