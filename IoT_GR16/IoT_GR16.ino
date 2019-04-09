
// Update these with values suitable for your network.
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "pitches.h"
int bpm = 100;
int bps = bpm / 60;
int fullNoteMs = 1300 / bps;
int halfNoteMs = fullNoteMs / 2;
int quarterNoteMs = halfNoteMs / 2;
int eighthNoteMs = quarterNoteMs / 2;
int note = 0;
int duration = 0;
int simpsons[] = {HALF, NOTE_C4, QUARTER, NOTE_E4, NOTE_FS4, EIGHTH, NOTE_A4, HALF, NOTE_G4, QUARTER, NOTE_E4, NOTE_C4, NOTE_G3, EIGHTH, NOTE_FS3, NOTE_FS3, NOTE_FS3, QUARTER, NOTE_G3};
int numNotes = sizeof(simpsons) / sizeof(int);
const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;

const char* password = "12121213";
const char* ssid = "darabo";
const char* mqtt_server = "m14.cloudmqtt.com";
const int mqtt_port = 10099;
const char* mqtt_user = "grupp16";
const char* mqtt_password = "grupp16";
const String keyGesture = "UP";
const int TRIG_PIN = D0;
const int ECHO_PIN = D1;
const int LEFT_LED_PIN = D2;
const int RIGHT_LED_PIN = D3;
const int RED_LED_PIN = D6;
const int WIFI_LED_PIN = D5;
const int UP_LED_PIN = D7;
const int DOWN_LED_PIN = D8;
String gesture = "";
boolean firstWarm = false;
boolean warm = false;
boolean login = false;
char* inTopic = "IoT/#";
boolean welcome = false;
float cm;
float inches;
int counter = 0;
String newTopic;
int keyCounter = 0;


// Anything over 400 cm (23200 us pulse) is "out of range"const unsigned int MAX_DIST = 23200;
WiFiClient espClient;
PubSubClient client(espClient);




