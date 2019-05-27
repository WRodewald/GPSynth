#include "GPSynthVoice.h"


bool GPSynthSound::appliesToNote(int midiNoteNumber)
{
	return true;
}

bool GPSynthSound::appliesToChannel(int midiChannel)
{
	return true;
}



GPSynthVoice::GPSynthVoice()
{
}

void GPSynthVoice::prepare(float sampleRate, float blockSize)
{
	this->sampleRate = sampleRate;
	this->blockSize  = blockSize;

	envelope.setSampleRate(sampleRate);
}

bool GPSynthVoice::canPlaySound(SynthesiserSound * sound)
{
	if (auto gpSound = dynamic_cast<GPSynthSound*>(sound))
	{
		return true;
	}
	return false;
}

void GPSynthVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound * sound, int currentPitchWheelPosition)
{
	float freq = 440.f * std::pow(2.f, (midiNoteNumber - 69.f) / 12.f);
	float freqNorm = freq / sampleRate;
	oscL.setFrequency(freqNorm);
	oscR.setFrequency(freqNorm);

	float updtFreq = 10;

	oscL.setProgressionFrequency(updtFreq / 44100.f);
	oscR.setProgressionFrequency(updtFreq / 44100.f);
	
	envelope.setParameters(ADSR::Parameters());

	envelope.noteOn();
}

void GPSynthVoice::stopNote(float velocity, bool allowTailOff)
{
	// todo impl
	envelope.noteOff();
}

void GPSynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
}

void GPSynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
}

void GPSynthVoice::renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	auto chL = outputBuffer.getWritePointer(0) + startSample;
	auto chR = outputBuffer.getWritePointer(1) + startSample;

	for (int i = 0; i < numSamples; i++)
	{
		float vol = envelope.getNextSample();

		chL[i] += vol * oscL.tick();
		chR[i] += vol * oscR.tick();
	}
}
