#include <Arduino.h>
#include "entradas_salidas.h"
#include "timers.h"

uint16_t corriente = CORRIENTE_LIMITE_DEFAULT;

void abrir_rele(void)
{
  digitalWrite(DESCARGA, HIGH);
}

void cerrar_rele(void)
{
  digitalWrite(DESCARGA, LOW);
}

void apagar_fuente(void)
{
  digitalWrite(CORTE, HIGH);
}

void encender_fuente(void)
{
  digitalWrite(CORTE, LOW);
}

void heartbeat(char id[])
{
  static bool estado = false;

  estado = (estado == false);
  if(estado)
    digitalWrite(LED, HIGH);
   else
    digitalWrite(LED, LOW);

   timer_configurar(MILLIS_HEARTBEAT, false, id, heartbeat);
}

void corriente_limite(uint16_t corriente_limite)
{
  uint8_t duty;
  
  corriente_limite = corriente_limite < CORRIENTE_MAXIMA? corriente_limite : CORRIENTE_MAXIMA;

  duty = (CORRIENTE_OFFSET - CORRIENTE_GANANCIA * corriente) * CORRIENTE_RESOLUCION / CORRIENTE_VREF;
  
//  duty = ((float)corriente_limite / CORRIENTE_MAXIMA) * 255;
  
  analogWrite(PWM, duty);
}
void entradas_salidas_inicializar(void)
{
  pinMode(CORTE, OUTPUT);
  pinMode(DESCARGA, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(LED, OUTPUT);

  timer_agregar(MILLIS_HEARTBEAT, false, "heart", heartbeat);
  corriente_limite(corriente);
}
