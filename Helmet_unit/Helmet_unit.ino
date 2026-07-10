// got some rf transmitters and receiver boards hence changing code because those dont understand radiohead signals 

const int leftIRPin = 2;
const int rightIRPin = 3;
const int rfDataPin = 12;

void setup() {
  pinMode(leftIRPin, INPUT);
  pinMode(rightIRPin, INPUT);
  pinMode(rfDataPin, OUTPUT);
  
  // Setting default state to HIGH 
  digitalWrite(rfDataPin, HIGH);
}

void loop() {
  int leftState = digitalRead(leftIRPin);
  int rightState = digitalRead(rightIRPin);

  // IR sensors usually output LOW when an obstacle (person head) is detected
  if (leftState == LOW && rightState == LOW) {
    // SAFE: Helmet is ON. ( ie helmet is put on) 
    digitalWrite(rfDataPin, HIGH); 
  } else {
    // UNSAFE: Helmet is OFF(ie user did not wear the helmet)
    digitalWrite(rfDataPin, LOW);  
  }
  
  delay(500); // delay for stability issues
}
