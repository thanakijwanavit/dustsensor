#include<string.h>
#include <LiquidCrystal.h> // include the library code:
byte buff[2];
int pin = 8;//DSM501A input D8
unsigned long duration;
unsigned long starttime;
unsigned long endtime;
unsigned long sampletime_ms = 30000;
unsigned long lowpulseoccupancy = 0;
double ratio = 0;
double concentration = 0;
int displayevery = 10; // display every nth reading

//averaging reading functions
const int numReadings=20;
double readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
double total = 0;                  // the running total
double average = 0;                // the average
double averagedReading=0;       //averagedReading

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 
int i=0;

void setup()
{
  Serial.begin(9600);
  pinMode(8,INPUT);
  starttime = millis(); 
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  //set up reading array
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}


void loop()
{
  starttime = millis(); //start timer
  duration = pulseIn(pin, LOW); //read from sensor
  Serial.print("the duration is :");
  Serial.println(duration);
  lowpulseoccupancy += duration; // add duration to low pulse occupancy
  Serial.print("lowpulseoccupancy is :");
  Serial.println(lowpulseoccupancy);
  endtime = millis();
  averagedReading=takeaverage();
  //Serial.print("the read index is ");
  //Serial.println(readIndex);
  if(readIndex % displayevery == 0){
    displayoutputs();
  }
  lowpulseoccupancy = 0;
}




void displayoutputs(){

  if (1)
    {
   
      //ratio = 100000;
      concentration = 1.1*pow(averagedReading,3)-3.8*pow(averagedReading,2)+520*averagedReading+0.62; // using spec sheet curve
      //concentration = ratio * 100;
      Serial.print("averaged ratio:");
      Serial.print(averagedReading);
      Serial.print("\n");
      Serial.print("ratio:");
      Serial.print(ratio);
      Serial.print("\n");
      Serial.print("DSM501A:  ");
      Serial.println(concentration);
      Serial.print(";\n\n");
      
      
      
      //delay(1000);
      lcd.clear(); // Clears the LCD screen
      
      lcd.setCursor(0,0);
      lcd.print("DustSensor"); // Prints "DstSensorDSM501A" on the LCD 
      //delay(500);
      lcd.setCursor(0,1);
      lcd.print(concentration); // Prints "DstSensorDSM501A" on the LCD 
      //delay(500);

    }


}

double takeaverage(){
  
  if (lowpulseoccupancy > 0){
    ratio = (lowpulseoccupancy - endtime+starttime + sampletime_ms)/(sampletime_ms*10.0);  // Integer percentage 0=>100
  
  // subtract the last reading:
  total = total - readings[readIndex];
  //Serial.println(total);
  // read from the sensor:
  readings[readIndex] = ratio;
  Serial.print("the current reading is :");
  Serial.println(readings[readIndex]);
  // add the reading to the total:
  total = total + readings[readIndex];
  //Serial.print("the current total is :");
  //Serial.println(total);
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  } 
  }else{
    Serial.println("reading error");
  }
  
  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  return average;
}

