#include "GarageHardware.h"

#define publishtime 2000
#define deltatime 45
#define fadetime 5000
#define motiondelaytime 10
#define motionHoldTime 90
int fadeValue;
unsigned long publishrolltime;
unsigned long rolltime;
unsigned long lighttime;
unsigned long fadestarttime;
unsigned long motionstarttime;
unsigned long MotorRolltime;

int fadingcycle = 100;
int doorAutoCloseTime = 5;

bool doorAutoCloseEnable = true;
bool buttonstateprev,buttonstatenow,buttonstate;
bool autoCloseFlag =false;
//light
bool prev_lightOn=false;
bool lightOn = false;
int lightAutoOffTime = 5;
int MaxBrightness = 100;

const String topicGet = "Assignment5-Get";
const String topicPost = "Assignment5-Post";

enum State {
  CLOSED,
  CLOSING,
  OPEN,
  OPENING,
  OPENING_STOPPED,
  CLOSING_STOPPED,
  OPENING_FAULT_STOPPED,
  CLOSING_FAULT_STOPPED
};
State cur_state;


Timer fadeTimer(fadetime/100, fadeHelper);
void fadeHelper(){
  if(fadingcycle >= 0){
        setLightPWM(((float)fadingcycle)/100*MaxBrightness);
        fadingcycle -= 1;
        Serial.println("Fading time is " + fadingcycle);
      }
      else{
        Serial.println("Stopped fading ");
        lightOn = false;
        setLight(lightOn);
        fadeTimer.stop();
      //  publishData();
      }
}

//Set the Auto Off time for the light
void startLightAutoOff(){
  if(lightOn) {
    setLightAutoOffTime();
    Serial.println("Light start to turn off");
  }
}

void ThenStartFading(){
  fadingcycle = 100;
  fadeTimer.reset();
  Serial.println("Then start fading");
}

Timer lightAutoOffTimer(lightAutoOffTime * 1000, ThenStartFading, true);
void setLightAutoOffTime(){
  lightAutoOffTimer.changePeriod(lightAutoOffTime * 1000);
  lightAutoOffTimer.reset();
  Serial.println("Light auto Off time: ");
  Serial.println(lightAutoOffTime);
}

//This function replace the setLight with max brightness
void TurnOnLightWithMaxBrightness(){
  lightOn = true;
  fadeTimer.stop();
  lightAutoOffTimer.stop();
  setLightPWM(MaxBrightness);
//  publishData();
}

//Set the Auto Close Time for the door
void doorhelper(){
  if(cur_state == OPEN){
    autoCloseFlag=true;
  }
}
Timer doorTimer(doorAutoCloseTime * 1000, doorhelper, true);

void checkMotion(State state){
  if((millis() - motionstarttime) > motionHoldTime){
  switch(state){
    case CLOSING:
      if (isDoorFullyClosed())
        stopMotor();
      else
        startMotorClosing();
      break;
    case OPENING:
      if (isDoorFullyOpen())
        stopMotor();
      else
        startMotorOpening();
      break;
    case OPEN:
      stopMotor();
      break;
    case CLOSED:
      stopMotor();
      break;
    case OPENING_STOPPED:
      stopMotor();
      break;
    case CLOSING_STOPPED:
      stopMotor();
      break;
    case OPENING_FAULT_STOPPED:
      stopMotor();
      break;
    case CLOSING_FAULT_STOPPED:
      stopMotor();
      break;
  }
  }
}

State  newState;
State  preState;

