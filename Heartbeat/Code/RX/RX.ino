/*
 * https://maniacbug.github.io/RF24/classRF24.html
 * 
 * VCC - 3.3v
 * GND - GND
 * CSN - 5
 * CE - 27
 * SCK - 18 mặc định SPI
 * MOSI - 23 SPI
 * MISO - 19 SPI
*/

#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include "ThingSpeak.h"

char ssid[]="landeptrai";
char password[]="5555777777";
WiFiClient client;

unsigned long myChannelNumber = 2649492;
const char * myWriteAPIKey = "Y9VK186SHYWH210X";

RF24 radio(27, 5); // CE, CSN
const byte diachi[6] = "12345";
float trangthai = 0;
unsigned long lastTime = 0;
unsigned long interval = 15000; // 15 giây
unsigned long lastReceivedTime = 0; // Thời gian nhận dữ liệu lần cuối
unsigned long timeout = 60000; // 3 phút = 180000 ms
unsigned long lastWiFiCheck = 0; // Thời gian kiểm tra WiFi lần cuối
unsigned long wifiCheckInterval = 60000; // 1 phút = 60000 ms
int HBvar = 0;

void setup() 
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  ConnectWIFI();

  if (!radio.begin()) 
  {
    Serial.println("Module không khởi động được...!!");
    while (1) {}
  } 
   
  radio.openReadingPipe(1, diachi);
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(80);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();
  
  if (!radio.available())
  {
    Serial.println("Chưa kết nối được với TX...!!");
    Serial.println("CHỜ KẾT NỐI.......");
  } 
}

void ConnectWIFI(){
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    int i=0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      ++i;
      if (i==20){
        i=0;
        Serial.println("\nTrying to Connect.");
        break;
      }
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected to WiFi");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
    }
  }
}

void loop() 
{
  // Kiểm tra kết nối WiFi mỗi 1 phút
  if (millis() - lastWiFiCheck >= wifiCheckInterval) {
    lastWiFiCheck = millis();
    if (WiFi.status() != WL_CONNECTED) {
      ConnectWIFI();
    }
  }

  if (WiFi.status() != WL_CONNECTED) {
    return; // Trả về ngay nếu chưa có kết nối WiFi để tránh thực hiện các tác vụ khác
  }
  
  if (radio.available()) 
  {
    radio.read(&HBvar, sizeof(HBvar));
    Serial.println(HBvar);
    
    // Cập nhật thời gian nhận được dữ liệu lần cuối
    lastReceivedTime = millis();
  }

  // Kiểm tra nếu đã quá 3 phút mà không nhận được dữ liệu
  if (millis() - lastReceivedTime >= timeout) {
    Serial.println("Không nhận được dữ liệu từ thiết bị gửi trong 3 phút!");
    // Gửi giá trị trạng thái cảnh báo lên 
    ThingSpeak.setField(2, 1);  // Giá trị 1 là giả định cho cảnh báo
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (x == 200) {
      Serial.println("Cảnh báo đã gửi lên ThingSpeak thành công");
    } else {
      Serial.println("Lỗi khi gửi cảnh báo lên ThingSpeak. Mã lỗi: " + String(x));
    }

    // Cập nhật lại thời gian để không liên tục gửi cảnh báo
    lastReceivedTime = millis();
  }

  // Gửi dữ liệu lên ThingSpeak mỗi 15 giây
  if (millis() - lastTime >= interval) 
  {
    ThingSpeak.setField(1, HBvar);
    Serial.println(HBvar);
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (x == 200) {
      Serial.println("Dữ liệu đã gửi lên ThingSpeak thành công");
    } else {
      Serial.println("Lỗi khi gửi dữ liệu lên ThingSpeak. Mã lỗi: " + String(x));
    }
    // Cập nhật lại thời gian gửi
    lastTime = millis();
  }
  delay(90);
}
