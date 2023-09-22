#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebServer.h>

#define LED    11
#define BUZZER 13
#define SERVO  3
#define LDR    A4

WebServer server(80);
Servo servo;

// CRIANDO E INICIANDO O ROTEADOR LOCAL
void startServer(char* nome, char* senha){
    IPAddress staticIP(192, 168, 4, 1);   // IP ESTÁTICO
    IPAddress gateway(192, 168, 4, 10);   // GATEWAY ESTÁTICO IP
    IPAddress subnet(255, 255, 255, 0);   // OCULTAR SUB REDE
  
    // MODO DE TRABALHO WIFI VIA ACESS POINT
    WiFi.mode(WIFI_AP);                 
    WiFi.softAP(nome, senha);
    WiFi.config(staticIP, gateway, subnet);
  
    // INICIALIZANDO O SERVIDOR E IMPRIMINDO INFORMAÇÕES
    server.begin(); 
    Serial.println("SERVER STARTED!"); 
    Serial.println(WiFi.softAPIP());
}

// CONECTANDO À REDE LOCAL PELO NOME E SENHA
void conectarRede(char* nomeRede, char* senhaRede){
    WiFi.mode(WIFI_STA);
    WiFi.begin(nomeRede, senhaRede);

    // (OPCIONAL) CONFIGURAÇÕES SECUNDÁRIAS DO SERVIDOR LOCAL
    IPAddress staticIP(192, 168, 4, 3);     // IP ESTÁTICO (REQUISIÇÕES)
    IPAddress gateway(192, 168, 4, 11);     // GATEWAY ESTÁTICO IP
    IPAddress subnet(255, 255, 255, 0);     // OCULTAR SUB REDE
    WiFi.config(staticIP, gateway, subnet);

    while(WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }

    server.begin();
    Serial.println();
    
    if (WiFi.status() == WL_CONNECTED)
        Serial.println("WIFI CONECTADO!");
    else
        Serial.println("WIFI NÃO CONECTADO!");
  
    Serial.print("ENDERECOIP: ");
    Serial.println(WiFi.localIP());
}

void processaLED(String request){
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "OK");
}

void processaBuzzer(String request){
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "OK");
}

void processaLDR(String request){
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "OK");
}

void processaServo(String request){
    int inicio = request.indexOf("$");
    int fim    = request.indexOf("!");
    int valor  = request.substring(inicio+1, fim).toInt();
    
    servo.write(valor);
    delay(50);

    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "OK");
}

void handleRequest(){
    String request = server.uri();

    if(request.startsWith("/LED"))
        processaLED(request);

    else if(request.startsWith("/LDR"))
        processaLDR(request);

    else if(request.startsWith("/BUZZER"))
        processaBuzzer(request);

    else if(request.startsWith("/SERVO"))
        processaServo(request);
}

void setup(){
    pinMode(LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);

    servo.setPeriodHertz(10);
    servo.attach(SERVO, 500, 2400);

    Serial.begin(9600);
    startServer("ProjetoSemaforo", "12345678");
    
    server.onNotFound(handleRequest);
    server.begin();
}

void loop(){
    server.handleClient();
}