/** Number Order
 *  A
 *  B
 *  Start
 *  Select
 *  Left
 *  Right
 *  Up
 *  Down;
 */

unsigned char mapping[8] = {1, 2, 4, 8, 16, 32, 64, 128};

void setup()
{
  Serial.begin(9600);
  for(int i = 0; i < 8; ++i)
  {
    pinMode(i + 2, INPUT_PULLUP);
  }
  pinMode(13, OUTPUT);
}

void loop()
{
  unsigned char state = 0;
  state |= digitalRead(2) == LOW ? mapping[0] : 0;
  state |= digitalRead(3) == LOW ? mapping[1] : 0;
  state |= digitalRead(4) == LOW ? mapping[2] : 0;
  state |= digitalRead(5) == LOW ? mapping[3] : 0;
  state |= digitalRead(6) == LOW ? mapping[4] : 0;
  state |= digitalRead(7) == LOW ? mapping[5] : 0;
  state |= digitalRead(8) == LOW ? mapping[6] : 0;
  state |= digitalRead(9) == LOW ? mapping[7] : 0;
  if(state)
  {
    digitalWrite(13, HIGH);
  }
  else
  {
    digitalWrite(13, LOW);
  }
  delay(8);
  Serial.write(state);
}
