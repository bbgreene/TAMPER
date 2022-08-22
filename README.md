# T A M P E R

A waveshaping and convolution distortion vst3 plugin made with JUCE.

![TAMPER](https://github.com/bbgreene/TAMPER/blob/master/Screenshots/TAMPER%20-%20gui.png?raw=true
 "TAMPER")
 
 
 
** Parameters**

<b>MODEL:</b>

 On/Off: toggles filters on or off\
 High: high pass filter (20 - 2000 Hz) pre waveshaping distortion\
 Low: low pass filter (5000 - 20000 Hz) post waveshaping distortion\
 On/Off: toggles waveshaping distortion on or off\
 Drive: amount of drive (distortion) applied to the incoming signal (0 - 24dB)\
 Model Type: the type of waveshaping applied (soft clip, hard clip, saturation, broken)\
 Oversample: toggles oversampling on or off. If on, then the signal will be oversampled x4 before any distortion processing then downsampled post distortion processing to avoid aliasing.
 
 <b>CAB SIM:</b>
 
 On/Off: toggles the cabinet simulator on or off\
 Cab Mix: adjusts the amount of waveshaped signal is sent into the cabinet simulation\
 Cab Type: a selection of impulse responses used for the convolution process
 
 <b>LIMITER:</b>
 
 On/Off: toggles the limiter on or off\
 Threshold: sets the threshold in dB of the limiter (0 to  -10dB)\
 Release: sets the release time in milliseconds of the limiter (1 - 1000ms)
 
 <b>OUT:</b>
 
 Mix: adjusts the balance between the input signal and the processed signal\
 Output: boosts or attenuates the output of the plugin (-24 to +24dB)
 
 Next steps: VST3 and AU installation builds for macOs (Intel, M1, M2) and Windows
