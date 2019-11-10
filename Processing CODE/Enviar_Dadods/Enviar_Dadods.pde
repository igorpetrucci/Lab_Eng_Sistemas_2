import processing.serial.*;

Serial porta;

void setup(){
  //Exibir as portas disponiveis 
  println(Serial.list());
  
  //Abrir porta ultilizada pelo Arduino
  porta = new Serial(this,Serial.list()[0],115200);
  
  //tamanho da TELA
  size(500,500);
  
  //alinhamento do texto
  textAlign(CENTER);
  
}

void draw(){
  //atualiza o fundo a cada iteração
  background(0,0,0);
  //Verificar se há bytes a serem lidos
  if (porta.available() > 0){
  // Ler String recebida
  String batimento = porta.readStringUntil('\n');
 // String umidade = porta.readStringUntil('\n');
  
  int intBatimento;
 // int intUmidade=0;
  
  //Transformar a minha string para in
  intBatimento = Integer.parseInt(batimento.trim());
 // intUmidade = Integer.parseInt(umidade.trim());
  
  
  //Exibir na saida padrao o dado recebido
  println(intBatimento);
  
  text(intBatimento,255,50);
  text("\n",255,50);
  }
  // delay(1000);

}
