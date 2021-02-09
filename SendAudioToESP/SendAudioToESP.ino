#include <FastLED.h>  // just for EVERY_N_MILLIS in DDAudio.h
#include <Statistic.h>
double mult = 1.00;
#include "DDAudio.h"



void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.println("Setting up");
  audioSetup();
  
}

void loop() {
  musicAnalytics();
  sendAudioToESP();
  printSpectrumSimple();
}

void sendAudioToESP() {
  if (fft1024.available()) {
    Serial1.write('A');
    for (int i = 0; i < numFFTBins; i++) {
      Serial1.write(spectrumValue[i]);
    }
  }
}

void printSpectrumSimple() {
  for (int i = 0; i < 16; i++) {
    Serial.print(spectrumValue[i]);
    Serial.print("\t");
  }
  Serial.println();
}
