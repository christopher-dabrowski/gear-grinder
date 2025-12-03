#pragma once

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
