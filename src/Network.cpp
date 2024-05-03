#include "Network.h"
#include <FirebaseClient.h>

void asyncCB(AsyncResult &aResult);
UserAuth user_auth(FIREBASE_API_KEY, USER_EMAIL, USER_PASSWORD, 3000 /* expire period in seconds (<= 3600) */);

FirebaseApp app;
Firestore::Documents Docs;

#include <WiFiClientSecure.h>
DefaultNetwork appNetwork(true); // initilize with boolean parameter to enable/disable network reconnection
WiFiClientSecure ssl_client;
using AsyncClient = AsyncClientClass;
AsyncClient aClient(ssl_client, getNetwork(appNetwork));

AsyncResult aResult_no_callback;

bool taskCompleted = false;

String getTimestampString(uint64_t sec, uint32_t nano);


static Network *instance = NULL;

Network::Network(){
  instance = this;
}

void Network::loop() {
  app.loop();
  Docs.loop();
}

void WiFiEventConnected(arduino_event_id_t event, arduino_event_info_t info){
  Serial.println("WIFI CONNECTED! BUT WAIT FOR THE LOCAL IP ADDR");
}

void WiFiEventGotIP(arduino_event_id_t event, arduino_event_info_t info){
  Serial.print("LOCAL IP ADDRESS: ");
  Serial.println(WiFi.localIP());
  instance->firebaseInit();
}

void WiFiEventDisconnected(arduino_event_id_t event, arduino_event_info_t info){
  Serial.println("WIFI DISCONNECTED!");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void Network::initWiFi(){
  WiFi.disconnect();
  WiFi.onEvent(WiFiEventConnected, ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(WiFiEventGotIP, ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(WiFiEventDisconnected, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void Network::firebaseInit() {
  Serial.println("Initializing app...");
  ssl_client.setInsecure();
  app.setCallback(asyncCB);
  Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);
}

void Network::firestoreDataUpdate(double temp, double humid) {
  Serial.println("\nsave measurement\n");
  if(WiFi.status() != WL_CONNECTED) return;
  Serial.println("check auth state");
  if (!app.isInitialized() || !app.ready()) {
    initializeApp(aClient, app, getAuth(user_auth));
    auto ms = millis();
    while (!app.ready() && millis() - ms < 120 * 1000) {
      delay(1000);
      Serial.println("Authenticating...");
    }
    Serial.println("Authentication Information");
    Firebase.printf("User UID: %s\n", app.getUid().c_str());
    Firebase.printf("Auth Token: %s\n", app.getToken().c_str());
    Firebase.printf("Refresh Token: %s\n", app.getRefreshToken().c_str());
  }

  app.getApp<Firestore::Documents>(Docs);
  
  Serial.println("create doc");
  // timestamp
  Values::TimestampValue tsV(getTimestampString(1712674441, 999999999));

  Document<Values::Value> doc("createdAt", tsV);
  doc.add("temp", Values::DoubleValue(temp));
  doc.add("humid", Values::DoubleValue(humid));

  Docs.createDocument(aClient, Firestore::Parent(FIREBASE_PROJECT_ID), "measurements", DocumentMask(), doc, asyncCB);
  Serial.println("doc created  T:" + String(temp) + " H:" + String(humid));
}

void asyncCB(AsyncResult &aResult)
{
    if (aResult.appEvent().code() > 0)
    {
        Firebase.printf("Event task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.appEvent().message().c_str(), aResult.appEvent().code());
    }

    if (aResult.isDebug())
    {
        Firebase.printf("Debug task: %s, msg: %s\n", aResult.uid().c_str(), aResult.debug().c_str());
    }

    if (aResult.isError())
    {
        Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());
    }
}

String getTimestampString(uint64_t sec, uint32_t nano)
{
    if (sec > 0x3afff4417f)
        sec = 0x3afff4417f;

    if (nano > 0x3b9ac9ff)
        nano = 0x3b9ac9ff;

    time_t now;
    struct tm ts;
    char buf[80];
    now = sec;
    ts = *localtime(&now);

    String format = "%Y-%m-%dT%H:%M:%S";

    if (nano > 0)
    {
        String fraction = String(double(nano) / 1000000000.0f, 9);
        fraction.remove(0, 1);
        format += fraction;
    }
    format += "Z";

    strftime(buf, sizeof(buf), format.c_str(), &ts);
    return buf;
}













