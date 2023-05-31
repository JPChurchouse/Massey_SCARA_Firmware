#include <Arduino.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif


#define CommandBufferSize 90
String CommandBuffer[CommandBufferSize];
String CurrentCommand[4];				// Strore the command to execute
uint8_t CB_WriteIndex = 0;		// Counter of the number of orders received
uint8_t CB_ReadIndex = 0;	// Counter of orders processed
uint8_t echo_commands = 0;
uint32_t LockoutTime = 0;

String Msg_Air; // String to send after AIR command


const byte led = LED_BUILTIN;

String data[4];
void process_orders(String content);
void read_and_extract(String content);
void ClearBuffer();
void air_process(String air_command);


void setup()
{
  pinMode(led,OUTPUT);
  digitalWrite(led, HIGH);

  Serial.begin(115200);
  Serial.setTimeout(5);
  while (!Serial){}// Wait for serial to be ready

  digitalWrite(led, LOW);  

  ClearBuffer();
}

void loop()
{
  if (Serial.available())
  {
    // Store Serial data into the buffer
    if (CB_WriteIndex >= CommandBufferSize) CB_WriteIndex = 0;
    if (CB_ReadIndex  >= CommandBufferSize) CB_ReadIndex  = 0;

    String rx = Serial.readStringUntil('\n');
    // "STOP" does NOT exist in the string
    if (rx.indexOf("STOP") == -1) 
    {
      CommandBuffer[CB_WriteIndex++] = rx;
      digitalWrite(led, LOW);
    }
    // "STOP" DOES exist in the string
    else
    {
      Serial.println("STOPPED");
      ClearBuffer();
      digitalWrite(led, HIGH);
    }

    if (echo_commands) Serial.println("RECEIVED: " + rx);
  }

  // Not currently moving
  if (true)
  {
    // Next command in the buffer is not blank
    if (CommandBuffer[CB_ReadIndex] != "" )
    {
      // Not waiting for a cooldown
      if ( millis() > LockoutTime )
      {
        process_orders(CommandBuffer[CB_ReadIndex]);
        CommandBuffer[CB_ReadIndex++] = "";
      }
    }
    // Next command in the buffer is blank
    else if (CB_ReadIndex == CB_WriteIndex)//be doubly sure before we delete stuff
    {
      ClearBuffer();
    }
  }
}

void process_orders(String content)
{
  read_and_extract(content);

       if (CurrentCommand[0] == "PING")
  { // Pings the controller to test communication
    Serial.println("PONG");
  }
  else if (CurrentCommand[0] == "MOVE") // Moves the robot arm from its current position to that indicated
  {

    Serial.println("MOVE: " + String(CurrentCommand[1]) + ' ' + String(CurrentCommand[2]) + ' ' + String(CurrentCommand[3]));
  }
  else if (CurrentCommand[0] == "ID") // Asks if the robot arm is right- or left-handed
  {
    Serial.println("ID, ROBOTL");
  }
  else if (CurrentCommand[0] == "HOME") // Homes the robot.
  {
    Serial.println("HOME");
  }
  else if (CurrentCommand[0] == "AIR") // Manually controls the pneumatic solenoids
  {
    String air_command = CurrentCommand[1];
    uint32_t wait_command = CurrentCommand[2].toInt();
    
    air_process(air_command);
    String message = "AIR: " + air_command;

    if (wait_command != 0)// may need to check that this works
    {
      LockoutTime = millis() + wait_command;
      message += " " + String(wait_command);
    }
    Serial.println(message);
  }
  else if (CurrentCommand[0] == "SOFFSET") // Sets the offsets for the three stepper axes
  {
    Serial.println("OFFSET," + String("~~~")); // Return the offset
  }
  else if (CurrentCommand[0] == "ROFFSET") // Reads the current offsets from Flash Memory
  {
    Serial.println("OFFSET," + String("~~~"));
  }
  else if (CurrentCommand[0] == "PROX") // Returns the current values of the proximity sensors
  {
    Serial.println("PROX," + String("~") + "," + String("~") + "," + String("~")); // Send the proximity sensor values(state)
  }
  else if (CurrentCommand[0] == "SPEEDSET") // Sets the maximum speed and acceleration of the robot arm to a percentage of the default values
  {
    Serial.println("SPEEDSET: " + CurrentCommand[1] + " " + CurrentCommand[2]);
  }
  else if (CurrentCommand[0] == "WAIT")
  {
    uint32_t del = CurrentCommand[1].toInt();
    LockoutTime = millis() + del;
    Serial.println("WAIT: " + String(del));
  }
  else if (CurrentCommand[0] == "DING")
  {
    Serial.println("DONG: " + String(CurrentCommand[1]));
  }
  else if (CurrentCommand[0] == "ECHO")
  {
    echo_commands = CurrentCommand[1].toInt();
    Serial.println("ECHO: " + String(echo_commands));
  }
  else if (CurrentCommand[0] == "CLEAR")
  {
    ClearBuffer();
    Serial.println("CLEAR");
  }
  else
  {
    Serial.println("NACK: " + String(CurrentCommand[0]));
  }
}

void ClearBuffer()
{
  for (int i = 0; i < CommandBufferSize; i++) CommandBuffer[i] = "";
  CB_ReadIndex = 0;
  CB_WriteIndex = 0;
}


void read_and_extract(String content)
{
  digitalWrite(LED_BUILTIN, HIGH);
  // Remove carriage return and line feed
  content.replace("\r", "");
  content.replace("\n", "");
  for (uint8_t i = 0; i < 4; i++) // 4 is the maximum number of argument that can be sent in single command separated by commas
  {
    int index = content.indexOf(",");              // locate the first ","
    CurrentCommand[i] = content.substring(0, index).c_str(); // Extract the string from start to the ","
    content = content.substring(index + 1);        // Remove what was before the "," from the string
  }
  digitalWrite(LED_BUILTIN,LOW);
}

void air_process(String air_command)
{
  if (air_command == "S")
  {
    Msg_Air = "AIR S";
  }
  else if (air_command == "U")
  {
    Msg_Air = "AIR U";
  }
  else if (air_command == "D")
  {
    Msg_Air = "AIR D";
  }
  else if (air_command == "V")
  {
    Msg_Air = "AIR V";
  }
  else if (air_command == "B")
  {
    Msg_Air = "AIR B";
  }
  else if (air_command == "O")
  {
    Msg_Air = "AIR O";
  }
  else
  {
    Msg_Air = "NO" + air_command;
  }
}
