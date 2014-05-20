//Constantes con pines de Arduino
  //Motor 1 (Cierra y Abre puertas)
  const int M1_0 = 0;
  const int M1_1 = 1;
  const int M1_2 = 2;
  const int M1_3 = 3;
  //Motor 2 (Subir y Bajar Ascensor)
  const int M2_0 = A0;
  const int M2_1 = A1;
  const int M2_2 = A2;
  const int M2_3 = A3;
  //Sensor BN
  const int B_N = A4;
  //Display
  const int D_0 =4;
  const int D_1 =5;
  //Botones Internos
  const int BI_0 = 9;
  const int BI_1 = 8;
  const int BI_2 = 7;
  const int BI_E = 6;
  //Botones Puertas
  const int BP0 = 13;
  const int BP1_S = 11;
  const int BP1_B = 12;
  const int BP2 = 10;
  //Piezzo
  const int PIEZO = A5;

//Variables
  int piso_actual;
  int piso_destino;
  int velocidad;
  int velocidad_puerta;
  int step;
  boolean frenar;
  float nv; //vueltas de cada llamda a motor
  float nvajuste;

void setup() {
  //Modo de cada pin
    pinMode(D_0,OUTPUT);
    pinMode(D_1,OUTPUT);
    pinMode(B_N,INPUT);
    pinMode(M1_0,OUTPUT);
    pinMode(M1_1,OUTPUT);
    pinMode(M1_2,OUTPUT);
    pinMode(M1_3,OUTPUT);
    pinMode(M2_0,OUTPUT);
    pinMode(M2_1,OUTPUT);
    pinMode(M2_2,OUTPUT);
    pinMode(M2_3,OUTPUT);
    pinMode(BI_0,INPUT);
    pinMode(BI_1,INPUT);
    pinMode(BI_2,INPUT);
    pinMode(BI_E,INPUT);
    pinMode(BP0,INPUT);
    pinMode(BP1_S,INPUT);
    pinMode(BP1_B,INPUT);
    pinMode(BP2,INPUT);
    pinMode(PIEZO, OUTPUT);
    
  //Situar al ascensor en el piso 0 al principio del todo
    step = 0;
    piso_actual = 0;
    piso_destino = 0;
    frenar = false;
    velocidad = 35;
    velocidad_puerta = 25;
    nv=0.1;
    nvajuste = 0.5;
    
  //Inicialización de Displays
    cambiarDisplay(0);
  
  //ParaTests
    //Serial.begin(9600);
}

void loop() {
  
  if(frenar){
    emergencia();
  }else{
 
   //Piso actual será el piso en el que estamos actualmente (hipotéticamente)
   //-----------------------------------------------------------------------------------------------------//
   if(piso_actual == 0){ //Estamos en el piso 0
     if(piso_destino == 0){ //Estoy en la planta baja sin destino
       if(digitalRead(BI_E)==LOW){
           emergencia();
       }
       if(digitalRead(BP0)==LOW || digitalRead(BI_0)==LOW){
         abrir_cerrar();
       }
       if(digitalRead(BP1_B) == LOW || digitalRead(BP1_S) == LOW || digitalRead(BI_1)==LOW){
         piso_destino = 1;
       }
       if(digitalRead(BP2) == LOW || digitalRead (BI_2)==LOW){
         piso_destino = 2;
       }
     }else if(piso_destino == 1){ //Debo de subir a la 1ª planta
       subirpiso(1);
     }else if(piso_destino == 2){ //Debo de subir a la 2ª planta
       subirpiso(2);
     }
   //-----------------------------------------------------------------------------------------------------//
   }else if (piso_actual == 1){ //Estamos en el piso 1
     if(piso_destino == 0){ //Debo de bajar a la planta baja
       bajarpiso(1); 
     }else if(piso_destino == 1){ //Estoy en la 1ª planta sin destino
       if(digitalRead(BI_E)==LOW){
           emergencia();
       }
       if(digitalRead(BP1_B)==LOW || digitalRead(BP1_S)==LOW || digitalRead(BI_1)==LOW){
         abrir_cerrar();
       }
       if(digitalRead(BP0) == LOW || digitalRead(BI_0)==LOW){
         piso_destino = 0;
       }
       if(digitalRead(BP2) == LOW || digitalRead(BI_2)==LOW){
         piso_destino = 2;
       }
     }else{ //Debo de subir a la 2ª planta
       subirpiso(1);
     }
   //-----------------------------------------------------------------------------------------------------//
   }else{ //estamos en el piso 2
     if(piso_destino == 0){ //Debo de bajar a la planta baja
       bajarpiso(2);  
     }else if(piso_destino == 1){ //Debo de bajar a la 1ª planta
       bajarpiso(1);
     }else{ //Estoy en la 2ª planta sin destino
       if(digitalRead(BI_E)==LOW){
           emergencia();
       }
       if(digitalRead(BP2)==LOW || digitalRead(BI_2)==LOW){
         abrir_cerrar();
       }
       if(digitalRead(BP0) == LOW || digitalRead(BI_0)==LOW){
         piso_destino = 0;
       }
       if(digitalRead(BP1_B)==LOW || digitalRead(BP1_S)==LOW || digitalRead(BI_1)==LOW){
         piso_destino = 1;
       }
     }
   }
   //-----------------------------------------------------------------------------------------------------//
  }
}

