#include <Arduino.h>
#include "comunicacion.h"
#include "estados.h"
#include "timers.h"
#include "medicion.h"
#include "entradas_salidas.h"

uart_comandos_t uart_comando;
bool timeout_uart_flag = false;

extern uint16_t tension_capacitor, tension_final, histeresis, corriente;
extern estado_t estado_debug;

void timeout_uart(char id[])
{
  timeout_uart_flag = true;
//  Serial.println("timeout");
}

void maquinaUart(char caracter)
{
  static uart_estado_t estado = ESPERANDO_INICIO;
  static char comando[LONGITUD_COMANDO] = {0};
  bool validar_trama = false;
  static uint8_t i = 0;
  uint16_t tension_aux, histeresis_aux, corriente_aux;

  if(timeout_uart_flag)
  {
    estado = ESPERANDO_INICIO;
    timeout_uart_flag = false;
    Serial.println("timeout");
  }

  switch(estado)
  {
    case ESPERANDO_INICIO:
      timer_cambiar_estado("tUART", true);
      if(caracter == '<')
        {
          timer_configurar(TIMEOUT_UART, false, "tUART", timeout_uart);
          estado = ESPERANDO_COMANDO;
          memset(comando, 0, sizeof(comando[0]) * LONGITUD_COMANDO);
          i = 0;
        }
    break;

    case ESPERANDO_COMANDO:
      if(caracter != '>' && i < LONGITUD_COMANDO)
        comando[i++] = caracter;

      if(caracter == '>')
      {
        validar_trama = true;
        estado = ESPERANDO_INICIO;
      }
        
      if(i == LONGITUD_COMANDO)
        estado = ESPERANDO_FINAL;
        
    break;

    case ESPERANDO_FINAL:
      if(caracter == '>')
      {
        validar_trama = true;
//        Serial.println("Trama recibida");
      }
      else
        Serial.println("Error");
      
      estado = ESPERANDO_INICIO;
    break;

    default:
      Serial.println("Error");
      estado = ESPERANDO_INICIO;
    break;
  }

  if(validar_trama == true)
  {
    validar_trama = false; 
   
    switch(comando[0])
    {
      //Cargar
      case 'C':
        uart_comando = CARGAR;
      break;

      //Descargar
      case 'D':
        uart_comando = DESCARGAR;
      break;

      //Reiniciar
      case 'R':
        uart_comando = REINICIAR;
      break;

      //Tension final
      case 'V':
        tension_aux = atoi(comando + 1);
        if(tension_aux > 0 && tension_aux < TENSION_MAXIMA)
          tension_final = tension_aux;
      break;

      //Histeresis
      case 'H':
        histeresis_aux = atoi(comando + 1);
        if(histeresis_aux > 0 && histeresis_aux < HISTERESIS_MAXIMA)
          histeresis = histeresis_aux;
      break;

      //Corriente maxima
      case 'I':
        corriente_aux = atoi(comando + 1);
        if(corriente_aux > 0 /*&& corriente_aux < CORRIENTE_MAXIMA*/)
        {
          corriente = corriente_aux;
          corriente_limite(corriente);
        }
      break;
        
      default:
        uart_comando = NADA;
      break;
    }
  }
}

void comunicacion_PC(char id[])
{  
  //Maquina de estados de la UART
  if(Serial.available() > 0)
    maquinaUart(Serial.read());

  Serial.print("Tension_capacitor:");Serial.print(tension_capacitor); Serial.print("\t");
  Serial.print("Estado:"); Serial.print(estado_debug); Serial.print("\n");

  timer_configurar(MILLIS_TRANSMITIR, false, id, comunicacion_PC);
}

void comunicacion_inicializar(void)
{
  Serial.begin(BAUDRATE);
  timer_agregar(MILLIS_TRANSMITIR, false, "UART", comunicacion_PC);
}