State nextState(State state){
  //State Transition
  unsigned long currentTime=millis();
  if((millis() - motionstarttime) > motionHoldTime){
  switch(state){
    case CLOSED:
      newState=CLOSED;
      preState=CLOSED;
      if (buttonstate){
        newState = OPENING;
        motionstarttime=millis();
        TurnOnLightWithMaxBrightness();
      //  publishData();
      }
      break;
    case OPENING:

      newState = OPENING;
      preState = OPENING;


      if (buttonstate){
        startLightAutoOff();
        newState = OPENING_STOPPED;
        motionstarttime=millis();
      //  publishData();
      }
      if(isFaultActive()){
        startLightAutoOff();
        newState = OPENING_FAULT_STOPPED;
        motionstarttime=millis();
        sendDebug("fault");
      //  publishData();
      }
      else if(isDoorFullyOpen()) {
        startLightAutoOff();
        newState = OPEN;
        doorAutoClose("");
        motionstarttime=millis();
      //  publishData();
      }
      break;
    case OPEN:
      newState=OPEN;
      preState = OPEN;
      if (buttonstate){
        newState = CLOSING;
        motionstarttime=millis();
        TurnOnLightWithMaxBrightness();

      //  publishData();
      }
      if (autoCloseFlag){
        newState=CLOSING;
        motionstarttime=millis();
        autoCloseFlag=false;
        TurnOnLightWithMaxBrightness();
//        publishData();
      }
      break;
    case CLOSING:

      newState = CLOSING;
      preState = CLOSING;
      if(buttonstate){
         startLightAutoOff();
         newState = CLOSING_STOPPED;
         motionstarttime=millis();
      //   publishData();
       }
      if(isFaultActive()){
        startLightAutoOff();
        newState = CLOSING_FAULT_STOPPED;
        motionstarttime=millis();
        sendDebug("Fault");
    //    publishData();
      }
      else if(isDoorFullyClosed()){
        startLightAutoOff();
         newState = CLOSED;
         motionstarttime=millis();
      //   publishData();
       }
      break;

    case OPENING_STOPPED:
        if(buttonstate){
        newState = CLOSING;
        preState = OPENING_STOPPED;
        TurnOnLightWithMaxBrightness();

        motionstarttime=millis();
    //    publishData();
      }
      else{
        newState=OPENING_STOPPED;
        preState=OPENING_STOPPED;
    //    publishData();
      }
      break;
    case CLOSING_STOPPED:
        if(buttonstate ){
          newState = OPENING;
          preState = CLOSING_STOPPED;
          TurnOnLightWithMaxBrightness();

          motionstarttime=millis();
      //    publishData();
        }
        else{
          newState=CLOSING_STOPPED;
          preState=CLOSING_STOPPED;
      //    publishData();
        }
        break;
    case OPENING_FAULT_STOPPED:
            if(buttonstate){
            newState = OPENING;
            preState = OPENING_FAULT_STOPPED;
            TurnOnLightWithMaxBrightness();
            motionstarttime=millis();
      //      publishData();
          }
          else{
            newState=OPENING_FAULT_STOPPED;
            preState=OPENING_FAULT_STOPPED;
      //      publishData();
          }
          break;
    case CLOSING_FAULT_STOPPED:
              if(buttonstate){
              newState = CLOSING;
              preState = CLOSING_FAULT_STOPPED;
              TurnOnLightWithMaxBrightness();

              motionstarttime=millis();
    //          publishData();
            }
            else{
              newState=CLOSING_FAULT_STOPPED;
              preState=CLOSING_FAULT_STOPPED;
    //          publishData();
            }
            break;
  }
  }
  return newState;
}
bool bs_p1=0,bs_p2=0,bs_p3=0;

State initialState(){
  if(isDoorFullyOpen()) {
    sendDebug("Open");
    return OPEN;
  }
  if(isDoorFullyClosed()){
    sendDebug("CLOSED");
    return CLOSED;
  }
  return CLOSED;
}

void setup() {
  rolltime=millis()+deltatime;
  lighttime=millis();
  publishrolltime=millis();
  MotorRolltime=millis()+motiondelaytime;
  setupHardware();
  Serial.begin(9600);
  Particle.function("getState", getState);
  Particle.function("sendButton", sendButton);
  Particle.function("setDoorAutoCloseEnable", setDoorAutoCloseEnable);
  Particle.function("doorAutoTime", doorAutoTime);
  //light cloud function
  Particle.function("setlightAutoOff",setLightAutoOff);
  Particle.function("setBrightness", setBrightness);
  Particle.function("toggleLight", toggleLight);

  Particle.variable("doorAutoCloseTime",doorAutoCloseTime);
  Particle.variable("doorAutoCloseEnable",doorAutoCloseEnable);

  cur_state = initialState();
  publishData();
  Serial.println(Particle.subscribe(topicGet, remoteMsg));

}
void remoteMsg(String name, String data){
  if(data == "state") publishData();
  else if (data == "doorBtn") sendButton("");
  else if (data == "lightBtn") toggleLight("");
  else{
    Serial.println("unknown data");
  }
}
void loop() {
  if ((long)(millis()-MotorRolltime)>=0){
    MotorRolltime+=motiondelaytime;
    checkMotion(cur_state);
  }
  if ((long)(millis()-publishrolltime)>=0){
    publishrolltime=millis()+publishtime;
//    publishData(); //demo_super+++
  }

  if ((long)(millis()-rolltime)>=0){
    rolltime+=deltatime;
    if (buttonstate==0){
      buttonstateprev=buttonstatenow;
      buttonstatenow=isButtonPressed();
      buttonstate=buttonstateprev and buttonstatenow;
    }
    bs_p3=bs_p2;
    bs_p2=bs_p1;
    bs_p1=buttonstate;
    if(bs_p3==1 and bs_p2==1){
      buttonstate=0;
    }
   //stateOutput(cur_state);
    cur_state = nextState(cur_state);
    if (prev_lightOn!=lightOn || cur_state!=preState){
      publishData();
    }

    prev_lightOn=lightOn;
    if(isDoorFullyOpen()) {
      char *message="OPEN";
    //  sendDebug(message);
    }
  }
}
/****************************
    Below are cloud functions
*****************************/
int doorAutoClose(String string){
  //doorAutoCloseEnable = !doorAutoCloseEnable;
  autoCloseFlag=false;
  //Serial.println(doorTimer.isActive());
  //Serial.println(doorAutoCloseTime);
  if(doorAutoCloseEnable){
    doorTimer.start();
    doorTimer.changePeriod(doorAutoCloseTime * 1000);
  }
  else{
    doorTimer.stop();
  }
  publishData();
  return doorAutoCloseEnable;
}

