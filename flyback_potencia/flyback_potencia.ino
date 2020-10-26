//Salidas digitales
#define CORTE     8
#define DESCARGA  11

//Salidas analogicas
#define PWM       5

//Entradas analogicas
#define ADC       9

//Velocidad de la comunicacion
#define BAUDRATE  9600

void setup() {
  // put your setup code here, to run once:
  pinMode(CORTE, OUTPUT);
  pinMode(DESCARGA, OUTPUT);
  pinMode(PWM, OUTPUT);

  Serial.begin(BAUDRATE);
}

void loop() {
  // put your main code here, to run repeatedly:

  
}