void abrir_cerrar(){
  alarma(2);
  abrir(velocidad_puerta,125);
  delay(5000);
  cerrar(velocidad_puerta,125);
}

boolean esBlanco(){
  if(analogRead(B_N)<100){
    return true;
  }else{
    return false;
  }
}

void cambiarDisplay(int numero){
    switch(numero){
    case 1:
      digitalWrite(D_0,HIGH);
      digitalWrite(D_1,LOW);
    break;
    case 2:
      digitalWrite(D_0,LOW);
      digitalWrite(D_1,HIGH);
    break;
    case 3:
      digitalWrite(D_0,HIGH);
      digitalWrite(D_1,HIGH);
    break;
    default:
      digitalWrite(D_0,LOW);
      digitalWrite(D_1,LOW);
    break;
  }
}

void abrir(int delayTime,float pasos){
    //48 pasos de 7,5 grados son 360 grados
   
    for (int i=0; i < pasos ; i++) {
      if ((step ==0) || (step ==3) ){
        digitalWrite(M1_0, HIGH);
      }
      else{ digitalWrite(M1_0, LOW);
      }
      
      if (step <=1 ){
        digitalWrite(M1_1, HIGH);
      }
      else{ digitalWrite(M1_1, LOW);
      }
      
      if ((step ==1) || (step ==2) ){
        digitalWrite(M1_2, HIGH);
      }
      else{ digitalWrite(M1_2, LOW);
      }
      
      if ((step ==2) || (step ==3)){
        digitalWrite(M1_3, HIGH);
      }
      else{ digitalWrite(M1_3, LOW);
      }
     
      delay(delayTime);
      step = (step+1)%4;
    }
      
}

void cerrar(int delayTime,int pasos){
   
    for (int i=0; i < pasos ; i++) {
      if ((step ==0) || (step ==3) ){
        digitalWrite(M1_3, HIGH);
      }
      else{ digitalWrite(M1_3, LOW);
      }
      
      if (step <=1 ){
        digitalWrite(M1_2, HIGH);
      }
      else{ digitalWrite(M1_2, LOW);
      }
      
      if ((step ==1) || (step ==2) ){
        digitalWrite(M1_1, HIGH);
      }
      else{ digitalWrite(M1_1, LOW);
      }
      
      if ((step ==2) || (step ==3)){
        digitalWrite(M1_0, HIGH);
      }
      else{ digitalWrite(M1_0, LOW);
      }
     
      delay(delayTime);
      step = (step+1)%4;
    }
}
void bajar(int delayTime,float vueltas){
    float pasos = vueltas * 48;
    //48 pasos de 7,5 grados son 360 grados
   
    for (int i=0; i < pasos ; i++) {
      if ((step ==0) || (step ==3) ){
        digitalWrite(M2_0, 255);
      }
      else{ digitalWrite(M2_0, 0);
      }
      
      if (step <=1 ){
        digitalWrite(M2_1, 255);
      }
      else{ digitalWrite(M2_1, 0);
      }
      
      if ((step ==1) || (step ==2) ){
        digitalWrite(M2_2, 255);
      }
      else{ digitalWrite(M2_2, 0);
      }
      
      if ((step ==2) || (step ==3)){
        digitalWrite(M2_3, 255);
      }
      else{ digitalWrite(M2_3, 0);
      }
     
      delay(delayTime);
      step = (step+1)%4;
    }
    funcionFrenar();
}

