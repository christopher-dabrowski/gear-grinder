#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

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
  WiFi.softAP(ssid);

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

// output HTML web page for user with virtual joystick
String SendHTML() {
  String html = "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "<title>GearGrinder</title>";
  html += "<meta name=\"viewport\" content=\"width=device-width, "
          "initial-scale=1, user-scalable=no\">";
  html += "<style>";
  html += "body {background-color: PeachPuff; margin: 0; padding: 20px; "
          "touch-action: none;}";
  html += "h1 {color: DarkBlue; margin-bottom: 5px;}";
  html += "p {color: #666; margin-top: 0;}";
  html += ".container {text-align: center;}";
  html += "#joystick {background: #f0f0f0; border-radius: 50%; border: 3px "
          "solid #888; touch-action: none;}";
  html += "#status {font-size: 24px; font-weight: bold; color: DarkBlue; "
          "margin-top: 20px;}";
  html += "</style>";
  html += "</head>";
  html += "<body>";
  html += "<div class=\"container\">";
  html += "<h1>GearGrinder is the best!</h1>";
  html += "<p>Michał & Krzysiek</p>";
  html += "<canvas id=\"joystick\" width=\"200\" height=\"200\"></canvas>";
  html += "<p id=\"status\">STOP</p>";
  html += "</div>";
  html += "<script>";
  html +=
      "var "
      "canvas=document.getElementById('joystick'),ctx=canvas.getContext('2d');";
  html +=
      "var centerX=100,centerY=100,stickX=100,stickY=100,maxR=75,stickR=25;";
  html += "var isDragging=false,sendInterval=null,lastLeft=0,lastRight=0;";
  html += "function draw(){";
  html += "ctx.clearRect(0,0,200,200);";
  html += "ctx.beginPath();ctx.arc(centerX,centerY,maxR,0,2*Math.PI);ctx."
          "strokeStyle='#888';ctx.lineWidth=2;ctx.stroke();";
  html += "ctx.beginPath();ctx.arc(stickX,stickY,stickR,0,2*Math.PI);ctx."
          "fillStyle='#0f8b8d';ctx.fill();";
  html += "}";
  html += "function getPos(e){";
  html += "var r=canvas.getBoundingClientRect();";
  html += "var x=(e.touches?e.touches[0].clientX:e.clientX)-r.left;";
  html += "var y=(e.touches?e.touches[0].clientY:e.clientY)-r.top;";
  html += "return{x:x,y:y};";
  html += "}";
  html += "function "
          "onStart(e){e.preventDefault();isDragging=true;onMove(e);"
          "startSending();}";
  html += "function onMove(e){";
  html += "if(!isDragging)return;e.preventDefault();";
  html +=
      "var "
      "p=getPos(e),dx=p.x-centerX,dy=p.y-centerY,dist=Math.sqrt(dx*dx+dy*dy);";
  html += "if(dist>maxR){dx=dx/dist*maxR;dy=dy/dist*maxR;}";
  html += "stickX=centerX+dx;stickY=centerY+dy;draw();";
  html += "}";
  html += "function "
          "onEnd(){isDragging=false;stickX=centerX;stickY=centerY;draw();"
          "stopSending();}";
  html += "function calcMotor(){";
  html += "var dx=(stickX-centerX)/maxR,dy=-(stickY-centerY)/maxR;";
  html += "var dist=Math.sqrt(dx*dx+dy*dy);";
  html += "if(dist<0.15){return{l:0,r:0,dir:'STOP'};}";
  html += "var l=Math.round((dy+dx)*1023),r=Math.round((dy-dx)*1023);";
  html +=
      "l=Math.max(-1023,Math.min(1023,l));r=Math.max(-1023,Math.min(1023,r));";
  html += "var dir='';";
  html += "if(dy>0.3)dir+='Forward';else if(dy<-0.3)dir+='Back';";
  html += "if(dx>0.3)dir+='-Right';else if(dx<-0.3)dir+='-Left';";
  html += "if(dir==='')dir='STOP';if(dir.startsWith('-'))dir=dir.substring(1);";
  html += "var pct=Math.round(dist*100);";
  html += "return{l:l,r:r,dir:dir+' '+pct+'%'};";
  html += "}";
  html += "function sendCmd(){";
  html += "var m=calcMotor();";
  html += "document.getElementById('status').textContent=m.dir;";
  html += "if(m.l!==lastLeft||m.r!==lastRight){";
  html += "fetch('/motor?left='+m.l+'&right='+m.r);lastLeft=m.l;lastRight=m.r;";
  html += "}}";
  html +=
      "function "
      "startSending(){if(!sendInterval)sendInterval=setInterval(sendCmd,100);}";
  html += "function stopSending(){";
  html += "if(sendInterval){clearInterval(sendInterval);sendInterval=null;}";
  html += "fetch('/motor?left=0&right=0');lastLeft=0;lastRight=0;";
  html += "document.getElementById('status').textContent='STOP';";
  html += "}";
  html += "canvas.addEventListener('touchstart',onStart,{passive:false});";
  html += "canvas.addEventListener('touchmove',onMove,{passive:false});";
  html += "canvas.addEventListener('touchend',onEnd);";
  html += "canvas.addEventListener('touchcancel',onEnd);";
  html += "canvas.addEventListener('mousedown',onStart);";
  html += "canvas.addEventListener('mousemove',onMove);";
  html += "canvas.addEventListener('mouseup',onEnd);";
  html += "canvas.addEventListener('mouseleave',onEnd);";
  html += "draw();";
  html += "</script>";
  html += "</body>";
  html += "</html>";
  return html;
}
