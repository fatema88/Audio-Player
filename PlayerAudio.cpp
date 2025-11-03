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
    reverb.setSampleRate(sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    transportSource.getNextAudioBlock(bufferToFill);

    if (reverbEnabled && bufferToFill.buffer->getNumChannels() >= 2)
    {
        juce::Reverb::Parameters reverbParams;
        reverbParams.roomSize = reverbLevel;
        reverbParams.damping = 0.5f;
        reverbParams.wetLevel = reverbLevel * 0.8f;
        reverbParams.dryLevel = 1.0f - (reverbLevel * 0.3f);
        reverbParams.width = 1.0f;
        reverbParams.freezeMode = 0.0f;

        reverb.setParameters(reverbParams);
        reverb.processStereo(bufferToFill.buffer->getWritePointer(0),
            bufferToFill.buffer->getWritePointer(1),
            bufferToFill.numSamples);
    }

    if (ABloop) {
        double CurrentPos = transportSource.getCurrentPosition();
        if (CurrentPos >= loopEnd) {
            transportSource.setPosition(loopStart);
        }
    }
}

void PlayerAudio::releaseResources()
{
    transportSource.releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& file)
{
    currentFile = file;

    if (auto* reader = formatManager.createReaderFor(file))
    {
        transportSource.stop();
        transportSource.setSource(nullptr);
        readerSource.reset();

        readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
        readerSource->setLooping(false);

        transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);
        transportSource.setLooping(false);

        return true;
    }
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

void PlayerAudio::setPosition(double pos)
{
    transportSource.setPosition(pos);
}

double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getLength() const
{
    return transportSource.getLengthInSeconds();
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

void PlayerAudio::setLoopPoints(double start, double end) {
    loopStart = start;
    loopEnd = end;
    ABloop = true;
    transportSource.setPosition(start);
}

void PlayerAudio::clearLoopPoints() {
    ABloop = false;
}

void PlayerAudio::setReverbLevel(float level)
{
    reverbLevel = juce::jlimit(0.0f, 1.0f, level);
}

void PlayerAudio::setReverbEnabled(bool enabled)
{
    reverbEnabled = enabled;
}



juce::StringArray PlayerAudio::getMetadata() const
{
    juce::StringArray metadata;

    if (!currentFile.existsAsFile())
    {
        metadata.add("No file loaded");
        return metadata;
    }


    metadata.add("File Name: " + currentFile.getFileName());
    metadata.add("File Size: " + juce::String(currentFile.getSize() / 1024) + " KB");
    metadata.add("Duration: " + getFormattedDuration());

    if (!readerSource)
    {
        metadata.add("Cannot read audio metadata");
        return metadata;
    }

    auto* reader = readerSource->getAudioFormatReader();
    if (!reader)
    {
        metadata.add("Cannot read audio metadata");
        return metadata;
    }


    metadata.add("Sample Rate: " + juce::String(reader->sampleRate) + " Hz");
    metadata.add("Channels: " + juce::String(reader->numChannels));
    metadata.add("Bit Depth: " + juce::String(reader->bitsPerSample));
    metadata.add("Total Samples: " + juce::String(reader->lengthInSamples));


    auto& allMetadata = reader->metadataValues;

    juce::String title = allMetadata.getValue("Title", "");
    juce::String artist = allMetadata.getValue("Artist", "");
    juce::String album = allMetadata.getValue("Album", "");
    juce::String year = allMetadata.getValue("Year", "");
    juce::String genre = allMetadata.getValue("Genre", "");
    juce::String comment = allMetadata.getValue("Comment", "");


    if (title.isNotEmpty())
    {
        metadata.add("Title: " + title);
    }
    else
    {
  
        metadata.add("Title: " + currentFile.getFileNameWithoutExtension());
    }

    if (artist.isNotEmpty())
    {
        metadata.add("Artist: " + artist);
    }
    else
    {
        metadata.add("Artist: Unknown Artist");
    }

    if (album.isNotEmpty()) metadata.add("Album: " + album);
    if (year.isNotEmpty()) metadata.add("Year: " + year);
    if (genre.isNotEmpty()) metadata.add("Genre: " + genre);
    if (comment.isNotEmpty()) metadata.add("Comment: " + comment);


    if (title.isEmpty() && artist.isEmpty() && album.isEmpty())
    {
        metadata.add("No embedded metadata found in file");
        metadata.add("Displaying basic file information");
    }

    return metadata;
}

juce::String PlayerAudio::getTitle() const
{
    if (!readerSource) return currentFile.getFileName();

    auto* reader = readerSource->getAudioFormatReader();
    if (!reader) return currentFile.getFileName();

    auto& metadata = reader->metadataValues;
    juce::String title = metadata.getValue("Title", "");

    return title.isNotEmpty() ? title : currentFile.getFileNameWithoutExtension();
}

juce::String PlayerAudio::getArtist() const
{
    if (!readerSource) return "Unknown Artist";

    auto* reader = readerSource->getAudioFormatReader();
    if (!reader) return "Unknown Artist";

    auto& metadata = reader->metadataValues;
    juce::String artist = metadata.getValue("Artist", "");

    return artist.isNotEmpty() ? artist : "Unknown Artist";
}

juce::String PlayerAudio::getAlbum() const
{
    if (!readerSource) return "Unknown Album";

    auto* reader = readerSource->getAudioFormatReader();
    if (!reader) return "Unknown Album";

    auto& metadata = reader->metadataValues;
    return metadata.getValue("Album", "Unknown Album");
}

juce::String PlayerAudio::getFormattedDuration() const
{
    double seconds = getLength();
    if (seconds <= 0.0) return "00:00";

    int minutes = static_cast<int>(seconds) / 60;
    int secs = static_cast<int>(seconds) % 60;

    return juce::String::formatted("%02d:%02d", minutes, secs);
}


