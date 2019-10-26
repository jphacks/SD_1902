#include <ESP32Servo.h>
#include <WiFi.h>
#include <string> 



//ロボット用宣言
Servo servo1,servo2,servo3; // create four servo objects 

int servo3Pin = 25;
int servo2Pin = 2;
int servo1Pin = 15;

// Published values for SG90 servos; adjust if needed
int minUs = 500;
int maxUs = 2400;
int pos = 0;      // position in degrees


//server用宣言
WiFiClient client;
const char* ssid     = "@";
const char* password = "m5sprmu4yvvum";
const char* server     = "toi-jphacks.herokuapp.com";
//const char* server     = "172.20.10.3";
const int httpPort = 80;

int fsr_array[10];
int count = 0;
String motion_number;
char number[1000];
char buff[256];
unsigned char a='A';


void setup() {

  Serial.begin(9600);
  delay(5000);
  
  //サーボポート等初期化
  servo1.attach(servo1Pin, minUs, maxUs);
  servo2.attach(servo2Pin, minUs, maxUs);
  servo3.attach(servo3Pin, minUs, maxUs);
  //向き固定
  servo1.write(0);
  servo2.write(45);
  servo3.write(180);

  
  Serial.println("===ロボット設定完了===");

  //wifi接続
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  //サーバー接続
  if (client.connect(server,httpPort)) {
    Serial.println("Client Connected");
  } else {
    Serial.println("No Connection　desu");
  }

}


void ButtunPush(Servo servo2){
   for (pos = 45; pos >= 30; pos -= 1) { // sweep from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo2.write(pos);
    delay(50);             // waits 20ms for the servo to reach the position
  }

   for (pos = 30; pos >= 0; pos -= 1) { // sweep from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo2.write(pos);
    delay(15);             // waits 20ms for the servo to reach the position
  }

  for (pos = 0; pos <= 45; pos += 1) { // sweep from 180 degrees to 0 degrees
    servo2.write(pos);
    delay(50);
  }
}




void loop() {

  if (client.connect(server,httpPort)) {
    Serial.println("Client Connected on loop"+(String)count+"回目のループ");
  } else {
    Serial.println("No Connection on loop");
  }
  count++;

  // We now create a URI for the request
  String url = "/ToI";

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + server + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(30000); //1秒ごとにサーバーへ。

  // Read all the lines of the reply from server and print them to Serial
  Serial.println("Respond:");
  int i = 0;
  String line;
  while(client.available()){
    line = client.readStringUntil('\r');//シリアルバッファから
  }
  //Serial.print(line);

  
    line.toCharArray(number,line.length()+1);
    Serial.println("motion_numberは");
    Serial.println(number[4]);

    motion_number = number[4];
    

    

    
//    *number = line.c_str();
//    
//    for (int j=0;j<10 ;j++){
//      Serial.print(j);
//      Serial.println("番目の出力");
//       Serial.println(number[j]);
//    }
    


  //*number = line.c_str();
  // Serial.println("line.c_str()の配列サイズは");
  // Serial.println(sizeof number);
 //  Serial.println("8番目の出力");
//       Serial.println(number[7]);
//
//       Serial.println("9番目の出力");
//       Serial.println(number[8]);
    



 
//  
//  Serial.println("  s[0]の値は、  ");
//  Serial.println("========================== ");
//  //Serial.println(ss.str());//受け取ったデータを表示
//  Serial.println("************************ ");
// 

  Serial.println();
  Serial.println("closing connection");

  //ここで文解析、motion_numberにだいにゅう。

  //受け取った値次第で変更
  if  (motion_number.equals("0")) //除湿
  {
      for (pos = 5; pos >= 0; pos -= 1) { // sweep from 0 degrees to 180 degrees
        // in steps of 1 degree
        servo1.write(pos);
        delay(50);             // waits 20ms for the servo to reach the position
      }
      ButtunPush(servo2);
   }else if (motion_number.equals("1")) { //冷房
      for (pos = 0; pos <= 5; pos += 1) { // sweep from 180 degrees to 0 degrees
          servo1.write(pos);
          delay(50);
      }
      ButtunPush(servo2);    
   }else {
    for (pos = 0; pos <= 5; pos += 1) { // sweep from 180 degrees to 0 degrees
          servo1.write(pos);
          delay(50);
      }
    }

}
