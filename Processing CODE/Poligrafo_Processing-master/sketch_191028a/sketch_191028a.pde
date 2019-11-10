import controlP5.*; //import ControlP5 library
import processing.serial.*;

Serial port;

ControlP5 cp5; //create ControlP5 object
PFont font;

int n = -1, res1 = 0, i = 0;
boolean calibrar = false, calibrado = false, run = false, fc = false, limpa = false; 
int num1[] = new int[10];
double p;
char key = '/';

void setup(){ //same as arduino program

  size(670, 400);    //window size, (width, height)
  
  //printArray(Serial.list());   //prints all available serial ports
  
  //port = new Serial(this, "/dev/ttyACM0", 9600);  //i have connected arduino to com3, it would be different in linux and mac os
    
  
  //Criar e colocar botões na janela 
  
  cp5 = new ControlP5(this);
  font = createFont("calibri light bold", 40);    // custom fonts for buttons and title
  
  cp5.addButton("A")     //"red" is the name of button
    .setCaptionLabel("1") 
    .setPosition(10, 110)  //x and y coordinates of upper left corner of button
    .setSize(60, 60)      //(width, height)
    .setFont(font)
  ;   

  cp5.addButton("B")     //"yellow" is the name of button
    .setCaptionLabel("2") 
    .setPosition(80, 110)  //x and y coordinates of upper left corner of button
    .setSize(60, 60)      //(width, height)
    .setFont(font)
  ;

  cp5.addButton("C")     //"blue" is the name of button
    .setCaptionLabel("3") 
    .setPosition(150, 110)  //x and y coordinates of upper left corner of button
    .setSize(60, 60)      //(width, height)
    .setFont(font)
  ;
  
  cp5.addButton("D")     //"alloff" is the name of button
    .setCaptionLabel("4") 
    .setPosition(10, 180)  //x and y coordinates of upper left corner of button
    .setSize(60, 60)      //(width, height)
    .setFont(font)
  ;
  
  cp5.addButton("E")     //"red" is the name of button
    .setCaptionLabel("5") 
    .setPosition(80, 180)  //x and y coordinates of upper left corner of button
    .setSize(60, 60)      //(width, height)
    .setFont(font)
  ;   

  cp5.addButton("F")     //"yellow" is the name of button
    .setCaptionLabel("6") 
    .setPosition(150, 180)  //x and y coordinates of upper left corner of button
    .setSize(60, 60)      //(width, height)
    .setFont(font)
  ;

  cp5.addButton("G")     //"blue" is the name of button
    .setCaptionLabel("7") 
    .setPosition(10, 250)  //x and y coordinates of upper left corner of button
    .setSize(60, 60)      //(width, height)
    .setFont(font)
  ;
  
  cp5.addButton("H")     //"alloff" is the name of button
    .setCaptionLabel("8") 
    .setPosition(80, 250)  //x and y coordinates of upper left corner of button
    .setSize(60, 60)      //(width, height)
    .setFont(font)
  ;
  
   cp5.addButton("I")     //"alloff" is the name of button
    .setCaptionLabel("9") 
    .setPosition(150, 250)  //x and y coordinates of upper left corner of button
    .setSize(60, 60)      //(width, height)
    .setFont(font)
  ;
  
   cp5.addButton("J")     //"blue" is the name of button
    .setCaptionLabel("*") 
    .setPosition(10, 320)  //x and y coordinates of upper left corner of button
    .setSize(60, 60)      //(width, height)
    .setFont(font)
  ;
  
  cp5.addButton("K")     //"alloff" is the name of button
    .setCaptionLabel("0") 
    .setPosition(80, 320)  //x and y coordinates of upper left corner of button
    .setSize(60, 60)      //(width, height)
    .setFont(font)
  ;
  
   cp5.addButton("L")     //"alloff" is the name of button
    .setCaptionLabel("#") 
    .setPosition(150, 320)  //x and y coordinates of upper left corner of button
    .setSize(60, 60)      //(width, height)
    .setFont(font)
  ;
  
  cp5.addButton("CALIBRAR")     //"alloff" is the name of button
    .setPosition(220, 110)  //x and y coordinates of upper left corner of button
    .setSize(220, 60)      //(width, height)
    .setFont(font)
  ;
  
   cp5.addButton("CONFIRMA")     //"alloff" is the name of button
    .setPosition(220, 180)  //x and y coordinates of upper left corner of button
    .setSize(220, 60)      //(width, height)
    .setFont(font)
  ;
  cp5.addButton("LIMPA")     //"alloff" is the name of button
    .setPosition(220, 250)  //x and y coordinates of upper left corner of button
    .setSize(220, 60)      //(width, height)
    .setFont(font)
  ;
   cp5.addButton("RUN")     //"alloff" is the name of button
    .setPosition(220, 320)  //x and y coordinates of upper left corner of button
    .setSize(100, 60)      //(width, height)
    .setFont(font)
  ;
  cp5.addButton("STOP")     //"alloff" is the name of button
    .setPosition(330, 320)  //x and y coordinates of upper left corner of button
    .setSize(110, 60)      //(width, height)
    .setFont(font)
  ;
}

