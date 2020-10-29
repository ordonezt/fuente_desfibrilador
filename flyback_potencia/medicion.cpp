#include <Arduino.h>
#include "medicion.h"
#include "timers.h"

uint16_t tension_capacitor, tension_final = 1000/*V*/, histeresis = 20/*V*/;

void muestrear_capacitor(char id[])
{
  //tension_capacitor = (uint16_t)(ADC_GANANCIA * ADC_REFERENCIA * (((float)analogRead(ADC_CAP) - ADC_OFFSET)/ ADC_RESOLUCION));
  tension_capacitor = analogRead(ADC_CAP);

  timer_configurar(MILLIS_ADC, false, id, muestrear_capacitor);
}

void medicion_inicializar(void)
{
  timer_agregar(MILLIS_ADC, false, "ADC", muestrear_capacitor);
}
