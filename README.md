# T A M P E R

A multi functional reverb vst3 plugin made with JUCE.

![COLOURAUM](https://github.com/bbgreene/COLOURAUM/blob/master/Screenshots/COLOURAUM%20-%20gui.png?raw=true
 "COLOURAUM")
 
 
 
** Parameters**

<b>FILTER:</b>

 High: high pass filter (20 - 2000 Hz) pre reverb\
 Low: low pass filter (5000 - 20000 Hz) pre reverb
 
 <b>EARLY REFLECTIONS:</b>
 
 Amount: amount of early reflections sent to the reverb\
 Type: the type of early reflections associated with a certain type of space\
 Mod Rate: the rate of modulation applied to the early reflection times (0.01 - 100 Hz)\
 Mod Depth: the amount of modulation applied to the early reflection times
 
 <b>REVERB:</b>
 
 Size: the size of the room\
 Freeze: the reverb is put into a continuous feedback loop\
 Predelay: controls the delay time before the onset of the first early reflection (0 - 200ms)\
 Damp: the amount of absorption of high frequencies in the reverb
 
 <b>GATE:</b>
 
 On/Off: toggles the gate processing on or off\
 Threshold: sets the threshold in dB of the gate (-20 - 0dB)\
 Ratio: sets the ratio of the gate (1:1 - 5:1)\
 Attack: sets the attack time in milliseconds of the gate (1 - 200ms)\
 Release: sets the release time in milliseconds of the gate (1 - 3000ms)
 
 
 <b>TREMOLO:</b>
 
 On/Off: toggles the tremolo processing on or off\
 Pre/Post: toggles the tremolo processing pre reverb or post reverb\
 Distortion: the amount of distortion applied to the signal pre tremolo\
 Wave: Choice of waveforms (sine, triangle, square, ring) for LFO 1. Ring changes the effect to ring modulation\
 Rate: the rate of amplitude modulation applied to the signal (0.01 - 100 Hz)\
 Rate: the amount of amplitude modulation applied to the signal
 
 <b>OUTPUT:</b>
 
 Mix: adjusts the balance between the input signal and the processed signal\
 Width: adjusts the stereo width of the processed signal
 
 Next steps: VST3 and AU installation builds for macOs (Intel, M1, M2) and Windows