/*void serialEvent (Serial myPort)
{
  // Guardar valor na porta serie numa variavel char
  char medida = myPort.readChar();

  if (medida == '0')
  {
    // Desenhar agua ate aos 0.2L
    fill(66, 245, 78, 100);
    rect(450, 380, 60, -100);
  }

  else if (medida == '1')
  {
    // Desenhar agua ate aos 0.4L
    fill(255, 255, 0, 100);
    rect(450, 380, 60, -200);
  }

  else if (medida == '2')
  {
    // Desenhar agua ate aos 0.6L
    fill(255, 0, 0, 100);
    rect(450, 380, 60, -300);
  }

  else if (medida == '3')
  {
    // Desenhar agua ate aos 0.8L
    fill(255, 0, 0, 100);
    rect(450, 380, 60, -370);
  }
  
   if (medida == '4')
  {
    // Desenhar agua ate aos 0.2L
    fill(66, 245, 78, 100);
    rect(520, 380, 60, -100);
  }

  else if (medida == '5')
  {
    // Desenhar agua ate aos 0.4L
    fill(255, 255, 0, 100);
    rect(520, 380, 60, -200);
  }

  else if (medida == '6')
  {
    // Desenhar agua ate aos 0.6L
    fill(255, 0, 0, 100);
    rect(520, 380, 60, -300);
  }

  else if (medida == '7')
  {
    // Desenhar agua ate aos 0.8L
    fill(255, 0, 0, 100);
    rect(520, 380, 60, -370);
  }
  
     if (medida == '7')
  {
    // Desenhar agua ate aos 0.2L
    fill(66, 245, 78, 100);
    rect(590, 380, 60, -100);
  }

  else if (medida == '8')
  {
    // Desenhar agua ate aos 0.4L
    fill(255, 255, 0, 100);
    rect(590, 380, 60, -200);
  }

  else if (medida == '9')
  {
    // Desenhar agua ate aos 0.6L
    fill(255, 0, 0, 100);
    rect(590, 380, 60, -300);
  }

  else if (medida == 'A')
  {
    // Desenhar agua ate aos 0.8L
    fill(255, 0, 0, 100);
    rect(590, 380, 60, -370);
  }

}*/

void draw(){  //same as loop in arduino

  background(0); // background color of window (r, g, b) or (0 to 255)
  textFont(font,12);
  
  //Display "LCD"
  fill(102,255,0);
  rect(10,10, 430,90);
  
  // Cria do medidor frequência cardiaca
  fill(255);
  rect(450, 10, 60, 370);
  
  // Marcacoes no medidor
  fill(0);
  text("80 bps", 460, 92.5);
  text("60 bps", 460, 185);
  text("40 bps", 460, 277.5);
  text("20 bps", 460, 370);
  
  // Cria do medidor frequência respiratória
  fill(255);
  rect(520, 10, 60, 370);

  // Marcacoes no medidor
  fill(0);
  text("80 bps", 530, 92.5);
  text("60 bps", 530, 185);
  text("40 bps", 530, 277.5);
  text("20 bps", 530, 370);
  
  // Cria do medidor frequência respiratória
  fill(255);
  rect(590, 10, 60, 370);

  // Marcacoes no medidor
  fill(0);
  text("80 bps", 600, 92.5);
  text("60 bps", 600, 185);
  text("40 bps", 600, 277.5);
  text("20 bps", 600, 370);
  
  
  if(limpa == true){
    calibrado = false;
    run = false;
  }
  
  //Tela inicial
  if(calibrar == false && run == false){
    textFont(font,40);
    text("Pressione calibrar", 30,50);
    text("para começar", 30,90);
  }
  
  //Telas no momento da calibração
  if(calibrar == true ){
    if(fc == false){
      textFont(font,40);
      text("Insira frequência", 30,50);
      text("cardiaca:", 30,90);
    }
    else{
      textFont(font,40);
      text("Insira frequência", 30,50);
      text("cardiaca:", 30,90);
      text(res1, 220,90);
    }
    
  }
  
 //Trecho de calibração
 if(calibrar == true){
   if (n==1|| n==2 || n==3 || n==4 || n==5 || n==6 || n==7 || n==8|| n==9|| n==0){//
     num1[i] = n; //
     i++;
     n =-1; //Evita inserir o n infinatamente
   }
   if (key == '*'){
     //Transforma os algarismos do vetor em unidades dezenas e centenas
     for(int j = 1; j<= i; j++){
       p = Math.pow(10,(i-j));  //Efetua operação de potenciação 
       num1[j-1] = num1[j-1] * (int)p; //Converte double para inteiro
       res1 += num1[j-1];
     }
     fc = true;
     if(fc == true){
       calibrado = true; //Só entra em operação após ser calibrado 
     } 
     i = 0;           
   } 
  }
  
 if(run == true && calibrado == true){
   textFont(font,40);
   text("Processando...", 50,50);
   // Se estiver a ser enviado algo pela porta serie..
   /*while(port.available() > 0) {  
    serialEvent(port);
   }*/
   // Esperar 1 segundos
   //delay(1000); 
 }
}


//Atribuir funções aos botões
void A(){
  n = 1; 
}

void B(){
  n = 2;
}

void C(){
  n = 3;
}

void D(){
  n = 4;
}

void E(){
  n = 5;
}

void F(){
  n = 6;
}

void G(){
  n = 7;
}

void H(){
  n = 8;
}

void I(){
  n = 9;
}

void J(){
   key = '*';
}

void K(){
  n = 9;
}

void L(){
  key = '#';
}

void CALIBRAR(){
  calibrar = true;
}

void RUN(){
  run = true;
  calibrar = false;
}

void CONFIRMA(){
   key = '*';
}

void LIMPA(){
  limpa = true;
}

void STOP(){
   calibrar = false;
   calibrado = false;
   run = false;
   fc = false; 
}
