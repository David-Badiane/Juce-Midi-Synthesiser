#SubSynth

The SubSynth is a simple synthesizer with eleven different oscillator voices implemented througha Juce Audio Plugin.   Therefore we have the PluginEditor,  the PluginProcessor and many other classesimplemented by header filesfiles.handfiles.cpp.The main components of this synthesizer are:
- a Synthesiser class object calledmySynth;
- elevenSythesiserSound/SynthesiserVoiceclasses;
- a multi typefilter;
- a virtual midi keyboard;
- many elements such as sliders and combo-boxes that allow the control of voices, filter and mastergain, pitch bend, ptich bend extension.

In thePluginProcessorwe find everything that processes sound and has to be controlled, therefore weneed to put in its relative header file all the declarations of objects and variables necessary to process thesound. In thePluginEditorwe find everything that allows the user to control the sound, therefore we needto put in its relative header file elements such as sliders and combo boxes that control some parameters ofthePluginProcessorelements.
