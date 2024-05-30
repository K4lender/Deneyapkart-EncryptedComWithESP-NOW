#include <esp_now.h>
#include <WiFi.h>
#include "arduino_base64.hpp"

// Receiver Olarak Çalışacak Cihazın MAC Adresi. Alıcı Cihazın MAC Adresi ile Değiştirin.
uint8_t broadcastAddress[] = {0x44, 0x17, 0x93, 0x4B, 0xD6, 0x0C};

esp_now_peer_info_t peerInfo;

char message[] = "Bu baglanti sifrelenmistir!";
int inputLen = strlen(message);
char xor_key = 'S';                                           //XOR Key. İstenilen Değer Girilebilir.

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nSon Gonderilen Paketin Durumu:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Gonderi Basarili" : "Gonderi Basarisiz");
}
 
void setup() {
  
  Serial.begin(115200);
 
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW baslatilamadi.");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  
  //Register Ayarları
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Baglanti kurulamadi");
    return;
  }
}
 
void loop() { 
  //XOR Encrypte
  char xor_str[255];
   
  for(int i = 0; i < strlen(message); i++){
    xor_str[i] = message[i] ^ xor_key;
  }
  
  //Base64 Encode
  int encodedLen = base64::encodeLength(inputLen);
  char encodedStr[encodedLen + 1];
  base64::encode((const uint8_t*)xor_str, inputLen, encodedStr);

  // ESP-NOW ile Mesaj Gönderme
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &encodedStr, sizeof(encodedStr));
   
  if (result == ESP_OK) {
    Serial.println("Baglanti basarili.");
  }
  else {
    Serial.println("Gonderilirken hata olustu.");
  }

  Serial.print("Mesaj: ");
  Serial.println(message);
  Serial.print("Gonderilen Data: ");
  Serial.println(encodedStr);

  delay(2000);
}