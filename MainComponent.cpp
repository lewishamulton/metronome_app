/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    playButton.setRadioGroupId(1);
    playButton.setToggleState(false, NotificationType::dontSendNotification);
    playButton.onClick = [this]() { play(); };

    addAndMakeVisible(playButton);
    
    stopButton.setRadioGroupId(1);
    playButton.setToggleState(true, NotificationType::dontSendNotification);
    stopButton.onClick = [this]() { stop(); };
    
    addAndMakeVisible(stopButton);
    
    bpmChanger.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    bpmChanger.setColour(Slider::backgroundColourId, Colours::ivory);
    bpmChanger.setRange(30, 300, 1);
    bpmChanger.setTextBoxStyle(Slider::TextBoxBelow, true, 40, 20);
    
   /* bpmLabel.setFont(15.0f);
    bpmLabel.setText("BPM", NotificationType::dontSendNotification);
    bpmLabel.setJustificationType(Justification::centredLeft);
    bpmLabel.attachToComponent(&bpmChanger, false); */
    
    addAndMakeVisible(bpmChanger);
    
    

    
    
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

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    
    FlexBox flexBox;
    flexBox.flexDirection = FlexBox::Direction::row;
    flexBox.flexWrap = FlexBox::Wrap::wrap;
    flexBox.alignContent = FlexBox::AlignContent::stretch;
    
    flexBox.items.add(FlexItem(100,100, playButton));
    flexBox.items.add(FlexItem(100,100, stopButton));
    flexBox.items.add(FlexItem(200,100, bpmChanger)); 
    flexBox.performLayout(bounds);
}
 

void MainComponent::sliderValueChanged(Slider *slider)
{
    if(slider == &bpmChanger){
        mMetronome.setCurrentBPM(bpmChanger.getValue()); 
    }
}
