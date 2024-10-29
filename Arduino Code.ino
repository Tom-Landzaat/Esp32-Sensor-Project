#include <LM75A.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(18,23,4,15,2);

double dataT [600];
double dataM [600];
double total = 0;
double totalSumT [144];
double totalSumM [144];
double avgT = 0, avgM = 0, minT = 0, maxT = 0, minM = 0, maxM = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;
int count = 0;
unsigned long time1 = 0;
const int sensor_pin = A0;

// Create I2C LM75A instance
LM75A lm75a_sensor(false,  // A0 LM75A pin state (connected to ground = false)
                   false,  // A1 LM75A pin state (connected to ground = false)
                   false); // A2 LM75A pin state (connected to ground = false)
// Equivalent to "LM75A lm75a_sensor;"

/*for displaying time since program started on external display*/
void clocks(){
  (hours < 10) ?display.print("0"):NULL;
  display.print(hours);
  display.print(":");
  (minutes < 10) ?display.print("0"):NULL;
  display.print(minutes);
  display.print(":");
  (seconds < 10) ?display.print("0"):NULL;
  display.print(seconds);
  display.display();
  stepUp();
}

/*function for calculating 10 minute average*/
double ave10(double data[], double num) 
{
  double total = 0, ave10 = 0;
  for(int i = 0; i < 600; i++)
  {
    total += data[i];
  }
  ave10 = (total/num);
  return ave10;
}
/*function for calculating 10 minute max*/
double maxV(double val[]){ 
  double maxV = 0;
  for(int i = 0; i < 600; i++){
    (val[i] > maxV) ? maxV = val[i] : NULL;
  } 
  return maxV;
}

/*function for calculating 10 minute min*/
double minV(double val[]){ 
  double minV = 100;
  for(int i = 0; i < 600; i++){
    (val[i] < minV && val[i] != 0) ? minV = val[i] : NULL;
  } 
  return minV;
}

/*Analog signal to Moisutre in percent*/
double AtoM(double input)
{
  return (100 - ((input/4095.00) * 100));
}

/*average over the 24 hour test period*/
double totalAve(double ave10[]){ 
  double sum = 0, total = 0;
  int i = 0;
  for(i = 0; i < 144; i++){
    if(ave10[i] == 0)
      break;
    }
  for(int index = 0; index < i; index++)
    sum += ave10[index];  
  total = (sum/i);
  return (sum/i);
}

void setup(void)
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Temperatures will be displayed every second:");
  Serial.println("Type \"a\" for 10 minute average");
  for(int i = 0; i < 600; i++){//initialize array to 0
    dataT[i] = 0;
    dataM[i] = 0;
  }
  for(int i = 0; i < 144; i++){
    totalSumT[i] = 0;
    totalSumM[i] = 0;
  }
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
  display.setTextColor(BLACK);
}

void loop()
{
  float temperature_in_degreesC = lm75a_sensor.getTemperatureInDegrees();
  
  if (temperature_in_degreesC == INVALID_LM75A_TEMPERATURE)
    Serial.println("Error while getting temperature"); //if an invalid reading is measured print error message and try to remeasure
  else
  {
    for(int i = 0; i < 600;){
      if(millis() > (time1 + 1000)){
        time1 = millis();
        dataT[i] = LM75A::degreesToFahrenheit(lm75a_sensor.getTemperatureInDegrees());//store temp data to 10 minute temp array
        dataM[i] = AtoM(analogRead(sensor_pin));//store moist data to 10 minute moist array
        Serial.print(dataT[i]);
        Serial.print("\t");       //Comment out to not spam during actual data colection
        Serial.println(dataM[i]);
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(BLACK);
        display.clearDisplay();   // clears the screen and buffer 
        clocks();
        display.setCursor(0,10); //Display instantaneous results to external display
        display.println("Readings:");
        display.setCursor(0,20);
        display.print(dataT[i]);
        display.println(" F");
        display.print(dataM[i]);
        display.print(" %");
        display.display();
        i++;
      }
      
      /*if 10 minutes have passed or user has entered something in the serial port, read the serial port*/
      if(Serial.available() > 0 || (time1 > (601000 * count))){
        char message = Serial.read();
        
        /*if ten minutes have passed, store the average of the 10 minute period into a 24 hour average array*/
        if(time1 > ((601000 + (600000 * count)))){
          totalSumT[count] = ave10(dataT, 600);
          totalSumM[count] = ave10(dataM, 600);
          count++; // increase count by 1 to delay another 10 mins
          message = 'a'; //set message to 'a' to satisfy next condition
        }

        if(message == 'a'){
          Serial.println("Average: ---------------");

          /*store average for the last 10 minutes in an average variable which gets printed*/
          avgT = ave10(dataT, 600.00); 
          avgM = ave10(dataM, 600.00);
          Serial.print("Average: ");
          Serial.print(avgT);
          Serial.print("\t");
          Serial.println(avgM);
          Serial.print("Minimum: ");
          Serial.print(minV(dataT));
          Serial.print("\t");

          /*print min/max values over the 10 minute period*/
          Serial.println(minV(dataM));
          Serial.print("Maximum: ");
          Serial.print(maxV(dataT));
          Serial.print("\t");
          Serial.println(maxV(dataM));
          Serial.print("Total Average: ");
          Serial.print(totalAve(totalSumT));
          Serial.print("\t");
          Serial.println(totalAve(totalSumM));
        }
      }
    }
  }
}

/*used for the clock() when one value reaches 60 we want it to reset to 0 and add one to the superseding value*/
void stepUp() {
  if(seconds < 60){
    seconds += 1;
  }
  if(seconds >= 59) {
    minutes += 1;
    seconds = 0; 
  }
  if (minutes > 59) {
    hours += 1;
    minutes = 0;
  }
}

