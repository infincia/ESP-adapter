#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
 
ESP8266WebServer server(80);
String jwt = "Insert JWT string here";

void setup() {
 
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  Serial.begin(115200);
  WiFi.begin("Network name", "Password");  //Connect to the WiFi network
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for connection
    delay(500);
    Serial.println("Waiting to connect…");
  }
 
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //Print the local IP
 
 //Define the handling function for the config response
  server.on("/thing", []() {   
    String configThing = "{\
   \"properties\": {\
     \"Clock\": {\
       \"type\":  \"number\",\
       \"unit\":  \"Ticks\",\
       \"description\":  \"The millisec clock count\",\
       \"value\":  \"0\"\
    },\
     \"led\": {\
       \"type\":  \"boolean\",\
       \"description\":  \"The onboard LED\",\
       \"value\":  \"false\"\
    }\
  }\
}";

    server.send(200, "text/plain", configThing); 
  });
 
  // respond to setting LED on/off
  // attempts to set clock simply return current clock value
  server.on("/thing/set", []() {   //Define the handling function for the config response
    char respondThing[512];
    sprintf(respondThing, "{}");
    if(server.arg("led")!= "") {
      if(server.arg("led")=="true" ) {
          digitalWrite(LED_BUILTIN, HIGH);
      } else {
          digitalWrite(LED_BUILTIN, LOW);
      }
      unsigned long currentMillis = millis();
      sprintf(respondThing, "{\"Clock\": %d}", currentMillis);
    }

    if(server.arg("Clock")!= "") {
      unsigned long currentMillis = millis();
      sprintf(respondThing, "{\"Clock\": %d}", currentMillis);
    }
    
    server.send(200, "text/plain", respondThing); 
  });
 
  server.begin();                                       //Start the server
  Serial.println("Server listening");
 
}
 
void loop() {
 
  server.handleClient();         //Handling of incoming requests
 
}
 