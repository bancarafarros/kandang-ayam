//dht11
#include <DHT.h>
DHT dht(7, DHT11);

//rtc
#include "RTClib.h"
RTC_DS3231 rtc;
char dataHari[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
String hari;
int tanggal, bulan, tahun, jam, menit, detik;
//rtc

//servo
#include <Servo.h>
Servo servoku;
//servo

//sensor ketinggian air
int analogPin = A1;
int ketinggianAir;
int ketinggianAirMm;
//sensor ketinggian air

// relay
const int kipas1 = 8; //relay1 suhu
const int kipas2 = 9; //relay2 kelembaban
const int pompa = 10; //relay3
const int lampu1 = 11; //relay4
const int lampu2 = 12; //relay5

//on off relay
int relayON = LOW; //relay nyala
int relayOFF = HIGH; //relay mati

// status relay
bool statusKipasSuhu;
bool statusKipasKelembaban;
bool statusPompa;
bool statusLampuSuhu;
bool statusLampuMalam;
// relay

void setup() {
    Serial.begin(9600);

    //  dht11
    dht.begin();

    // servo
    servoku.attach(6);
    servoku.write(0);

    // rtc ds3231
    if (! rtc.begin()) {
        Serial.println("RTC Tidak Ditemukan");
        Serial.flush();
        abort();
    }

    //Atur Waktu
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // rtc ds3231

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
    // BACA SUHU DAN KELEMBABAN
    float kelembaban = dht.readHumidity();
    float suhu = dht.readTemperature();
    // BACA SUHU DAN KELEMBABAN

    // BACA KETINGGIAN AIR
    ketinggianAir = analogRead(analogPin);
    ketinggianAirMm = map(ketinggianAir, 0, 1023, 0, 255);
    // BACA KETINGGIAN AIR

    // BACA WAKTU
    DateTime now = rtc.now();
    hari    = dataHari[now.dayOfTheWeek()];
    tanggal = now.day(), DEC;
    bulan   = now.month(), DEC;
    tahun   = now.year(), DEC;
    jam     = now.hour(), DEC;
    menit   = now.minute(), DEC;
    detik   = now.second(), DEC;
    // BACA WAKTU

    // KONDISI MODUL MONITORING SUHU DAN KELEMBABAN
    //  suhu normal
    if (suhu >= 29 && suhu <= 31) {
        matikanModulPeningkatanSuhu();
        matikanModulPenurunanSuhu();

    //  suhu di bawah standar minimum
    } else if (suhu < 29) {
        modulPeningkatanSuhu();
    
    //  suhu di atas standar maksimal
    } else if (suhu > 31) {
        modulPenurunanSuhu();
    }
    
    //  kelembaban normal
    if (kelembaban >= 50 && kelembaban <= 70) {
        matikanModulPeningkatanKelembaban();
        matikanModulPenurunanKelembaban();

    //  kelembaban di bawah standar minimum
    } else if (kelembaban < 50) {
        modulPeningkatanKelembaban();
        
    //  kelembaban di atas standar maksimal
    } else if (kelembaban > 70) {
        modulPenurunanKelembaban();
    }
    // KONDISI MODUL MONITORING SUHU DAN KELEMBABAN
    
    // KONDISI MODUL MINUM
    if (ketinggianAirMm < 5) {
        modulIsiBakMinum();

    } else if (ketinggianAirMm > 40) {
        matikanModulIsiBakMinum();
    }
    // KONDISI MODUL MINUM

    // KONDISI MODUL PAKAN
    if(jam == 7 && menit == 0 && detik == 0) {
        modulPakan(); 
    }

    if(jam == 12 && menit == 0 && detik == 0) {
        modulPakan(); 
    }

    if(jam == 17 && menit == 0 && detik == 0) {
        modulPakan(); 
    }
    // KONDISI MODUL PAKAN

   // KONDISI MODUL MODE MALAM
    if (jam >= 7 && jam <= 17) {
        matikanModulModeMalam();

    } else if (jam >= 17 || jam <= 7) {
        modulModeMalam();
    }
   // KONDISI MODUL MODE MALAM

    // KIRIM DATA KE NODEMCU
    modulKirimData();
    // KIRIM DATA KE NODEMCU
}

// MODUL PAKAN
void modulPakan() {
    unsigned long previousMillis = millis();  // menyimpan waktu sebelumnya
    const unsigned long interval = 10;  // interval waktu dalam milidetik (0.01 detik)
    int posisi = 0;  // posisi awal servo

    while (posisi <= 160) {
        unsigned long currentMillis = millis();  // mendapatkan waktu saat ini

        // Cek apakah sudah mencapai interval waktu tertentu
        if (currentMillis - previousMillis >= interval) {
            servoku.write(posisi);  // set posisi servo
            posisi++;  // naikkan posisi servo
            previousMillis = currentMillis;  // menyimpan waktu saat ini sebagai waktu sebelumnya
        }
    }

    // Reset waktu sebelumnya
    previousMillis = millis();

    while (posisi >= 0) {
        unsigned long currentMillis = millis();  // mendapatkan waktu saat ini

        // Cek apakah sudah mencapai interval waktu tertentu
        if (currentMillis - previousMillis >= interval) {
            servoku.write(posisi);  // set posisi servo
            posisi--;  // turunkan posisi servo
            previousMillis = currentMillis;  // menyimpan waktu saat ini sebagai waktu sebelumnya
        }
    }
}
// MODUL PAKAN

// MODUL MINUM
void modulIsiBakMinum() {
    // mengisi bak minum dengan menyalakan pompa air ketika bak minum hampir kosong
    //relay3
    digitalWrite(pompa, relayON);
    statusPompa = HIGH;
}

void matikanModulIsiBakMinum() {
    // mematikan pompa air ketika bak minum sudah hampir penuh
    //relay3
    digitalWrite(pompa, relayOFF);
    statusPompa = LOW;
}
// MODUL MINUM

// MODUL MONITORING SUHU DAN KELEMBABAN
void modulPeningkatanSuhu() {
    //meningkatkan suhu kandang ketika suhu di bawah normal dengan menyalakan lampu
    //relay4
    digitalWrite(lampu1, relayON);
    statusLampuSuhu = HIGH;
}

void matikanModulPeningkatanSuhu() {
    //mematikan modul peningkatan suhu ketika suhu normal tercapai dengan mematikan lampu
    //relay4
    digitalWrite(lampu1, relayOFF);
    statusLampuSuhu = LOW;
}

void modulPenurunanSuhu() {
    //menurunkan suhu kandang ketika suhu di atas normal dengan menyalakan kipas1
    //relay1
    digitalWrite(kipas1, relayON);
    statusKipasSuhu = HIGH;
}

void matikanModulPenurunanSuhu() {
    //mematikan modul penurunan suhu ketika suhu normal tercapai dengan mematikan kipas1
    //relay1
    digitalWrite(kipas1, relayOFF);
    statusKipasSuhu = LOW;
}

void modulPeningkatanKelembaban() {
    //meningkatkan kelembaban kandang ketika kelembaban di bawah normal dengan menyalakan kipas
    //relay2
    digitalWrite(kipas2, relayON);
    statusKipasKelembaban = HIGH;
}

void matikanModulPeningkatanKelembaban() {
    //mematikan modul peningkatan kelembaban ketika kelembaban normal tercapai dengan mematikan kipas
    //relay2
    digitalWrite(kipas2, relayOFF);
    statusKipasKelembaban = LOW;
}

void modulPenurunanKelembaban() {
    //menurunkan kelembaban kandang ketika kelembaban di atas normal dengan menyalakan lampu
    //relay4
    digitalWrite(lampu1, relayON);
    statusLampuSuhu = HIGH;
}

void matikanModulPenurunanKelembaban() {
    //mematikan modul penurunan kelembaban ketika kelembaban normal tercapai dengan mematikan lampu
    //relay4
    digitalWrite(lampu1, relayOFF);
    statusLampuSuhu = LOW;
}
// MODUL MONITORING SUHU DAN KELEMBABAN

// MODUL MODE MALAM
void modulModeMalam() {
    // menyalakan lampu kandang ketika waktu sudah memasuki malam hari
    //relay4
    digitalWrite(lampu2, relayON);
    statusLampuMalam = HIGH;
}

void matikanModulModeMalam() {
    // mematikan lampu kandang ketika waktu sudah memasuki pagi hari
    //relay4
    digitalWrite(lampu2, relayOFF);
    statusLampuMalam = LOW;
}
// MODUL MODE MALAM

// MODUL KIRIM DATA
void modulKirimData() {
    static unsigned long previousMillis = 0;  // menyimpan waktu sebelumnya
    const unsigned long interval = 1000;  // interval waktu dalam milidetik (1 detik)

    // BACA SUHU DAN KELEMBABAN
    float kelembaban = dht.readHumidity();
    float suhu = dht.readTemperature();
    // BACA SUHU DAN KELEMBABAN
    
    unsigned long currentMillis = millis();  // mendapatkan waktu saat ini

    // Cek apakah sudah mencapai interval waktu tertentu
    if (currentMillis - previousMillis >= interval) {
        String dataKirim = "#" + String(ketinggianAirMm) + "#" + String(suhu) + "#" + String(kelembaban) + "#" + String(String() + jam + ":" + menit + ":" + detik) + "#" + String(statusKipasSuhu) + "#" + String(statusKipasKelembaban) + "#" + String(statusPompa) + "#" + String(statusLampuSuhu) + "#" + String(statusLampuMalam);
        Serial.println(dataKirim);
        previousMillis = currentMillis;  // menyimpan waktu saat ini sebagai waktu sebelumnya
    }
}
// MODUL KIRIM DATA