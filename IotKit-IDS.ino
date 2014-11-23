//Program by Hemanth S
//Intel IoT Roadshow,Bangalore 2014

//Let the magic begin!

#include <IoTkit.h>    // include IoTkit.h to use the Intel IoT Kit
#include <Ethernet.h>  // must be included to use IoTkit
#include <aJSON.h>
#define cutoff 10
#define buzzer 2
#define Rled 4
#define Gled 3
// create an object of the IoTkit class
IoTkit iotkit;        
 
int x_avg=0;
int y_avg=0;
int z_avg=0;

//The following function is for taking the initial orientation of the accelerometer
void get_default_val()
{
#define N 100 
for(int i=0;i<N;i++)
  {
  x_avg+=analogRead(A0);
  y_avg+=analogRead(A1);
  z_avg+=analogRead(A2);
  }
x_avg/=N;
y_avg/=N;
z_avg/=N;

}

void setup()
{
  Serial.begin(115200);
  // call begin on the IoTkit object before calling any other methods
  iotkit.begin();
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(Rled,OUTPUT);
  pinMode(Gled,OUTPUT);
  get_default_val();
}

void loop() {
  int x=analogRead(A0);
  x-=x_avg;
  int y=analogRead(A1);
  y-=y_avg;
  int z=analogRead(A2);
  z-=z_avg;
//Print the difference in accelerometer readings to the screen
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.print(z);
  Serial.println(" ");
  
//Set the buzzer off if the door/window is opened.

  if(abs(x)>cutoff||abs(y)>cutoff||abs(z)>cutoff)
  {
      
    iotkit.send("powerswitch", true);  //For sending the component and the Value
    //Remember to change that name-powerswitch is too generic
      

      // you can also send a full JSON string with your own variables:
  //
  // aJsonObject* root = aJson.createObject();
  // if (root != NULL) {
  //    aJson.addItemToObject(root, "n", aJson.createItem(metric));
  //    aJson.addItemToObject(root, "v", aJson.createItem(value));
  //    iotkit.send(aJson.print(root)); // this sends your full json
  //    aJson.deleteItem(root);
  // }
  // 

  // If you are receiving incoming commands, listen for them with receive
  // If you have your own custom json-parsing receive function, pass as argument
  // such as iotkit.receive(callback);
  // It must follow this prototype, but any name: void callback(char* json)
  //

      digitalWrite(Gled,LOW);
      digitalWrite(buzzer,HIGH);
      digitalWrite(Rled,HIGH);
      delay(1000);
      digitalWrite(buzzer,LOW);
      digitalWrite(Rled,LOW);
      digitalWrite(Gled,HIGH);
      delay(1000);
  }
  digitalWrite(Gled,HIGH);
  // If you are receiving incoming commands, listen for them with receive
  // If you have your own custom json-parsing receive function, pass as argument
  // such as iotkit.receive(callback);
  // It must follow this prototype, but any name: void callback(char* json)
  //

  //iotkit.receive();
  //***iotkit.receive(callback);
  
  delay(100);
}



//A more sophisticated attempt with JSON

/*
// this is an example callback that parses a user-created JSON in the form
 {
    "component": "led",
    "command": "off"
 }
// and turns off LED at pin 13 hard-coded


void callback(char* json) {
  Serial.println(json);
  aJsonObject* parsed = aJson.parse(json);
  if (&parsed == NULL) {
    // invalid or empty JSON
    Serial.println("recieved invalid JSON");
    return;
  }
  aJsonObject* component = aJson.getObjectItem(parsed, "component");
  aJsonObject* command = aJson.getObjectItem(parsed, "command");
  if ((component != NULL)) {
    if (strcmp(component->valuestring, "led") == 0) {
      if ((command != NULL)) {
        if (strcmp(command->valuestring, "off") == 0) {
          pinMode(13, OUTPUT);
          digitalWrite(13, false);
        }
        if (strcmp(command->valuestring, "on") == 0) {
          pinMode(13, OUTPUT);
          digitalWrite(13, true);
        }
      }
    }
  }
}


//Getting the sensor Readings

int getReadings_JSON(){
  char scale[4];
  char raw[4];
  char offset[4];
  
  int raw_i;
  int scale_i;
  int offset_i;

    
  FILE *fp_raw;
  fp_raw = fopen("/sys/bus/iio/devices/iio:device0/in_temp0_raw", "r");     //read the values from scale, raw and offset files.
  fgets(raw, 4, fp_raw);                                                    //we need all three values, because the formula for
  fclose(fp_raw);                                                           //calulating the actual value in milli-degrees
  
  FILE *fp_scale;
  fp_scale = fopen("/sys/bus/iio/devices/iio:device0/in_temp0_scale", "r");
  fgets(scale, 4, fp_scale);
  fclose(fp_scale);
  
  FILE *fp_offset;
  fp_offset = fopen("/sys/bus/iio/devices/iio:device0/in_temp0_offset", "r");
  fgets(offset, 4, fp_offset);
  fclose(fp_offset);
   
  raw_i = atoi(raw);         //we have the values now, but they are in ASCII form-                                                       
  scale_i = atoi(scale);     //we need them as integers so we can use them for calculations.
  offset_i = atoi(offset);
  
  int temp = (raw_i + offset_i) * scale_i;  //Calculate temperature in milli-degrees 
  temp /= 1000;                         //divide by 1000 to convert to degrees
  return temp;   
}
*/
