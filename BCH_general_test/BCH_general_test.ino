#include <BCH_general.h>
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  int* msg = new int[19*7];
  Serial.print("msg: ");
  for(int j = 0; j<15*7; j++){
    msg[j] = j%2;
    Serial.print(msg[j]);
  }
  Serial.println();
  int* codeword = new int[19*15];
  codeword = encode(msg, 19);
  Serial.print("codeword: ");
  for(int j = 0; j<15*7; j++){
    Serial.print(codeword[j]);
  }
  Serial.println();
  codeword[1] ^= 1;
  codeword[15];
  msg = decode(codeword, 19);
  Serial.print("corrected msg: ");
  for(int j = 0; j<15*7; j++){
    Serial.print(msg[j]);
  }
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:

}
