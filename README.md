![logo](https://github.com/David-Badiane/Juce-Midi-Synthesiser/blob/master/Synthesizer/images/whatever.png)

The SubSynth is a simple synthesizer with eleven different oscillator voices implemented through Juce.   
The code is divided into the PluginEditor,  the PluginProcessor, the two main objects, and all the other classes controlled by these first two.
The main components of this synthesizer are:
- a Synthesiser class object called mySynth;
- eleven SythesiserSound/SynthesiserVoice class objects;
- a multi-type filter;
- ADSR volume envelope
- pitch/mod FXs

Those elements are controlled by :
- the Oscillator GUI;
- the Envelope GUI;
- the Filter GUI;
- Master gain slider, found in the right part of the app;
- Pitch and mod wheels
- Pitch amount slider
- a virtual midi keyboard;

From **options** it is possible to acquire external midi inputs.

<img src = "Synthesizer/images/interface.PNG" width = "1000" >
