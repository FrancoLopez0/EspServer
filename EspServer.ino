/*********
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp32-web-server-slider-pwm/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/
// Import required libraries
#include <WiFi.h>
// #include <WebServer.h>
#include "ESPAsyncWebServer.h"
#include <ESP32Servo.h>
// Replace with your network credentials
#define SERVO0 19
#define SERVO1 18
#define SERVO2 5
#define SERVO3 17
#define SERVO4 16
#define SERVO5 4

const int SERVO_PIN[6] = {SERVO0, SERVO1, SERVO2, SERVO3, SERVO4, SERVO5};

const char* ssid = "ESP_Servo";
const char* password = "cualquiera";

const int output = 2;

bool LEDStatus = LOW;

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

// Create AsyncWebServer object on port 80
// WebServer server(80);
AsyncWebServer server(80);

Servo servo0,servo1,servo2,servo3,servo4,servo5;

Servo servo[6] = {servo0,servo1,servo2,servo3,servo4,servo5};

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html></html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP Servo Tester</title>
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 2.3rem;}
    p {font-size: 1.9rem;}
    body {max-width: 400px; margin:0px auto; padding-bottom: 25px;}
    .slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #FFD65C;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #003249; cursor: pointer;}
    .slider::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; } 
    .btn {
      border-radius:5cqmin;
      width:25%;
    }
    .btn.on{
      background-color: aqua;
    }
 </style>
</head>
<body>
  <h2>ESP Servo Tester</h2>
  <button id="led" class="btn">LED</button>
  <p><span id="servo0angle"> Servo 0</span></p>
  <p><input type="range" onchange="updateServo(this, 0, 'servo0angle')" id="pwmSlider" min="0" max="180" value="0" step="1" class="slider"></p>
  <p><span id="servo1angle"> Servo 1</span></p>
  <p><input type="range" onchange="updateServo(this, 1, 'servo1angle')" id="pwmSlider1" min="0" max="180" value="0" step="1" class="slider" ></p>
  <p><span id="servo2angle"> Servo 2</span></p>
  <p><input type="range" onchange="updateServo(this, 2, 'servo2angle')" id="pwmSlider1" min="0" max="180" value="0" step="1" class="slider" ></p>
  <p><span id="servo3angle"> Servo 3</span></p>
  <p><input type="range" onchange="updateServo(this, 3, 'servo3angle')" id="pwmSlider1" min="0" max="180" value="0" step="1" class="slider" ></p>
  <p><span id="servo4angle"> Servo 4</span></p>
  <p><input type="range" onchange="updateServo(this, 4, 'servo4angle')" id="pwmSlider1" min="0" max="180" value="0" step="1" class="slider" ></p>
  <p><span id="servo5angle"> Servo 5</span></p>
  <p><input type="range" onchange="updateServo(this, 5, 'servo5angle')" id="pwmSlider1" min="0" max="180" value="0" step="1" class="slider" ></p>
<script>
var led = document.getElementById('led');
var ledStatus = 'off';
led.addEventListener('click', ()=>{
  var xhr = new XMLHttpRequest();
  if(ledStatus == 'off')
  {
    led.classList.add('on');
    ledStatus = 'on';
  }
  else{
    led.classList.remove('on');
    ledStatus = 'off';
    // led.classList.add('off');
  }
  xhr.open("GET", ledStatus=='on'?"/?led=On":"/?led=Off", true);
  xhr.send();

})

function updateServo(element, servo_id, servo_angle) {
  var sliderValue = element.value;
  document.getElementById(servo_angle).innerHTML = sliderValue;
  console.log("/?servo"+servo_id+"="+sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/?servo"+servo_id+"="+sliderValue, true);
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

String processor(const String& var){
  Serial.println(var);
  return String();
}

void setup(){
  Serial.begin(115200);
  pinMode(output, OUTPUT);

  for(int i=0; i<6; i++){

  servo[i].attach(SERVO_PIN[i]);
  }

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway,subnet);
  delay(1000);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
 
    int paramsNr = request->params();
    Serial.println(paramsNr);
 
    for(int i=0;i<paramsNr;i++){
 
        AsyncWebParameter* p = request->getParam(i);
        Serial.print("Param name: ");
        Serial.println(p->name());
        Serial.print("Param value: ");
        Serial.println(p->value());
        Serial.println("------");
        if(p->name()=="led")
        {
          if(p->value()=="On"){
            digitalWrite(output, HIGH);
          }
          else{
            digitalWrite(output, LOW);
          }
        }
        if(p->name()=="servo0")
        {
          String val = p->value();
          servo[0].write(val.toInt());
        }
        if(p->name()=="servo1")
        {
          String val = p->value();
          servo[1].write(val.toInt());
        }
        if(p->name()=="servo2")
        {
          String val = p->value();
          servo[2].write(val.toInt());
        }
        if(p->name()=="servo3")
        {
          String val = p->value();
          servo[3].write(val.toInt());
        }
        if(p->name()=="servo4")
        {
          String val = p->value();
          servo[4].write(val.toInt());
        }
        if(p->name()=="servo5")
        {
          String val = p->value();
          servo[5].write(val.toInt());
        }
    }
 
    // request->send(200, "text/plain", "message received");
    request->send_P(200, "text/html", index_html, processor);
  });
  // server.on("/", handle_OnConnect);
  // server.on("/ledOn", handle_ledOn);
  // server.on("/ledOff", handle_ledOff);
  // server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP SERVER STARTED");
  Serial.println(WiFi.localIP());
}

void loop(){
  // server.handleClient();

  // if(LEDStatus){
  //   digitalWrite(output, HIGH);
  // }
  // else{
  //   digitalWrite(output, LOW);
  // }
}

String getHTML () {
    String htmlcode = "<!DOCTYPE html> <html>\n";
    htmlcode += "<head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n"; htmlcode += "<title>LED Control</title>\n";
    htmlcode += "</head>\n";
    htmlcode + "<body>\n";
    htmlcode += "<h1>ESP32 Servo Tester\n";
    if(LEDStatus){
      htmlcode +="<p> <a href=\"/ledOff\">Turn Off</a> </p>";
    }
    else{
      htmlcode +="<p> <a href=\"/ledOn\">Turn ON</a> </p>";
    }
    htmlcode += "</body>\n";
    htmlcode +"</html>\n";
    return htmlcode;
}

// void handle_OnConnect(){
//   LEDStatus = LOW;
//   Serial.println("LED Status: OFF");
//   server.send(200, "text/html", getHTML());
// }

// void handle_ledOn(){
//   LEDStatus = HIGH;
//   Serial.println("LED: HIGH");
//   server.send(200,"text/html", getHTML());
// }

// void handle_ledOff(){
//   LEDStatus = LOW;
//   Serial.println("LED: LOW");
//   server.send(200,"text/html", getHTML());
// }

// void handle_NotFound(){
//   server.send(404, "text/plain", "NotFound");
// }