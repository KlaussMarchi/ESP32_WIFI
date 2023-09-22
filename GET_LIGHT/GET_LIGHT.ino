#include <WiFi.h>
#include <WebServer.h>

#define LDR 15
WebServer server(80);


// CRIANDO E INICIANDO O ROTEADOR LOCAL
void startServer(char* nome, char* senha){
    IPAddress staticIP(192, 168, 4, 1);   // IP ESTÁTICO
    IPAddress gateway(192, 168, 4, 10);   // GATEWAY ESTÁTICO IP
    IPAddress subnet(255, 255, 255, 0);   // OCULTAR SUB REDE
  
    // MODO DE TRABALHO WIFI VIA ACESS POINT
    WiFi.mode(WIFI_AP);                 
    WiFi.config(staticIP, gateway, subnet);
    WiFi.softAP(nome, senha);
  
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

void handleRequest(){
    String request = server.uri();

    if(request.startsWith("/LDR")){
        String resposta = String(analogRead(LDR));
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/plain", resposta);
    }
}

void setup(){
    pinMode(LDR, INPUT);

    Serial.begin(9600);
    startServer("ESP32_WIFI", "12345678");
    
    server.onNotFound(handleRequest);
    server.begin();
}

void loop(){
    server.handleClient();
}
