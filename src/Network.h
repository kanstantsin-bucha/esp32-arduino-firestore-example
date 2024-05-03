#ifndef Network_H_
#define Network_H_

#include <WiFi.h>

class Network{
private:
  void firebaseInit();
  friend void WiFiEventConnected(WiFiEvent_t event, WiFiEventInfo_t info);
  friend void WiFiEventGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
  friend void WiFiEventDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);

public:
  Network();
  void initWiFi();
  void loop();
  void firestoreDataUpdate(double temp, double humi);
};


#endif