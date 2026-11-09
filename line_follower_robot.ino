// LINE FOLLOWER ROBOT - Arduino Uno + L293D + 2 sensor IR digital

// Sensor output: OUT/DO sensor kiri ke A0, sensor kanan ke A1.
// A0/A1 dapat digunakan sebagai pin digital pada Arduino Uno.
const int IR_KIRI  = A0;
const int IR_KANAN = A1;

// L293D: ENA dan ENB harus ke pin PWM Arduino Uno.
// Motor kiri: ENA -> D10, IN1 -> D2, IN2 -> D3
// Motor kanan: ENB -> D11, IN3 -> D4, IN4 -> D5
const int ENA = 10;
const int IN1 = 2;
const int IN2 = 3;
const int ENB = 11;
const int IN3 = 4;
const int IN4 = 5;

// Banyak modul sensor IR digital memberi LOW saat melihat garis hitam.
// Jika modulmu memberi HIGH pada garis hitam, ubah LOW menjadi HIGH.
const int GARIS_TERDETEKSI = LOW;

const int KEC_LURUS = 170;
const int KEC_KOREKSI = 90;

void setMotor(int inA, int inB, int en, int speed, bool balik) {
  speed = constrain(speed, -255, 255);
  if (balik) speed = -speed;

  if (speed > 0) {
    digitalWrite(inA, HIGH);
    digitalWrite(inB, LOW);
  } else if (speed < 0) {
    digitalWrite(inA, LOW);
    digitalWrite(inB, HIGH);
  } else {
    digitalWrite(inA, LOW);
    digitalWrite(inB, LOW);
  }
  analogWrite(en, abs(speed));
}

void motorKiri(int speed) {
  // Balik arah karena motor kiri biasanya terpasang berlawanan arah.
  setMotor(IN1, IN2, ENA, speed, true);
}

void motorKanan(int speed) {
  setMotor(IN3, IN4, ENB, speed, false);
}

void maju() {
  motorKiri(KEC_LURUS);
  motorKanan(KEC_LURUS);
}

void setup() {
  pinMode(IR_KIRI, INPUT);
  pinMode(IR_KANAN, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Buka Serial Monitor untuk mengecek nilai sensor.
  Serial.begin(9600);
}

void loop() {
  const bool kiriPadaGaris = (digitalRead(IR_KIRI) == GARIS_TERDETEKSI);
  const bool kananPadaGaris = (digitalRead(IR_KANAN) == GARIS_TERDETEKSI);

  Serial.print("Kiri: ");
  Serial.print(kiriPadaGaris);
  Serial.print(" | Kanan: ");
  Serial.println(kananPadaGaris);

  if (kiriPadaGaris && !kananPadaGaris) {
    // Garis terdeteksi di kiri: belok/koreksi ke kiri.
    motorKiri(KEC_KOREKSI);
    motorKanan(KEC_LURUS);
  } else if (!kiriPadaGaris && kananPadaGaris) {
    // Garis terdeteksi di kanan: belok/koreksi ke kanan.
    motorKiri(KEC_LURUS);
    motorKanan(KEC_KOREKSI);
  } else {
    // Kedua sensor sama-sama membaca garis atau latar.
    // Tetap maju untuk kondisi garis di tengah / persimpangan.
    maju();
  }

  delay(10);
}