int setDoorAutoCloseEnable(String string){
  doorAutoCloseEnable = !doorAutoCloseEnable;
  if (doorAutoCloseEnable && cur_state==OPEN)
            doorAutoClose("");
  publishData();
  return 1;
}

int doorAutoTime(String string){
  char * myword = new char[string.length() + 1];
  strcpy(myword, string.c_str());
  doorAutoCloseTime = atoi(myword);
  int newPeriod = doorAutoCloseTime * 1000;
  if (doorAutoCloseEnable && cur_state==OPEN)
            doorAutoClose("");
  publishData();
  return doorAutoCloseTime;
}
//light cloud function
int setLightAutoOff(String string){
  lightAutoOffTime = string.toInt();
  lightAutoOffTimer.changePeriod(lightAutoOffTime * 1000);
  publishData();
  return lightAutoOffTime;
}

int setBrightness(String string){
  MaxBrightness = string.toInt();
  if(lightOn) setLightPWM(MaxBrightness);
  publishData();
  return MaxBrightness;
}

int toggleLight(String string){
//  if(cur_state != OPENING && cur_state != CLOSING) {
    lightOn = !lightOn;
//  }
  //toggle it when light is off ->then lightOn becomes true
  if(lightOn){
//    Serial.println("light is turned on from UI");
    //make it turn on immediately and reset/restart the auto-off timer.
    TurnOnLightWithMaxBrightness();
    if(cur_state != OPENING && cur_state != CLOSING){
      setLightAutoOffTime();
    }
  }
  //Toggling it while fading (and on) will make it turn off immediately.
  else{
  //  Serial.println("light is turned off from UI");
    fadeTimer.stop();
    setLight(lightOn);
  }

  publishData();
  return lightOn;
}

int getState(String string){
  publishData();
  return 1;
}
int sendButton(String string){
  buttonstate = true;
  return 1;
}
String garageState[] = {"CLOSED", "CLOSING", "OPEN", "OPENING", "OPENING: Interrupted", "CLOSING: Interrupted", "OPENING: !Fault!", "CLOSING: !Fault!"};

int publishData(){
//  Serial.println("Publish");

  String s = "{";
  s += "\"cur_state\":";
  s += String(cur_state);
  s += ",\"doorAutoClose\":";
  s += String(doorAutoCloseEnable);
  s += ",\"doorAutoTime\":";
  s += String(doorAutoCloseTime);
  s += ",\"lightOn\":";
  s += String(lightOn);
  s += ",\"lightAutoOffTime\":";
  s += String(lightAutoOffTime);
  s += ",\"MaxBrightness\":";
  s += String(MaxBrightness);
  s += "}";
  Serial.println("Publishing:");
  Serial.println(s);
  Particle.publish("Assignment5", s, 60, PRIVATE);
  //Then publisd to the remote control
  String ss = "";
  ss += "Door: ";
  ss += String(garageState[cur_state]);
  ss += " , Light: ";
  ss += lightOn ? "ON": "OFF";
  Particle.publish(topicPost, ss,60,PRIVATE);
  return 0;
}