void setup() {
  // The Trigger pin will tell the sensor to range find, since the default mode is input we don’t need to define Echo pin as   input
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(UP_LED_PIN, OUTPUT);
  pinMode(DOWN_LED_PIN, OUTPUT);
  pinMode(LEFT_LED_PIN, OUTPUT);
  pinMode(RIGHT_LED_PIN, OUTPUT);
  pinMode(LEFT_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(WIFI_LED_PIN, OUTPUT);
  digitalWrite(DOWN_LED_PIN, LOW);
  digitalWrite(UP_LED_PIN, LOW);
  digitalWrite(LEFT_LED_PIN, LOW);
  digitalWrite(RIGHT_LED_PIN, LOW);
  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(WIFI_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  // We'll use the serial monitor to view the sensor output

  /*
    Simsonss();
    delay(2000);
    StarWars();
  */
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.connect("ESP8266Client2", mqtt_user, mqtt_password);
  client.setCallback(callback);
  client.subscribe(inTopic);
}



void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(WIFI_LED_PIN, HIGH);
}

void callback(char* topic, byte* payload, unsigned int length) {
  String str_message = String((char*)payload).substring(0, length);
  str_message.toUpperCase();
  int delimiter = str_message.indexOf('#');
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println();
  Serial.println(str_message);

  if (!login && warm && str_message.equals(keyGesture)) {

    //login = true;
    keyCounter = keyCounter + 1;
  }
  else {
    keyCounter = 0;
  }
  if (keyCounter == 2) {
    client.publish(topic, "");
    newTopic = String(topic);
    client.unsubscribe(inTopic);
    client.subscribe(newTopic.c_str());
    Serial.println("Correct password");
    login = true;
    keyCounter = 0;
  }



  if (login && !welcome && (str_message == keyGesture)) {
    Serial.println("start if condition... if login");
    if (newTopic == "IoT/G15")
      Simsonss();
    if (newTopic == "IoT/G16")
      StarWars();
    welcome = true;
  }
  else if (login && welcome && (newTopic == String(topic))) {
    gesture = str_message;
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    digitalWrite(WIFI_LED_PIN, LOW);

    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //  client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  digitalWrite(WIFI_LED_PIN, HIGH);

}
void loop() {
  if (login && welcome) {
    if (gesture.equals("UP")) {
      digitalWrite(UP_LED_PIN, HIGH);
      digitalWrite(DOWN_LED_PIN, LOW);
      digitalWrite(RIGHT_LED_PIN, LOW);
      digitalWrite(LEFT_LED_PIN, LOW);
    }
    else if (gesture.equals("DOWN")) {
      digitalWrite(DOWN_LED_PIN, HIGH);
      digitalWrite(UP_LED_PIN, LOW);
      digitalWrite(RIGHT_LED_PIN, LOW);
      digitalWrite(LEFT_LED_PIN, LOW);
    }
    else if (gesture.equals("RIGHT")) {
      digitalWrite(RIGHT_LED_PIN, HIGH);
      digitalWrite(UP_LED_PIN, LOW);
      digitalWrite(DOWN_LED_PIN, LOW);
      digitalWrite(LEFT_LED_PIN, LOW);
    }
    else if (gesture.equals("LEFT")) {
      digitalWrite(LEFT_LED_PIN, HIGH);
      digitalWrite(UP_LED_PIN, LOW);
      digitalWrite(DOWN_LED_PIN, LOW);
      digitalWrite(RIGHT_LED_PIN, LOW);
    }
    else if (gesture.equals("ROTATE_LEFT")) {
      digitalWrite(LEFT_LED_PIN, LOW);
      digitalWrite(UP_LED_PIN, LOW);
      digitalWrite(DOWN_LED_PIN, LOW);
      digitalWrite(RIGHT_LED_PIN, LOW);
    }
    else if (gesture.equals("ROTATE_RIGHT")) {
      digitalWrite(LEFT_LED_PIN, HIGH);
      digitalWrite(UP_LED_PIN, HIGH);
      digitalWrite(DOWN_LED_PIN, HIGH);
      digitalWrite(RIGHT_LED_PIN, HIGH);
    }
  }

  if (counter >= 4) {
    unsigned long t1;
    unsigned long t2;
    unsigned long pulse_width;

    // Hold the trigger pin high for at least 10 us
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    // Wait for pulse on echo pin
    while ( digitalRead(ECHO_PIN) == 0 );
    // Measure how long the echo pin was held high (pulse width)
    // Note: the micros() counter will overflow after ~70 min
    t1 = micros();
    while ( digitalRead(ECHO_PIN) == 1);
    t2 = micros();
    pulse_width = t2 - t1;
    // Calculate distance in centimeters and inches. The constants
    // are found in the datasheet, and calculated from the assumed speed
    //of sound in air at sea level (~340 m/s).
    cm = pulse_width / 58.0;
    inches = pulse_width / 148.0;

    if ( cm < 30 ) {
      if (!firstWarm)
        firstWarm = true;
      else {
        digitalWrite(RED_LED_PIN, HIGH);
        warm = true;
      }
    }
    else {
      digitalWrite(RED_LED_PIN, LOW);
      digitalWrite(UP_LED_PIN, LOW);
      digitalWrite(DOWN_LED_PIN, LOW);
      digitalWrite(RIGHT_LED_PIN, LOW);
      digitalWrite(LEFT_LED_PIN, LOW);
      if (warm && firstWarm) {
        firstWarm = false;
      }

      else if (warm && ! firstWarm)
      {
        warm = false;
        login = false;
        welcome = false;
        client.unsubscribe(newTopic.c_str());
        newTopic = "";
        client.subscribe(inTopic);
        gesture = "";
        keyCounter = 0;

      }

    }
    // Print out results
    Serial.print(cm);
    Serial.print(" cm \t");
    Serial.print(inches);
    Serial.println(" in");
    counter = 0;

  }
  else {
    counter++;
  }
  delay(50);
  if (!client.connected()) {
    reconnect();
    Serial.println("Reconnecting cloud");
  }
  client.loop();


}

void Simsonss() {
  while (note < numNotes) {
    Serial.println("simsons");
    if (note < numNotes) {
      Serial.println("simsons");
      if (simpsons[note] == HALF) {

        duration = halfNoteMs;
        digitalWrite(RIGHT_LED_PIN, LOW);
        note++;
        // break;
      }
      if (simpsons[note] == QUARTER) {
        duration = quarterNoteMs;
        note++;
        //     break;
      }
      if (simpsons[note] == EIGHTH) {
        duration = eighthNoteMs;
        note++;
        //   break;
      }

      if (simpsons[note] == SILENCE) {
        digitalWrite(RIGHT_LED_PIN, HIGH);
        noTone(LEFT_LED_PIN);
        delay(duration);
        digitalWrite(RIGHT_LED_PIN, LOW);
      } else {
        digitalWrite(RIGHT_LED_PIN, HIGH);
        tone(LEFT_LED_PIN, simpsons[note]);
        delay(duration);
        noTone(LEFT_LED_PIN);
        digitalWrite(RIGHT_LED_PIN, LOW);
      }

      note++;
    }
  }
  note = 0;
}
void StarWars() {
  beep(a, 500);
  beep(a, 500);
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);

  delay(500);

  beep(eH, 500);
  beep(eH, 500);
  beep(eH, 500);
  beep(fH, 350);
  beep(cH, 150);
  beep(gS, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
  digitalWrite(LEFT_LED_PIN, LOW);
  digitalWrite(RIGHT_LED_PIN, LOW);

}

void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(LEFT_LED_PIN, note, duration);

  //Play different LED depending on value of 'counter'
  if (counter % 2 == 0)
  {
    digitalWrite(RIGHT_LED_PIN, HIGH);
    delay(duration);
    digitalWrite(RIGHT_LED_PIN, LOW);
  } else
  {
    digitalWrite(LEFT_LED_PIN, HIGH);
    delay(duration);
    digitalWrite(LEFT_LED_PIN, LOW);
  }

  //Stop tone on buzzerPin
  noTone(LEFT_LED_PIN);

  delay(50);

  //Increment counter
  counter++;
}
