  #include <MQTT.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <ESP8266WiFi.h>

 #include <Adafruit_Sensor.h>
//...................................................................................................................................................................................
#define Relay1  D3
DHT dht(D4, DHT11);
#define Relay2  D5
#define piezo   D6
#define pirPin D7   
#define Door_Sensor_Pin D8
#define PIN_PHOTO_SENSOR A0
//...................................................................................................................

const int admin = 28092003;
const int test_manya = 664080;

const int admin_e = 280920033;
const int test_manya_e = 6640800;

const char *ssid =  "abcdefgh";  // Имя вайфай точки доступа
const char *pass =  "12345678"; // Пароль от точки доступа

const char *mqtt_server = "m16.cloudmqtt.com"; // Имя сервера MQTT
const int mqtt_port = 11729; // Порт для подключения к серверу MQTT
const char *mqtt_user = "tizzoqtl"; // Логи от сервер
const char *mqtt_pass = "sqCYE8vpFV1P"; // Пароль от сервера

WiFiClient wclient;      
PubSubClient client(wclient, mqtt_server, mqtt_port);


#define BUFFER_SIZE 100

int control=0;
int login=0;
int ohr=0;
int magnit;
int number =0;
int motionDetected = 0;
int pirValue;    
int raw;
int sv=0;
int mute=0;
int r2;
int tm=4;
int timer=30;
float temp=0;

// Функция получения данных от сервера
//.....................................................................................................................................   M   Q    T    T    ....................................................................................................................
void callback(const MQTT::Publish& pub)
{
  Serial.print(pub.topic());   // выводим в сериал порт название топика
  Serial.print(" => ");
  Serial.print(pub.payload_string()); // выводим в сериал порт значение полученных данных
  
  String payload = pub.payload_string();
  
  if(String(pub.topic()) == "Relay/1"  && login==1) // проверяем из нужного ли нам топика пришли данные 
  {
  int stled = payload.toInt(); // преобразуем полученные данные в тип integer
  Serial.println("GET 1");  //  включаем или выключаем светодиод в зависимоти от полученных значений данных
  Serial.println(stled);
  if(stled==1 && sv==0){
  digitalWrite(Relay1, HIGH);
  if(mute==0) Piezo_play();
  }else{
    digitalWrite(Relay1, LOW);
    if(mute==0) Piezo_OK();
  }
}

if(String(pub.topic()) == "Relay/2" && login==1) // проверяем из нужного ли нам топика пришли данные 
  {
  int stled = payload.toInt(); // преобразуем полученные данные в тип integer
  Serial.println("GET 2");  //  включаем или выключаем светодиод в зависимоти от полученных значений данных
  Serial.println(stled);
  if(stled==1 && r2==0){
  digitalWrite(Relay2, HIGH);
  if(mute==0) Piezo_play();
  }else{
    digitalWrite(Relay2, LOW);
    if(mute==0) Piezo_OK();
  }
}
if(String(pub.topic()) == "piezo" && login==1) // проверяем из нужного ли нам топика пришли данные 
  {
int stled = payload.toInt();
if(stled==1) mute=1;
if(stled==0) mute=0;
}
if(String(pub.topic()) == "password") // проверяем из нужного ли нам топика пришли данные 
  {
int stled = payload.toInt();
if((stled==admin ||stled==test_manya) && login==0){ login=1; client.publish("Log in", "Hello!"); if(mute==0){Piezo_play();}  if(stled==test_manya){ client.publish("to_admin", "Пользователь: ок"); }}
if((stled==admin_e ||stled==test_manya_e) && login==1){ login=0; client.publish("Log in", "Goodbye"); if(mute==0)Piezo_OK();}
}
if(String(pub.topic()) == "svet"  && login==1) // проверяем из нужного ли нам топика пришли данные 
  {
int stled = payload.toInt();
if(stled==1){ sv=1; if(mute==0) Piezo_play();}
if(stled==0){ sv=0; if(mute==0) Piezo_OK();}
}
if(String(pub.topic()) == "Ohrana"  && login==1) // проверяем из нужного ли нам топика пришли данные 
  {
int stled = payload.toInt();
if(stled==1){ ohr=1; client.publish("ohrana","Охрана включена");if(mute==0) Piezo_play();}
if(stled==0){ ohr=0; client.publish("ohrana","Охрана выключена");if(mute==0)Piezo_OK();}
}
if(String(pub.topic()) == "Temp"  && login==1) // проверяем из нужного ли нам топика пришли данные 
  {
  int stled = payload.toInt();
  // преобразуем полученные данные в тип integer
  if(stled==1){ control=1; client.publish("control","Отопление включено");} else {control=0; client.publish("control", "Отопление выключено");
  }}




  
}


