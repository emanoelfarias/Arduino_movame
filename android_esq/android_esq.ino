#include <Servo.h>

const int ENC_D = 3;
const int ENC_E = 2;

const int ME_CONTROL_1 = 8;
const int ME_CONTROL_2 = 9;
const int MD_CONTROL_1 = 6;
const int MD_CONTROL_2 = 7;

const int ME_PWM = 10;
const int MD_PWM = 11;

volatile int long contador_enc_dir = 0;
volatile int long contador_enc_esq = 0;

String serialCmd = "";
bool flagControlRxSerial = false;
char motor_dir_state = 'P';
char motor_esq_state = 'P';
int pwm_motors_power = 50;

void setup() {

  //kinect_servo.write(81);
  Serial.begin(9600);

  pinMode(ENC_D, INPUT_PULLUP);
  pinMode(ENC_E, INPUT_PULLUP);
  pinMode(ME_CONTROL_1, OUTPUT);
  pinMode(ME_CONTROL_2, OUTPUT);
  pinMode(MD_CONTROL_1, OUTPUT);
  pinMode(MD_CONTROL_2, OUTPUT);
  pinMode(ME_PWM, OUTPUT);
  pinMode(MD_PWM, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ENC_D), encoder_direito_change, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_E), encoder_esquerdo_change, RISING);

}

void loop() {

  if (flagControlRxSerial) {


    // ambos os motores

    if (serialCmd == "f\n") {

      motor_esq_state = 'F';
      motor_dir_state = 'F';
      digitalWrite(ME_CONTROL_1, HIGH);
      digitalWrite(ME_CONTROL_2, LOW);
      analogWrite(ME_PWM, pwm_motors_power);

      digitalWrite(MD_CONTROL_1, HIGH);
      digitalWrite(MD_CONTROL_2, LOW);
      analogWrite(MD_PWM, pwm_motors_power);
    }
    else if (serialCmd == "t\n") {

      motor_esq_state = 'T';
      motor_dir_state = 'T';

      analogWrite(ME_PWM, pwm_motors_power);
      analogWrite(MD_PWM, pwm_motors_power);

      digitalWrite(ME_CONTROL_1, LOW);
      digitalWrite(ME_CONTROL_2, HIGH);
      digitalWrite(MD_CONTROL_1, LOW);
      digitalWrite(MD_CONTROL_2, HIGH);

    }
    else if (serialCmd == "p\n") {

      motor_esq_state = 'P';
      motor_dir_state = 'P';

      analogWrite(ME_PWM, 0);
      analogWrite(MD_PWM, 0);

      digitalWrite(ME_CONTROL_1, HIGH);
      digitalWrite(ME_CONTROL_2, HIGH);
      digitalWrite(MD_CONTROL_1, HIGH);
      digitalWrite(MD_CONTROL_2, HIGH);
    }


    //motor esquerdo

    else if ( serialCmd == "ef\n" ) {
      Serial.println("\nCOMANDO RECEBIDO: FRENTE");
      motor_esq_state = 'F';
      digitalWrite(ME_CONTROL_1, HIGH);
      digitalWrite(ME_CONTROL_2, LOW);
      analogWrite(ME_PWM, pwm_motors_power);

    }


    else if ( serialCmd == "et\n") {

      // Serial.println("\nCOMANDO RECEBIDO: TRAS");
      motor_esq_state = 'T';
      digitalWrite(ME_CONTROL_1, LOW);
      digitalWrite(ME_CONTROL_2, HIGH);
      analogWrite(ME_PWM, pwm_motors_power);

    }

    else if ( serialCmd == "ep\n") {

      // Serial.println("\nCOMANDO RECEBIDO: PARA");
      motor_esq_state = 'P';
      digitalWrite(ME_CONTROL_1, HIGH);
      digitalWrite(ME_CONTROL_2, HIGH);
      analogWrite(ME_PWM, 0);

    }
    // motor direito

    else if ( serialCmd == "df\n" ) {
      //  Serial.println("\nCOMANDO RECEBIDO: FRENTE");
      motor_dir_state = 'F';
      digitalWrite(MD_CONTROL_1, HIGH);
      digitalWrite(MD_CONTROL_2, LOW);
      analogWrite(MD_PWM, pwm_motors_power);

    }
    else if ( serialCmd == "dt\n") {

      //  Serial.println("\nCOMANDO RECEBIDO: TRAS");
      motor_dir_state = 'T';
      digitalWrite(MD_CONTROL_1, LOW);
      digitalWrite(MD_CONTROL_2, HIGH);
      analogWrite(MD_PWM, pwm_motors_power);

    }

    else if ( serialCmd == "dp\n") {

      //  Serial.println("\nCOMANDO RECEBIDO: PARA");
      motor_dir_state = 'P';
      digitalWrite(MD_CONTROL_1, HIGH);
      digitalWrite(MD_CONTROL_2, HIGH);
      analogWrite(MD_PWM, 0);

      // contagem encoders


    }
    else if (serialCmd == "ed\n") {
      //  Serial.println("\nCONTAGEM ENCODER DIREITO:");
      Serial.println(contador_enc_dir);
    }
    else if (serialCmd == "ee\n") {
      //Serial.println("\nCONTAGEM ENCODER KINECT:");
      Serial.println(contador_enc_esq);
    }
    else {
      Serial.print("\n---- COMANDO INVALIDO ----");
    }

    /* Resetando valores para nova recepcao */
    serialCmd = "";
    flagControlRxSerial = false;
  }
}






//INTERRUÇÕES

void encoder_direito_change() {
  //Serial.println("\n\nMudanca encoder motor direito");
  //Serial.println(contador_enc_dir);
  if (motor_dir_state == 'F')
    contador_enc_dir ++;
  if (motor_dir_state == 'T')
    contador_enc_dir --;
}

void encoder_esquerdo_change() {
  //Serial.println("\n\nMudanca encoder motor esquerdo");
  //Serial.println(contador_enc_esq);
  if (motor_esq_state == 'F')
    contador_enc_esq ++;
  if (motor_esq_state == 'T')
    contador_enc_esq --;
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

