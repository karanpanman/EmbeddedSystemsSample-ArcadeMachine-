
int VRx = A0;
int VRy = A1;
int SW = 41;

int xVal;
int yVal;
int SW_Val;
int dt=200;
int mapX;
int mapY;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT);


}

void loop() {
  // put your main code here, to run repeatedly:
  xVal=analogRead(VRx);
  yVal=analogRead(VRy);
  SW_Val=digitalRead(SW);

  delay(dt);

  Serial.print ("X Value = ");
  Serial.print(xVal);
  Serial.print("Y Value = ");
  Serial.print(yVal);
  // Serial.print("Switch Value: ");
  // Serial.print(SW_Val);
  // mapX = map(xVal, 0, 1023, -512, 512);
  // mapY = map(yVal, 0, 1023, -512, 512);


}
