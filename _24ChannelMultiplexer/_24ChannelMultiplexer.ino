//Analogue multiplexer
//CSIR 2015
//Author: Tony Lumbwe
//Reference: Martin Nawrath
//
//THis sketch is interfacing an Arduino Uno with three 8 channel Analogue multiplexers and one serial shift register.
//The Analogue multiplexers scan 24 Analogue channels continuously upon receiving an address from the shift register.
//This address selects which channel to be enables and the readings are displayed on the serial monitor. 
//
//A future alteration will be to select from an LCD screen which one of the channels one would want to read, preferably 8
//of these channels and the user will have the readings available either on the serial monitor, on a user interface (Processing)
//or on a set of banana plug connectors attached to a box holding the wntire setup.
//

int ar_1[24];            //Array of channels
                         //Note: All3 Analogue outputs from the MUX are connected to Arduino Analog Input
                         
int dataPin  = 5;
int clockPin = 6;
int latchPin = 7;

int led;

void setup(){
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  Serial.begin(57600);
  Serial.println("Analog Multiplexer");
}
void loop(){

  delay (1000);
  readMpx(ar_1, 0);        //Function to read the MUX data
  
  
  Serial.println(" ");
  for (int i = 0; i<12; i++){
    Serial.print(i+1); Serial.print(":");Serial.print(ar_1[i]);Serial.print(" ");
    Serial.print(i+13); Serial.print(":");Serial.println(ar_1[i+12]);
  }
  Serial.print(" ");
}                         

//************************************************************
//This is the function that reads data from the Multiplexer

void readMpx(int aa[], int ain){
  int addr;
  int val;
  
  led = !led; //led blink when channel is read
  
  for(int cnt = 0; cnt <24; cnt++) {
    addr = cnt;
    addr = addr & 7;
    if (cnt <= 7 ) addr = addr | 48;
    if (cnt >= 8 && cnt <= 15) addr = addr | 40;
    if (cnt >15 ) addr = addr | 24;
    
    if (led == 0) addr = addr | 128;    //Bit 7 of shift register is connected to the LED
    
    digitalWrite(latchPin, LOW);          //Disable storage register in order to push data packet inside shift register (Addressing)
    shiftOut(dataPin, clockPin, MSBFIRST, addr);
    digitalWrite(latchPin, HIGH);          //Enable storage register to flush out the data 
    delayMicroseconds(50);                //50Microseconds to allow signal to settle
    val = analogRead(ain);
    aa[cnt] = ain;
    
    
  }
  
}