void subir(int delayTime,float vueltas){
    float pasos = vueltas * 48;
   
    for (int i=0; i < pasos ; i++) {
      if ((step ==0) || (step ==3) ){
        digitalWrite(M2_3, 255);
      }
      else{ digitalWrite(M2_3, 0);
      }
      
      if (step <=1 ){
        digitalWrite(M2_2, 255);
      }
      else{ digitalWrite(M2_2, 0);
      }
      
      if ((step ==1) || (step ==2) ){
        digitalWrite(M2_1, 255);
      }
      else{ digitalWrite(M2_1, 0);
      }
      
      if ((step ==2) || (step ==3)){
        digitalWrite(M2_0, 255);
      }
      else{ digitalWrite(M2_0, 0);
      }
     
      delay(delayTime);
      step = (step+1)%4;
    }
    funcionFrenar();
}

//-----------------------------------------------------------------------------------//
void subirpiso(int npisos){
  boolean parar = false;
  
  while(esBlanco() && !frenar){ 
    if(npisos==2){
      if(digitalRead(BP1_S)==LOW){
        parar=true;
      } 
    }
    subir(velocidad,nv);
  }
  while(!esBlanco() && !frenar){ 
    if(npisos==2){
      if(digitalRead(BP1_S)==LOW){
        parar=true;
      } 
    }
    subir(velocidad,nv);
  }
  
  if(!frenar){
    subir(velocidad,nvajuste);
    cambiarDisplay(piso_actual+1);
    piso_actual += 1;
    if(parar){
      abrir_cerrar();
    }
    if(npisos!=2){
      abrir_cerrar();
    }
  }
  
  if (npisos == 2){
    while(esBlanco() && !frenar){ 
           subir(velocidad,nv);
    }
    while(!esBlanco() && !frenar){ 
           subir(velocidad,nv);
    }  
    
    if(!frenar){
      subir(velocidad,nvajuste);
      piso_actual = piso_destino;
      cambiarDisplay(piso_actual);;
      abrir_cerrar();
    }
  }
}

void bajarpiso(int npisos){
  boolean parar = false;
  
  while(esBlanco() && !frenar){ 
    if(npisos==2){
      if(digitalRead(BP1_B)==LOW){
        parar=true;
      } 
    }
    bajar(velocidad,nv);
  }
  while(!esBlanco() && !frenar){
    if(npisos==2){
      if(digitalRead(BP1_B)==LOW){
        parar=true;
      } 
    }
    bajar(velocidad,nv);
  }
  
   if(!frenar){
     bajar(velocidad,nvajuste);
    cambiarDisplay(piso_actual-1);
    piso_actual -= 1;
    if(parar){
      abrir_cerrar();
    }
    if(npisos!=2){
      abrir_cerrar();
    }
  }
  
  if(npisos == 2){
    piso_destino = 0;
    while(esBlanco() && !frenar){ 
           bajar(velocidad,nv);
    }
    while(!esBlanco() && !frenar){ 
           bajar(velocidad,nv);
    }
    
    if(!frenar){
      bajar(velocidad,nvajuste);
      piso_actual = piso_destino;
      cambiarDisplay(piso_actual);;
      abrir_cerrar();
    }
    
  }
}

void funcionFrenar(){
  if(digitalRead(BI_E)==LOW){
    frenar = true;
  }
}

void emergencia(){
  alarma(1);
  cambiarDisplay(3);
  if(piso_actual < piso_destino){
    piso_destino = piso_actual;
    bajaremergencia();
  }else if(piso_actual > piso_destino){
    piso_actual--;
    piso_destino = piso_actual;
    bajaremergencia();
  }else
    abrir(velocidad_puerta,125);
    while(digitalRead(BI_E)==HIGH){}
    cerrar(velocidad_puerta,125);
    cambiarDisplay(piso_actual);
    frenar=false;
}

void bajaremergencia(){
  while(!esBlanco()){ 
      bajar(velocidad,nv);
    }
  bajar(velocidad,nvajuste);
  abrir(velocidad_puerta,125);
}



void alarma(int tipo){
  switch(tipo){
   case 1: //alarma emergencia
    tone(PIEZO, 440, 1000);
    delay(2000);
    tone(PIEZO, 440, 1000);
    delay(2000);
    tone(PIEZO, 440, 1000);
   break;
   case 2: //alarma llegada a piso destino
     tone(PIEZO, 500, 1000);
     delay(800);
     tone(PIEZO, 420, 1000);
   break;
  default:
   tone(PIEZO, 440, 3000);
  break; 
  }
}
