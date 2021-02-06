//FRONT SIDE CODE
#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;
File root;
int file_id = -1;


//PINS
int selectorCtrl[] = {6, 5, 4, 3};
int inputPin = A0;    
int sensorVal = 0;


//Sensor Traccking
int vals[] = {0,0,0,0,0,0,0,0,0,0,0}; //the sampled value at this moment
int last[] = {0,0,0,0,0,0,0,0,0,0,0}; //the last sampled value
int window_sums[] = {0,0,0,0,0,0,0,0,0,0,0}; //how many "presses" have been registered within the window
int states[] = {0,0,0,0,0,0,0,0,0,0,0}; //0 = no press, 1 = pressed, -1 = null or likely error val


//thresholds - a change between min and max is registered as a press or unpress, something greater than make is likely an error
int step_min = 50;
int step_max = 500;

//Envt
int res = 100; //how frequently a sample is taken from the data stream (once every res cycles); 
int window = 1000; //how frequently a data log is sent to the SD Ccardd and cleared (once every window cycles);
int timer = 0;
int first = true;


//prestore the selector values for the pins that are connected in this interation
//Order >> 0, 1, 2, 3
int select5[] = {1,0,1,0};
int select6[] = {0,1,1,0};
int select7[] = {1,1,1,0};
int select8[] = {0,0,0,1};
int select9[] = {1,0,0,1};
int select10[] = {0,1,0,1};
int select11[] = {1,1,0,1};
int select12[] = {0,0,1,1};
int select13[] = {1,0,1,1};
int select14[] = {0,1,1,1};
int select15[] = {1,1,0,1};

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("\nInitializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  root = SD.open("/");

  file_id = countDirectory(root, 0);
  Serial.println(file_id);

  Serial.println("done!");

  //init multiplex outs
  int it;
  for(it = 0; it < 4; it++){
    pinMode(selectorCtrl[it], OUTPUT);
  }  
}

int countDirectory(File dir, int numTabs) {
  int count = 0;
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      countDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
      count++;
    }
    entry.close();
  }

  return count;
}


void loop() {



   //get the values
   vals[0] = getValue(select15);
   vals[1] = getValue(select14);
   vals[2] = getValue(select13);
   vals[3] = getValue(select12);
   vals[4] = getValue(select11);
   vals[5] = getValue(select10);
   vals[6] = getValue(select9);
   vals[7] = getValue(select8);
   vals[8] = getValue(select7);
   vals[9] = getValue(select6);
   vals[10] = getValue(select5);


   if(!first){
     int it; 
    //print to screen
    for(it = 0; it < 11; it++){
      //get the different between this and the last value
      int diff = vals[it] - last[it];
      //if this counts as a valid step, add it to the state
      if(abs(diff) > step_min && abs(diff) < step_max){

          if(diff < 0 ){
            states[it] = 1;
          }else{
            states[it] = 0;
          }
        
      }else if(abs(diff) >= step_max){
        states[it] = -1;     
      }


      if(states[it] != -1) window_sums[it] += states[it];
    
    }


    printVals(states, true);
    Serial.println();


   }
   int it;

    if(timer > window){
       first = false;
       SD_Write(printVals(window_sums, true));
      for(it = 0; it < 11; it++){
        window_sums[it] = 0;
      }

      
      timer = 0;
    }


   delay(res);
    timer += res;
   for(it = 0; it < 11; it++){
      last[it] = vals[it];
   }
}

String printVals(int vals[], boolean out){
  
    int it; 
    String dataString = "";    
    //print to screen
  
   for(it = 0; it < 11; it++){
      dataString += String(vals[it]);
      if(out) Serial.print(vals[it]);
      if(it != 10){
        dataString  += ",";
        if(out) Serial.print(",");
      }
   }

   return dataString;
}

void SD_Write(String dataString){

  String filename = file_id + String(".txt");
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(filename, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    //Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening"+filename);
  }
}

int getValue( int selection[4]){
    int it;
 
   for(it = 0; it < 4; it++){
    digitalWrite(selectorCtrl[it], selection[it]);
   }
   
   return analogRead(inputPin);
}
