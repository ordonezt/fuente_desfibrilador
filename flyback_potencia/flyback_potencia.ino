/*######## DEFINICION DE PINES ########*/
//Salidas digitales
#define CORTE     8
#define DESCARGA  11
//Salidas analogicas
#define PWM       5
//Entradas analogicas
#define ADC_CAP   3

/*######## PARAMETROS DE LA UART ########*/
//Velocidad de la comunicacion
#define BAUDRATE  9600

/*######## PARAMETROS DE TEMPORIZACION ########*/
//Velocidad de muestreo 50ms, 20Hz
#define MILLIS_ADC  50
//Velocidad de transmicion a PC
#define MILLIS_TRANSMITIR 300

/*######## PARAMETROS DEL ADC ########*/
#define ADC_GANANCIA    1
#define ADC_REFERENCIA  5//
#define ADC_OFFSET      0//Porcentual
#define ADC_RESOLUCION  1024//Cuentas

/*######## TENSION DE SEGURIDAD ########*/
#define TENSION_PELIGROSA_MINIMA  20//V

/*######## ENUMERACIONES Y TIPOS DE DATOS ########*/
//Enumeracion de estados de la maquina
typedef enum{
  RESET,
  REPOSO,
  CARGANDO,
  CARGADO,
  DESCARGANDO,
}estado_t;
//Enumeracion de estados de la recepcion de la UART
typedef enum{
  ESPERANDO_INICIO,
  ESPERANDO_COMANDO,
  ESPERANDO_FINAL
}uart_estado_t;
//Enum de los comandos de recepcion por UART
typedef enum{
  NADA,
  CARGAR,
  DESCARGAR,
  REINICIAR
}uart_comandos_t;

/*######## CABECERAS DE FUNCIONES ########*/
void inicializar(void);

void maquina_de_estados(void);
void maquinaUart(char caracter);
void muestrear_capacitor(void);

void transicion_cargar(estado_t *estado);
void transicion_cargado(estado_t *estado);
void transicion_descargar(estado_t *estado);
void transicion_reposo(estado_t *estado);

bool condicion_cargar(void);
bool condicion_cargado(void);
bool condicion_descargar(void);
bool condicion_reposo(void);

void accion_cargar(void);
void accion_cargado(void);
void accion_descargar(void);
void accion_reposo(void);

uart_comandos_t uart_comando;
uint16_t tension_capacitor, tension_final = 1000/*V*/, histeresis = 20/*V*/;
bool flag_tiempo_cumplido = false;

estado_t estado_debug;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*######## INICIO DE CODIGO ########*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*######## SETUP ########*/
void setup() {
  inicializar();
}
/*######## LOOP ########*/
void loop() {
  comunicacion_PC();
  muestrear_capacitor();
  maquina_de_estados();
}


void inicializar(void)
{
  pinMode(CORTE, OUTPUT);
  pinMode(DESCARGA, OUTPUT);
  pinMode(PWM, OUTPUT);

  Serial.begin(BAUDRATE);
}

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

void comunicacion_PC(void)
{
  static unsigned long tiempo_transmitir = 0;
  
  //Maquina de estados de la UART
  if(Serial.available() > 0)
    maquinaUart(Serial.read());

  if(millis() - tiempo_transmitir > MILLIS_TRANSMITIR)
  {
    Serial.print("Tension_capacitor:");Serial.print(tension_capacitor); Serial.print("\t");
    Serial.print("Estado:"); Serial.print(estado_debug); Serial.print("\n");
    tiempo_transmitir = millis();
  }
  
}

/*######### ESTADO CARGANDO ##############*/
//El timer puede ser uno solo que le doy distinto timeout
void transicion_cargar(estado_t *estado)
{
  //setear timer de carga
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
  //setear timer de cargado
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
  //cancelar_timer_carga
  //candelar timer descarga
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

void muestrear_capacitor(void)
{
  static unsigned long tiempo_adc = 0;
  
  if(millis() - tiempo_adc > MILLIS_ADC)
  {
    //tension_capacitor = (uint16_t)(ADC_GANANCIA * ADC_REFERENCIA * (((float)analogRead(ADC_CAP) - ADC_OFFSET)/ ADC_RESOLUCION));
    tension_capacitor = analogRead(ADC_CAP);
    tiempo_adc = millis();
  }
}
