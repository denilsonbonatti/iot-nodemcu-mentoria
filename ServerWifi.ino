
#include <ESP8266WiFi.h>

const char* ssid = "Casa";
const char* senha = "dLb1979**";

int ledPin = 13; //corresponde ao pino D7 do nodemcu

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.print("Conectando a rede:");
  Serial.println(ssid);

  WiFi.begin(ssid, senha);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi conectado");

  server.begin();
  Serial.println("Servidor iniciado");

  Serial.print("Use esta URL:");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
  WiFiClient cliente = server.available();
  if (!cliente) {
    return;
  }
  Serial.println("Novo cliente");
  while (!cliente.available()) {
    delay(1);
  }
  String requisicao = cliente.readStringUntil('\r');
  Serial.println(requisicao);
  cliente.flush();

  int status = LOW;
  if (requisicao.indexOf("/LED=ON") != -1) {
    status = HIGH;
    digitalWrite(ledPin, HIGH);
  }
  if (requisicao.indexOf("/LED=OFF") != -1) {
    status = LOW;
    digitalWrite(ledPin, LOW);
  }
  Serial.print("status:");
  Serial.println(status);
  //digitalWrite(ledPin, status);

  cliente.println("HTTP/1.1 200 OK");
  cliente.println("Content-Type:text/html");
  cliente.println("");
  cliente.println("<!DOCTYPE HTML>");

  cliente.println("<html>");
  cliente.println("<br><br>");
  cliente.println("<a href=\"/LED=ON\"\"> <button> Ligar </button></a>");
  cliente.println("<a href=\"/LED=OFF\"\"> <button> Desligar </button></a><br/>");
  cliente.println("</html>");
  delay(1);
}
