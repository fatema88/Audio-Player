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

    for (auto* btn : { &nextButton, &prevButton, &removeButton, &clearButton, &setAButton, &setBButton, &clearABButton, &muteButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }
    
    reverbToggle.addListener(this);
    reverbToggle.setToggleState(false, juce::NotificationType::dontSendNotification);
    addAndMakeVisible(reverbToggle);

    reverbSlider.setRange(0.0, 1.0, 0.01);
    reverbSlider.setValue(0.5);
    reverbSlider.addListener(this);
    addAndMakeVisible(reverbSlider);
    addAndMakeVisible(reverbLabel);

    speedSlider.setRange(0.5, 2.0, 0.1);
    speedSlider.setValue(1.0);
    speedSlider.addListener(this);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedLabel);

    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    positionSlider.setRange(0.0, 1.0);
    positionSlider.addListener(this);
    addAndMakeVisible(positionSlider);
    addAndMakeVisible(posLabel);
    addAndMakeVisible(durationLabel);
    posLabel.setText("00:00", juce::dontSendNotification);
    durationLabel.setText("00:00", juce::dontSendNotification);

    metadataDisplay.setReadOnly(true);
    metadataDisplay.setMultiLine(true);
    metadataDisplay.setColour(juce::TextEditor::backgroundColourId, juce::Colours::darkgrey);
    metadataDisplay.setColour(juce::TextEditor::textColourId, juce::Colours::white);
    metadataDisplay.setFont(juce::FontOptions(14.0f));
    addAndMakeVisible(metadataDisplay);

    playlistBox.setModel(this);
    playlistBox.setColour(juce::ListBox::backgroundColourId, juce::Colours::darkgrey);
    playlistBox.setColour(juce::ListBox::textColourId, juce::Colours::white);
    addAndMakeVisible(playlistBox);
    loadButton.setButtonText("Load File");
    playButton.setButtonText("Play");
    pauseButton.setButtonText("Pause");
    stopButton.setButtonText("Stop");
    restartButton.setButtonText("Restart");
    loopButton.setButtonText("Loop: OFF");
    muteButton.setButtonText("Mute");
    nextButton.setButtonText("Next >");
    prevButton.setButtonText("< Prev");
    removeButton.setButtonText("Remove");
    clearButton.setButtonText("Clear All");
    setAButton.setButtonText("Set A");
    setBButton.setButtonText("Set B");
    clearABButton.setButtonText("Clear AB");
    reverbToggle.setButtonText("Reverb: OFF");
    speedLabel.setText("Speed:", juce::dontSendNotification);
    reverbLabel.setText("Reverb:", juce::dontSendNotification);

    isMuted = false;
    isLooping = false;
    previousVol = 0.5f;

    setSize(850, 700);
    displayMetadata();
    startTimer(100);
}

