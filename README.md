# Esp32-Sensor-Project UNDER CONSTRUCTION
For my ENGR 103 Final Project I wanted to determine if there was a relationship between temperature and soil moisture. My hypothesis was soil moisture will decrease as temperature increases due to more evaporation. For my project I used a resistive hygrometer which has two legs, one which sends out low voltage and the other leg reads the voltage coming in. From this you can calculate the resistance of the soil, lower if it is more wet and higher if it is dry. For my temperature sensor I used a digital thermometer (LM75a) which has precision within 2 °C. In addition I hooked up a pixel by pixel display to display the current readings and the duration of data collection.


```markdown
# Nokia 5510 LCD Display       # ESP 32
Pin1 (RST)                     GPIO2
Pin2 (CE)                      GPIO15
Pin3 (DC)                      GPIO4
Pin4 (Din)                     GPIO23
Pin5 (Clk)                     GPIO18
Pin6 (Vcc)                     3.3V
Pin7 (BL)                      GND
Pin8 (GND)                     GND


LM75a Temperature Sensor      ESP 32
Pin1 (Vcc)                    3.3V
Pin2 (GND)                    GND
Pin3 (SDA)                    GPIO21
Pin4 (SCL)                    GPIO22


Soil Moisture Sensor          ESP 32
Pin1 (AO)                     GPIO36
Pin3 (GND)                    GND
Pin4(Vcc)                     3.3V
```


