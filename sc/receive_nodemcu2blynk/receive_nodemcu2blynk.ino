//#define BLYNK_TEMPLATE_ID "TMPL69iGXFtzB"
//#define BLYNK_TEMPLATE_NAME "kandang ayam"
//#define BLYNK_AUTH_TOKEN "7RyZrouJIw0UjHCwF0YN1iWY-qcmfkdn"

#define BLYNK_AUTH_TOKEN "7RyZrouJIw0UjHCwF0YN1iWY-qcmfkdn"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

//char ssid[] = "RINJANI01";
//char pass[] = "12345678";
//char ssid[] = "bringthaton";
//char pass[] = "12345666";
char ssid[] = "Hotspot Area";
char pass[] = "sekolahvokasimadiun";

BlynkTimer timer;

String dataIn;
String dt[9];
int i;
boolean parsing = false;

void setup(){
  Serial.begin(9600);
  
  dataIn = "";

  // koneksi ke wifi
  WiFi.begin(ssid, pass);
  
  // cek koneksi wifi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("Berhasil terhubung ke WiFi");
  // koneksi ke wifi

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  timer.setInterval(1000L, sendSensor);
}

void loop(){
  // cek apakah memungkinkan dilakukan komunikasi serial
  while(Serial.available() > 0) {
    // merubah tipe data yang diterima menjadi char
    char inChar = (char)Serial.read();
    dataIn += inChar;

    // parsing data yang sudah diterima
    if (inChar == '\n') {
      parsing = true;
    }
  }
  
  // parsing data selesai
  if(parsing) {
    parsingData();
    parsing = false;
    dataIn = "";
  }
  
  // jalankan library Blynk
  Blynk.run();
  timer.run();
}

// MODUL PARSING DATA
void parsingData() {
  int j = 0;

  Serial.print("data masuk: ");
  Serial.print(dataIn);

  // penampung data yang akan diparsing
  dt[j] = "";

  // perulangan for untuk parsing data dalam dataIn
  for(i=1; i<dataIn.length(); i++) {

    // # sebagai separator antar nilai sensor
    if((dataIn[i] == '#')) {
      // increment j untuk merubah index penampung
      j++;
      // dt[j] dikosongkan untuk menampung data selanjutnya
      dt[j] = "";
      
    } else {
      // hasil prasing data ditampung dalam dt[j]
      dt[j] = dt[j] + dataIn[i];
    }
  }
}
// MODUL PARSING DATA

// MODUL KIRIM DATA KE BLYNK
void sendSensor(){
  // function virtualWrite untuk mengirim data ke pin virtual di Blynk
  Blynk.virtualWrite(V0, dt[0].toInt()); // ketinggian air
  Blynk.virtualWrite(V1, dt[1].toFloat()); // suhu
  Blynk.virtualWrite(V2, dt[2].toFloat()); // kelembaban
  Blynk.virtualWrite(V4, dt[4].toInt()); // statusKipasSuhu
  Blynk.virtualWrite(V5, dt[5].toInt()); // statusKipasKelembaban
  Blynk.virtualWrite(V6, dt[6].toInt()); // statusPompa
  Blynk.virtualWrite(V7, dt[7].toInt()); // statusLampuSuhu
  Blynk.virtualWrite(V8, dt[8].toInt()); // statusLampuMalam
}
// MODUL KIRIM DATA KE BLYNK