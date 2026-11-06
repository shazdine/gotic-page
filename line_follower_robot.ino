// LINE FOLLOWER ROBOT

// SENSOR INFRARED
const int IR_KANAN = 11;  // Sensor kanan ke pin 11
const int IR_KIRI  = 12;  // Sensor kiri ke pin 12 (tetap bebas dari pin motor ENB)

// MOTOR 1 (KIRI) -> OUT1 / OUT2
const int ENA = 9;
const int IN1 = 2;
const int IN2 = 3;

// MOTOR 2 (KANAN) -> OUT3 / OUT4
const int ENB = 10;
const int IN3 = 4;
const int IN4 = 5;

// PENGATURAN KECEPATAN
const int KEC_LURUS = 220;
const int KEC_BELOK = 200;
const int KEC_PUTAR = 120;

// FUNGSI MOTOR
void setMotor(int inA, int inB, int en, int speed, bool balik) {
  if (balik) speed = -speed;

  if (speed > 0) {
    digitalWrite(inA, HIGH);
    digitalWrite(inB, LOW);
  }
  else if (speed < 0) {
    digitalWrite(inA, LOW);
    digitalWrite(inB, HIGH);
  }
  else {
    digitalWrite(inA, LOW);
    digitalWrite(inB, LOW);
  }

  analogWrite(en, abs(speed));
}

// MOTOR KIRI DAN KANAN
void motorKiri(int speed) {
  setMotor(IN1, IN2, ENA, speed, true);
}

void motorKanan(int speed) {
  setMotor(IN3, IN4, ENB, speed, false);
}

void setup() {
  pinMode(IR_KANAN, INPUT);
  pinMode(IR_KIRI, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  int kanan = digitalRead(IR_KANAN);
  int kiri = digitalRead(IR_KIRI);

  // KEDUA SENSOR TIDAK MENDETEKSI GARIS
  if (kanan == 0 && kiri == 0) {
    motorKiri(KEC_LURUS);
    motorKanan(KEC_LURUS);
  }

  // SENSOR KANAN MENDETEKSI GARIS
  else if (kanan == 1 && kiri == 0) {
    motorKiri(KEC_BELOK);
    motorKanan(KEC_PUTAR);
  }

  // SENSOR KIRI MENDETEKSI GARIS
  else if (kanan == 0 && kiri == 1) {
    motorKiri(KEC_PUTAR);
    motorKanan(KEC_BELOK);
  }

  // KEDUA SENSOR MENDETEKSI GARIS
  else {
    motorKiri(0);
    motorKanan(0);
  }
}
