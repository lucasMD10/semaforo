#include <Arduino.h>
#include <Wire.h>
#define botaoSemaforo1 32
#define botaoSemaforo2 33
#define verdeCarro1 21
#define vermelhoCarro1 18
#define vermelhoPedestre1 22
#define verdePedestre1 23
#define verdeCarro2 12
#define vermelhoCarro2 14
#define verdePedestre2 26
#define vermelhoPedestre2 25
#define vermelho_1_carro 0
#define verde_1_carro 1
#define vermelho_1_pedestre 3
#define verde_1_pedestre 4
#define vermelho_2_carro 5
#define verde_2_carro 6
#define vermelho_2_pedestre 8
#define verde_2_pedestre 9


void semaforoPrincipal();
int ledMillis1 = 0;
int ledMillis2 = 0;
int ledMillisBlink = millis();
int semaforo = 0; 
int ledEstado = 0;
int tempoReduzido = 0;
int botaoPedestre = digitalRead(botaoSemaforo1);
int botaoPedestre2 = digitalRead(botaoSemaforo2);  
bool pedestrePassa = false;
bool pedestrePassa2 = false;
bool botaoPedestreAux = false;
bool botaoPedestreAux2 = false;
bool blockBotao = false;
bool blockBotao2 = false;
int botaoAtivo = 0;




void setup() {
Serial.begin (115000);
Serial.println("comecando");
pinMode (botaoSemaforo1, INPUT);
pinMode (botaoSemaforo2, INPUT);

ledcSetup (0, 1000, 12);
ledcAttachPin (vermelhoCarro1, vermelho_1_carro);
ledcSetup (2, 1000, 12);
ledcAttachPin (verdeCarro1, verde_1_carro);
ledcSetup (3, 1000, 12);
ledcAttachPin (vermelhoPedestre1, vermelho_1_pedestre);
ledcSetup (4, 1000, 12);
ledcAttachPin (verdePedestre1, verde_1_pedestre);

// semaforo secundario

ledcSetup (6, 1000, 12);
ledcAttachPin (verdeCarro2, verde_2_carro);
ledcSetup (7, 1000, 12);
ledcAttachPin (vermelhoCarro2, vermelho_2_carro);
ledcSetup (8, 1000, 12);
ledcAttachPin (vermelhoPedestre2, vermelho_2_pedestre);
ledcSetup (9, 1000, 12);
ledcAttachPin (verdePedestre2, verde_2_pedestre);
}

void loop(){
  botaoPedestre = digitalRead(botaoSemaforo1);
  botaoPedestre2 = digitalRead(botaoSemaforo2);
  
  if (botaoPedestre && botaoAtivo == 0) {
    blockBotao = true;  
    blockBotao2 = false;
    botaoAtivo = 1;      
  }

  if (botaoPedestre2 && botaoAtivo == 0) {
    blockBotao2 = true;  
    blockBotao = false;
    botaoAtivo = 2;    
  }

  if (botaoAtivo == 1) {
    blockBotao2 = false; 
  } else if (botaoAtivo == 2) {
    blockBotao = false; 
  }
 
  semaforoPrincipal ();
}

