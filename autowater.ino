
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); //(주소, 16열, 2행)
// lcd 연결 위치 A4,A5

// 핀 연결
int sensorPin = A0;   //토양 센서 연결 위치 A0
int relayPin = 7;     // 릴레이 모듈 연결 위치 7


long sensorP;         // 수분 %

int MaxP = 60, MinP = 45; //모터 작동 습도 범위
int sensorValue = 0;  //토양 센서의 습도 값 초기화

void printMV() //습도를 lcd에 출력하는 함수
{
  lcd.setCursor(0, 1);
  lcd.print("Mo: ");
  lcd.print(sensorP);
  lcd.print("%");
  
  if(sensorP>=60)
  lcd.print(" WET ");
  else if(sensorP<60 && sensorP>40)
  lcd.print(" OK  ");
  else
  lcd.print(" DRY  ");
}

long VtoP() //습도 값을 퍼센티지로 변환하는 함수
{
  sensorValue = analogRead(sensorPin);
  return map(sensorValue, 600, 200, 0, 100);
}

void setup()
{
  /* 초기 설정 */
  pinMode(relayPin, OUTPUT); //릴레이 모듈 출력모드로 설정
  digitalWrite(relayPin, LOW); //릴레이 모듈 OFF
  lcd.init(); //lcd 초기화
  lcd.backlight(); //lcd ON
  
  lcd.setCursor(0, 0);
  lcd.print("J Software");
  delay(2000);
  lcd.clear();
}



void loop()
{
  sensorP = VtoP(); // 습도센서로부터 값 불러오기
  

  //습도에 따른 모터 가동 여부 판별하는 조건문
  if( sensorP < MinP ) // 습도가 낮다면 모터 가동
  {
    digitalWrite(relayPin, HIGH); // 모터 ON
    lcd.setCursor(0, 0);
    lcd.print("WATERING..");
      
    while( sensorP < MaxP ) // 모터 가동중 수분측정
    { 
      sensorP = VtoP();
      printMV();
      delay(1000);
    }
    digitalWrite(relayPin, LOW); // 모터 OFF
    lcd.setCursor(0, 0);
    lcd.print("WAITING");
    delay(3000); //5분간 대기 (토양에 수분이 균일하게 퍼지기 위함)
    lcd.setCursor(0, 0);
    lcd.print("             ");
  }
  else
  {
    // 습도 높으면 모터 정지
    digitalWrite(relayPin, LOW); // 모터 OFF
    lcd.setCursor(0, 0);
    lcd.print("             ");
  }
  
 
  printMV();

  delay(1000);
}