PlayerGUI::~PlayerGUI() {}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void PlayerGUI::resized()
{
    int y = 10;
    int margin = 15;
    int buttonWidth = 75;
    int buttonHeight = 30;
    int spacing = 8;

    loadButton.setBounds(margin, y, buttonWidth, buttonHeight);
    playButton.setBounds(margin + buttonWidth + spacing, y, buttonWidth, buttonHeight);
    pauseButton.setBounds(margin + 2 * (buttonWidth + spacing), y, buttonWidth, buttonHeight);
    stopButton.setBounds(margin + 3 * (buttonWidth + spacing), y, buttonWidth, buttonHeight);
    restartButton.setBounds(margin + 4 * (buttonWidth + spacing), y, buttonWidth, buttonHeight);
    loopButton.setBounds(margin + 5 * (buttonWidth + spacing), y, buttonWidth, buttonHeight);

    int controlButtonWidth = 75;
    int controlStartX = getWidth() - margin - (5 * controlButtonWidth + 4 * spacing);

    removeButton.setBounds(controlStartX, y, controlButtonWidth, buttonHeight);
    clearButton.setBounds(controlStartX + controlButtonWidth + spacing, y, controlButtonWidth, buttonHeight);
    setAButton.setBounds(controlStartX + 2 * (controlButtonWidth + spacing), y, controlButtonWidth, buttonHeight);
    setBButton.setBounds(controlStartX + 3 * (controlButtonWidth + spacing), y, controlButtonWidth, buttonHeight);
    clearABButton.setBounds(controlStartX + 4 * (controlButtonWidth + spacing), y, controlButtonWidth, buttonHeight);

    y += buttonHeight + 15;
    int progressSliderWidth = getWidth() - 2 * margin - 120;
    posLabel.setBounds(margin, y, 50, 20);
    positionSlider.setBounds(margin + 55, y, progressSliderWidth, 40);
    durationLabel.setBounds(margin + 55 + progressSliderWidth + 5, y, 50, 20);

    y += 45;
    int controlSliderHeight = 45;
    int controlSliderWidth = 130;

    volumeSlider.setBounds(margin, y, controlSliderWidth, controlSliderHeight);
    muteButton.setBounds(margin + controlSliderWidth + 10, y, 70, buttonHeight);
    
    int navButtonWidth = 70;
    int navStartX = (getWidth() - (4 * navButtonWidth + 3 * spacing)) / 2;

    prevButton.setBounds(navStartX, y, navButtonWidth, buttonHeight);
    nextButton.setBounds(navStartX + navButtonWidth + spacing, y, navButtonWidth, buttonHeight);
    goToStartButton.setBounds(navStartX + 2 * (navButtonWidth + spacing), y, navButtonWidth, buttonHeight);
    goToEndButton.setBounds(navStartX + 3 * (navButtonWidth + spacing), y, navButtonWidth, buttonHeight);

    y += controlSliderHeight + 15;
    speedLabel.setBounds(margin, y, 60, buttonHeight);
    speedSlider.setBounds(margin + 65, y, controlSliderWidth, controlSliderHeight);

    reverbToggle.setBounds(margin + controlSliderWidth + 85, y, 110, buttonHeight);
    reverbSlider.setBounds(margin + controlSliderWidth + 200, y, controlSliderWidth, controlSliderHeight);

    y += controlSliderHeight + 25;
    metadataDisplay.setBounds(margin, y, getWidth() - 2 * margin, 70);
   
    y += 75;
    int playlistHeight = 120;
    playlistBox.setBounds(margin, y, getWidth() - 2 * margin, playlistHeight);
}
void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select audio files...",
            juce::File{},
            "*.wav;*.mp3;*.aiff;*.flac");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode |
            juce::FileBrowserComponent::canSelectFiles |
            juce::FileBrowserComponent::canSelectMultipleItems,
            [this](const juce::FileChooser& fc)
            {
                auto results = fc.getResults();
                if (results.size() > 0)
                {
                    for (int i = 0; i < results.size(); ++i)
                    {
                        playlist.push_back(results[i]);
                    }

                    playlistBox.updateContent();

                    if (currentTrackIndex == -1)
                    {
                        currentTrackIndex = 0;
                        loadPlaylistFile(playlist[0]);
                    }
                    else
                    {
                        playlistBox.selectRow(currentTrackIndex);
                    }

                    playlistBox.repaint();
                    displayMetadata();
                }
            });
    }

    if (button == &restartButton)
    {
        playerAudio.goToStart();
        playerAudio.play();
    }

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
            previousVol = static_cast<float>(volumeSlider.getValue());
            playerAudio.setGain(0.0f);
            volumeSlider.setValue(0.0);
            muteButton.setButtonText("Unmute");
            isMuted = true;
        }
        else
        {
            playerAudio.setGain(previousVol);
            volumeSlider.setValue(static_cast<double>(previousVol));
            muteButton.setButtonText("Mute");
            isMuted = false;
        }
    }

    if (button == &reverbToggle)
    {
        bool reverbEnabled = reverbToggle.getToggleState();
        playerAudio.setReverbEnabled(reverbEnabled);
        reverbToggle.setButtonText(reverbEnabled ? "Reverb: ON" : "Reverb: OFF");
        reverbSlider.setEnabled(reverbEnabled);
    }

    if (button == &nextButton)
    {
        if (!playlist.empty())
        {
            currentTrackIndex = (currentTrackIndex + 1) % (int)playlist.size();
            loadPlaylistFile(playlist[currentTrackIndex]);
        }
    }

    if (button == &prevButton)
    {
        if (!playlist.empty())
        {
            currentTrackIndex = (currentTrackIndex - 1 + (int)playlist.size()) % (int)playlist.size();
            loadPlaylistFile(playlist[currentTrackIndex]);
        }
    }

    if (button == &removeButton)
    {
        removeSelectedTrack();
    }

    if (button == &clearButton)
    {
        clearPlaylist();
    }

    if (button == &setAButton)
    {
        aPoint = playerAudio.getPosition();
        metadataDisplay.setText("Point A set at: " + juce::String(aPoint, 2) + " seconds");
    }

    if (button == &setBButton)
    {
        bPoint = playerAudio.getPosition();
        if (aPoint != bPoint) {
            playerAudio.setLoopPoints(aPoint, bPoint);
            playerAudio.setPosition(aPoint);
            metadataDisplay.setText("A-B Loop: " + juce::String(aPoint, 2) + "s to " + juce::String(bPoint, 2) + "s");
        }
    }

    if (button == &clearABButton)
    {
        playerAudio.clearLoopPoints();
        aPoint = 0.0;
        bPoint = 0.0;
        metadataDisplay.setText("A-B Loop cleared");
    }
}

void PlayerGUI::loadTrack(int trackIndex)
{
    if (trackIndex >= 0 && trackIndex < (int)playlist.size())
    {
        currentTrackIndex = trackIndex;
        loadPlaylistFile(playlist[trackIndex]);
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());

    if (slider == &positionSlider) {
        double newPos = slider->getValue() * playerAudio.getLength();
        playerAudio.setPosition(newPos);
        updatePositionDisplay();
    }

    if (slider == &reverbSlider) {
        playerAudio.setReverbLevel((float)reverbSlider.getValue());
    }

    if (slider == &speedSlider) {
        playerAudio.setSpeed(speedSlider.getValue());
        displayMetadata();
    }
}

