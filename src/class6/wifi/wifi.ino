#include "./secrets.h"
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <pgmspace.h>

// WiFi settings
const char *ssid = "GearGrinder";

// motor settings
#define RIGHT_MOTOR_PIN1 D1          // pin 1 of right motor (D2)
#define RIGHT_MOTOR_PIN2 D3          // pin 2 of right motor (D1)
#define LEFT_MOTOR_PIN1 D2           // pin 1 of left motor (D3)
#define LEFT_MOTOR_PIN2 D4           // pin 2 of left motor (D4)
#define RIGHT_MOTOR_SPEED 1023       // speed for right motor (0-1023)
#define LEFT_MOTOR_SPEED 1023        // speed for left motor (0-1023)
#define ENGINE_BALANCING_DECREASE 15 // compensate for motor differences
#define LED_PIN 2                    // built-in LED (D4)

IPAddress local_ip(192, 168, 4, 1);  // IP for AP mode
IPAddress gateway(192, 168, 100, 1); // IP for AP mode
IPAddress subnet(255, 255, 255, 0);  // IP for AP mode
ESP8266WebServer server(80);

int car_mode = 0;        // set car drive mode (0 = stop)
int leftMotorSpeed = 0;  // left motor speed (-1023 to 1023)
int rightMotorSpeed = 0; // right motor speed (-1023 to 1023)

// initialize
void setup() {

  Serial.begin(115200);
  Serial.println("SIC_Robot");
  pinMode(RIGHT_MOTOR_PIN1, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN2, OUTPUT);
  pinMode(LEFT_MOTOR_PIN1, OUTPUT);
  pinMode(LEFT_MOTOR_PIN2, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  car_control(); // stop the car

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, WIFI_PASSWORD);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // setup web server to handle specific HTTP requests
  server.on("/", HTTP_GET, handle_OnConnect);
  server.on("/forward", HTTP_GET, handle_forward);
  server.on("/backward", HTTP_GET, handle_backward);
  server.on("/left", HTTP_GET, handle_left);
  server.on("/right", HTTP_GET, handle_right);
  server.on("/stop", HTTP_GET, handle_stop);
  server.on("/motor", HTTP_GET, handle_motor);
  server.onNotFound(handle_NotFound);

  // start server
  server.begin();
  Serial.println("ESP8266 car server started.");
  digitalWrite(LED_PIN, LOW);
}

// handle HTTP requests and control car
void loop() {
  server.handleClient();
  car_control();
}

// HTTP request: on connect
void handle_OnConnect() {
  car_mode = 0;
  Serial.println("Client connected");
  server.send(200, "text/html", SendHTML());
}

// HTTP request: stop car
void handle_stop() {
  car_mode = 0;
  Serial.println("Stopped");
  server.send(200, "text/html", SendHTML());
}

// HTTP request: go forward
void handle_forward() {
  car_mode = 1;
  Serial.println("Go forward...");
  server.send(200, "text/html", SendHTML());
}

// HTTP request: go backward
void handle_backward() {
  car_mode = 2;
  Serial.println("Go backward...");
  server.send(200, "text/html", SendHTML());
}

// HTTP request: turn left
void handle_left() {
  car_mode = 3;
  Serial.println("Turn left...");
  server.send(200, "text/html", SendHTML());
}

// HTTP request: turn right
void handle_right() {
  car_mode = 4;
  Serial.println("Turn right...");
  server.send(200, "text/html", SendHTML());
}

// HTTP request: motor control with speed
void handle_motor() {
  if (server.hasArg("left") && server.hasArg("right")) {
    leftMotorSpeed = server.arg("left").toInt();
    rightMotorSpeed = server.arg("right").toInt();
    // Clamp values to valid range
    leftMotorSpeed = constrain(leftMotorSpeed, -1023, 1023);
    rightMotorSpeed = constrain(rightMotorSpeed, -1023, 1023);
  }
  server.send(200, "text/plain", "OK");
}

// HTTP request: other
void handle_NotFound() {
  car_mode = 0;
  Serial.println("Page error");
  server.send(404, "text/plain", "Not found");
}

// control car movement using PWM differential drive
void car_control() {
  // Set direction pins based on speed sign
  digitalWrite(RIGHT_MOTOR_PIN2, rightMotorSpeed >= 0 ? HIGH : LOW);
  digitalWrite(LEFT_MOTOR_PIN2, leftMotorSpeed >= 0 ? HIGH : LOW);

  // Set PWM speed (apply engine balancing to left motor)
  int leftPWM = max(0, abs(leftMotorSpeed) - ENGINE_BALANCING_DECREASE);
  int rightPWM = abs(rightMotorSpeed);

  analogWrite(RIGHT_MOTOR_PIN1, rightPWM);
  analogWrite(LEFT_MOTOR_PIN1, leftPWM);
}

