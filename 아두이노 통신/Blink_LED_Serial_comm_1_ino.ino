String inputString = "";         // a string to hold incoming data
String outputString = "";
boolean stringComplete = false;  // whether the string is complete
int led=13;
 void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 256 bytes for the inputString:
  inputString.reserve(256);
  outputString.reserve(256);
  pinMode(led, OUTPUT);  
}
void loop() 
{
  if (stringComplete) 
  {
    if(inputString.equals("A"))
    {
      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(10);               // wait for a second
      outputString=inputString;
    } 
    if(inputString.equals("B"))
    {
      digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
      delay(10);               // wait for a second
      outputString=inputString;
    }
    if(!(inputString.equals("A")||inputString.equals("B")))
    {      
      inputString.setCharAt(0,'-');
      outputString=inputString;
      delay(10);
    }
    if(Serial.available())
    {
      Serial.println(outputString);
      //Serial.flush();
    }
    delay(10);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() 
{
  inputString="";
  while (Serial.available()) 
  {
    // get the new byte:
    char inChar = (char)Serial.read(); 
      inputString += inChar;
      stringComplete = true;
      inputString += '\0';
  }
}
