# metronome_app
Basic Metronome Audio App with GUI to allow user to change BPM of  the click. 

Metronome.cpp contains most of the 'back end' code for generating the click at a sample specific time by converting bpm time into samples via sample rate (usually 44.1kHz)

Main Component.cpp contains most of code generating the GUI and its various listener functions for when slider changes value etc. 
