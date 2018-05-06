#define TIME_INTERMITENTE 500
#define VECES_INTERMITENTE 5
#define TIEMPO_ONOFF 500
#define TIEMPO_PULSAR 1000
#define LAT "40 19.732"
#define LONG "03 43.295"
#define INTENTOS 14
enum colores {rojo=1,verde=2,azul=3,naranja=4};
const int puls[4]={10,3,8,6};
const int led[4]={9,4,7,5};
int sieteSegAG[8]={18,11,17,15,13,16,14,12};
int secuencia[INTENTOS];

int numeros[13][8]={
{1,1,1,1,1,1,0,0},//0
{0,1,1,0,0,0,0,0},
{1,1,0,1,1,0,1,0},
{1,1,1,1,0,0,1,0},
{0,1,1,0,0,1,1,0},
{1,0,1,1,0,1,1,0},
{1,0,1,1,1,1,1,0},
{1,1,1,0,0,0,0,0},
{1,1,1,1,1,1,1,0},
{1,1,1,0,0,1,1,0},//9
{1,0,1,1,0,1,1,0},//S 10
{1,0,0,1,1,1,1,0},//E 11
{0,0,0,0,0,0,0,0}//espacio
};

void enciendeNumero(int cifra,boolean punto)
{
  if(punto)digitalWrite(sieteSegAG[7],HIGH);else digitalWrite(sieteSegAG[7],LOW);
  for(int n=0;n<7;n++)digitalWrite(sieteSegAG[n],numeros[cifra][n]);
}

void setup() {
 
  // put your setup code here, to n once:
  Serial.begin(9600);
  randomSeed(analogRead(A7));// inicia el array con los numeros aleatorios
  for(int n=0;n<4;n++){
   pinMode(led[n],OUTPUT);
   pinMode(puls[n],INPUT_PULLUP); 
  }
   for(int n=0;n<8;n++){
   pinMode(sieteSegAG[n],OUTPUT);

  }
  

}

void loop() {
  // put your main code here, to run repeatedly:

  ledsInicio();
  for(int k=0;k<INTENTOS;k++)
  {
    enciendeNumero((k>9)?k%10:k,(k>9)?true:false);
    for(int n=0;n<k;n++) { //muestra secuencia
      enciendeLed(secuencia[n]);
      delay(TIEMPO_ONOFF);
    }
    for(int n=0;n<k;n++){          
            if(!leePulsador(secuencia[n]))
            {
              ledsError();
              return;
            }
    }
    todosLeds(HIGH);
    delay(TIME_INTERMITENTE);
    todosLeds(LOW);
    delay(TIME_INTERMITENTE);
  }
  while(1){
    escribeCadena(LAT);
    delay(1000);
    escribeCadena(LONG);
    delay(1000);
  }
}

void ledsError() {
  enciendeNumero(11,false);
  for(int k=0;k<VECES_INTERMITENTE;k++){ 
    todosLeds(HIGH);
    delay(TIME_INTERMITENTE);
    todosLeds(LOW);  
    delay(TIME_INTERMITENTE);
  } 
}
void ledsInicio(){
  enciendeNumero(10,false);//pone una s en el siete segmentos
  for( int n=0;n<INTENTOS;n++) {secuencia[n]=random(4);Serial.print(secuencia[n]);}// para eso utiliza randon
  Serial.println();
  for(int k=0;k<VECES_INTERMITENTE;k++){
    for(int n=0;n<4;n++)digitalWrite(led[n],!(n%2));
    delay(TIME_INTERMITENTE);
   for(int n=0;n<4;n++) digitalWrite(led[n],(n%2));
    delay(TIME_INTERMITENTE);   
   }
  todosLeds(LOW);
  delay(TIME_INTERMITENTE); 
}

boolean leePulsador(int cual)
{
  int tiempo=TIEMPO_PULSAR/10;
  for(int k=0;k<tiempo;k++)
    for(int n=0;n<4;n++){
      if(!digitalRead(puls[n])){
        enciendeLed(n);
        if(n==cual) return true; else return false;
      }
    delay(10);
  }
  return false;
}
void enciendeLed(int n){
  digitalWrite(led[n],HIGH);
  delay(TIEMPO_ONOFF);
  digitalWrite(led[n],LOW);
  //delay(TIEMPO_ONOFF);
}
void todosLeds(boolean encendidos)
{
  for(int n=0;n<4;n++) digitalWrite(led[n],encendidos);
}
void escribeCadena(char *cadena)
{
int n=0;
int k[8];
int c;
boolean punto=false;
while(cadena[n]!=0)
{
c=cadena[n];
if (c==32)c=12;else c-=48;
if (cadena[n+1]=='.') {punto=true;n++;}else punto=false ;
enciendeNumero(c,punto);
delay(800);  
enciendeNumero(12,false                                                                                                                                                                            );
delay(100);
n++;
}
}

  


