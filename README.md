# Testing

ambush.cpp will generate the values of the grid from the path provided in inpmaze.txt and will save the values in prob.txt

bkt.cpp will generate that path from the values in prob.txt 

So to check we can compare the input we give in inpmaze.txt with the output from the bkt.cpp which is genmaze.txt. If they are same you get the correct result.

pathfinder.cpp generates the commands that will be provided to arduino.

#IMPORTANT LINKS

_RFID_
1. https://lastminuteengineers.com/how-rfid-works-rc522-arduino-tutorial/
2. https://2020.robotix.in/tutorial/event/tesseract/

_LCD_
https://create.arduino.cc/projecthub/techmirtz/using-16x2-lcd-with-arduino-d89028

**IR Sensor Connections:-**

D0-31

D1-33

D2-35

D3-37

D4-39

D5-41

D6-43

D7-45

IRVCC-5V

IRGND-GND

**MOTOR Driver Connections:-**

EN1-36

EN2-38

EN3-40

EN4-42

**LCD:-**

PIN1-GND

PIN2-5V

PIN3-POt

PIN4(RS)12

PIN5(RW)-GND

PIN6(E)-10

PIN11(D4)-2

PIN12(D5)-3

PIN13(D6)-4

PIN14(D7)-5

**RFID**

RST- 8

SDA - 53

SCK - 52

MOSI - 51

MISO - 50





