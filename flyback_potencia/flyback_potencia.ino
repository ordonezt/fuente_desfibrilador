#include "comunicacion.h"
#include "estados.h"
#include "entradas_salidas.h"
#include "medicion.h"
#include "timers.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*######## INICIO DE CODIGO ########*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*######## SETUP ########*/
void setup() {
  comunicacion_inicializar();
  entradas_salidas_inicializar();
  medicion_inicializar();
}
/*######## LOOP ########*/
void loop() {
  timer_tarea();
  maquina_de_estados();
}
