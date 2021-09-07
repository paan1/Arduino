#define button1 12
#define button2 11
#define button3 10
#define led1 4
#define led2 3
#define led3 2
bool off1= 0,off2= 0,off3= 0;
unsigned long int time1,time2,time3;
void setup()
{
  Serial.begin(9600);
  pinMode(button1,INPUT);
  pinMode(led1,OUTPUT);
  pinMode(button2,INPUT);
  pinMode(led2,OUTPUT);
  pinMode(button3,INPUT_PULLUP);
  pinMode(led3,OUTPUT);
  
}
void loop()
{
  int ReadSwitch1 = digitalRead(button1);
  int ReadSwitch2 = digitalRead(button2);
  int ReadSwitch3 = digitalRead(button3);
  
  //led1
  if(ReadSwitch1 == HIGH )
  {
  if(millis() - time1 >= 10)
  {
    off1 = !off1;
  }
   time1 = millis();
  }
  if(off1 == 1 &&(millis()-time1)<= 3000)
  {
    digitalWrite(led1,HIGH);
  }
  else 
  {
    off1 = 0;
  digitalWrite(led1,LOW);
  }
    
  
  //led2
  if(ReadSwitch2 == LOW )
  {
  if(millis() - time2 >= 10 && off1 == 0 && off3 == 0)
  {
    off2 = !off2;
  }
   time2 = millis();
  }
  if(off2 == 1 && (millis()-time2) <= 2000)
  {
    
    
  if((millis()-time2) <= 500)
  {
  digitalWrite(led2,HIGH);
  }
  else if((millis()-time2) <= 1000)
  {
  digitalWrite(led2,LOW);
  }  
  else if((millis()-time2) <= 1500)
  {
    digitalWrite(led2,HIGH);
  }
  else if((millis()-time2) <= 2000)
  {
    digitalWrite(led2,LOW);
  }
  }
  
  
  else 
  {
    off2 = 0;
  digitalWrite(led2,LOW);
  }
  
    
  
  
  //led3
  if(ReadSwitch3 == LOW )
  {
  if(millis() - time3 >= 50&&off1 == 0)
  {
    off3= !off3;
  }
  else if(off3 ==1 && off1 == 1)
  {
    off3 = !off3;
    digitalWrite(led3,LOW);
  }
   time3 = millis();
  }
  if(off3 == 1&&(millis()-time3)<= 3000)
  {
    digitalWrite(led3,HIGH);
  }
  else 
  {
    off3 = 0;
  digitalWrite(led3,LOW);
  }
}
