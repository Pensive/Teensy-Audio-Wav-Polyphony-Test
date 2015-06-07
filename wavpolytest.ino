#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Bounce.h>

// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs

AudioPlaySdWav           playWav1;       //xy=285,673
AudioPlaySdWav           playWav2;       //xy=284,730
AudioPlaySdWav           playWav3;       //xy=285,673
AudioPlaySdWav           playWav4;       //xy=284,730
AudioPlaySdWav           playWav5;       //xy=285,673
AudioPlaySdWav           playWav6;       //xy=284,730
AudioPlaySdWav           playWav7;       //xy=285,673
AudioPlaySdWav           playWav8;       //xy=284,730

AudioMixer4              mixerSampleLMixer;         //xy=732,327
//AudioMixer4              mixerSampleRMixer;         //xy=732,327
AudioMixer4              mixerSampleA;         //xy=557,138
AudioMixer4              mixerSampleB;         //xy=541,297
//AudioMixer4              mixerSampleC;         //xy=557,138
//AudioMixer4              mixerSampleD;         //xy=541,297/

AudioOutputI2S     headphones;
//AudioOutputAnalog  dac;     // uncomment for DAC output


// Create Audio connections between the components
//WAVSample Left Channels 1-4
AudioConnection          patchCord1(playWav1, 0, mixerSampleA, 0);
AudioConnection          patchCord2(playWav2, 0, mixerSampleA, 1);
AudioConnection          patchCord3(playWav3, 0, mixerSampleA, 2);
AudioConnection          patchCord4(playWav4, 0, mixerSampleA, 3);
//WAVSample Left Channels 4-8
AudioConnection          patchCord5(playWav5, 0, mixerSampleB, 0);
AudioConnection          patchCord6(playWav6, 0, mixerSampleB, 1);
AudioConnection          patchCord7(playWav7, 0, mixerSampleB, 2);
AudioConnection          patchCord8(playWav8, 0, mixerSampleB, 3);

//WAVSample Right Channels 1-4
//AudioConnection          patchCord9(playWav1, 1, mixerSampleC, 0);
//AudioConnection          patchCord10(playWav2, 1, mixerSampleC, 1);
//AudioConnection          patchCord11(playWav3, 1, mixerSampleC, 2);
//AudioConnection          patchCord12(playWav4, 1, mixerSampleC, 3);
//WAVSample Right Channels 4-8
//AudioConnection          patchCord13(playWav5, 1, mixerSampleD, 0);
//AudioConnection          patchCord14(playWav6, 1, mixerSampleD, 1);
//AudioConnection          patchCord15(playWav7, 1, mixerSampleD, 2);
//AudioConnection          patchCord16(playWav8, 1, mixerSampleD, 3);


//Main SampleMixer
AudioConnection          patchCord17(mixerSampleA, 0, mixerSampleLMixer, 0);
AudioConnection          patchCord18(mixerSampleB, 0, mixerSampleLMixer, 1);
//AudioConnection          patchCord19(mixerSampleC, 0, mixerSampleRMixer, 0);
//AudioConnection          patchCord20(mixerSampleD, 0, mixerSampleRMixer, 1);


//Outputs

AudioConnection         patchCord21(mixerSampleLMixer, 0, headphones, 0);
AudioConnection         patchCord22(mixerSampleLMixer, 0, headphones, 1);

//AudioConnection patchCord23(mixerSampleMixer, 0, dac, 0); //uncomment for DAC output

// Create an object to control the audio shield.
// 
AudioControlSGTL5000 audioShield;

// Bounce objects to read six pushbuttons (pins 0-5)
//
Bounce button0 = Bounce(0, 5);
Bounce button1 = Bounce(1, 5);  // 5 ms debounce time
Bounce button2 = Bounce(2, 5);
Bounce button3 = Bounce(3, 5);
Bounce button4 = Bounce(4, 5);
Bounce button5 = Bounce(5, 5);
Bounce button8 = Bounce(8, 5);

unsigned long SerialMillisecondCounter;