void PlayerGUI::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    (void)comboBoxThatHasChanged;
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
    updatePositionDisplay();
}

void PlayerGUI::goToEndButtonClicked()
{
    playerAudio.goToEnd();
    updatePositionDisplay();
}

void PlayerGUI::displayMetadata()
{
    if (currentTrackIndex >= 0 && currentTrackIndex < (int)playlist.size())
    {
        auto metadata = playerAudio.getMetadata();
        juce::String metadataText;

        metadataText += "=== NOW PLAYING ===\n\n";
        metadataText += "Track: " + juce::String(currentTrackIndex + 1) + " of " + juce::String((int)playlist.size()) + "\n\n";

        for (auto& line : metadata)
        {
            metadataText += line + "\n";
        }

        metadataDisplay.setText(metadataText);
    }
    else
    {
        metadataDisplay.setText(
            "No audio file loaded\n\n"
            "Click 'Load File' to select audio files\n"
            "Supported formats: WAV, MP3, AIFF, FLAC\n\n"
            "Playlist Features:\n"
            "- Load multiple files (select multiple with Ctrl+Click)\n"
            "- Navigate with Next/Prev buttons\n"
            "- Click on any track in the playlist\n"
            "- Remove tracks or clear entire playlist\n"
            "- Currently loaded: " + juce::String((int)playlist.size()) + " tracks"
        );
    }
}

void PlayerGUI::loadPlaylistFile(const juce::File& file)
{
    if (playerAudio.loadFile(file))
    {
        playerAudio.play();
        displayMetadata();
        playlistBox.selectRow(currentTrackIndex);
        playlistBox.repaint();
        updatePositionDisplay();
    }
}

void PlayerGUI::updatePlaylistDisplay()
{
    playlistBox.updateContent();
}

void PlayerGUI::updateUI()
{
    if (playerAudio.getLength() > 0.0)
    {
        double currentPos = playerAudio.getPosition();
        double length = playerAudio.getLength();
        positionSlider.setValue(currentPos / length, juce::dontSendNotification);
        updatePositionDisplay();
    }
}
void PlayerGUI::updatePositionDisplay()
{
    posLabel.setText(playerAudio.getFormattedPosition(), juce::dontSendNotification);
    durationLabel.setText(playerAudio.getFormattedDuration(), juce::dontSendNotification);
}

void PlayerGUI::removeSelectedTrack()
{
    int selectedRow = playlistBox.getSelectedRow();
    if (selectedRow >= 0 && selectedRow < (int)playlist.size())
    {
        playlist.erase(playlist.begin() + selectedRow);

        if (currentTrackIndex == selectedRow)
        {
            currentTrackIndex = -1;
            playerAudio.stop();
            displayMetadata();
        }
        else if (currentTrackIndex > selectedRow)
        {
            currentTrackIndex--;
        }

        playlistBox.updateContent();

        if (currentTrackIndex >= 0)
        {
            playlistBox.selectRow(currentTrackIndex);
        }
    }
}

void PlayerGUI::clearPlaylist()
{
    playlist.clear();
    currentTrackIndex = -1;
    playerAudio.stop();
    playlistBox.updateContent();
    displayMetadata();
}

int PlayerGUI::getNumRows()
{
    return (int)playlist.size();
}

void PlayerGUI::paintListBoxItem(int rowNumber, juce::Graphics& g,
    int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::lightblue);
        g.setColour(juce::Colours::darkblue);
    }
    else
    {
        g.fillAll(rowNumber % 2 == 0 ? juce::Colours::darkgrey : juce::Colours::grey);
        g.setColour(juce::Colours::white);
    }

    g.setFont(juce::FontOptions(14.0f));

    if (rowNumber < (int)playlist.size())
    {
        juce::String trackNumber = juce::String(rowNumber + 1) + ".";
        juce::String fileName = playlist[rowNumber].getFileName();

        if (rowNumber == currentTrackIndex)
        {
            fileName = "> " + fileName;
            g.setColour(juce::Colours::yellow);
        }

        g.drawText(trackNumber, 10, 0, 30, height, juce::Justification::left);
        g.drawText(fileName, 45, 0, width - 45, height, juce::Justification::left);
    }
}

void PlayerGUI::listBoxItemClicked(int row, const juce::MouseEvent&)
{
    if (row >= 0 && row < (int)playlist.size())
    {
        currentTrackIndex = row;
        loadPlaylistFile(playlist[row]);
    }
}

void PlayerGUI::listBoxItemDoubleClicked(int row, const juce::MouseEvent& event)
{
    listBoxItemClicked(row, event);
}

juce::Component* PlayerGUI::refreshComponentForRow(int rowNumber, bool isRowSelected,
    juce::Component* existingComponentToUpdate)
{
    (void)rowNumber;
    (void)isRowSelected;
    (void)existingComponentToUpdate;
    return nullptr;
}

void PlayerGUI::timerCallback()
{
    updateUI();

    if (currentTrackIndex >= 0 && playlistBox.getSelectedRow() != currentTrackIndex)
    {
        playlistBox.selectRow(currentTrackIndex);
    }

    playlistBox.repaint();
}
