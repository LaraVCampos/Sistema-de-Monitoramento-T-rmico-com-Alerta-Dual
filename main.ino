/**
 * @file Main.ino
 * @brief Algoritmo homologado de monitoramento termico com alerta dual.
 * @authors Avelar, A. G.; Lodi, A. G.; Campos, L. V. S.; Sousa, M.
 * @date 2026
 */

const int ledVerde = 9;    // Led verde (Seguro)
const int ledAmarelo = 12; // Led amarelo (Atenção)
const int ledAzul = 13;    // Led azul (Alarme/Crítico)
const int sensorTp = A0;   // Sensor de temperatura
const int buzzer = 10;     // Buzzer de alerta

// Controle temporizado do alarme sonoro (Não-bloqueante)
unsigned long tempoBuzzer = 0;
bool buzzerLigado = false;

void setup() {
  Serial.begin(9600);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledAzul, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  // Conversão da leitura analógica para a escala Celsius
  int leitura = analogRead(sensorTp);
  float tensao = leitura * (5.0 / 1023.0);
  float temperatura = tensao * 100.0;

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" *C");

  // CASO 1: Temperatura até 30.0 *C (Normal / Seguro)
  if (temperatura <= 30.0) {
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledAzul, LOW);
    
    noTone(buzzer);
    buzzerLigado = false;
  }
  // CASO 2: Temperatura entre 30.0 *C e 40.0 *C (Atenção)
  else if (temperatura > 30.0 && temperatura <= 40.0) {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(ledAzul, LOW);
    
    noTone(buzzer);
    buzzerLigado = false;
  }
  // CASO 3: Temperatura maior que 40.0 *C (Crítico / Alarme)
  else {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledAzul, HIGH);

    // Dispara a sinalização sonora uma única vez por ciclo crítico
    if (!buzzerLigado) {
      tone(buzzer, 500);
      tempoBuzzer = millis(); // Registra o timestamp inicial de ativação
      buzzerLigado = true;
    }
  }

  // Monitoramento do tempo de execução do buzzer (Desliga após 5 segundos)
  if (buzzerLigado) {
    if (millis() - tempoBuzzer >= 5000) {
      noTone(buzzer);
    }
  }

  delay(500); // Janela de estabilização de amostragem
}
