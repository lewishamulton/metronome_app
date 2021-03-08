/*
  ==============================================================================
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Metronome.h"

//==============================================================================

class MainComponent   : public AudioAppComponent,
                        public Slider::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponsent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

    void play();
    void stop();

    //enumerated class PlayState defining two states of a metronome
    enum class PlayState
    {
       Playing,
       Stopped
    };


    void sliderValueChanged (Slider* slider) override;

private:

    TextButton playButton { "Play" };
    TextButton stopButton { "Stop" };

    Slider bpmChanger;
    Label bpmLabel;

    // instations of possible metronome states and metronome object itself
    
    /* metronome is initally instantiated with playingState Stopped
       so it doesnt start once app is opened */
    PlayState mPlayState {PlayState::Stopped};

    Metronome mMetronome;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