void setup() {
  // Configure the pushbutton pins for pullups.
  // Each button should connect from the pin to GND.
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  Serial.begin(38400); // open the serial

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(40);

  // turn on the output
  audioShield.enable();
  audioShield.volume(0.5);

  // by default the Teensy 3.1 DAC uses 3.3Vp-p output
  // if your 3.3V power has noise, switching to the
  // internal 1.2V reference can give you a clean signal
  //dac.analogReference(INTERNAL);

  // reduce the gain on mixer channels, so more than 1
  // sound can play simultaneously without clipping
  mixerSampleA.gain(0, 0.7);
  mixerSampleA.gain(1, 0.7);
  mixerSampleA.gain(2, 0.7);
  mixerSampleA.gain(3, 0.7);
  mixerSampleB.gain(0, 0.7);
  mixerSampleB.gain(1, 0.7);
  mixerSampleB.gain(2, 0.7);
  mixerSampleB.gain(3, 0.7);
 // mixerSampleC.gain(0, 0.7);
 // mixerSampleC.gain(1, 0.7);
 // mixerSampleC.gain(2, 0.7);
 // mixerSampleC.gain(3, 0.7);
 // mixerSampleD.gain(0, 0.7);
 // mixerSampleD.gain(1, 0.7);
 // mixerSampleD.gain(2, 0.7);
//  mixerSampleD.gain(3, 0.7);
  mixerSampleLMixer.gain(0,0.7);
  mixerSampleLMixer.gain(1,0.7);
//  mixerSampleRMixer.gain(0,0.7);
//  mixerSampleRMixer.gain(1,0.7);
    
  //SDCard Initialise
  SPI.setMOSI(7);
  SPI.setSCK(14);
  if (!(SD.begin(10))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
       
  SerialMillisecondCounter = millis();
  
}

int val; //temporary variable for memory usage reporting.

void loop() {
  
  if(millis() - SerialMillisecondCounter >= 5000) {
    Serial.print("Proc = ");
    Serial.print(AudioProcessorUsage());
    Serial.print(" (");    
    Serial.print(AudioProcessorUsageMax());
    Serial.print("),  Mem = ");
    Serial.print(AudioMemoryUsage());
    Serial.print(" (");    
    Serial.print(AudioMemoryUsageMax());
    Serial.println(")");
    SerialMillisecondCounter = millis();
    AudioProcessorUsageMaxReset();
    AudioMemoryUsageMaxReset();
  }
  
  
  // Update all the button objects
  button0.update();
  button1.update();
  button2.update();
  button3.update();
  button4.update();
  button5.update();
  button8.update();
  
  // Start test sound if it is not playing. This will loop infinitely.
   if (! (playWav1.isPlaying())){
    playWav1.play("1644100M.WAV");
    }
  
if (button0.fallingEdge()) {
	 playWav2.play("1644100M.WAV");
	 Serial.println("Playing wav2");
 }

  if (button1.fallingEdge()) {
    playWav3.play("1644100M.WAV"); // enable/disable chorus on SDCard Sample
	 Serial.println("Playing wav3");
   }
   
  if (button2.fallingEdge()) {
   playWav4.play("1644100M.WAV"); // enable/disable chorus on SDCard Sample
   Serial.println("Playing wav4");
    }
    
  if (button3.fallingEdge()) {
  playWav5.play("1644100M.WAV"); // enable/disable chorus on SDCard Sample
  Serial.println("Playing wav5");
     
  }
  if (button4.fallingEdge()) {
   playWav6.play("1644100M.WAV"); // enable/disable chorus on SDCard Sample
   Serial.println("Playing wav6");
   }
   
  if (button5.fallingEdge()) {
   playWav7.play("1644100M.WAV"); // enable/disable chorus on SDCard Sample
   Serial.println("Playing wav7");
   }
   if (button8.fallingEdge()) {
    playWav8.play("1644100M.WAV"); // enable/disable chorus on SDCard Sample
    Serial.println("Playing wav8");
   }
}

