//abbreviations
#define PT(s) Serial.print(s)  //makes life easier
#define PTL(s) Serial.println(s)
#define PTF(s) Serial.print(F(s))//trade flash memory for dynamic memory with F() function
#define PTLF(s) Serial.println(F(s))
void printCmd(char token, int8_t size, int8_t *arr) {
  PTL(size);
  PT(token);
  for (int i = 0; i < size; i++)
    PT((char)arr[i]);
  PTL('~');
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  PTL("start");
  int8_t cmd[] = {8, 15, 14, 15, 2, 30, 15, 15};
  //move joint 8 to 5 degree, 9 to 0, 40 to 78, 15 to 60
  printCmd('i', sizeof(cmd), cmd);//token i(ndexed joint movements)
  //encryption as char string with non-displayable characters: i~

  int8_t cmd2[] = {0, 0, 0, 0, 0, 0, 0, 0, 20, 20, 30, 30, 44, 46, 20, 20};
  printCmd('l', 16, cmd2);//token l(ist of 16 joint angles)
  //encryption as char string with non-displayable characters: l/      ,.~
}

void loop() {
  // put your main code here, to run repeatedly:
  if ( Serial.available() > 0) {
    char token = Serial.read();
    switch (token) {
      case 'i':
      case 'l':
        String inBuffer = Serial.readStringUntil('~');
        int8_t numArg = inBuffer.length();
        PT(numArg);
        char* c = inBuffer.c_str();
        if (token == 'i') {
          PTL(" moves");
          PTL("joint\tangle");
          for (int i = 0; i < numArg; i += 2) {
            Serial.print(c[i], DEC);//joint index
            PT(",\t");
            Serial.println(c[i + 1], DEC);//target angle
          }
        }
        else if (token == 'l') {
          PT(" angles: ");
          for (int i = 0; i < numArg; i ++) {
            Serial.print(c[i], DEC);//target angle
            PT(", ");
          }
          PTL();
        }
    }
  }

}
