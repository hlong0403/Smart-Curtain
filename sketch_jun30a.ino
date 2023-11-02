#define BLYNK_TEMPLATE_ID "TMPL6KHzraR0g"   
#define BLYNK_TEMPLATE_NAME "remcua"
#define BLYNK_AUTH_TOKEN "ugDrskFNsAfKuX87XW597-baGIMTdGTD"  //mã bảo mật

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>  //kết nối wifi
#include <WiFiClient.h> //truyền dữ liệu
#include <BlynkSimpleEsp32.h>  // thư viện này phải cài bên ngoài

int tren = 34;
int duoi = 35;
int ct1 = 18;
int ct2 = 19;
int cbm = 21;
int len = 32;
int xuong = 33;
char ssid[] = "Son";        //tk wifi
char pass[] = "0123456789"; //mật khẩu wifi
int trangthaiduoi = 0;
int trangthaitren = 0;
int trangthaixuong = 0;
int trangthailen = 0;
int ttcbm = 1;    //trạng thái cảm biến mưa

void setup()
{

  Serial.begin(115200);  //tốc độ baud để fix lỗi

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);  //kết nối blynk
  pinMode(len, OUTPUT);
  pinMode(xuong, OUTPUT);
  pinMode(ct1, INPUT);
  pinMode(ct2, INPUT);
  pinMode(tren, INPUT_PULLUP);
  pinMode(duoi, INPUT_PULLUP);
  pinMode(cbm, INPUT);
}

void loop()
{
  Blynk.run();
  trangthaiduoi = digitalRead(duoi);
  trangthaitren = digitalRead(tren);
  ttcbm = digitalRead(cbm);
  //ko mưa
  if (ttcbm == 1)  //ko mưa
  {
    if (trangthaiduoi == 0 && digitalRead(ct1)==1)
    {
      trangthaixuong = 1;
      trangthailen = 0;
      digitalWrite(len, LOW);
      digitalWrite(xuong, HIGH);  // bắt đầu kéo rèm xuống
      Blynk.virtualWrite(V2, trangthailen);   //nút ảo trên điện thoại V2,V1
      Blynk.virtualWrite(V1,trangthaixuong);
    }
    else if (trangthaitren == 0 && digitalRead(ct2)== 1)
    {
      trangthailen = 1;
      trangthaixuong = 0;  //bắt đầu kéo rèm lên
      digitalWrite(len, HIGH);
      digitalWrite(xuong, LOW);
      Blynk.virtualWrite(V1, trangthaixuong);
      Blynk.virtualWrite(V2,trangthailen);
    }
    else if(trangthaiduoi == 1 || trangthaitren == 1 || (digitalRead(ct2)==0 && trangthailen == 1) || (digitalRead(ct1)==0 && trangthaixuong == 1))
    {
      trangthailen = 0;      //hàm để chạm đáy hoặc lên trên cùng thì nó sẽ dừng lại
      trangthaixuong = 0;   
      digitalWrite(len, LOW);
      digitalWrite(xuong, LOW);
      Blynk.virtualWrite(V1,0);
      Blynk.virtualWrite(V2,0);
    }
  }
  else // có mưa
  {
    if (trangthaiduoi == 0)
    {
      digitalWrite(len, 0);
      digitalWrite(xuong, 1);  //bắt đầu kéo rèm xuống
      Blynk.virtualWrite(V1,1);  //V1 là nút ảo kéo rèm xuống
      Blynk.virtualWrite(V2,0);
    }
    else
    {
      digitalWrite(len, 0);
      digitalWrite(xuong, 0);
      Blynk.virtualWrite(V1,0);
      Blynk.virtualWrite(V2,0);
    }
  }
}

BLYNK_WRITE(V1)  // hàm điều khiển trên điện thoại vào công tắc V1
{
  Serial.println("321");  //bỏ đi 
  int p = param.asInt();  
  if(digitalRead(ct1)==0 && digitalRead(ct2)==0) {
    digitalWrite(trangthaixuong, p);
    digitalWrite(xuong, p);
    digitalWrite(trangthailen, 0);
    digitalWrite(len, 0);
    Blynk.virtualWrite(V2, 0);
  }else{
    Blynk.virtualWrite(V1,0);
    Blynk.virtualWrite(V2,0);
  }
}
BLYNK_WRITE(V2)
{
  Serial.println("123");
  int p = param.asInt();
  if(digitalRead(ct1)==0 && digitalRead(ct2)==0) {
    digitalWrite(trangthailen, p);
    digitalWrite(len, p);
    digitalWrite(trangthaixuong, 0);
    digitalWrite(xuong, 0);
    Blynk.virtualWrite(V1, 0);
  }else {
    Blynk.virtualWrite(V1,0);
    Blynk.virtualWrite(V2,0);
  }
}


  
