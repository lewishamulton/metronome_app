/*
  ==============================================================================

    Metronome.cpp
    Created: 17 Jun 2020 12:37:58pm
    Author:  Lewis Hamilton

  ==============================================================================
*/

#include "Metronome.h"

Metronome::Metronome()
{
    
    //registers wav, aiff to format manager
    mFormatManager.registerBasicFormats();
    
    //myFile locates the file, mySamples loads the file
    File myFile =  File::getSpecialLocation(File::SpecialLocationType::userDesktopDirectory);
    auto mySamples = myFile.findChildFiles(File::TypesOfFileToFind::findFiles, true, "wood_block_h.aif");
    
    jassert(mySamples[0].exists());
    
    auto formatReader =  mFormatManager.createReaderFor(mySamples[0]);
    
    pMetronomeSample.reset(new AudioFormatReaderSource (formatReader, true));
    
    mInterval = 60.0 / mBpm * mSampleRate;
    
}

void Metronome::prepareToPlay(int samplesPerBlock, double sampleRate)
{
    mSampleRate = sampleRate;
    mInterval = 60.0 / mBpm * mSampleRate;
    //abstract method with abitrary number
    HighResolutionTimer::startTimer(60.0);
    
    if (pMetronomeSample != nullptr)
    {
        pMetronomeSample->prepareToPlay(samplesPerBlock, sampleRate);
        DBG("File loaded!");
    }

}

void Metronome::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    
    auto bufferSize = bufferToFill.numSamples;
    mTotalSamples += bufferSize;
    
    //expression gives remainder of how many samples are left to process before
    //we hear the next downbeat
    mSamplesRemaining = mTotalSamples % mInterval;
    
    
    //if samples remaining are greater than or equal to interval then want to click
    if ((mSamplesRemaining + bufferSize) >= mInterval)
    {
        
        
        const auto timeToStartPlaying = mInterval - mSamplesRemaining;
        //sets sample back to beginning
        pMetronomeSample->setNextReadPosition(0);
        for (auto sample = 0; sample < bufferSize; sample ++)
        {
            
            if (sample == timeToStartPlaying)
            {
                //plays the sound
                pMetronomeSample->getNextAudioBlock(bufferToFill);

            }
        }
       
    }
    if (pMetronomeSample->getNextReadPosition() != 0)
    {
        pMetronomeSample->getNextAudioBlock(bufferToFill);
    }
}

void Metronome::reset()
{
    
    mTotalSamples = 0; 
}

//May need if we want to let user change bpm
void Metronome::hiResTimerCallback()
{
    
    mInterval = 60.0 / mBpm * mSampleRate;
}

void Metronome::setCurrentBPM(int newBpm)
{
     mBpm = newBpm;
}

