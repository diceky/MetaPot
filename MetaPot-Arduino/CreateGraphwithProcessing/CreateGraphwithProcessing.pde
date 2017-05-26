import processing.serial.*;

PrintWriter output;
 
int NUM = 3;
Serial myPort;

final int WAITCOUNT = 5;
final int PULSE_SENSOR_COUNT = 4;
final int PUPIL_SENSOR_COUNT = 1;
int[][] temp = new int[PULSE_SENSOR_COUNT][WAITCOUNT];
int[] count = new int[PULSE_SENSOR_COUNT];
int[] average = new int[PULSE_SENSOR_COUNT];

int[] sensors = new int[NUM];
int cnt;
static int done_flag = 1;
 
color[] col = new color[6];
float[] last_tx = new float[PULSE_SENSOR_COUNT];  
float[] last_ty = new float[PULSE_SENSOR_COUNT];  

void setup() {
  
  size(2000, 800);
  frameRate(60);

  myPort = new Serial( this, "COM4", 230400);
  myPort.bufferUntil('\n');
  initGraph1();
  initGraph2();
  
  for(int m = 0; m < PULSE_SENSOR_COUNT; m++){
    for(int i = 0; i < WAITCOUNT; i++){
     temp[m][i] = 0; 
    }
    count[m] = 0;
    average[m] = 0;
  }
  
  stroke(255);
  line(1000,0,1000,800);
  
  String filename = nf(year(),4) + nf(month(),2) + nf(day(),2) + "_" + nf(hour(),2) + nf(minute(),2) + nf(second(),2);
  output = createWriter( filename + "_calculateAverage.csv"); 
  output.println("epochtime,type,who,value");
   
}
 
void draw() {
  
  /*******  PULSE SENSOR  ********/
  
  if(sensors[0] == 3){
  //CALCULATE AVERAGE
  if(count[sensors[1]] <  WAITCOUNT){//COLLECT DATA FOR AVERAGE
    temp[sensors[1]][count[sensors[1]]] = sensors[2];
    count[sensors[1]]++;
  }
  else{//GET AVERAGE & DRAW AVERAGE VALUE
   for(int i = 0; i < WAITCOUNT; i++){
     average[sensors[1]] += temp[sensors[1]][i];
     temp[sensors[1]][i] = 0;
    }
   count[sensors[1]] = 0;
   average[sensors[1]] = average[sensors[1]] / WAITCOUNT;
   
   //draw rectangle to cover past number
   stroke(255);
   fill(47);
   rect(700, (sensors[1] + 1)*50 - 15, 200, 50);
   //draw breath num
   fill(col[sensors[1]]);
   textSize(40);
   text(average[sensors[1]], 700, (sensors[1] + 1)*50 + 20);
  // DRAW GRAPH
  for (int i = 0; i < PULSE_SENSOR_COUNT; i++) {
    
    stroke(col[i]);
    float[] tx = new float[PULSE_SENSOR_COUNT];
    tx[i] = map(cnt, 0, width / 2, 0, width / 2);
    float[] ty = new float[PULSE_SENSOR_COUNT];
    ty[i] = map(average[i], 0, 400, height, 0);
    line(tx[i], ty[i], last_tx[i], last_ty[i]);
    
    last_tx[i] = tx[i];
    last_ty[i] = ty[i];
    //println(tx+":"+ty);
    if(last_tx[i] >= width / 2) last_tx[i] = 0;
  }
  if (cnt > width / 2) {
    initGraph1();
  }
  cnt+=2;  //determins resolution of graph
  
  }//close else

  }//close if(PULSE SENSOR)

  
  /*******  PUPIL SENSOR  ********/
  
  if(sensors[0] == 2){
  /*
  //CALCULATE AVERAGE
  if(count[sensors[1]] <  WAITCOUNT){//COLLECT DATA FOR AVERAGE
    temp[sensors[1]][count[sensors[1]]] = sensors[2];
    count[sensors[1]]++;
  }
  else{//GET AVERAGE & DRAW AVERAGE VALUE
   for(int i = 0; i < WAITCOUNT; i++){
     average[sensors[1]] += temp[sensors[1]][i];
     temp[sensors[1]][i] = 0;
    }
   count[sensors[1]] = 0;
   average[sensors[1]] = average[sensors[1]] / WAITCOUNT;
   
   //draw rectangle to cover past number
   stroke(255);
   fill(47);
   rect(1700, (sensors[1] + 1)*50 - 15, 200, 50);
   //draw breath num
   fill(col[sensors[1]]);
   textSize(40);
   text(average[sensors[1]], 1700, (sensors[1] + 1)*50 + 20);
  // DRAW GRAPH
  for (int i = 0; i < PUPIL_SENSOR_COUNT; i++) {
    
    stroke(col[i]);
    float[] tx = new float[PUPIL_SENSOR_COUNT];
    tx[i] = map(cnt, 1000, width, 1000, width);
    float[] ty = new float[PUPIL_SENSOR_COUNT];
    ty[i] = map(average[i], 0, 300, height, 0);
    line(tx[i], ty[i], last_tx[i], last_ty[i]);
    
    last_tx[i] = tx[i];
    last_ty[i] = ty[i];
    //println(tx+":"+ty);
    if(last_tx[i] >= width) last_tx[i] = 0;
  }
  if (cnt > width / 2) {
    initGraph2();
  }
  cnt+=2;  //determins resolution of graph
  }//close else
  */
  }//close if(PUPIL SENSOR)
  
  /*******  TALKING RATE  ********/
  if(sensors[0] == 1){
    /*
   //draw rectangle to cover past number
   stroke(255);
   fill(47);
   rect(1200, (sensors[1] + 1)*50 - 15, 200, 50);
   //draw talking rate
   fill(col[sensors[1]]);
   textSize(40);
   text(sensors[2], 1200, (sensors[1] + 1)*50 + 20);
   //println(sensors[0] + "," + sensors[1] + "," + sensors[2]);
   */
  }//close if(TALKING RATE)
  
}//close draw()
 

