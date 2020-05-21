# SubSynth

The SubSynth is a simple synthesizer with eleven different oscillator voices implemented througha Juce Audio Plugin.   
The code is divided into the PluginEditor,  the PluginProcessor and many other classes implemented by header files and  cpp files.
The main components of this synthesizer are:
- a Synthesiser class object calledmySynth;
- elevenSythesiserSound/SynthesiserVoiceclasses;
- a multi typefilter;
- a virtual midi keyboard;
- other sound controlling elements such as an ADSR, the pitch bend, the mod bend, etc.

Those elements are controlled by :
- the Oscillator GUI;
- the Envelope GUI;
- the Filter GUI;
- Master gain slider, found in the right part of the app;
- Pitch bend extension slider, Pitch bend slider and Modulation bend slider next to the keyboard.

