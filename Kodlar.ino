#include "DHT.h"
#define wifi  "wifiismi" //Bağlanılacak wifi ismi
#define sifre "wifisifresi" //Wifi Şifresi
#define SERVER "api.pushetta.com" // Kullanılan Server'in Alan adı
String APP ("f34db0f20e52719cd511a1e0c795272276163d10"); // Server'in vermiş olduğu App kodu
String KANAL ("kanalismi");  // Server'de oluşturmuş olduğunuz kanal isminiz.

#define DHTPIN 2     // Sıcaklık modül kütüphanesi eklendi
#define DHTTYPE DHT22   // Sıcaklık sensörü için isim belirlendi
int trigPin = 10; // Sensorun trig pini Arduinonun 6 numaralı ayağına bağlandı 
int echoPin = 9; // Sensorun echo pini Arduinonun 7 numaralı ayağına bağlandı 

long sure;
long uzaklik;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    pinMode(trigPin, OUTPUT); /* trig pini çıkış olarak ayarlandı */
  pinMode(echoPin, INPUT);
  dht.begin();
  Serial.begin(115200);
  Serial.println("AT");
  delay(5000);

  if (Serial.find("OK")) {       //At Komutu Gönderdiğimiz Esp8266 bize geri Ok mesajı gönderiyor
    Serial.println("AT+CWMODE=1"); //Bu kodla esp modülümüzü STA yani diğer ağlara bağlanabilmesine izin veriyoruz.
    delay(2000);
    String EspBaglanti = String("AT+CWJAP=\"") + wifi + "\",\"" + sifre + "\"";
    Serial.println(EspBaglanti);

    delay(5000);
  }
}

void loop()
{

   

  if (tcrtdeger < 10) { // Kutuda Mektup algılandığında.
    veriuygulama (KANAL, "Postanız Var!!"); //Bu mesajı SERVER'daki kanala gönder.
  }

}

// Uygulamaya bildirim gönderme
void veriuygulama (String KANAL, String text) {

  Serial.println(String("AT+CIPSTART=\"TCP\",\"") + SERVER + "\",80"); //server'a bağlanma

  delay(3000);
  String   deneme = ("POST /api/pushes/");
  deneme += (KANAL);
  deneme += ("/ HTTP/1.1");
  deneme += ("\r\n");
  deneme += ("Host: ");
  deneme += (SERVER);
  deneme += ("\r\n");
  deneme += ("Authorization: Token ");
  deneme += (APP);
  deneme += ("\r\n");
  deneme += ("Content-Type: application/json");
  deneme += ("\r\n");
  deneme += ("Content-Length: ");
  deneme += (text.length() + 46);
  deneme += ("\r\n\r\n");
  deneme += ("{ \"body\" : \"");
  deneme += (text);
  deneme += ("\", \"message_type\" : \"text/plain\" }");
  deneme += ("\r\n\r\n");

  Serial.print("AT+CIPSEND=");
  delay(100);
  Serial.println(deneme.length());
  // Sorun oluşmadıysa veriyi gönder.
  if (Serial.find(">")) {
    Serial.println(deneme);
    Serial.println("AT+CIPCLOSE=0");
    delay(100);

  }
  else {
    Serial.println("AT+CIPCLOSE=0");
  }

}
