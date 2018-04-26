#include <Servo.h>

const int ENC_K = 2;
const int PWM_SERVO_K = 5;

volatile int long contador_kinect = 0;

String serialCmd = "";
bool flagControlRxSerial = false;
char sentido_rotacao = 'P';
Servo kinect_servo;


void setup() {

  //kinect_servo.write(81);
  Serial.begin(9600);

  pinMode(ENC_K, INPUT_PULLUP); 
  pinMode(PWM_SERVO_K, OUTPUT);

  
  
  
  attachInterrupt(digitalPinToInterrupt(ENC_K), encoder_kinect_change, RISING);  
}

void loop() {

  if (flagControlRxSerial) {

    // gira o kinect 180 sentido horario
    if (serialCmd == "f\n") {
    
    }
    //gira 180 para um lado e 180 para o outro

    //gira sentido horario
    else if (serialCmd == "g\n") { 
    contador_kinect = 0;
    sentido_rotacao = 'H';
    while(contador_kinect<1260){    
    kinect_servo.attach(PWM_SERVO_K);
    kinect_servo.write(79);
    }
    kinect_servo.detach();
    //delay(1000);
    contador_kinect=0;
    sentido_rotacao = 'A';
    while(contador_kinect<1260){    
    kinect_servo.attach(PWM_SERVO_K);
    kinect_servo.write(83);
    //delay(1000);      
    }
    
    contador_kinect = 0;
    sentido_rotacao = 'A';
    while(contador_kinect<1260){    
    kinect_servo.attach(PWM_SERVO_K);
    kinect_servo.write(83);
    //delay(1000);      
    }
    
    contador_kinect = 0;
    sentido_rotacao = 'H';
    while(contador_kinect<1260){    
    kinect_servo.attach(PWM_SERVO_K);
    kinect_servo.write(79);
    }


    
     kinect_servo.detach();
    }

    // gira o kinect 180 sentido anti horario    
    else if (serialCmd == "t\n") {
     
    }

   

    // retorna o valor do contador do encoder do kinect
    else if ( serialCmd == "ek\n" ) {
      Serial.print(contador_kinect);
    }
    // caso contrario...
    else {
      Serial.print("\n---- COMANDO INVALIDO ----");
    }

    // Resetando valores para nova recepcao...
    
    serialCmd = "";
    flagControlRxSerial = false;
  }
}






//INTERRUÇÕES

void encoder_kinect_change() {
  
  contador_kinect ++;
  
}



void serialEvent() {

  noInterrupts();
  while (Serial.available()) {

    char caractere = (char)Serial.read();

    serialCmd += caractere;

    if (caractere == '\n') {
      flagControlRxSerial = true;
    }
  }
  //Serial.println(serialCmd);
  interrupts();
}

