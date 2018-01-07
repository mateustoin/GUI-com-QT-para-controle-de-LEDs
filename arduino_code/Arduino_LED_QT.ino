#define LED_1 9
#define LED_2 10
#define LED_3 11

void setup()
{
  /* Iniciando a Serial e passando a taxa de baudrate */
  Serial.begin(9600); 
  pinMode (LED_1, OUTPUT);
  digitalWrite (LED_1, LOW);
  pinMode (LED_2, OUTPUT);
  digitalWrite (LED_2, LOW);
  pinMode (LED_3, OUTPUT);
  digitalWrite (LED_3, LOW);
}
 
void loop()
{
  if (Serial.available()) {
    char letra = Serial.read();
    const int leitura_brilho = Serial.parseInt();
    
    escreve_led(letra, leitura_brilho);
  }
}

void escreve_led (char letra, int brilho){
  if (letra == 'a'){
    analogWrite (LED_1, brilho);
  }

  if (letra == 'b'){
    analogWrite (LED_2, brilho);
  }

  if (letra == 'c'){
    analogWrite (LED_3, brilho);
  }
}