// ........................................................................................................... S  E T U P  ................................................................................................................................
void setup() {
  Serial.begin(115200);
   pinMode(pirPin, INPUT);
   Serial.println("PIR.Waiting PIR sensor");
   delay(60000);
   Serial.println("PIR.PIR is Ready");
  delay(10);
  Serial.println();
  Serial.println();
  pinMode(Door_Sensor_Pin, INPUT); // установить Door_Sensor_Pin как вход
 dht.begin();
 Serial.println("DHT is Ready");
 Serial.println("Photosensor is Ready");
 pinMode(Relay1, OUTPUT);
 pinMode(Relay2, OUTPUT);
 pinMode(piezo, OUTPUT);
 pinMode(PIN_PHOTO_SENSOR, INPUT);
 Piezo_ON();
}
// ................................................................................................................ L O O P ..................................................................................................................................
void loop()
{
  // подключаемся к wi-fi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
  }

  // подключаемся к MQTT серверу
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      Serial.println("Connecting to MQTT server");
      if (client.connect(MQTT::Connect("arduinoClient2")
                         .set_auth(mqtt_user, mqtt_pass))) {
        Serial.println("Connected to MQTT server");
        client.set_callback(callback);
        //  .....................................................................................................................П  О Д П И С К И .............................................................................
        client.subscribe("Temp");
        client.subscribe("Ohrana");
        client.subscribe("password");
        client.subscribe("svet");
        client.subscribe("piezo");
        client.subscribe("Relay/1");
        client.subscribe("Relay/2");// подписывааемся на топик с данными для реле
      } else {
        Serial.println("Could not connect to MQTT server");   
      }
    }
//............................................................................................................................... E   N   D   ............................................................................................
if (client.connected()){
client.loop();

}
TempSend();
Svet();
gercon();
climat_control();
}

   
} // конец основного цикла

//................................................................................................................................... Ф  У Н К Ц И И ..................................................................................
void TempSend(){
  Serial.println("Tm:");
  Serial.print(tm);
    
  if (tm==0)
  {
    if(login==0) client.publish("Inform5","Введите пароль");
    if(login==1){
    if(sv==1) client.publish("Inform1", "Включена автоматическая система освещения"); else client.publish("Inform1", "Система освещения выключена");
  if(control==1) client.publish("Inform2", "Включено отопление"); else client.publish("Inform2", "Отопление выключено");
  if(mute==1) client.publish("Inform3", "Включен бесшумный режим"); else client.publish("Inform3", "Звук включен");
  if(ohr==1) client.publish("Inform4", "Охрана включена"); else client.publish("Inform4", "Охрана отключена");
  temp = dht.readTemperature();  
  client.publish("Inform","Temperature is:" + String(temp)); // отправляем в топик для термодатчика значение температуры
  Serial.println(temp);
    // пауза меду отправками значений температуры  коло 15 секунд
  }tm = 15;
  }
  tm--; 
  delay(1000);  
}

void Piezo_ON(){
  tone(piezo, 392);
  delay(1000);
  noTone(piezo);
  tone(piezo,494);
  delay(1000);
  noTone(piezo);
  tone(piezo, 587);
  delay(1000);
  noTone(piezo);
  tone(piezo, 698);
  delay(4000);
  noTone(piezo);
}

void Piezo_play(){
  tone(piezo,261);
  delay(500);
  noTone(piezo);
  tone(piezo,293);
  delay(1000);
  noTone(piezo);
}

void Piezo_OK(){
  tone(piezo,293);
  delay(1000);
  noTone(piezo);
  tone(piezo,261);
  delay(500);
  noTone(piezo);
}

void Svet(){
  if(sv==1&&login==1){
  raw = analogRead(PIN_PHOTO_SENSOR);
  timer = 30;
  Serial.println("Svet:");
  Serial.print(sv);
  Serial.println("Pokazatel:");
  Serial.print(raw);
  if(raw < 400){
    digitalWrite(Relay1, HIGH);
  }else{
    digitalWrite(Relay1, LOW);    
  }
  }
 
delay(100);
}


void gercon(){
  if(ohr==1&&login==1){
    magnit = digitalRead(Door_Sensor_Pin); // читать Door_Sensor_Pin
if (magnit == HIGH) { // Если Door_Sensor N.C. (без магнита) -> HIGH : Дверь открыта / LOW : Дверь закрыта
// Если Door_Sensor N.0. (nc с магнитом) -> HIGH : Дверь открыта / LOW : Дверь закрыта
client.publish("Door","Door is open");
} else {
client.publish("Door", "Door is close"); //включить светодиод Door_Led
}

delay(100);
 pirValue = digitalRead(pirPin);    // Считываем значение от датчика движения
  if (pirValue == 1&&login==1)                 // Если движение есть, делаем задержку в 3 с.
   {               
    Serial.println("Detected");
    client.publish("PIR", "Detected");
    delay(1500);
    } 
  }
delay(500);

}



void climat_control(){
  if(control==1){
      r2=1;
  temp = dht.readTemperature();
  if(temp<20){
  digitalWrite(Relay2, HIGH);
  delay(200);
  Serial.println("Temperature is");
  Serial.print(temp);
  }
  else{
    digitalWrite(Relay2, LOW);
  r2=0;
  }
  }
  delay(100);
}