void semaforoPrincipal(){
  if (semaforo == 0){
    botaoAtivo = 0;
  }
  switch (semaforo){
    case 0: // amarelo pra carro 2
      Serial.println ("estou no case 0");
      ledcWrite(vermelho_1_carro, 4095);
      ledcWrite(verde_2_carro, 4095);
      ledcWrite(vermelho_2_carro, 3000);

      ledcWrite(vermelho_2_pedestre, 4095);
      ledcWrite(vermelho_1_pedestre, 4095);
      
      if (millis ()- ledMillis1 >= 3000){
        ledMillis1 =  millis();
        semaforo = 1;
        
        ledcWrite(vermelho_1_carro, 0);
        ledcWrite(verde_1_pedestre,0);
        ledcWrite(verde_2_carro, 0);

        ledcWrite(vermelho_2_carro, 0);
        ledcWrite(vermelho_2_pedestre, 0); 
      }
      break;


    case 1: // vermelho pra carro 2 e verde pro 1
      Serial.println ("estou no case 1");
      ledcWrite(verde_1_carro, 4095);
      ledcWrite(vermelho_2_carro, 4095);

      ledcWrite(vermelho_1_pedestre, 4095);
      ledcWrite(vermelho_2_pedestre, 4095);
    
      if (blockBotao){
        if (millis () - ledMillis1 >= 4000){
          ledMillis1 = millis ();
          pedestrePassa2 = true;
          semaforo = 2;
          botaoPedestreAux = true;
          blockBotao = false;
        }
      }  
    
      if (millis()- ledMillis1 >= 8000 - tempoReduzido){
        ledMillis1 = millis();
        ledMillisBlink = millis();
        tempoReduzido = 0;
        semaforo = 2;
        ledcWrite(verde_1_carro, 0);
        ledcWrite(verde_1_pedestre, 0);
        ledcWrite(vermelho_2_carro, 0);
        ledcWrite(verde_2_pedestre, 0);
        ledcWrite(vermelho_1_carro, 0);
        ledcWrite(vermelho_2_pedestre, 4095);
      }
      break;

    case 2: // amarelo pro carro 1
      Serial.println ("estou no case 2");
      ledcWrite(verde_1_carro, 4095);
      ledcWrite(vermelho_1_carro, 3000);
      
      ledcWrite(vermelho_2_carro, 4095);
      ledcWrite(vermelho_1_pedestre, 4095);

      if (millis()- ledMillis1 >= 3000){
        ledMillis1 = millis();
      if (pedestrePassa2){
          semaforo = 4;
      } else {
        semaforo = 3;
      }
        ledcWrite(verde_1_carro, 0);
        ledcWrite(vermelho_1_carro, 0);
        ledcWrite(vermelho_1_pedestre, 0);

        ledcWrite(vermelho_2_carro, 0);
        ledcWrite(verde_2_pedestre, 0);
      } 
      break;

    case 3: // vermelho pra carro 1 e verde pra 2
      Serial.println ("estou no case 3");
      ledcWrite (vermelho_1_carro, 4095);
      ledcWrite (verde_2_carro, 4095);
      ledcWrite (vermelho_1_pedestre, 4095);
      ledcWrite (vermelho_2_pedestre, 4095);

      if (blockBotao2){
        if (millis () - ledMillis1 >= 4000 ){
          ledMillis1 = millis();
          semaforo = 6;
          pedestrePassa = true;
          botaoPedestreAux2  = true;
          blockBotao2 = false;
        } 
      }
      
      if (millis ()- ledMillis1 >= 8000 - tempoReduzido){
        ledMillis1 = millis();
        ledMillisBlink = millis();
        tempoReduzido = 0;
        semaforo = 0;
        ledcWrite (vermelho_1_carro, 0);
        ledcWrite (vermelho_1_pedestre, 0);
        ledcWrite (verde_2_carro, 0);
        ledcWrite (vermelho_2_pedestre, 0);
        ledcWrite(vermelho_1_pedestre, 4095);
      }
      break;

          
      
// cases exclusivos dos botoes quando ativados 
    case 4: 
      Serial.println ("estou no case 4");
      ledcWrite (verde_1_pedestre, 4095);
      ledcWrite (verde_2_pedestre, 4095);
      ledcWrite (vermelho_1_carro, 4095);
      ledcWrite (vermelho_2_carro, 4095);
      ledcWrite (vermelho_1_pedestre, 0);
      ledcWrite (vermelho_2_pedestre, 0);
  
      if (millis() - ledMillis1 >= 3000){
        ledMillis1 = millis ();
        semaforo = 5;
        ledcWrite (verde_1_pedestre, 0);
        ledcWrite (verde_2_pedestre, 0);
        ledcWrite (vermelho_1_carro, 0);
        ledcWrite (vermelho_2_carro, 0);
        ledcWrite (vermelho_2_pedestre, 0);
        ledcWrite (vermelho_1_pedestre, 0); 
      } 
      break;

    case 5 :
      Serial.println ("estou no case 5");
      ledcWrite (vermelho_1_carro, 4095);
      ledcWrite (vermelho_2_carro, 4095);

      if(millis()- ledMillisBlink >= 500){
          ledMillisBlink = millis();
        if(ledcRead(vermelho_2_pedestre) <= 0 && ledcRead (vermelho_1_pedestre)  <= 0){
            ledcWrite (vermelho_2_pedestre, 4095);
            ledcWrite (vermelho_1_pedestre, 4095);
        } else {
            ledcWrite (vermelho_2_pedestre, 0);
            ledcWrite (vermelho_1_pedestre, 0);
        }
      }
        if (millis() - ledMillis1 >= 2000){
          ledMillis1 = millis();
          pedestrePassa = false;
          pedestrePassa2 = false;
          ledcWrite (vermelho_1_pedestre, 0);
          ledcWrite (vermelho_2_pedestre, 0);
          ledcWrite (vermelho_1_carro, 0);
          ledcWrite (vermelho_2_carro, 0);

          if (botaoPedestreAux){
          semaforo = 3;
          botaoPedestreAux = false;
          } else if (botaoPedestreAux2){
          semaforo = 1;
          botaoPedestreAux2 = false;
          }   
        }
        break;

    case 6: // amarelo pra carro 2
      Serial.println ("estou no case 6");
      ledcWrite(vermelho_1_carro, 4095);
      ledcWrite(verde_2_carro, 4095);
      ledcWrite(vermelho_2_carro, 3000);
      ledcWrite(vermelho_2_pedestre, 4095);
      ledcWrite(vermelho_1_pedestre, 4095);
      
      if (millis ()- ledMillis1 >= 3000){
        ledMillis1 =  millis();
        if (pedestrePassa){
          semaforo = 4;
        } else {
          semaforo = 1;
        }
        ledcWrite(vermelho_1_carro, 0);
        ledcWrite(verde_1_pedestre,0);
        ledcWrite(verde_2_carro, 0);

        ledcWrite(vermelho_2_carro, 0);
        ledcWrite(vermelho_2_pedestre, 0); 
      }
      break;
  } 
}