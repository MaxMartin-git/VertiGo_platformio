#include "webpage.h"
#include "../include/commands.h"
#include "batterieStatus.h"
#include <Arduino.h>
#include <WiFiS3.h>

#define BTN_SIZE_COMMON "width:140px; height:45px; font-size:16px; text-align:center; vertical-align:middle;"

#define BTN_INACTIVE "#e0e0e0"
#define BTN_ACTIVE   "#b0b0b0"
const char* activeWeight   = "font-weight:bold;";
const char* inactiveWeight = "font-weight:normal;";

void sendWebpage(WiFiClient &client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();

  client.println("<!DOCTYPE html>");
  client.println("<html><head><meta name='viewport' content='width=device-width, initial-scale=1'>");

  if (mode != MANUAL) {
    Serial.println("autoRefresh aktiv");
    client.println("<meta http-equiv='refresh' content='2'>");
  }

  client.println("<style>");
  client.println("button{padding:20px;font-size:20px;margin:10px;}");
  client.println("</style></head><body>");

  client.println("<h1>VertiGo - Bedienpanel</h1>");

  client.print("<p>Batterie: <b>");
  client.print(batterySoC);
  client.print(" %</b></p>");

  client.print("<p>Antriebsfreigabe <b>");

  client.print("<p>Status: <b>");
  client.print(enableMotors ? "FREIGEGEBEN" : "GESPERRT");
  client.println("</b></p>");

  client.println("<a href='/?motor=on'><button style='background:lightgreen; " BTN_SIZE_COMMON "'>FREIGEBEN</button></a>");
  client.println("<a href='/?motor=off'><button style='background:salmon; " BTN_SIZE_COMMON "'>SPERREN</button></a>");

  const char* manualColor = (mode == MANUAL) ? BTN_ACTIVE : BTN_INACTIVE;
  const char* autoColor   = (mode == WALL_ALIGN) ? BTN_ACTIVE : BTN_INACTIVE;
  const char* manualWeight = (mode == MANUAL) ? activeWeight : inactiveWeight;
  const char* autoWeight   = (mode == WALL_ALIGN) ? activeWeight : inactiveWeight;

  client.print("<p><b>Fahrmodus</b></p>");

  client.println(
    "<a href='/?steeringmode=manual'>"
    "<button style='background:" + String(manualColor) + "; "
    + String(BTN_SIZE_COMMON) + String(manualWeight) + "'>Manuell</button>"
    "</a>"
  );

  client.println(
    "<a href='/?steeringmode=auto'>"
    "<button style='background:" + String(autoColor) + "; "
    + String(BTN_SIZE_COMMON) + String(autoWeight) + "'>Auto</button>"
    "</a>"
  );

  client.println("<h2>Joystick</h2>");
  client.println("<div id='joyContainer'>");
  client.println("  <div id='joy'></div>");
  client.println("</div>");

  client.println("<style>");
  client.println("#joyContainer {");
  client.println("  width: 300px;");
  client.println("  height: 300px;");
  client.println("  background: #eee;");
  client.println("  border-radius: 50%;");
  client.println("  position: relative;");
  client.println("  touch-action: none;");
  client.println("  margin-top: 20px;");
  client.println("}");
  client.println("#joy {");
  client.println("  width: 60px;");
  client.println("  height: 60px;");
  client.println("  background: #ccc;");
  client.println("  border-radius: 50%;");
  client.println("  position: absolute;");
  client.println("  left: 50%;");
  client.println("  top: 50%;");
  client.println("  transform: translate(-50%, -50%);");
  client.println("}");
  client.println("</style>");

  client.println("<script>");
  client.println("const joy = document.getElementById('joy');");
  client.println("const box = document.getElementById('joyContainer');");

  client.println("let active = false;");
  client.println("let boxRect = box.getBoundingClientRect();");

  client.println("let lastX = 0;");
  client.println("let lastY = 0;");
  client.println("let centerX = 0;");
  client.println("let centerY = 0;");
  client.println("let maxRadius = 0;");
  client.println("let calculated = false;");
    
  client.println("let lastSendTime = 0;");
  client.println("let stickRadius = 30;");
  client.println("const SEND_INTERVAL = 200");

  client.println("function send(x, y) {");
  client.println("  if (x === lastX && y === lastY) return;");
  client.println("  const now = Date.now();");
  client.println("  if (((x != 0) || (y != 0)) && now - lastSendTime < SEND_INTERVAL) return;");
  client.println("  lastSendTime = now;");
  client.println("  lastX = x;");
  client.println("  lastY = y;");
  client.println("  fetch(`/joy?x=${x}&y=${y}`);");
  client.println("}");

  client.println("function CalcJoyGeom() {");
  client.println("  centerX = boxRect.width / 2 + boxRect.left;");
  client.println("  centerY = boxRect.height / 2 + boxRect.top;");
  client.println("  maxRadius = centerX - stickRadius;");
  client.println("  calculated = true;");
  client.println("}");

  client.println("function handle(e) {");
  client.println("  if (!active) return;");
  client.println("  if (!calculated) CalcJoyGeom();");
  client.println("  let x = e.clientX - centerX;");
  client.println("  let y = e.clientY - centerY;");

  client.println("  const dist2 = x*x + y*y;");
  client.println("  if (dist2 > maxRadius * maxRadius) {");
  client.println("    const dist = Math.sqrt(dist2);");
  client.println("    x = x / dist * maxRadius;");
  client.println("    y = y / dist * maxRadius;");
  client.println("  }");

  client.println("  joy.style.left = (centerX + x) + 'px';");
  client.println("  joy.style.top  = (centerY + y) + 'px';");

  client.println("  const nx = (x / maxRadius).toFixed(2);");
  client.println("  const ny = (-y / maxRadius).toFixed(2);");

  client.println("  send(nx, ny);");
  client.println("}");

  client.println("box.addEventListener('mousedown', e => { active = true; handle(e); });");
  client.println("window.addEventListener('mousemove', handle);");

  client.println("box.addEventListener('touchstart', e => { active = true; handle(e.touches[0]); });");
  client.println("window.addEventListener('touchmove', e => handle(e.touches[0]));");

  client.println("function resetJoy() {");
  client.println("  active = false;");
  client.println("  joy.style.left = '50%';");
  client.println("  joy.style.top  = '50%';");
  client.println("  joy.style.transform = 'translate(-50%, -50%)';");
  client.println("  send(0, 0);");
  client.println("}");

  client.println("window.addEventListener('mouseup', resetJoy);");
  client.println("window.addEventListener('touchend', resetJoy);");

  client.println("</script>");
  client.println("</body></html>");
}
