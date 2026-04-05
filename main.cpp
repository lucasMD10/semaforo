#include <Wire.h>
#include <main.hpp>



void setup() {
Serial.begin(115000);
Serial.println("setup");
pinMode(BOTAO_PEDESTRE1 , INPUT);
pinMode(BOTAO_PEDESTRE2 , INPUT);

ledcSetup(0 , 1000 , 12);
ledcAttachPin(VERMELHO_CARRO_1 , VERMELHO_CAR1_CHANNEL);
ledcSetup(1 , 1000 , 12);
ledcAttachPin(VERDE_CARRO_1 , VERDE_CAR1_CHANNEL);
ledcSetup(2, 1000 , 12);
ledcAttachPin(VERDE_PEDESTRE_1 , VERDE_PEDESTRE1_CHANNEL);
ledcSetup(3 , 1000 , 12);
ledcAttachPin(VERMELHO_PEDESTRE_1 , VERMELHO_PEDESTRE1_CHANNEL);

ledcSetup(4, 1000 , 12);
ledcAttachPin(VERDE_CARRO_2, VERDE_CAR2_CHANNEL);
ledcSetup(5, 1000, 12);
ledcAttachPin(VERMELHO_CARRO_2, VERMELHO_CAR2_CHANNEL);
ledcSetup(6, 1000, 12);
ledcAttachPin(VERDE_PEDESTRE_2, VERDE_PEDESTRE2_CHANNEL);
ledcSetup(7, 1000, 12);
ledcAttachPin(VERMELHO_PEDESTRE_2, VERMELHO_PEDESTRE2_CHANNEL);

xTaskCreate (semaforoCarro , "semaforoCarro" , 1024 , NULL , 1 , &semaforoCarroHandler);
xTaskCreate (semaforoPedestre , "semaforoPedestre" , 1024 , NULL , 1 , &semaforoPedestreHandler);
}


void desligaTudo (){
  ledcWrite(VERMELHO_CAR1_CHANNEL, 0);
  ledcWrite(VERMELHO_CAR2_CHANNEL, 0);
  ledcWrite(VERMELHO_PEDESTRE1_CHANNEL, 0);
  ledcWrite(VERMELHO_PEDESTRE2_CHANNEL, 0);
  ledcWrite(VERDE_CAR1_CHANNEL, 0);
  ledcWrite(VERDE_CAR2_CHANNEL, 0);
  ledcWrite(VERDE_PEDESTRE1_CHANNEL, 0);
  ledcWrite(VERDE_PEDESTRE2_CHANNEL, 0);

}


void loop(){
int botaoSemaforo1 = digitalRead(BOTAO_PEDESTRE1);
int botaoSemaforo2 = digitalRead(BOTAO_PEDESTRE2);
    if(botaoSemaforo1 && !botaoAtivo  &&  millis()- botaoMillis >= 150){
      botaoAtivo = 1;
      blockBotao1 = true;
      blockBotao2 = false;
    }
    if(botaoSemaforo2 && !botaoAtivo &&  millis() - botaoMillis >= 150){
      botaoAtivo = 2;
      blockBotao2 = true;
      blockBotao1 = false;
    }
    if(botaoAtivo == 1) {
      blockBotao2 = false; 
    } else if(botaoAtivo == 2) {
      blockBotao1 = false; 
    } 
    delay(10);
}

