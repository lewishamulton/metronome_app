/*
  ==============================================================================

    Metronome.h
    Created: 17 Jun 2020 12:37:58pm
    Author:  Lewis Hamilton

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Metronome : public HighResolutionTimer
{
public:
    
    Metronome();
    //public methods
    void prepareToPlay(int samplesPerBlock, double sampleRate);
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill);
    void reset();
    
    //High Resolution Timer
    void hiResTimerCallback() override;
    
private:
    int mTotalSamples { 0 };
    double mSampleRate { 0 };
    int mInterval { 0 };
    //initialised to 60 to avoid possible division errors
    int mBpm { 132 };
    int mSamplesRemaining;
    
    AudioFormatManager mFormatManager;
    
    std::unique_ptr<AudioFormatReaderSource> pMetronomeSample { nullptr }; 
    
};
