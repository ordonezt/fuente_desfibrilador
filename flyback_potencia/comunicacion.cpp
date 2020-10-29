#include <Arduino.h>
#include "comunicacion.h"
#include "estados.h"
#include "timers.h"

uart_comandos_t uart_comando;
extern uint16_t tension_capacitor, tension_final, histeresis;

extern estado_t estado_debug;

void maquinaUart(char caracter)
{
  static uart_estado_t estado = ESPERANDO_INICIO;
  static char comando = 0;
  bool validar_trama = false;
  
  switch(estado)
  {
    case ESPERANDO_INICIO:
      if(caracter == '<')
        estado = ESPERANDO_COMANDO;
    break;

    case ESPERANDO_COMANDO:
      comando = caracter;
      estado = ESPERANDO_FINAL;
    break;

    case ESPERANDO_FINAL:
      if(caracter == '>')
      {
        validar_trama = true;
        Serial.println("Trama recibida");
      }
      else
        Serial.println("Error de trama");
      
      estado = ESPERANDO_INICIO;
    break;

    default:
      Serial.println("Error de trama");
      estado = ESPERANDO_INICIO;
    break;
  }

  if(validar_trama == true)
  {
   validar_trama = false; 
   
    switch(comando)
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
