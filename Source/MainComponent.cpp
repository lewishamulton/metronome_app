/*
  ==============================================================================
  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{

    // sets up playButton object with relevant attributes
    playButton.setRadioGroupId(1);
    playButton.setToggleState(false, NotificationType::dontSendNotification);

    // when play button is clicked it sets the enumerated class playState to to Playing
    playButton.onClick = [this]() { play(); };

    addAndMakeVisible(playButton);

    // sets up stopButton object with relevant attributes
    stopButton.setRadioGroupId(1);
    playButton.setToggleState(true, NotificationType::dontSendNotification);
    stopButton.onClick = [this]() { stop(); };

    addAndMakeVisible(stopButton);

    // sets up bpmChanger slider and sets minimum bpm to 30 and maximum to 300

    bpmChanger.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    bpmChanger.setColour(Slider::backgroundColourId, Colours::ivory);
    bpmChanger.setRange(30, 300, 1);
    bpmChanger.setTextBoxStyle(Slider::TextBoxBelow, true, 40, 20);

   // optional bpm label that can be added, left out since i thought it made
   // GUI cluttered
   /* bpmLabel.setFont(15.0f);
    bpmLabel.setText("BPM", NotificationType::dontSendNotification);
    bpmLabel.setJustificationType(Justification::centredLeft);
    bpmLabel.attachToComponent(&bpmChanger, false); */

    addAndMakeVisible(bpmChanger);



    // set size of overall component
    setSize (200, 200);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::play()
{
    mPlayState = PlayState::Playing;
    sliderValueChanged(&bpmChanger);
}

void MainComponent::stop()
{
    mPlayState = PlayState::Stopped;
    mMetronome.reset();

}
//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    mMetronome.prepareToPlay(samplesPerBlockExpected,sampleRate);

}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{

    bufferToFill.clearActiveBufferRegion();

    //has the play button been pressed so mPlayState object been set to Playing
    if (mPlayState == PlayState::Playing)
    {
        mMetronome.getNextAudioBlock(bufferToFill);
    }
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

}

void MainComponent::resized()
{
    // returns our components bounds
    Rectangle<int> bounds = getLocalBounds();

    // creates a flexbox, a resizable box that realigns items within it, in our component
    FlexBox flexBox;
    flexBox.flexDirection = FlexBox::Direction::row;
    flexBox.flexWrap = FlexBox::Wrap::wrap;
    flexBox.alignContent = FlexBox::AlignContent::stretch;

    // adds GUI items, buttons, sliders into the box with ints representing their
    // size in pixels
    flexBox.items.add(FlexItem(100,100, playButton));
    flexBox.items.add(FlexItem(100,100, stopButton));
    flexBox.items.add(FlexItem(200,100, bpmChanger));
    // creates flexbox using boundaires of main component i.e start as 200x200 box
    flexBox.performLayout(bounds);
}


void MainComponent::sliderValueChanged(Slider *slider)
{
    if(slider == &bpmChanger){
        mMetronome.setCurrentBPM(bpmChanger.getValue());
    }
}
