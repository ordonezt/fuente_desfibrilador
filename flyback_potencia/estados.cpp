#include <Arduino.h>
#include "estados.h"
#include "entradas_salidas.h"
#include "comunicacion.h"
#include "medicion.h"
#include "timers.h"

extern uint16_t tension_capacitor, tension_final, histeresis;
extern uart_comandos_t uart_comando;

estado_t estado_debug;
bool flag_tiempo_cumplido = false;

void maquina_de_estados(void)
{
  //Estado de la maquina (global)
  static estado_t estado = RESET;
  
  estado_debug = estado * 256;
  
  switch(estado)
  {
    case REPOSO:
      accion_reposo();
      if(condicion_cargar() == true)
        transicion_cargar(&estado);
        
      break;
      
    case CARGANDO:
      accion_cargar();
      if(condicion_cargado() == true)
        transicion_cargado(&estado);
      else if(condicion_descargar() == true)
        transicion_descargar(&estado);
        
      break;
    
    case CARGADO:
      accion_cargado();
      if(condicion_descargar() == true)
        transicion_descargar(&estado);
        
      break;
      
    case DESCARGANDO:
      accion_descargar();
      if(condicion_reposo() == true)
        transicion_reposo(&estado);
      break;
    
    default:
      transicion_reposo(&estado);
      break;
  }
}

void timeout(char id[])
{
  flag_tiempo_cumplido = true;
}

/*######### ESTADO CARGANDO ##############*/
//El timer puede ser uno solo que le doy distinto timeout
void transicion_cargar(estado_t *estado)
{
  timer_configurar(TIMEOUT_CARGA, false, "tmout", timeout);
  *estado = CARGANDO;
//  Serial.print("Estado:"); Serial.print(*estado); Serial.print("\n");
}

void accion_cargar(void)
{
  if(tension_capacitor < tension_final - histeresis)
  {
    abrir_rele();
    encender_fuente();
  }
    
  if(tension_capacitor > tension_final + histeresis)
  {
    apagar_fuente();
    cerrar_rele();
  }
}

bool condicion_cargar(void)
{
  bool retorno;
  
  retorno = (uart_comando == CARGAR)? true : false;
  uart_comando = NADA;
  
  return retorno;
}

/*######### ESTADO CARGADO ##############*/
void transicion_cargado(estado_t *estado)
{
  timer_configurar(TIMEOUT_CARGADO, false, "tmout", timeout);
  *estado = CARGADO;
//  Serial.print("Estado:"); Serial.print(*estado); Serial.print("\n");
}

void accion_cargado(void)
{
  apagar_fuente();
  abrir_rele();
}

bool condicion_cargado(void)
{
  return (tension_capacitor > (tension_final - histeresis)) && (tension_capacitor < (tension_final + histeresis));
}

/*######### ESTADO DESCARGANDO ##############*/
void transicion_descargar(estado_t *estado)
{
  timer_borrar("tmout");
  *estado = DESCARGANDO;
//  Serial.print("Estado:"); Serial.print(*estado); Serial.print("\n");
}

void accion_descargar(void)
{
  apagar_fuente();
  cerrar_rele();
}

bool condicion_descargar(void)
{
  bool condicion_uart, condicion_tiempo_limite;

  condicion_uart = (uart_comando == DESCARGAR)? true : false;
  uart_comando = NADA;

  condicion_tiempo_limite = flag_tiempo_cumplido;
  flag_tiempo_cumplido = false;
  
  return condicion_uart || condicion_tiempo_limite;
}

/*######### ESTADO REPOSO ##############*/
void transicion_reposo(estado_t *estado)
{
  *estado = REPOSO;
//  Serial.print("Estado:"); Serial.print(*estado); Serial.print("\n");
}
void accion_reposo(void)
{
  apagar_fuente();
  cerrar_rele();
}

bool condicion_reposo(void)
{
  return tension_capacitor < TENSION_PELIGROSA_MINIMA;
}
