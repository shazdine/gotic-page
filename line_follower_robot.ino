// LINE FOLLOWER ROBOT - Arduino Uno + L293D + 2 sensor IR digital
// Asumsi: kedua sensor bisa membaca garis hitam saat robot berada di atas garis.
// Jika output modul HIGH saat melihat hitam, ubah GARIS_TERDETEKSI menjadi HIGH.

const int IR_KIRI  = A0;
const int IR_KANAN = A1;

// L293D: ENA/ENB harus terhubung ke pin PWM Arduino Uno.
// Motor kiri: ENA -> D10, IN1 -> D2, IN2 -> D3
// Motor kanan: ENB -> D11, IN3 -> D4, IN4 -> D5
const int ENA = 10;
const int IN1 = 2;
const int IN2 = 3;
const int ENB = 11;
const int IN3 = 4;
const int IN4 = 5;

const int GARIS_TERDETEKSI = LOW;

// Atur kecepatan 0-255. Ubah nilai bila motor kiri/kanan tidak seimbang.
const int KEC_KIRI = 155;
const int KEC_KANAN = 155;
const int KEC_KOREKSI = 65;
const int KEC_CARI = 110;

// Arah terakhir saat salah satu sensor masih melihat garis:
// -1 = cari ke kiri, 1 = cari ke kanan, 0 = belum diketahui.
int arahTerakhir = 0;

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
  // Balik arah sesuai orientasi pemasangan motor kiri.
  setMotor(IN1, IN2, ENA, speed, true);
}

void motorKanan(int speed) {
  setMotor(IN3, IN4, ENB, speed, false);
}

void berhenti() {
  motorKiri(0);
  motorKanan(0);
}

void maju() {
  motorKiri(KEC_KIRI);
  motorKanan(KEC_KANAN);
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

  Serial.begin(9600);
  berhenti();
}

void loop() {
  const int bacaKiri = digitalRead(IR_KIRI);
  const int bacaKanan = digitalRead(IR_KANAN);
  const bool kiriPadaGaris = (bacaKiri == GARIS_TERDETEKSI);
  const bool kananPadaGaris = (bacaKanan == GARIS_TERDETEKSI);

  Serial.print("Raw kiri/kanan: ");
  Serial.print(bacaKiri);
  Serial.print("/");
  Serial.print(bacaKanan);
  Serial.print(" | garis kiri/kanan: ");
  Serial.print(kiriPadaGaris);
  Serial.print("/");
  Serial.println(kananPadaGaris);

  if (kiriPadaGaris && kananPadaGaris) {
    // Kedua sensor melihat garis: lanjut lurus (misalnya garis lebar/persimpangan).
    maju();
  } else if (kiriPadaGaris) {
    // Garis terdeteksi di kiri: koreksi ke kiri.
    arahTerakhir = -1;
    motorKiri(max(0, KEC_KIRI - KEC_KOREKSI));
    motorKanan(KEC_KANAN);
  } else if (kananPadaGaris) {
    // Garis terdeteksi di kanan: koreksi ke kanan.
    arahTerakhir = 1;
    motorKiri(KEC_KIRI);
    motorKanan(max(0, KEC_KANAN - KEC_KOREKSI));
  } else {
    // Garis hilang: cari perlahan ke arah terakhir sensor melihat garis.
    // Jika belum pernah melihat garis, berhenti agar robot tidak melaju tanpa kendali.
    if (arahTerakhir < 0) {
      motorKiri(-KEC_CARI);
      motorKanan(KEC_CARI);
    } else if (arahTerakhir > 0) {
      motorKiri(KEC_CARI);
      motorKanan(-KEC_CARI);
    } else {
      berhenti();
    }
  }

  delay(10);
}
