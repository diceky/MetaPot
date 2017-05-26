


volatile int rate[PULSE_SENSOR_COUNT][10];                    // array to hold last ten IBI values
volatile unsigned long sampleCounter[PULSE_SENSOR_COUNT];          // used to determine pulse timing
volatile unsigned long lastBeatTime[PULSE_SENSOR_COUNT];           // used to find IBI
volatile int P[PULSE_SENSOR_COUNT];                      // used to find peak in pulse wave, seeded
volatile int T[PULSE_SENSOR_COUNT];                     // used to find trough in pulse wave, seeded
volatile int thresh[PULSE_SENSOR_COUNT];                // used to find instant moment of heart beat, seeded
volatile int amp[PULSE_SENSOR_COUNT];                   // used to hold amplitude of pulse waveform, seeded
volatile boolean firstBeat[PULSE_SENSOR_COUNT];        // used to seed rate array so we startup with reasonable BPM
volatile boolean secondBeat[PULSE_SENSOR_COUNT];      // used to seed rate array so we startup with reasonable BPM

void interruptSetup(){     
  
  //initialize arrays
  for(int y = 0; y < PULSE_SENSOR_COUNT; y++){ 
  sampleCounter[y] = 0;
  lastBeatTime[y] = 0;
  P[y] = 512;
  T[y] = 512;
  firstBeat[y] = true;
  secondBeat[y] = false;
  thresh[y] = 525;
  amp[y] = 100;
  };
  
  // Initializes Timer2 to throw an interrupt every 2mS.
  TCCR2A = 0x02;     // DISABLE PWM ON DIGITAL PINS 3 AND 11, AND GO INTO CTC MODE
  TCCR2B = 0x06;     // DON'T FORCE COMPARE, 256 PRESCALER 
  OCR2A = 0X7C;      // SET THE TOP OF THE COUNT TO 124 FOR 500Hz SAMPLE RATE
  TIMSK2 = 0x02;     // ENABLE INTERRUPT ON MATCH BETWEEN TIMER2 AND OCR2A
  sei();             // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED      
} 


// THIS IS THE TIMER 2 INTERRUPT SERVICE ROUTINE. 
// Timer 2 makes sure that we take a reading every 2 miliseconds
ISR(TIMER2_COMPA_vect){                         // triggered when Timer2 counts to 124
  for(int x = 0; x < PULSE_SENSOR_COUNT; x++){  //do this for every pulse sensor
  cli();                                      // disable interrupts while we do this
  Signal[x] = analogRead(pulsePin[x]);              // read the Pulse Sensor 
  sampleCounter[x] += 2;                         // keep track of the time in mS with this variable
  int N = sampleCounter[x] - lastBeatTime[x];       // monitor the time since the last beat to avoid noise

    //  find the peak and trough of the pulse wave
  if(Signal[x] < thresh[x] && N > (IBI[x]/5)*3){       // avoid dichrotic noise by waiting 3/5 of last IBI
    if (Signal[x] < T[x]){                        // T is the trough
      T[x] = Signal[x];                         // keep track of lowest point in pulse wave 
    }
  }

  if(Signal[x] > thresh[x] && Signal[x] > P[x]){          // thresh condition helps avoid noise
    P[x] = Signal[x];                             // P is the peak
  }                                        // keep track of highest point in pulse wave

  //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
  if (N > 250){                                   // avoid high frequency noise
    if ( (Signal[x] > thresh[x]) && (Pulse[x] == false) && (N > (IBI[x]/5)*3) ){        
      Pulse[x] = true;                               // set the Pulse flag when we think there is a pulse
//      digitalWrite(blinkPin,HIGH);                // turn on pin 13 LED
      IBI[x] = sampleCounter[x] - lastBeatTime[x];         // measure time between beats in mS
      lastBeatTime[x] = sampleCounter[x];               // keep track of time for next pulse

      if(secondBeat[x]){                        // if this is the second beat, if secondBeat == TRUE
        secondBeat[x] = false;                  // clear secondBeat flag
        for(int i=0; i<=9; i++){             // seed the running total to get a realisitic BPM at startup
          rate[x][i] = IBI[x];                      
        }
      }

      if(firstBeat[x]){                         // if it's the first time we found a beat, if firstBeat == TRUE
        firstBeat[x] = false;                   // clear firstBeat flag
        secondBeat[x] = true;                   // set the second beat flag
        sei();                               // enable interrupts again
        return;                              // IBI value is unreliable so discard it
      }   


      // keep a running total of the last 10 IBI values
      word runningTotal[PULSE_SENSOR_COUNT];                  // clear the runningTotal variable
      for(int y = 0; y < PULSE_SENSOR_COUNT; y++){runningTotal[y] = 0;};    

      for(int i=0; i<=8; i++){                // shift data in the rate array
        rate[x][i] = rate[x][i+1];                  // and drop the oldest IBI value 
        runningTotal[x] += rate[x][i];              // add up the 9 oldest IBI values
      }

      rate[x][9] = IBI[x];                          // add the latest IBI to the rate array
      runningTotal[x] += rate[x][9];                // add the latest IBI to runningTotal
      runningTotal[x] /= 10;                     // average the last 10 IBI values 
      BPM[x] = 60000/runningTotal[x];               // how many beats can fit into a minute? that's BPM!
      QS[x] = true;                              // set Quantified Self flag 
      // QS FLAG IS NOT CLEARED INSIDE THIS ISR
    }                       
  }

  if (Signal[x] < thresh[x] && Pulse[x] == true){   // when the values are going down, the beat is over
//    digitalWrite(blinkPin,LOW);            // turn off pin 13 LED
    Pulse[x] = false;                         // reset the Pulse flag so we can do it again
    amp[x] = P[x] - T[x];                           // get amplitude of the pulse wave
    thresh[x] = amp[x]/2 + T[x];                    // set thresh at 50% of the amplitude
    P[x] = thresh[x];                            // reset these for next time
    T[x] = thresh[x];
  }

  if (N > 2500){                           // if 2.5 seconds go by without a beat
    thresh[x] = 512;                          // set thresh default
    P[x] = 512;                               // set P default
    T[x] = 512;                               // set T default
    lastBeatTime[x] = sampleCounter[x];          // bring the lastBeatTime up to date        
    firstBeat[x] = true;                      // set these to avoid noise
    secondBeat[x] = false;                    // when we get the heartbeat back
  }

  sei();                                   // enable interrupts when youre done!
  }//for
}// end isr






