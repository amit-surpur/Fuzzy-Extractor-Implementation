#include <BCHCodec.h>
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  int* msg = new int[7];
  for(int i = 0; i<7; i++){
    msg[i] = i%2;
  }
  int* codeword = new int[15];
  encoder_15_7(msg, codeword);
  Serial.print("Codeword: ");
  for(int j = 0; j<15; j++){
    Serial.print(codeword[j]);
  }
  Serial.println();
  codeword[1] ^= 1;
  decode_15_7(codeword, msg);
  Serial.print("corrected: ");
  for(int j = 0; j<7; j++){
    Serial.print(msg[j]);
  }
  Serial.println();

}

void loop() {
  // put your main code here, to run repeatedly:

}
