#pragma once

#include "JuceHeader.h"

#include "GPContinuousOsc.h"


class GPSynthSound : public SynthesiserSound
{
public:
	virtual bool appliesToNote(int midiNoteNumber) override;
	virtual bool appliesToChannel(int midiChannel) override;
};


class GPSynthVoice : public SynthesiserVoice
{
public:
	GPSynthVoice();

	void prepare(float sampleRate, float blockSize);

public:
	virtual bool canPlaySound(SynthesiserSound *) override;
	virtual void startNote(int midiNoteNumber, float velocity, SynthesiserSound * sound, int currentPitchWheelPosition) override;
	virtual void stopNote(float velocity, bool allowTailOff) override;
	virtual void pitchWheelMoved(int newPitchWheelValue) override;
	virtual void controllerMoved(int controllerNumber, int newControllerValue) override;
	virtual void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;


private:

	GP::ContinuousOsc oscL{ 256 };
	GP::ContinuousOsc oscR{ 256 };
	
	ADSR envelope;

	float sampleRate{ 44100 };
	float blockSize{ 0 };
};