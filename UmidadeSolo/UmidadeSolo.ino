#include <EEPROM.h>
#define EEPROM_UNIT sizeof(float)
float freq;
float H2O;
float AR;

void setup() {
	pinMode(3,INPUT);
	pinMode(13,OUTPUT);
	Serial.begin(9600);
}

void loop() {  
	digitalWrite(13, HIGH);
	EEPROM.get(0, H2O);
	EEPROM.get(100, AR);
	lerFreq(500);
	float Frelativa = (freq-AR)/(H2O-AR);
	Serial.println(Frelativa,4);
	digitalWrite(13, LOW);
	delay(500);
}

void lerFreq(float intervalo){
	float pulsos = 0;
	float tempoInicial = millis();
	float tempo = tempoInicial;
	boolean pulso;
	while(tempo <= (tempoInicial + intervalo)){
		tempo = millis();
		if(digitalRead(3) == HIGH){
			if(pulso == HIGH){
				pulsos = pulsos + 1;
			}
			pulso=LOW; 
		}
		else{
			pulso=HIGH;
		}
	}
	freq = pulsos/(intervalo/1000); 
}

void serialEvent(){
	while(Serial.available()){
		String inputString = Serial.readStringUntil('\n');
		if(inputString.substring(0,3) == "AR#"){
			for(int i=1; i <= 10; i++){
				lerFreq(1000);
				int soma = soma+freq;
				AR = soma/i;
			}
			Serial.println(AR);
			EEPROM.put(100, AR);
			Serial.println(AR);
		}
		if(inputString.substring(0,3) == "H2O"){
			for(int i=1; i <= 10; i++){
				lerFreq(1000);
				int soma = soma+freq;
				H2O = soma/i;
			}
			Serial.println(H2O);
			EEPROM.put(0, H2O);
			Serial.println(H2O);
		}
	}
}
