#include <ESP8266WiFi.h>

String heading(const char *text, int level);

/* HTML Content */
const char HEAD[] = R"=====(
<!DOCTYPE html>
<title>Hushbox &Uuml;berwachung</title>
<html>
<body>
)=====";

const char FORM[] = R"=====(
<br>
<form action="/change_values">
  <table>
    <tr><td>New Overtemperature Threshold 0</td><td><input type="text" name="ot0" placeholder="new value ..."></td></tr>
    <tr><td>New Overtemperature Threshold 1</td><td><input type="text" name="ot1" placeholder="new value ..."></td></tr>
    <tr><td>New Lightsensor Threshold 0</td><td><input type="text" name="light_th0" placeholder="new value ..."></td></tr>
    <tr><td>New Lightsensor Threshold 1</td><td><input type="text" name="light_th1" placeholder="new value ..."></td></tr>
    <tr><td>New Vfan</td><td><input type="text" name="vfan" placeholder="new value ..."></td></tr>
  </table>
  <br>
  <input type="submit" value="Submit">
</form> 
)=====";

const char FOOT[] = R"=====(

</body>
</html>
)=====";