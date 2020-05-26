#define timer_signal 1500
#define out_number 20
#define in_number 20

// Com pins
#define com_pulse_in 3   //interrupt
#define com_set_in 2     //interrupt
#define com_pulse_out 4
#define com_set_out 5



// Troca de pallet OUT
#define palletP1O  27
#define palletP2O  26
#define palletFrenteO  24
#define palletTrazO  25
#define portaPalletO  28
#define palletTravaO  23
#define palletDestraO 22
//#define palletAir  8

// Troca de Ferramenta OUT
//#define stepServo  9
//#define dirServo  10

//#define atcFrenteO  11
//#define atcTrazO  12
//#define atcFechaO  13
//#define atcAbreO  14
//#define atcP1O  15
//#define atcP2O  16
//#define atcAir  17
//#define atcPortaO 21

//sinal 7i76e OUT
//#define erro  19

// Troca de pallet IN
#define palletP1I  43
#define palletP2I  44
#define palletFrenteI  45
#define palletTrazI  46
//#define palletTravaI  34
//#define palletPorta1 38
//#define palletPorta2 39

//Troca de Ferramneta IN
//#define atcHome  35
//#define atcFrenteI  40
//#define atcTrazI  41
//#define atcFechaI  42
//#define atcAbreI  43
//#define atcP1I  44
//#define atcP2I  45
//#define atcPortaI 46

//sinal 7i76e IN
//#define sinalProntoI  37
//#define contagPulsosI  36
//#define machineFault 47

bool travaLeit = LOW;
bool travaCount = LOW;
int countCom = 0; // Contagem de pulsos permanetes
int countVol = 0; // contagem de pulsos volateis

int inputs[out_number];
int outputs[in_number];

unsigned int count_in = 0;     // variaveis leitura
unsigned int val_save = 0;

int countDownPulse = 0;
int counterSendPin = 0;

int bit_out = 0;
int number_to_out = 0;
int state_to_out = 0;
int enviando = 0;

unsigned long timer = 0;
unsigned long timer2 = 0;
unsigned long timer3 = 0;
unsigned long oldTime1 = 0;
unsigned long currentTime1 = 0;
unsigned long oldTime2 = 0;
unsigned long currentTime2 = 0;
unsigned long oldTime3 = 0;
unsigned long currentTime3 = 0;

int i;

int count_debug;

void setup() {

  pinMode(com_pulse_in, INPUT);
  attachInterrupt(digitalPinToInterrupt(com_pulse_in), pulse_void, FALLING);

  pinMode(com_set_in, INPUT);
  attachInterrupt(digitalPinToInterrupt(com_set_in), set_void, FALLING);
  
  pinMode(com_pulse_out,OUTPUT);
  pinMode(com_set_out,OUTPUT);

  
  pinMode(palletP1O, OUTPUT);
  pinMode(palletP2O, OUTPUT);
  pinMode(palletFrenteO, OUTPUT);
  pinMode(palletTrazO, OUTPUT);
  pinMode(portaPalletO, OUTPUT);
  pinMode(palletTravaO, OUTPUT);
  pinMode(palletDestraO, OUTPUT);
  //pinMode(palletAir, OUTPUT);
  
  //pinMode(stepServo, OUTPUT);
  //pinMode(dirServo, OUTPUT);
  //pinMode(atcFrenteO, OUTPUT);
  //pinMode(atcTrazO, OUTPUT);
  //pinMode(atcFechaO, OUTPUT);
  //pinMode(atcAbreO, OUTPUT);
  //pinMode(atcP1O, OUTPUT);
  //pinMode(atcP2O, OUTPUT);
  //pinMode(atcAir, OUTPUT);
  
  //pinMode(palletP1, OUTPUT);
  //pinMode(palletP1, OUTPUT);
  
  //pinMode(concluido, OUTPUT);
  //pinMode(erro, OUTPUT);




  
  pinMode(palletP1I, INPUT);
  pinMode(palletP2I, INPUT);
  pinMode(palletFrenteI, INPUT);
  pinMode(palletTrazI, INPUT);
  //pinMode(palletTravaI, INPUT);
  //pinMode(palletPorta1, INPUT);
  //pinMode(palletPorta2, INPUT);
  
  //pinMode(atcHome, INPUT);
  //pinMode(atcFrenteI, INPUT);
  //pinMode(atcTrazI, INPUT);
  //pinMode(atcFechaI, INPUT);
  //pinMode(atcAbreI, INPUT);
  //pinMode(atcP1I, INPUT);
  //pinMode(atcP2I, INPUT);
  //pinMode(atcPortaI, INPUT);

  //pinMode(, INPUT);
  //pinMode(, INPUT);
  //pinMode(, INPUT);
  //pinMode(sinalProntoI, INPUT);
  //pinMode(contagPulsosI, INPUT);
  //pinMode(machineFault, INPUT);

  Serial.begin(9600);
}

