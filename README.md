# Project Esp32 Firestore Example

# Goal: connect to Firestore and create a Doc with the measurements (temp and humidity)

# Starting

* create a project in Firebase console
* create email authentication in the Firebase console
* create a User to connect in the Firebase console
* create Firestore database in the Firebase console

* update the build flags with you values for WiFi and Firestore/Firebase
    -DWIFI_SSID='"ssid"'
    -DWIFI_PASSWORD='"pass"'
    -DFIREBASE_AUTH_DOMAIN='"domain.firebaseapp.com"'
    -DFIREBASE_API_KEY='"key"'
    -DFIREBASE_PROJECT_ID='"project"'
    -DUSER_EMAIL='"user email"'
    -DUSER_PASSWORD='"user pass"'

# Troubleshooting

to convert a stack trace into callback symbols, use script from `tools` directory.

cmd example:
```bash
swift stackToLine.swift 0x400ee659:0x3ffb3ca0 0x400eef89:0x3ffb3db0 0x400ef45a:0x3ffb3e60 0x400ec919:0x3ffb3f70 0x400efdb1:0x3ffb3fa0 0x400efbfd:0x3ffb4270 0x400e913e:0x3ffb43c0 0x400e753c:0x3ffb4640 0x400e79b7:0x3ffb4670 0x400e3c16:0x3ffb46b0 0x400e3c96:0x3ffb46f0 0x400e4755:0x3ffb4730 0x400e488b:0x3ffb4790 0x400e48c9:0x3ffb47c0 0x400e1f9d:0x3ffb47f0 0x400e1fb3:0x3ffb4810 0x400d756c:0x3ffb4830 0x400d7661:0x3ffb4860 0x400d745a:0x3ffb4880 0x400d986c:0x3ffb48a0 0x400daf66:0x3ffb4930 0x400db8ae:0x3ffb4980 0x400d5f62:0x3ffb49a0 0x400d2c61:0x3ffb49f0 0x400d2c8d:0x3ffb4a10 0x400d2759:0x3ffb4a40 0x400d49a1:0x3ffb4af0 0x400d4a10:0x3ffb4c80
```
output example:
```bash
0x400e48c9: BSSL_SSL_Client::connect(char const*, unsigned short)
        /Users/kanstantsinbucha/I/coder/esp32/esp32-firestore-2/.pio/libdeps/esp32dev/Firebase/src/client/SSLClient/client/BSSL_SSL_Client.cpp:141
```