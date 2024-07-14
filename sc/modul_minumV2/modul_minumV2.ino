//sensor ketinggian air
//penampung nilai ketinggian air
int analogPin = A1;
int ketinggianAir;
int ketinggianAirMm;

//relay
const int kipas1 = 8; //relay1 suhu
const int kipas2 = 9; //relay2 kelembaban
const int pompa = 10; //relay3
const int lampu1 = 11; //relay4
const int lampu2 = 12; //relay5

//on off relay
int relayON = LOW; //relay nyala
int relayOFF = HIGH; //relay mati

void setup() {
    Serial.begin(9600);

    //  setup relay
    pinMode(kipas1, OUTPUT);
    pinMode(kipas2, OUTPUT);
    pinMode(pompa, OUTPUT);
    pinMode(lampu1, OUTPUT);
    pinMode(lampu2, OUTPUT);
    digitalWrite(kipas1, relayOFF);
    digitalWrite(kipas2, relayOFF);
    digitalWrite(pompa, relayOFF);
    digitalWrite(lampu1, relayOFF);
    digitalWrite(lampu2, relayOFF);
}

void loop() {
    ketinggianAir = analogRead(analogPin);
    ketinggianAirMm = map(ketinggianAir, 0, 1023, 0, 255);

    // kondisi coba2
    if (ketinggianAirMm < 39) {
        modulIsiBakMinum();

    } else if (ketinggianAirMm > 40) {
        matikanModulIsiBakMinum();
    }

    Serial.println(ketinggianAirMm);
}

void modulIsiBakMinum() {
    // mengisi bak minum dengan menyala pompa air ketika bak minum hampir kosong
    //relay3
    digitalWrite(pompa, relayON);
    }

void matikanModulIsiBakMinum() {
    // mematikan pompa air ketika bak minum sudah hampir penuh
    //relay3
    digitalWrite(pompa, relayOFF);
}