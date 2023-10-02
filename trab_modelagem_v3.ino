#define ntc_pin A0         // Pin,to which the voltage divider is connected
#define vd_power_pin 2        // 5V for the voltage divider
#define nominal_resistance 10000       //Nominal resistance at 25⁰C
#define nominal_temeprature 26   // temperature for nominal resistance (almost always 25⁰ C)
#define samplingrate 5    // Number of samples
#define beta 4550  // The beta coefficient or the B value of the thermistor (usually 3000-4000) check the datasheet for the accurate value.
#define Rref 10000   //Value of  resistor used for the voltage divider

int samples = 0;   //array to store the samples
int mosfetPin = 9;//Atribui o pino 9 a variável ledPin
int potPin = A5;//Atribui o pino analógico A5 a variável potPin
int valorpot = 0;//Variável responsável pelo armazenamento da leitura bruta do potenciometro
int pwm = 0;//Variável responsável pelo armazenamento do valor convertido pela função map

float temperatura = 0; //VARIÁVEL DO TIPO FLOAT
void setup(){
  pinMode(ledPin, OUTPUT);//Configura ledPin como saída
  pinMode(potPin, INPUT);//Configura potPin como entrada
  pinMode(vd_power_pin, OUTPUT);

  Serial.begin(9600);//Inicializa a serial com velocidade de comunicação de 9600.
  
}

void loop(){
  //Incrementar pwm;
  if (pwm<255){
    pwm = pwm +1;
  }
  else{
    pwm = 255;
  }
  
  analogWrite(mosfetPin, pwm);
  Serial.print("PWM = ");
  Serial.print(pwm);//Imprime valor do PWM na serial
  delay(500);//Intervalo de 500 milissegundos
  
  //Função de leitura de temperatura com o NTC
  readTemp();
}

float readTemp(){
  uint8_t i;
  float average;
  samples = 0;

  // take voltage readings from the voltage divider
  digitalWrite(vd_power_pin, HIGH);
  for (i = 0; i < samplingrate; i++) {

    samples += analogRead(ntc_pin);

    delay(10);

  }

  digitalWrite(vd_power_pin, LOW);
  average = 0;
  average = samples / samplingrate;

  // Calculate NTC resistance
  average = 1023 / average - 1;
  average = Rref / average;

  float temperature;
  temperature = average / nominal_resistance;     // (R/Ro)
  temperature = log(temperature);                  // ln(R/Ro)
  temperature /= beta;                   // 1/B * ln(R/Ro)
  temperature += 1.0 / (nominal_temeprature + 273.15); // + (1/To)
  temperature = 1.0 / temperature;                 // Invert
  temperature -= 273.15;                         // convert absolute temp to C

  Serial.print(", Temperature = ");

  Serial.println(temperature);
  delay(500);
  return temperature;
}