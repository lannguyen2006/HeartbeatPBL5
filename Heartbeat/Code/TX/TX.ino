
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
#define PulseSensor 32
#define HBPin 2

#include <SPI.h>
#include "printf.h"
#include "RF24.h"

int PulseSensorSignal;
int UpperThreshold = 1970;
int LowerThreshold = 1990;
int cntHB = 0;
boolean Threshold = true;
int BPMval = 0;
bool firsttime = true;
unsigned long lastMillisGetHB = 0;
unsigned long currentMillisGetHB = 0;

RF24 radio(27, 5); // CE, CSN
const byte diachi[6] = "12345"; //Mảng kí tự dạng chuỗi có 6 kí tự

float Heartrate = 60;
unsigned long lastTime = 0;
unsigned long interval = 15000; // 15 giây, để tránh gửi quá thường xuyên

void setup() 
{
  Serial.begin(115200); 
  pinMode(HBPin, OUTPUT); 
  
  if (!radio.begin()) 
  {
    Serial.println("Module không khởi động được...!!");
    while (1) {}
  } 
  
  radio.openWritingPipe(diachi);
  //Lệnh openWritingPipe mặc định là đường truyền 0
  //mở 1 kênh có địa chỉ 12345 trên đường truyền 0
  // kênh này ghi data lên địa chỉ 12345  
  radio.setPALevel(RF24_PA_MIN); 
  //Cài bộ khuyết địa công suất ở mức MIN, MAX, HIGH, LOW
  radio.setChannel(80); // 125 kênh từ 0-124; TX và RX phải cùng kênh
                        // 2.4GHz ~ 2400Mhz, bước kênh là 1MHz
                        // setchannel(1) => 2401Mhz
                        // Cao nhất là 2525MHz, Tức là 2.525GHz
                        
  radio.setDataRate(RF24_250KBPS); //Tốc độ truyền dữ liệu trong không khí
                                   //250kbps, 1Mbps hoặc 2Mbps
                                   //250 thấp nhất nhưng truyền xa, 1Mb và 2Mb mạnh nhưng truyền không xa                                   
  /*                                   
   * Tốc độ truyền dữ liệu không khí 2Mbps, băng thông 2MHz bị chiếm dụng nhiều tần số kênh 
   * rộng hơn độ phân giải của cài đặt tần số kênh RF
   * Vì vậy, để đảm bảo các kênh không chồng chéo và giảm kết nối chéo ở chế độ 2Mbps
   * bạn cần giữ khoảng cách 2MHz giữa hai kênh.                                    
   * 
   * A: Xe TX-RX kênh 80, tốc độ truyền là 2Mb 80, 81, 82
   * B: Máy bay TX-RX kênh 83, tốc độ truyền là 250Kb                                    
   */
  radio.stopListening(); //Cài đặt module là TX
  
  if (!radio.available())
  {
    Serial.println("Chưa kết nối được với RX...!!");
    Serial.println("CHỜ KẾT NỐI.......");
  }  
  else
  {
    Serial.println("ĐỢI ĐỂ GỬI TÍN HIỆU.........");
  }
}

void loop() 
{
  currentMillisGetHB = millis();
    if (firsttime) 
    {
      lastMillisGetHB = currentMillisGetHB;
      cntHB = 0;
      firsttime = false;
    }
      if (currentMillisGetHB - lastMillisGetHB >= 15000) {
        BPMval = cntHB * 4;
        lastMillisGetHB = currentMillisGetHB;
        cntHB = 0;
        firsttime = true;
      } 
    else {
      PulseSensorSignal = analogRead(PulseSensor);
      Serial.print("PulseSensorSignal: ");
      Serial.println(PulseSensorSignal);
        if (PulseSensorSignal > UpperThreshold && Threshold == true) {
          cntHB++;
          Threshold = false;
          digitalWrite(HBPin, HIGH);
          Serial.print("Count: ");
          Serial.println(cntHB);
        }
        if (PulseSensorSignal < LowerThreshold) {
          Threshold = true;
          digitalWrite(HBPin, LOW);
        }
          if (millis() - lastTime >= interval)
        {
          radio.write(&BPMval, sizeof(BPMval));
          // &: Trả lại địa chỉ của một biến.
          // sizeof: trả về số byte bộ nhớ của một biến 
          //hoặc là trả về tổng số byte bộ nhớ của một mảng
          Serial.print("Đã gửi:");
          Serial.println(BPMval);
          lastTime = millis();
        }
    }
  
  delay(90);
}
