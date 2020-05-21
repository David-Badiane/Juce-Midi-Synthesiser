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


In thePluginProcessorwe find everything that processes sound and has to be controlled, therefore weneed to put in its relative header file all the declarations of objects and variables necessary to process thesound. In thePluginEditorwe find everything that allows the user to control the sound, therefore we needto put in its relative header file elements such as sliders and combo boxes that control some parameters ofthePluginProcessorelements.
