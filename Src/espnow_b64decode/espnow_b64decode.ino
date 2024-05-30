#include <esp_now.h>
#include <WiFi.h>
#include "arduino_base64.hpp"

char xor_key = 'S';                               //XOR Key. Şifreleyen Key ile Aynı Olmalıdır.
char r_encodedStr[255];                           //Alınan Data
char r_message[255];                              //Decode İşlemleri Sonucunda Elde Edilen Mesaj


void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&r_encodedStr, incomingData, len);
  Serial.print("Alinan Byte Miktari: ");
  Serial.println(len);
  Serial.print("Alinan Data: ");
  Serial.println(r_encodedStr);

  //Base64 Decode
  int decodedLen = base64::decodeLength(r_encodedStr);
  char decodedStr[decodedLen];
  base64::decode(r_encodedStr, (uint8_t *)decodedStr);
  
  //XOR Decrypte
  for(int i = 0; i < decodedLen; i++){
    r_message[i] = decodedStr[i] ^ xor_key;
  }

  //Decode Edilmiş Mesaj İçeriği
  Serial.print("Mesaj: ");
  Serial.println(r_message);
}


void setup() {

  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  //Serial.println(WiFi.macAddress());           Alıcı Cihazın MAC Adresini Öğrenmek İçin Kullanabilirsiniz.

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW baslatilamadi.");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
 
}