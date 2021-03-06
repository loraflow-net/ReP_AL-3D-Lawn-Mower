void Check_Wire_In_Out() {
  // Check the mower is inside (0) or outside (1) the perimeter wire

  if (Perimeter_Wire_Enabled == 1) {
    UpdateWireSensor();                                               // Read the wire sensor and see of the mower is now  or outside the wire  
    ADCMan.run();
    PrintBoundaryWireStatus();        
    if ( (perimeter.isInside(0)) == 0 )  {
      Outside_Wire = 1;                                               // Mower is outside the perimeter wire
      Print_LCD_Wire();
      Outside_Wire_Count = Outside_Wire_Count + 1;                    // Number of outside wire counts is increased.
      }
    if ( (perimeter.isInside(0)) == 1 )   {
      Outside_Wire = 0;                                               // Mower is inside the perimeter wire
      Outside_Wire_Count = 0;                                         // The number of outside wire counts is reset to 0
      }
    }

  // Options are set in the Mower Setup
  if (Outside_Wire_Count >= Outside_Wire_Count_Max)  {
    if  (Action_On_Over_Wire_Count_Max == 1) Manouver_Hibernate_Mower();                  // Put the mower to sleep and wait
    if  (Action_On_Over_Wire_Count_Max == 2) Manouver_Outside_Wire_ReFind_Function();     // re-find Garden using Sonar 1 and wire detect
  }
    
  }

//Check that boundary wire is turned on
//************************************************************************************
void TestforBoundaryWire()  {
  ADCMan.run();
  UpdateWireSensor();
  
  if (Perimeter_Wire_Enabled == 1) {                                               // Perimeter use is ON - Perimter_USE can be turned on or off in the setup.

    /*Checks to see if the boundary wire is turned OFF.
      uses the mag field calculated. Between -50 and 50 normally the field is off.     */

    MAG_Now = perimeter.getMagnitude(0);
    if ( (MAG_Now > -20 ) && (MAG_Now < 20 )  ) {
      Wire_Detected = 0;
      Print_LCD_NO_Wire();
      }

    //Checks to see that the boundary fence is turned ON.
    //  uses the mag field. under -50 or above 50 means the field is on
    if (   (MAG_Now < -50 ) || (MAG_Now > 50 )  ) {
      Wire_Detected = 1;
      Mower_Running = 1;
      }

    Serial.print(F("Wire"));
    Serial.print(F(":"));
    if (Wire_Detected == 0) Serial.print(F("OFF|"));
    if (Wire_Detected == 1) Serial.print(F("ON|"));
    Serial.print(F("Mag:"));
    Serial.print(MAG_Now);
    Serial.print("|");
    
  }

  // If the wire is in test mode
  if (Perimeter_Wire_Enabled == 0) {   
    Serial.print(F("Wire"));
    Serial.print(F(":"));                                            
    Serial.print(F("TEST|"));
    Wire_Detected = 1;

  }
}


void UpdateWireSensor()   {
  if (millis() >= nextTime)  {
    nextTime = millis() + 50;
    if (perimeter.isInside(0) != inside) {
      inside = perimeter.isInside(0);
      counter++;
    }
  }
}


void PrintBoundaryWireStatus() {

  //perimeter.is(0);
  // = perimeter.is(0);
  Serial.print("IN/OUT:");
  Serial.print((perimeter.isInside(0)));
  Serial.print("|");
  Serial.print("Mag:");
  Serial.print((int)perimeter.getMagnitude(0));
  Serial.print("|");
  Serial.print("Smag:");
  Serial.print((int)perimeter.getSmoothMagnitude(0));
  Serial.print("|");
  Serial.print("Q:");
  Serial.print((perimeter.getFilterQuality(0)));
  Serial.print("|");
  //Serial.print(" |  Variable: ");
  //Serial.print(inside);
  }