void loop() {

  //inputs[0] = digitalRead();

  //digitalWrite(,outputs[0]);

  digitalWrite(palletP1O,outputs[2]);
  digitalWrite(palletP2O,outputs[3]);
  digitalWrite(palletFrenteO,outputs[4]);
  digitalWrite(palletTrazO,outputs[5]);
  digitalWrite(portaPalletO,outputs[6]);
  digitalWrite(palletTravaO,outputs[1]);
  digitalWrite(palletDestraO,outputs[0]);

  if(val_save > 0){                  // input pin

    for(i = 0; i <= in_number; i++){
  
      bit_out = val_save -(i+(i+1));
  
      if(bit_out ==  0 || bit_out ==  1){
        number_to_out = i;
        state_to_out = bit_out;
      }
    }
    val_save = 0;
    outputs[number_to_out] = state_to_out;
  }

  count_debug++;
  if(count_debug >= in_number){
    count_debug = 0;
    Serial.println();
  }
  Serial.print(outputs[count_debug]);


  inputs[0] = digitalRead(palletP1I);
  inputs[1] = digitalRead(palletP2I);
  inputs[2] = digitalRead(palletFrenteI);
  inputs[3] = digitalRead(palletTrazI);


  // parte q faz o envio do sinal de entrada para o linuxcnc
  enviando = 1;
  while(enviando == 1){

    timer = micros();
    currentTime1 = timer - oldTime1;
    if(countDownPulse <= ((counterSendPin*2) + inputs[counterSendPin])){
      
      if(currentTime1 <= timer_signal){
        digitalWrite(com_pulse_out, HIGH);
      } else {
        digitalWrite(com_pulse_out, LOW);
      }
      
      if(currentTime1 >= (timer_signal*2)){
        oldTime1 = timer; // reset timer
        countDownPulse++;
      }
    
    } else {
      
      if(currentTime1 <= timer_signal){
        digitalWrite(com_set_out, HIGH);
      } else {
        digitalWrite(com_set_out, LOW);
      }
  
      if(currentTime1 >= (timer_signal*2)){
        oldTime1 = timer; // reset timer 
        countDownPulse = 0;
        counterSendPin++;
        enviando = 0;
        if(counterSendPin >= out_number) counterSendPin = 0;
      }
    }
  }
}

void pulse_void() {
  /*timer2 = micros();
  if(digitalRead(com_pulse_in) == 0){
    oldTime2 = micros();
    count_in++;
  } else {
    currentTime2 = timer2 - oldTime2;
    if(currentTime2 < 400){ // caso fique ligado por pouco tempo anula o valor 
      count_in--;
    }
  }*/
  count_in++;
}

void set_void() {
  /*timer3 = micros();
  if(digitalRead(com_set_in) == 1){
    oldTime3 = micros();
  } else {
    currentTime3 = timer3 - oldTime3;
    if(currentTime3 > 400){ // caso fique ligado por bastante tempo valida a leitura
      val_save = count_in;
      count_in = 0;
    }
  }*/
  val_save = count_in;
  count_in = 0;
}
