#include <Fuzzy.h>

Fuzzy *fuzzy = new Fuzzy();

int trig = 12; 
int echo = 11;
long lecture_echo;
long cm;

int ENA_m1 = 5    ;    // Enable/speed motor Front Right 
int ENB_m1 = 3     ;   // Enable/speed motor Back Right
int ENA_m2 = 10    ;   // Enable/speed motor Front Left
int ENB_m2 = 9     ;  // Enable/speed motor Back Left
int count = 0;

void setup()
{  
  pinMode(ENA_m1, OUTPUT);
  pinMode(ENB_m1, OUTPUT);
  pinMode(ENA_m2, OUTPUT);
  pinMode(ENB_m2, OUTPUT);
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW); 
  pinMode(echo, INPUT); 
  // Set the Serial output
  Serial.begin(9600);
  // Set a random seed
  randomSeed(analogRead(0));
//______________________________________________________________________________________________________________________  
  //                                                        Inputs -.-
//______________________________________________________________________________________________________________________
  FuzzyInput *Distance = new FuzzyInput(1);
//______________________________________________________________________________________________________________________

//                                                         The Sets -.-
//______________________________________________________________________________________________________________________
  FuzzySet *Close = new FuzzySet(0, 20, 20, 40);
  Distance->addFuzzySet(Close);
  
  FuzzySet *Mid = new FuzzySet(30, 50, 50, 70);
  Distance->addFuzzySet(Mid);
  
  FuzzySet *Away = new FuzzySet(60, 80, 80, 3000);
  Distance->addFuzzySet(Away);
  
  fuzzy->addFuzzyInput(Distance);
//______________________________________________________________________________________________________________________

//                                                         The output -.-
//______________________________________________________________________________________________________________________
  FuzzyOutput *speed = new FuzzyOutput(1);
//______________________________________________________________________________________________________________________

//                                                         The Sets -.-
//______________________________________________________________________________________________________________________
  FuzzySet *slow_stop = new FuzzySet(0, 50, 50, 80);
  speed->addFuzzySet(slow_stop);
  
  FuzzySet *avg = new FuzzySet(60, 100, 100, 150);
  speed->addFuzzySet(avg);
  
  FuzzySet *fast = new FuzzySet(130, 200, 200, 255);
  speed->addFuzzySet(fast);
  
  fuzzy->addFuzzyOutput(speed);
//______________________________________________________________________________________________________________________

//                                                         The Rules -.-
//______________________________________________________________________________________________________________________
  // Rule 1) IF Distance is Close THEN speed is slow_stop
  
  FuzzyRuleAntecedent *ifDistanceClose = new FuzzyRuleAntecedent();
  ifDistanceClose->joinSingle(Close);
  
  FuzzyRuleConsequent *thenSpeedslow_stop = new FuzzyRuleConsequent();
  thenSpeedslow_stop->addOutput(slow_stop);
  
  FuzzyRule *fuzzyRule1 = new FuzzyRule(1, ifDistanceClose, thenSpeedslow_stop);
  fuzzy->addFuzzyRule(fuzzyRule1);

  // Rule 2) IF Distance is Mid THEN speed is avg
  
  FuzzyRuleAntecedent *ifDistanceMid = new FuzzyRuleAntecedent();
  ifDistanceMid->joinSingle(Mid);
  
  FuzzyRuleConsequent *thenSpeedavg = new FuzzyRuleConsequent();
  thenSpeedavg->addOutput(avg);
  
  FuzzyRule *fuzzyRule2 = new FuzzyRule(2, ifDistanceMid, thenSpeedavg);
  fuzzy->addFuzzyRule(fuzzyRule2);

  // Rule 3) IF Distance is Away THEN speed is fast
  
  FuzzyRuleAntecedent *ifDistanceAway = new FuzzyRuleAntecedent();
  ifDistanceAway->joinSingle(Away);
  
  FuzzyRuleConsequent *thenSpeedFast = new FuzzyRuleConsequent();
  thenSpeedFast->addOutput(fast);
  
  FuzzyRule *fuzzyRule3 = new FuzzyRule(3, ifDistanceAway, thenSpeedFast);
  fuzzy->addFuzzyRule(fuzzyRule3);
}

void loop()
{
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trig, LOW); 
  lecture_echo = pulseIn(echo, HIGH);
  cm = lecture_echo / 58; 
  //Serial.print("Distance en cm : "); 
  //Serial.println(cm);
  //delay(1000); 
  int input = cm;
  // Printing something
  Serial.println("\n\n\nEntrance: ");
  Serial.print("\t\t\tDistance: ");
  Serial.println(input);

  fuzzy->setInput(1, input);

  fuzzy->fuzzify();

  float output = fuzzy->defuzzify(1);
  
  if (output>255){
    output = 255  ;
  }
  if(output<50){
    count ++;
  }
  else
    count = 0;
  if(count>3){
    analogWrite(ENA_m1,255);
    analogWrite(ENB_m1,0);
    analogWrite(ENA_m2,255);
    analogWrite(ENB_m2,0);
    delay(410);
    analogWrite(ENA_m1,0);
    analogWrite(ENB_m1,0);
    analogWrite(ENA_m2,0);
    analogWrite(ENB_m2,0);
  }

  Serial.println("Result: ");
  Serial.print("\t\t\tSpeed: ");
  Serial.println(output);
  analogWrite(ENA_m1,output);
  analogWrite(ENB_m1,0);
  analogWrite(ENA_m2,0);
  analogWrite(ENB_m2,output); 

  delay(1000);
}
