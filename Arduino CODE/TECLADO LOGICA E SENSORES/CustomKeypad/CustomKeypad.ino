//Carrega a biblioteca Keypad
#include <Keypad.h>
//Carrega a biblioteca LiquidCrystal
#include <LiquidCrystal.h>
 
//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//Define o tamnho do teclado
const byte ROWS = 4; //quatro linhas
const byte COLS = 4; //Quatro colunas

//Define a configuraço dos simbolos para o teclado
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
  
};
byte rowPins[ROWS] = {6, 7, 8, 9}; //Define os pinos usados para as linhas
byte colPins[COLS] = {10, 11, 12, 13}; //Define os pinos usados para as colunas

//Cria o teclado
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

//Definiçoes para comunicaço serial
void setup(){
  //Parmetro da porta serial
  Serial.begin(9600);
  //Define o número de colunas e linhas do LCD
  lcd.begin(16, 2);
}  
  //Declaraçod e variaveis
  int k = 0, c, limiar = 0, veredito = 0;  //Numeros digitado no formato inteiro
  int res_fis_inc[3]={0,0,0}, pesos[3] = {1,1,1}, res_fis_atu[3]={0,0,0};
  String num1, mens2;       //Numeros que digitados no tipo string
  boolean calibrar = false, calibrado = false, run = false, confirma = false, fc = false, fr = false, um = false, limpa = false, verdade = false, mentira = false;
  
  //Variaveis usadas somente para testar o codigo
  boolean mens1 = false;

void loop(){
      char key = customKeypad.getKey();
      //Inserir rotina para LCD
      if(mens1 == false){
         mens2 = "Pressione C para calibrar.";
         Serial.println(mens2);
         mens1 = true;
      }
      
      if(calibrado == true && run == false){
         mens2 = "Pressione D modo de deteccao.";
         Serial.println(mens2);
         run = true;
      }
      switch(key){
            case 'C':
                //Rotina para inserir dados de calibraçao
                while(k < 3){  
                    switch(k){
                      case 0:
                        if(fc == false){
                          //Nesse trecho vamos inserir rotinas para imprimir no LCD
                          mens2 = "Insira a frequencia cardiaca:";
                          Serial.println(mens2);
                          fc = true;
                        }
                      break;
                      case 1:
                        if(fr == false){
                           //Nesse trecho vamos inserir rotinas para imprimir no LCD
                           mens2 = "Insira a frequencia respiratoria:";
                           Serial.println(mens2);
                          fr = true;
                        }
                       
                      break;
                      case 2:
                         if(um == false){
                           //Nesse trecho vamos inserir rotinas para imprimir no LCD
                           mens2 = "Insira o valor de umidade:";
                           Serial.println(mens2);
                           um = true;
                        }
                         
                      break;
                    };
                    char key = customKeypad.getKey(); //Guarda o dado lido teclado na varivel key
                    if (key != NO_KEY && (key=='1' || key=='2' || key=='3' || key=='4' || key=='5' || key=='6' || key=='7' || key=='8'|| key=='9' || key=='0')){
                          num1 = num1 + key; //Concatena string
                      }
                  
                      if (key !=NO_KEY && key == '*'){
                         res_fis_inc[k] = num1.toInt(); //Converte para interiro
                         Serial.println(res_fis_inc[k]);
                         k++;
                         num1 = "";
                      }
              
                }
               //Define o limiar para pessoa em questao 
               for(int i = 0; i<k; i++){
                 limiar += res_fis_inc[i] * pesos[i];  
               }
               k = 0;
               calibrado = true;
               if(calibrado == true){
                 mens2 ="Equipamento calibrado.\nLimiar:";
                 Serial.println(mens2);
                 Serial.println(limiar);
               }
                //Fim da rotina para inserir dados de calibraçao
            break;
            
            case 'D':
              if(calibrado == true){
                 mens2 = "Processando...";
                 Serial.println(mens2);
                 //Rotina para verificar se  mentira
                 
                 for(int j = 0; j<4000; j++){
                   //Le dados dos sensores e guarada em res_fis_atu
                   veredito +=  1;
                   //for(int i = 0; i<3; i++){
                     //veredito +=  1;
                   //}
                   
                 }
                 
                 if(veredito > limiar){
                   mens2 = "Mentira Detectada.";
                   Serial.println(mens2);
                 }
                 else{
                   mens2 = "Veradade Detectada.";
                   Serial.println(mens2);
                 }
                 //Fim rotina para verificar se  mentira
              }
             break;        
      }; //Fim do switch case menu     
}

void reset(){
  num1 = "";
  res_fis_inc[0] = 0;
  res_fis_inc[1] = 0;
  res_fis_inc[2] = 0;
  c = 0;
  fc = false;
  fr = false; 
  um = false;
  mens1 = false;
}

//Funçao para imprimir mensagens no LCD
void imprime_no_LCD(String mens1/*, String mens2*/){
  //Limpa a tela
  lcd.clear();
  //Posiciona o cursor no canto superior esquerdo do display
  lcd.home();
  //Ativa a rolagem automática do LCD
  lcd.autoscroll();
  //Envia o texto entre recebido como parametro para o LCD
  lcd.print(mens1);
  //Posiciona o cursor na coluna 1, linha 0;
  //lcd.setCursor(1, 1);
  //lcd.print(mens2);
}
