#define ENA 11
#define ENB 10
#define in1 2
#define in2 3
#define in3 4
#define in4 5

const int trigTr = A1;
const int echoTr = A0;
const int trigS = A3;
const int echoS = A2;


char data = 0; 
int l=0;
int q=0;
int count = 0;
int val =0;
int dem;



float cambienben;
float XungENA=0, XungENB=0;
float goc;

#include <Wire.h>
#include <MechaQMC5883.h>
MechaQMC5883 qmc;


int demzone()
{
  val=analogRead(A2);//hoac A2
  Serial.print("\n val=");
  Serial.print(val);
  if (val<150)
  {
    dem=0;
  }
  else if (val>500)//tuy cuong do sang ma set lai gia tri cho dung
  {
    dem=dem+1;
  }
  if (dem==1)
  {
    count=count+1;
  }
  Serial.print("\n dem: "); //50
  Serial.print(dem);
  Serial.print("\n count: "); //50
  Serial.print(count);
  //delay(3000);
  return count;
}


float hamdoccambien(int trig, int echo)
{
  unsigned long a;
  float d;
  digitalWrite(trig, 0);
  delayMicroseconds(2);
  digitalWrite(trig, 1);
  delayMicroseconds(5);
  digitalWrite(trig, 0);
  a = pulseIn(echo, 1);
  d = a / 2 / 29.412;
  return d;
}


void doccambien()
{
 cambienben = hamdoccambien(trigTr, echoTr);
 Serial.print("\n ben "); //50
 Serial.print(cambienben);
  
}


float hamdocgoc()
{
  float d;
  int x,y,z;
  qmc.read(&x,&y,&z);
  d = atan2(x,y)*360/PI;
  if(d < 0) d+=360;
  d=360-d; // N=0/360, E=85, S=185, W=270

  return d;
  //return round(d);
}

void docgoc()
{
  goc=round(hamdocgoc());
 
  Serial.print("\nGoc: ");
  Serial.println(goc);
}


void nghichtrai()
{
  digitalWrite(in2, LOW);
  digitalWrite(in1, HIGH);
}
void thuantrai()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}
void nghichphai()
{
  digitalWrite(in4, LOW);
  digitalWrite(in3, HIGH);
}
void thuanphai()
{
  digitalWrite(in4, LOW);
  digitalWrite(in3, HIGH);
}
void stop()
{
  digitalWrite(in4, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);
  Serial.print("stop bro");
}

void dithang(float setag)
{
  
  docgoc();
  float Kp = 2.8, Kd = 0, Ki = 0; //float Kp = 0.05 , Kd =0.05 , Ki = 0.7;
  float P, I, D;
  float lasterror;
  int out;
  float SamplingTime = 0.02;      //0.05
  float error = setag - goc; 
  if (abs(error) < 2)
  {
    error=0;
  }
  
  thuantrai();
  thuanphai();
  P = error * Kp;
  I = Ki * error * SamplingTime;
  D = (Kd * (error - lasterror)) / SamplingTime;
  out = P + I + D;
  lasterror = error;
  
  Serial.print("\n error=");

  Serial.print(error);
  if (error>200)
  {
    XungENA=100-out;
    XungENB=100+out;
  }
  else
  {
  if (error>0)
  {
    XungENB=100+out;
    XungENA=100-out;
  }
  else if (error<0)
  {
   XungENB=100-out;
   XungENA=100+out; 
  }
  XungENA = 100 + out; // XungENA = 50 - out;
  XungENB = 100 - out; // XungENB = 50 + out;
  }
    if (XungENB > 255)
      XungENB = 200;
    else if (XungENB < 70)
      XungENB = 60;

    if (XungENA > 255)
      XungENA = 200;
    else if (XungENA < 70)
      XungENA = 60;//35
    Serial.print("\nout: ");
    Serial.print(out);
    Serial.print("\nXungphai: ");
    Serial.print(XungENB);
    Serial.print("                        Xungtrai= ");
    Serial.print(XungENA);
    Serial.print("\n");
    
  analogWrite(ENB, XungENB); //100
  analogWrite(ENA, XungENA); //100
}
void dithang2(int setag)
{
  docgoc();
  float Kp = 2.8, Kd = 0.05, Ki = 0.78; //float Kp = 0.05 , Kd =2 , Ki = 0.5;
  float P, I, D;
  float lasterror;
  int out;
  float SamplingTime = 0.01;//0.05;      //0.05
  float error = setag - goc; 
  thuantrai();
  thuanphai();
  P = error * Kp;
  I = Ki * error * SamplingTime;
  D = (Kd * (error - lasterror)) / SamplingTime;
  out = P + I + D;
  lasterror = error;
//  if(out>=155)
//  {
//    out=-out;
//  }
  Serial.print("\n error=");

  Serial.print(error);
   if (error>200)
  {
    XungENA=100-out;
    XungENB=100+out;
  }
  else 
  {
  if (error>=0) //&& error <=20)
  {
    XungENB=100-out;
    XungENA=100+out;
  }
  else if (error<0)// && error >= 240)
  {
   XungENB=100+out;
   XungENA=100-out; 
  }
  XungENA = 100 + out; // XungENA = 50 - out;
  XungENB = 100 - out; // XungENB = 50 + out;
  }
    if (XungENB > 255)
      XungENB = 200;
    else if (XungENB < 0)
      XungENB = 60;

    if (XungENA > 255)
      XungENA = 200;
    else if (XungENA < 0)
      XungENA = 60;//35
    Serial.print("\nout2: ");
    Serial.print(out);
    Serial.print("\nXungphai: ");
    Serial.print(XungENB);
    Serial.print("                        Xungtrai= ");
    Serial.print(XungENA);
    Serial.print("\n");
    
  analogWrite(ENB, XungENB); //100
  analogWrite(ENA, XungENA); //100
}





void setup()
{
  Wire.begin();
  Serial.begin(9600);
  qmc.init();
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(trigTr, OUTPUT);
  pinMode(echoTr, INPUT);
  pinMode(13, OUTPUT);
//  pinMode(trigC, OUTPUT);
//  pinMode(echoC, INPUT);
}


void loop()
{
  
//  doccambien();
  demzone();
  //docgoc();
  if (count < 3)
  {
    dithang(126);
    
//    docgoc();
  }
  else{
    l=l+1;
  }
  Serial.print("\n l=");
  Serial.print(l);
  
  while(l!=0)
  {
//    docgoc();

    demzone();
    doccambien();
    if (cambienben <= 20)
    {
      
      
      dithang2(285);      
    
    }

    else
    {

      
      dithang2(352); //test, doi lai sau thanh -80
      
    }
    Serial.print("\n q=");
    Serial.print(q);
    while ( count>= 6 )
    {
      stop();
    }
    
//   delay(2000);
    
  }
  Serial.print("\n Ben "); //50
  Serial.print(cambienben);

//  delay(3000);


}
