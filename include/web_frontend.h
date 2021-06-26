#include <ESP8266WiFi.h>

void handle_wifi(WiFiClient client);

/* HTML Content */
const char MAIN_page_top[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
)=====";

const char MAIN_page_bot[] PROGMEM = R"=====(
<h2>Circuits4you<h2>
<h3> HTML Form ESP8266</h3>

<form action="/action_page">
  First name:<br>
  <input type="text" name="firstname" value="Mickey">
  <br>
  Last name:<br>
  <input type="text" name="lastname" value="Mouse">
  <br><br>
  <input type="submit" value="Submit">
</form> 

</body>
</html>
)=====";