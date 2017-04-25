#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet2.h>
#include <EthernetUdp2.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x10, 0xAA, 0x74
};
//IPAddress ip(127, 0, 0, 1);
IPAddress ip(131, 113, 137, 49);
unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  //buffer to hold incoming packet,

char * id;
char * value;
static float talkSubTotal_AB = 0, talkSubTotal_CD = 0;
static float TalkCountA = 0, TalkCountB = 0, TalkCountC = 0, TalkCountD = 0;
float talkTotal = 0;
float TalkRate[4] = {0,0,0,0};

const int TALKINGRATE_THRESH = 50;

//  VARIABLES FOR PULSE SENSOR
const int PULSE_SENSOR_COUNT = 2;

const int NORMAL_PULSE[PULSE_SENSOR_COUNT] = {80, 80};
const int NORMAL_PUPIL = 90; 

// Ethernet Shield uses up pins 10, 11, 12, 13 !!
const int led_pin[PULSE_SENSOR_COUNT] = {5, 6};

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

int pulsePin[PULSE_SENSOR_COUNT] = {0, 1};                 // Pulse Sensor purple wire connected to analog pin 0, 1, 2, 3
int blinkPin = 13;                // pin to blink led at each beat
int fadePin = 5;                  // pin to do fancy classy fading blink at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin

// Volatile Variables, used in the interrupt service routine!
volatile int BPM[PULSE_SENSOR_COUNT];                // int that holds raw Analog in 0. updated every 2mS
volatile int Signal[PULSE_SENSOR_COUNT];                // holds the incoming raw data
volatile int IBI[PULSE_SENSOR_COUNT];             // int that holds the time interval between beats! Must be seeded! 
volatile boolean Pulse[PULSE_SENSOR_COUNT];     // "True" when User's live heartbeat is detected. "False" when not a "live beat". 
volatile boolean QS[PULSE_SENSOR_COUNT];        // becomes true when Arduoino finds a beat.

// Regards Serial OutPut  -- Set This Up to your needs
static boolean serialVisual = false;   // Set to 'false' by Default.  Re-set to 'true' to see Arduino Serial Monitor ASCII Visual Pulse

void setup() {

  Serial.begin(115200);
  
  // start the Ethernet and UDP:
  Serial.println("Begin Ethernet");
  Ethernet.begin(mac, ip);
  Serial.println("Begin localport");
  Udp.begin(localPort);

  //FOR PULSE SENSOR
  for(int y = 0; y < PULSE_SENSOR_COUNT; y++){ //initialize arrays
    IBI[y] = 600;
    Pulse[y] = false;
    QS[y] = false;
  };
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS   
}

void loop() {
       

  // if there's UDP data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    //Serial.print("Received packet of size ");
    //Serial.println(packetSize);
    //Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i = 0; i < 4; i++) {
      //Serial.print(remote[i], DEC);
      if (i < 3) {
        //Serial.print(".");
      }
    }
    //Serial.print(", port ");
    //Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    //Serial.println("Contents:");
    //Serial.println(packetBuffer);

    //split packet with comma
    id = strtok(packetBuffer, ' ');
    int idInt = atoi(id);
    value = strchr(packetBuffer, ',');
    ++value;
    float valueFloat = atof(value);
    value = strchr(value+1, ',');
    ++value;
    int whoInt = atoi(value);
    value = strchr(value+1, ',');
    ++value;
    int totalInt = atoi(value);

      /****** TALKING RATE ******/

    if(idInt == 2){
      /*
      Serial.print("idInt:");
      Serial.print(idInt);
      Serial.print(" valueFloat:");
      Serial.print(valueFloat);
      Serial.print(" whoInt:");
      Serial.print(whoInt);
      Serial.print(" totalInt:");
      Serial.println(totalInt);
      */
      if(whoInt == 1){   //A SPOKE
        Serial.print("TalkA: ");
        Serial.println(valueFloat);
        TalkCountA = valueFloat;
        talkSubTotal_AB = totalInt;
      }
      else if(whoInt == 2){   //B SPOKE
        Serial.print("TalkB: ");
        Serial.println(valueFloat);
        TalkCountB = valueFloat;
        talkSubTotal_AB = totalInt;
      }
      else if(whoInt == 3){   //C SPOKE
        Serial.print("TalkC: ");
        Serial.println(valueFloat);
        TalkCountC = valueFloat;
        talkSubTotal_CD = totalInt;
      }
      else if(whoInt == 4){   //D SPOKE
        Serial.print("TalkD: ");
        Serial.println(valueFloat);
        TalkCountD = valueFloat;
        talkSubTotal_CD = totalInt;
      }
      talkTotal = talkSubTotal_AB + talkSubTotal_CD;
      
      if((TalkCountA + TalkCountB + TalkCountC + TalkCountD) == talkTotal){ // CHECK SUM MATCHES TALKTOTAL
        TalkRate[0] = TalkCountA / talkTotal * 100;
        TalkRate[1] = TalkCountB / talkTotal * 100;
        TalkRate[2] = TalkCountC / talkTotal * 100;
        TalkRate[3] = TalkCountD / talkTotal * 100;
        for(int i = 0; i < 4; i++){
          Serial.print(" TalkRate");
          Serial.print(i+1);
          Serial.print(": ");
          Serial.print(TalkRate[i]);
        }
        Serial.println("");
      }
    }

      /****** LEAN Y ******/
    /*
    if(idInt == 3){
      Serial.print("leanY: ");
      Serial.println(valueFloat);
      //analogWrite(led_pin4, valueFloat * 30);
      Serial.print("Pot Lean:");
      Serial.println(valueFloat);
    }
    */

      /****** PUPIL ******/
      
    else if(idInt == 5){
      if(TalkRate[3] < TALKINGRATE_THRESH){   //activate Pot only when TalkRate is less than close to equal
        //Serial.print("pupil: ");
        //Serial.println(valueFloat);
        float treePupil;
        if(valueFloat < NORMAL_PUPIL) {treePupil = 0;}
        else if(valueFloat > (NORMAL_PUPIL + 30)) {treePupil = 30;}
        else {treePupil = (valueFloat - NORMAL_PUPIL) * 30 / 30;}
        //analogWrite(led_pin4, treePupil);
        Serial.print("Pot Pupil:");
        Serial.println(treePupil);
      }
    }
    
  }//end if(packetsize)

    /****** PULSE ******/

  for(int x = 0; x < PULSE_SENSOR_COUNT; x++){

   if (QS[x] == true){     //  A Heartbeat Was Found
                       // BPM and IBI have been Determined
                       // Quantified Self "QS" true when arduino finds a heartbeat
     Serial.print("BPM");
     Serial.print(x+1);
     Serial.print(",");              
     Serial.println(BPM[x]);  // for Unity visualization

     float treePulse[4];
     if(BPM[x] < NORMAL_PULSE[x]) {treePulse[x] = 0;}
      else if(BPM[x] > (NORMAL_PULSE[x] + 20)) {treePulse[x] = 30;}
      else {treePulse[x] = (BPM[x] - NORMAL_PULSE[x]) * 30 / 20;}
      
      if(TalkRate[x] < TALKINGRATE_THRESH){   //Talk Rate is less than close to equal
        analogWrite(led_pin[x], treePulse[x]);
        Serial.print("Pot Pulse");
        Serial.print(x+1);
        Serial.print(",");
        Serial.println(treePulse[x]);
      }
      else{
        analogWrite(led_pin[x], 0);
      }
                
      QS[x] = false;                      // reset the Quantified Self flag for next time 
    }

  }//end for
}

