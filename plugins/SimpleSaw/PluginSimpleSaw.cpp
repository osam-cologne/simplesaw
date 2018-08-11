/*
 * Simple Saw audio efffect based on DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2018 Christopher Arndt <info@chrisarndt.de>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <math.h>

#include "PluginSimpleSaw.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

PluginSimpleSaw::PluginSimpleSaw()
    : Plugin(paramCount, 1, 0)  // paramCount params, 1 program(s), 0 states
{
    osc = sawOsc();
    loadProgram(0);
}

// -----------------------------------------------------------------------
// Init

void PluginSimpleSaw::initParameter(uint32_t index, Parameter& parameter) {
    if (index >= paramCount)
        return;

    parameter.ranges.min = 20.0f;
    parameter.ranges.max = 10000.0f;
    parameter.ranges.def = 440.0f;
    parameter.hints = kParameterIsAutomable | kParameterIsLogarithmic;

    switch (index) {
        case paramFrequency:
            parameter.name = "Frequence";
            parameter.symbol = "frequency";
            break;
    }
}

/**
  Set the name of the program @a index.
  This function will be called once, shortly after the plugin is created.
*/
void PluginSimpleSaw::initProgramName(uint32_t index, String& programName) {
    switch (index) {
        case 0:
            programName = "Default";
            break;
    }
}

// -----------------------------------------------------------------------
// Internal data

/**
  Optional callback to inform the plugin about a sample rate change.
*/
void PluginSimpleSaw::sampleRateChanged(double newSampleRate) {
    osc->setFrequency(fParams[paramFrequency] / newSampleRate);
}

/**
  Get the current value of a parameter.
*/
float PluginSimpleSaw::getParameterValue(uint32_t index) const {
    return fParams[index];
}

/**
  Change a parameter value.
*/
void PluginSimpleSaw::setParameterValue(uint32_t index, float value) {
    fParams[index] = value;

    switch (index) {
        case paramFrequency:
            osc->setFrequency(value / getSampleRate());
            break;
    }
}

/**
  Load a program.
  The host may call this function from any context,
  including realtime processing.
*/
void PluginSimpleSaw::loadProgram(uint32_t index) {
    switch (index) {
        case 0:
            setParameterValue(paramFrequency, 440.0f);
            break;
    }
}

// -----------------------------------------------------------------------
// Process

void PluginSimpleSaw::activate() {
    // plugin is activated
}

void PluginSimpleSaw::run(const float**, float** outputs, uint32_t frames) {
    // get the left and right audio outputs
    float* const outL = outputs[0];
    float* const outR = outputs[1];

    // output the oscillator waveform
    for (uint32_t i=0; i < frames; ++i) {
        outL[i] = osc->getOutput() * 0.8;
        outR[i] = osc->getOutput() * 0.8;
        osc->updatePhase();
    }
}

// -----------------------------------------------------------------------

Plugin* createPlugin() {
    return new PluginSimpleSaw();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
