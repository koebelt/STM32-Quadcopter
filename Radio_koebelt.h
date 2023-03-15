#ifndef RADIO_KOEBELT
#define RADIO_KOEBELT


#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

#define tunnel1 "PIPE2"
#define tunnel2 "PIPE1"
#define tunnel3 "PIPE3"
#define tunnel4 "PIPE4"
#define tunnel5 "PIPE5"
#define tunnel6 "PIPE6"

const byte adresses[][6] = {tunnel1, tunnel2, tunnel3, tunnel4, tunnel5, tunnel6};


class Radio {
  public:
    Radio() {};
    Radio(int pinCE, int pinCSN) {
        _radio = RF24(pinCE, pinCSN, 4000000);
        _remoteData = {{0, 0}, {0, 0}, 0, 0, 0, 0};
        if(!_radio.begin()) {
          Serial.println("Radio initialisation failed");
          while(1);
        }
        Serial.println("Radio initialisation success");
        // if (!_radio.begin())
        //     Serial.println("Radio initialisation failed");
        // else
        //     Serial.println("Radio initialisation success");
        _radio.setPALevel(RF24_PA_HIGH);
        _radio.setDataRate(RF24_250KBPS);
        _radio.openWritingPipe(adresses[0]);
        _radio.openReadingPipe(1, adresses[1]);
        _radio.openReadingPipe(2, adresses[2]);
        _radio.openReadingPipe(3, adresses[3]);
        _radio.openReadingPipe(4, adresses[4]);
        _radio.openReadingPipe(5, adresses[5]);
        _radio.startListening();
    };
    ~Radio() = default;
  
    struct joystick {
        int16_t x;
        int16_t y;
    };
  
    struct remoteData {
        struct joystick ljoystick;
        struct joystick rjoystick;
        byte button1;
        byte button2;
        byte button3;
        byte button4;
    };
  
    void listener(void) {
        unsigned long time = millis();
        if (time - _stopMillis >= 1) {
            _radio.startListening();
            _startMillis = millis();
                if (_radio.available()) {
                    while (_radio.available()) {
                        _remoteDataOld = _remoteData;
                        _radio.read(&_remoteData, sizeof(struct remoteData));
                    }
                }
        }
    };
    bool send(char const *message) {
        unsigned long time = millis();
        if (time - _startMillis >= 1) {
            _radio.stopListening();
            _stopMillis = millis();
            return _radio.write(message, sizeof(char) * 32);
        }
        return false;
    };
  
    RF24 getLegacyRadio(void) { return _radio; };
  
    unsigned long _startMillis;
    unsigned long _stopMillis;
    struct remoteData _remoteData;
    struct remoteData _remoteDataOld;
    
    RF24 _radio;
};

#endif
