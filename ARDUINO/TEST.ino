#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// Cập nhật thông tin
// Thông tin về wifi
#define ssid "OKOKOK"
#define password "qwerty555"
// Thông tin về MQTT Broker
#define mqtt_server "m16.cloudmqtt.com" // Thay bằng thông tin của bạn
#define mqtt_topic_pub "topic1"   //Giữ nguyên nếu bạn tạo topic tên là demo
#define mqtt_topic_sub "topic2"
#define mqtt_user "dsbwiuis"    //Giữ nguyên nếu bạn tạo user là esp8266 và pass là 123456
#define mqtt_pwd "glc9YQlmbcJS"

const uint16_t mqtt_port = 12064; //Port của CloudMQTT

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port); 
  client.setCallback(callback);
  
  pinMode(D5, OUTPUT);
  digitalWrite(D5, LOW);
}
// Hàm kết nối wifi
void setup_wifi() {
  delay(10);
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
}
// Hàm call back để nhận dữ liệu
void callback(char* topic, byte* payload, unsigned int length) {
 
  if (!strncmp((char *)payload, "ON", length)) 
  {
    digitalWrite(D5, HIGH);
    Serial.println("ON");
  }
  else if (!strncmp((char *)payload, "OFF", length)) 
  {
    digitalWrite(D5, LOW);
    Serial.println("OFF");
  }
  
}
// Hàm reconnect thực hiện kết nối lại khi mất kết nối với MQTT Broker
void reconnect() {
  // Chờ tới khi kết nối
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Thực hiện kết nối với mqtt user và pass
    if (client.connect("ESP8266Client",mqtt_user, mqtt_pwd)) {
      Serial.println("connected");
      // Khi kết nối sẽ publish thông báo
      client.publish(mqtt_topic_pub, "ESP_reconnected");
      // ... và nhận lại thông tin này
      client.subscribe(mqtt_topic_sub);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Đợi 5s
      delay(5000);
    }
  }
}
void loop() {
  // Kiểm tra kết nối
 
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  // Sau mỗi 2s sẽ thực hiện publish dòng hello world lên MQTT broker
  long now = millis();
  if (now - lastMsg > 10000) {
    lastMsg = now; 
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(mqtt_topic_pub, msg);
  }

}