void initGraph1() {
  fill(47);
  rect(0,0,1000,800);
  strokeWeight(2);
  //noStroke();
  cnt = 0;
  col[0] = color(255, 127, 31);
  col[1] = color(31, 255, 127);
  col[2] = color(127, 31, 255);
  col[3] = color(31, 127, 255);
  col[4] = color(127, 255, 31);
  col[5] = color(127);
}

void initGraph2() {
  fill(47);
  rect(1000,0,1000,800);
  strokeWeight(2);
  //noStroke();
  cnt = 0;
  col[0] = color(255, 127, 31);
  col[1] = color(31, 255, 127);
  col[2] = color(127, 31, 255);
  col[3] = color(31, 127, 255);
  col[4] = color(127, 255, 31);
  col[5] = color(127);
}
 
void serialEvent(Serial myPort) {
  try{
    String myString = myPort.readStringUntil('\n');
    myString = trim(myString);
    sensors = int(split(myString, ','));  //[0]:ID(talkrate, pupil, pulse, turnTaking, SuperMarioTime), [1]:who, [2]:value
    println(sensors[0] + "," + sensors[1] + "," + sensors[2]);
    long timeMillis = System.currentTimeMillis();
    output.println(timeMillis+","+myString);
    String time = nf(year(),4) + "-" + nf(month(),2) + "-" + nf(day(),2) + " " + nf(hour(),2) + ":" + nf(minute(),2) + ":" + nf(second(),2) + ":" + nf(millis(),2);
    output.println(time+","+myString);
    }
  catch(RuntimeException e) {
    e.printStackTrace();
  }
}

void keyPressed(){
  if( key == 'q' ){
    output.flush(); 
    output.close(); 
    exit();               
  }
}
