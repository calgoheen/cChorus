#include "ChorusProcessor.h"

ChorusProcessor::ChorusProcessor()
    : foleys::MagicProcessor (BusesProperties().withInput ("Input", juce::AudioChannelSet::stereo(), true)
                                               .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
    vts (*this, nullptr, "PARAMS", { parameters.type.release(),
                                     parameters.rate.release(),
                                     parameters.amount.release(),
                                     parameters.feedback.release(),
                                     parameters.flipFeedback.release(),
                                     parameters.width.release(),
                                     parameters.output.release(),
                                     parameters.mix.release() }),
    chorus (parameters)
{
    magicState.setGuiValueTree (BinaryData::layout_xml, BinaryData::layout_xmlSize);
}

bool ChorusProcessor::isBusesLayoutSupported (const juce::AudioProcessor::BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainInputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}

void ChorusProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    chorus.prepareToPlay (sampleRate, samplesPerBlock, getMainBusNumOutputChannels());
}

void ChorusProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;

    if (getMainBusNumInputChannels() == 1 && getMainBusNumOutputChannels() == 2)
        buffer.copyFrom (1, 0, buffer.getReadPointer (0), buffer.getNumSamples());
    
    chorus.processBlock (buffer, midi, getPlayHead());
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChorusProcessor();
}