void semaforoCarro(void * pv){
    while(1){
    if(semaforo == 0){
      botaoAtivo = 0;
    }
        switch(semaforo){
        case 0 : 
          // amarelo pro carro 2
          ledcWrite (VERMELHO_CAR1_CHANNEL , 4095);
          vermelhoPedestre();
          amareloCarro2 ();
          if(millis() - tempoMillis >= 3000){
            tempoMillis = millis();
            ledcWrite(VERMELHO_CAR1_CHANNEL, 0);
            ledcWrite(VERMELHO_CAR2_CHANNEL, 0);
            ledcWrite(VERDE_CAR2_CHANNEL, 0);
            desligaPedestre();
            semaforo = 1;
          }
          break;

        case 1: 
        // vermelho pro carro 2 e verde pro carro 1 
          ledcWrite(VERDE_CAR1_CHANNEL , 4095);
          ledcWrite(VERMELHO_CAR2_CHANNEL , 4095);
          vermelhoPedestre();
          if(blockBotao1 || blockBotao2){
            if(millis()- tempoMillis >= 3500){
              tempoMillis = millis();
              passaPedestre1 = true;
              botAuxilia1 = true;
              blockBotao1 = false;
              semaforo = 2;
            }
          }
          if(millis() - tempoMillis >= 7000){
              tempoMillis = millis();
              ledcWrite(VERDE_CAR1_CHANNEL, 0);
              ledcWrite(VERMELHO_CAR2_CHANNEL, 0);
              ledcWrite(VERMELHO_PEDESTRE1_CHANNEL, 0);
              ledcWrite(VERMELHO_PEDESTRE2_CHANNEL, 4095);
              semaforo = 2;
          }
          break;

        case 2 : 
          // amarelo pro carro 1
          ledcWrite(VERMELHO_CAR2_CHANNEL, 4095);
          vermelhoPedestre();
          amareloCarro1();
          if(millis() - tempoMillis >= 3000){
            tempoMillis = millis();
            if(passaPedestre1 || passaPedestre2){
              if(eTaskGetState (semaforoPedestreHandler) != eRunning){
                vTaskResume (semaforoPedestreHandler);
                desligaTudo();
              }
              vTaskSuspend (NULL);
            } 
              ledcWrite(VERMELHO_CAR2_CHANNEL, 0);
              ledcWrite(VERMELHO_CAR1_CHANNEL, 0);
              ledcWrite(VERDE_CAR1_CHANNEL, 0);
              desligaPedestre();
              semaforo = 3;
          }
          break;

        case 3:
          // vermelho carro 1 e verde pro carro 2
          ledcWrite(VERMELHO_CAR1_CHANNEL, 4095);
          ledcWrite(VERDE_CAR2_CHANNEL, 4095);
          vermelhoPedestre();
          if(blockBotao2 || blockBotao1){
            if(millis()- tempoMillis >= 3500){
              tempoMillis = millis();
              passaPedestre2 = true;
              botAuxilia2 = true;
              blockBotao2  = false;
              semaforo = 4;
            }
          }
          if(millis() - tempoMillis >= 7000){
            tempoMillis = millis();
            ledcWrite(VERMELHO_CAR1_CHANNEL, 0);
            ledcWrite(VERDE_CAR2_CHANNEL, 0);
            ledcWrite(VERMELHO_PEDESTRE2_CHANNEL, 0);
            ledcWrite(VERMELHO_PEDESTRE1_CHANNEL,  4095);
            semaforo = 0;
          }
          break;

        case 4 :
          ledcWrite(VERMELHO_CAR1_CHANNEL, 4095);
          vermelhoPedestre();
          amareloCarro2();
          if(millis() - tempoMillis >= 3000){
              tempoMillis = millis ();
              if (passaPedestre2 || passaPedestre1){
                if (eTaskGetState (semaforoPedestreHandler) != eRunning){
                  vTaskResume (semaforoPedestreHandler);
                  desligaTudo();
                }
                vTaskSuspend(NULL);
              }
              ledcWrite(VERMELHO_CAR1_CHANNEL, 0);
              ledcWrite(VERMELHO_CAR2_CHANNEL, 0);
              ledcWrite(VERDE_CAR2_CHANNEL, 0);
              desligaPedestre();
              semaforo = 1;
          }
          break;
          }
          delay(10);     
    }
}


void semaforoPedestre(void * pv){
  vTaskSuspend(NULL);
  while(1){
      switch(semaforo1){
      case 0 :
        ledcWrite(VERMELHO_CAR1_CHANNEL, 4095);
        ledcWrite(VERMELHO_CAR2_CHANNEL, 4095);
        ledcWrite(VERDE_PEDESTRE1_CHANNEL, 4095);
        ledcWrite(VERDE_PEDESTRE2_CHANNEL, 4095);
        ledcWrite(VERMELHO_PEDESTRE1_CHANNEL, 0);
        ledcWrite(VERMELHO_PEDESTRE2_CHANNEL, 0);
          if(millis () - tempoMillis >= 5000){
            tempoMillis = millis();
            semaforo1 = 1;
            ledPedestreDesliga();
          }
        break;

      case 1 :
        ledcWrite(VERMELHO_CAR1_CHANNEL, 4095);
        ledcWrite(VERMELHO_CAR2_CHANNEL, 4095);
          if(millis() - ledMillisBlink >= 400){
            ledMillisBlink = millis();
            if(ledcRead(VERMELHO_PEDESTRE1_CHANNEL) <= 0 && ledcRead(VERMELHO_PEDESTRE2_CHANNEL) <= 0){
              ledcWrite(VERMELHO_PEDESTRE1_CHANNEL, 4095);
              ledcWrite(VERMELHO_PEDESTRE2_CHANNEL, 4095);
            } else {
              ledcWrite(VERMELHO_PEDESTRE1_CHANNEL, 0);
              ledcWrite(VERMELHO_PEDESTRE2_CHANNEL, 0);
            }
          }
          if(millis() - tempoMillis >= 3000){
              tempoMillis = millis();
              passaPedestre1 = false;
              passaPedestre2 = false; 
              ledcWrite(VERMELHO_CAR1_CHANNEL, 0);
              ledcWrite(VERMELHO_CAR2_CHANNEL, 0);
              ledcWrite(VERMELHO_PEDESTRE1_CHANNEL, 0);
              ledcWrite(VERMELHO_PEDESTRE2_CHANNEL, 0); 
              if(botAuxilia1){
                  semaforo = 3;
                  botAuxilia1 = false;
                  if(eTaskGetState(semaforoCarroHandler) != eRunning){
                    vTaskResume(semaforoCarroHandler);
                  }
              } else if(botAuxilia2){
                  semaforo = 1;
                  botAuxilia2 = false;
                  if (eTaskGetState(semaforoCarroHandler) != eRunning){
                    vTaskResume(semaforoCarroHandler);
                  }
              }
              semaforo1 = 0;
              vTaskSuspend(NULL);
          }
          break;
      }
      delay(10);
  }
}




