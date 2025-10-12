#pragma once

#include <JuceHeader.h>

class ChorusProcessor : public foleys::MagicProcessor
{
public:
    ChorusProcessor();
    ~ChorusProcessor() override = default;

    bool isBusesLayoutSupported (const juce::AudioProcessor::BusesLayout& layouts) const override;
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi) override;
    void releaseResources() override;

private:
    cgo::Chorus::Params parameters;
    juce::AudioProcessorValueTreeState vts;
    cgo::Chorus chorus;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChorusProcessor)
};
