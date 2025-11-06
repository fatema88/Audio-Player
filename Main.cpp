/*
********************************** Audio Player project *******************************
* Names                          Ids                    Distribution:
* Fatma Tarek Mostafa             20242237              *Features Number 
                                                           4,7,10
                                                        *The first bonus feature

* sara Mosaad talaat              20240230              *Features Number
                                                            2,5,8
                                                       *the second and the third bonus features

* Demiana Nabil sabry             20242117             Features Number 
                                                           3,6,9
Section Number:S21.





*Githup Link:https://github.com/fatema88/Audio-Player
* Report Link:https://drive.google.com/file/d/15A-IUeMSqmkV7TZMAH2tmh9eJxcONQWZ/view?usp=drivesdk
* video Link:https://drive.google.com/file/d/1a6Tqnli0KJZzxfFxhjpr4Qta0WWXK7gb/view?usp=drivesdk
*/
#include <JuceHeader.h>
#include "MainComponent.h"

class Application : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "Audio Player"; }
    const juce::String getApplicationVersion() override { return "1.0"; }

    void initialise(const juce::String&) override
    {
        mainWindow.reset(new MainWindow(getApplicationName()));
    }

    void shutdown() override
    {
        mainWindow = nullptr;
    }

    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name)
            : DocumentWindow(name,
                juce::Desktop::getInstance().getDefaultLookAndFeel()
                .findColour(juce::ResizableWindow::backgroundColourId),
                DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);
            setResizable(true, true);
            setResizeLimits(900, 700, 2000, 1500);
            centreWithSize(1000, 800);
            setVisible(true);
        }

        void closeButtonPressed() override
        {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }
        void resized() override
        {
            DocumentWindow::resized();
            if (auto* content = getContentComponent())
                content->resized();
        }
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(Application)

