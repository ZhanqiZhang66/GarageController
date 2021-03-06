#include "Adafruit_SSD1306/Adafruit_SSD1306.h"
// use hardware SPI
#define OLED_DC     D3
#define OLED_CS     D4
#define OLED_RESET  D5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

const int doorBtn = D0;
const int lightBtn = D1;
const int deltatime=100;

long rolltime;
int  x, minX; // variables for scrolling code
const String topicGet = "Assignement5";
const String topicPost = "Assignement5";
String msg;
int msgLength=10;

bool readingData;

void setup()   {
// start the data retrieval timer
Serial.begin(9600);
Particle.connect();
Particle.subscribe(topicGet, gotData);

Particle.publish(topicPost, "state"); // publish the event to trigger the data

pinMode(doorBtn, INPUT_PULLUP);
pinMode(lightBtn, INPUT_PULLUP);

// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
display.begin(SSD1306_SWITCHCAPVCC);
display.setTextSize(7);       // text size
display.setTextColor(WHITE); // text color
display.setTextWrap(false); // turn off text wrapping so we can do scrolling
x    = display.width(); // set scrolling frame to display width

rolltime=millis();
minX = -msgLength*6*7*2; // 630 = 6 pixels/character * text size 7 * 15 characters * 2x slower
}

boolean isDoorButtonPressed(){
    return !digitalRead(doorBtn);
}

boolean isLightButtonPressed(){
    return !digitalRead(lightBtn);
}

bool buttonstate,lightbuttonstate;
bool buttonstateprev,buttonstatenow,bs_p3,bs_p2,bs_p1,lightbuttonstateprev,lightbuttonstatenow,lbs_p3,lbs_p2,lbs_p1;


void loop() {

// this code displays the next bus times on the OLED screen with fancy scrolling
if ((long)(millis()-rolltime)>=0){
    rolltime+=deltatime;
    if (buttonstate==0){
      buttonstateprev=buttonstatenow;
      buttonstatenow=isDoorButtonPressed();
      buttonstate=buttonstateprev and buttonstatenow;
    }
    bs_p3=bs_p2;
    bs_p2=bs_p1;
    bs_p1=buttonstate;
    if(bs_p3==1 and bs_p2==1){
      buttonstate=0;
    }
    if (buttonstate) msg="It works!";
    if (lightbuttonstate==0){
      lightbuttonstateprev=lightbuttonstatenow;
      lightbuttonstatenow=isLightButtonPressed();
      lightbuttonstate=lightbuttonstateprev and lightbuttonstatenow;
    }
    lbs_p3=lbs_p2;
    lbs_p2=lbs_p1;
    lbs_p1=lightbuttonstate;
    if (lightbuttonstate) msg="It lights!";
    if(lbs_p3==1 and lbs_p2==1){
      lightbuttonstate=0;
    }
}


display.clearDisplay();
display.setCursor(x/2, 7);
display.print(msg);
display.display();
if(--x < minX) x = display.width()*2;

}

// This function will get called when NextBus webhook data comes in.
// It turns the full NextBus XML page into numbers to be displayed on the screen
void gotData(const char *name, const char *data) {
  Serial.println("Haha!");
  // put the incoming data (the XML page) into a string called "str"
  readingData = true;
  String str = String(data);
  Serial.println(str);
  msg = str;
  msgLength = str.length();
}