// The HTML is stored in flash (PROGMEM) to avoid repeated RAM allocations
const char index_html[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html>
<head>
<title>GearGrinder</title>
<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
<style>
body {background-color: PeachPuff; margin: 0; padding: 20px; touch-action: none;}
h1 {color: DarkBlue; margin-bottom: 5px;}
p {color: #666; margin-top: 0;}
.container {text-align: center;}
#joystick {background: #f0f0f0; border-radius: 50%; border: 3px solid #888; touch-action: none;}
#status {font-size: 24px; font-weight: bold; color: DarkBlue; margin-top: 20px;}
</style>
</head>
<body>
<div class="container">
<h1>GearGrinder is the best!</h1>
<p>Michal & Krzysiek</p>
<canvas id="joystick" width="200" height="200"></canvas>
<p id="status">STOP</p>
</div>
<script>
var canvas=document.getElementById('joystick'),ctx=canvas.getContext('2d');
var centerX=100,centerY=100,stickX=100,stickY=100,maxR=75,stickR=25;
var isDragging=false,sendInterval=null,lastLeft=0,lastRight=0;
function draw(){
ctx.clearRect(0,0,200,200);
ctx.beginPath();ctx.arc(centerX,centerY,maxR,0,2*Math.PI);ctx.strokeStyle='#888';ctx.lineWidth=2;ctx.stroke();
ctx.beginPath();ctx.arc(stickX,stickY,stickR,0,2*Math.PI);ctx.fillStyle='#0f8b8d';ctx.fill();
}
function getPos(e){
var r=canvas.getBoundingClientRect();
var x=(e.touches?e.touches[0].clientX:e.clientX)-r.left;
var y=(e.touches?e.touches[0].clientY:e.clientY)-r.top;
return{x:x,y:y};
}
function onStart(e){e.preventDefault();isDragging=true;onMove(e);startSending();}
function onMove(e){
if(!isDragging)return;e.preventDefault();
var p=getPos(e),dx=p.x-centerX,dy=p.y-centerY,dist=Math.sqrt(dx*dx+dy*dy);
if(dist>maxR){dx=dx/dist*maxR;dy=dy/dist*maxR;}
stickX=centerX+dx;stickY=centerY+dy;draw();
}
function onEnd(){isDragging=false;stickX=centerX;stickY=centerY;draw();stopSending();}
function calcMotor(){
var dx=(stickX-centerX)/maxR,dy=-(stickY-centerY)/maxR;
var dist=Math.sqrt(dx*dx+dy*dy);
if(dist<0.15){return{l:0,r:0,dir:'STOP'};}
var l=Math.round((dy+dx)*1023),r=Math.round((dy-dx)*1023);
l=Math.max(-1023,Math.min(1023,l));r=Math.max(-1023,Math.min(1023,r));
var dir='';
if(dy>0.3)dir+='Forward';else if(dy<-0.3)dir+='Back';
if(dx>0.3)dir+='-Right';else if(dx<-0.3)dir+='-Left';
if(dir==='')dir='STOP';if(dir.startsWith('-'))dir=dir.substring(1);
var pct=Math.round(dist*100);
return{l:l,r:r,dir:dir+' '+pct+'%'};
}
function sendCmd(){
var m=calcMotor();
document.getElementById('status').textContent=m.dir;
if(m.l!==lastLeft||m.r!==lastRight){
fetch('/motor?left='+m.l+'&right='+m.r);lastLeft=m.l;lastRight=m.r;
}}
function startSending(){if(!sendInterval)sendInterval=setInterval(sendCmd,100);}
function stopSending(){
if(sendInterval){clearInterval(sendInterval);sendInterval=null;}
fetch('/motor?left=0&right=0');lastLeft=0;lastRight=0;
document.getElementById('status').textContent='STOP';
}
canvas.addEventListener('touchstart',onStart,{passive:false});
canvas.addEventListener('touchmove',onMove,{passive:false});
canvas.addEventListener('touchend',onEnd);
canvas.addEventListener('touchcancel',onEnd);
canvas.addEventListener('mousedown',onStart);
canvas.addEventListener('mousemove',onMove);
canvas.addEventListener('mouseup',onEnd);
canvas.addEventListener('mouseleave',onEnd);
draw();
</script>
</body>
</html>)rawliteral";

// Return a single String instance constructed once from the PROGMEM HTML
String SendHTML() {
  static String html;
  if (html.length() == 0) {
    html = FPSTR(index_html);
  }
  return html;
}
