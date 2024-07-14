//rtc
#include "RTClib.h"
RTC_DS3231 rtc;
char dataHari[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
String hari;
int tanggal, bulan, tahun, jam, menit, detik;

//servo
#include <Servo.h>
Servo servoku;

void setup() {
    Serial.begin(9600);

    servoku.attach(6);

    if (! rtc.begin()) {
        Serial.println("RTC Tidak Ditemukan");
        Serial.flush();
        abort();
    }

    //Atur Waktu
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    //  rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}

void loop() {
    DateTime now = rtc.now();
    hari    = dataHari[now.dayOfTheWeek()];
    tanggal = now.day(), DEC;
    bulan   = now.month(), DEC;
    tahun   = now.year(), DEC;
    jam     = now.hour(), DEC;
    menit   = now.minute(), DEC;
    detik   = now.second(), DEC;

//      Serial.println(String() + hari + ", " + tanggal + "-" + bulan + "-" + tahun);
      Serial.println(String() + jam + ":" + menit + ":" + detik);
      Serial.println();

//      modulPakan();
//      delay(5000);

    // kondisi coba2
    if(jam == 15 && menit == 10 && detik == 0) {
        modulPakan(); 
    }

    if(jam == 15 && menit == 10 && detik == 15) {
        modulPakan(); 
    }

    if(jam == 15 && menit == 10 && detik == 30) {
        modulPakan(); 
    }

    // // kondisi real
    //  if(jam == 7 && menit == 0 && detik == 0) {
    //    modulPakan(); 
    //  }
    //
    //  if(jam == 12 && menit == 0 && detik == 0) {
    //    modulPakan(); 
    //  }
    //
    //  if(jam == 17 && menit == 0 && detik == 0) {
    //    modulPakan(); 
    //  }
}

void modulPakan() {
    unsigned long previousMillis = millis();  // menyimpan waktu sebelumnya
    const unsigned long interval = 10;  // interval waktu dalam milidetik (0.01 detik)
    int posisi = 0;  // posisi awal servo

    while (posisi <= 180) {
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
