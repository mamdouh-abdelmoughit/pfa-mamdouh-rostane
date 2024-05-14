#include <Arduino.h>
#include <PinChangeInterrupt.h>


#define MAX_BITS                  100      // max number of bits 
#define WEIGAND_WAIT_TIME         500      // time to wait for another weigand pulse.  
#define NUM_READERS               8
#define Nbr_of_AuthorizedCards    100  

unsigned char databits[NUM_READERS][MAX_BITS];    // stores all of the data bits for both readers
unsigned char bitCount[NUM_READERS];              // number of bits currently captured
unsigned char flagDone[NUM_READERS];              // goes low when data is currently being captured
unsigned long weigand_counter[NUM_READERS];        // countdown until we assume there are no more bits

unsigned long facilityCode[NUM_READERS];          // decoded facility code
unsigned long cardCode[NUM_READERS];              // decoded card code

unsigned long authorized_Cards[NUM_READERS][100]; // array of authorized cards for each reader 

int relays[]={22,24,26,28,30,32,34,31,33,35,37,39,41,43};

const int triggerType = LOW;






void ISR_INT0_Reader1() {
  databits[0][bitCount[0]] = 0;
  bitCount[0]++;
  flagDone[0] = 0;
  weigand_counter[0] = WEIGAND_WAIT_TIME;
   
}

void ISR_INT1_Reader1() {
  databits[0][bitCount[0]] = 1;
  bitCount[0]++;
  flagDone[0] = 0;
  weigand_counter[0] = WEIGAND_WAIT_TIME;
}
void ISR_ch1_0(){
  databits[1][bitCount[1]] = 0;
  bitCount[1]++;
  flagDone[1] = 0;
  weigand_counter[1] = WEIGAND_WAIT_TIME;  

}
void ISR_ch1_1()
{
  databits[1][bitCount[1]] = 1;
  bitCount[1]++;
  flagDone[1] = 0;
  weigand_counter[1] = WEIGAND_WAIT_TIME;
 
}
void ISR_ch2_0(){
  databits[2][bitCount[2]] = 0;
  bitCount[2]++;
  flagDone[2] = 0;
  weigand_counter[2] = WEIGAND_WAIT_TIME;  

}
void ISR_ch2_1()
{
  databits[2][bitCount[2]] = 1;
  bitCount[2]++;
  flagDone[2] = 0;
  weigand_counter[2] = WEIGAND_WAIT_TIME;
 
}

void ISR_ch3_0(){
  databits[3][bitCount[3]] = 0;
  bitCount[3]++;
  flagDone[3] = 0;
  weigand_counter[3] = WEIGAND_WAIT_TIME;  

}
void ISR_ch3_1()
{
  databits[3][bitCount[3]] = 1;
  bitCount[3]++;
  flagDone[3] = 0;
  weigand_counter[3] = WEIGAND_WAIT_TIME;
 
}

void ISR_ch4_0(){
  databits[4][bitCount[4]] = 0;
  bitCount[4]++;
  flagDone[4] = 0;
  weigand_counter[4] = WEIGAND_WAIT_TIME;  

}
void ISR_ch4_1()
{
  databits[4][bitCount[4]] = 1;
  bitCount[4]++;
  flagDone[4] = 0;
  weigand_counter[4] = WEIGAND_WAIT_TIME;
 
}

void ISR_ch5_0(){
  databits[5][bitCount[5]] = 0;
  bitCount[5]++;
  flagDone[5] = 0;
  weigand_counter[5] = WEIGAND_WAIT_TIME;  

}
void ISR_ch5_1()
{
  databits[5][bitCount[5]] = 1;
  bitCount[5]++;
  flagDone[5] = 0;
  weigand_counter[5] = WEIGAND_WAIT_TIME;
 
}

void ISR_ch6_0(){
  databits[6][bitCount[6]] = 0;
  bitCount[6]++;
  flagDone[6] = 0;
  weigand_counter[6] = WEIGAND_WAIT_TIME;  

}
void ISR_ch6_1()
{
  databits[6][bitCount[6]] = 1;
  bitCount[6]++;
  flagDone[6] = 0;
  weigand_counter[6] = WEIGAND_WAIT_TIME;
 
}

void ISR_ch7_0(){
  databits[7][bitCount[7]] = 0;
  bitCount[7]++;
  flagDone[7] = 0;
  weigand_counter[7] = WEIGAND_WAIT_TIME;  

}
void ISR_ch7_1()
{
  databits[7][bitCount[7]] = 1;
  bitCount[7]++;
  flagDone[7] = 0;
  weigand_counter[7] = WEIGAND_WAIT_TIME;
 
}





void initialize_cards(void){

  authorized_Cards[0][0] = 23138;
  authorized_Cards[0][1] = 45814;
  authorized_Cards[0][2] = 45100;
  authorized_Cards[0][3] = 38064;
  authorized_Cards[0][4] = 34875;
  authorized_Cards[0][5] = 62663;
  authorized_Cards[0][6] = 61001;
  authorized_Cards[1][0] = 45100;
  authorized_Cards[2][0] = 38064;
  authorized_Cards[3][0] = 28880;
  authorized_Cards[4][0] = 34875;
  authorized_Cards[5][0] = 62663;
  authorized_Cards[6][0] = 61001;
  authorized_Cards[7][0] = 23138;
  
}

bool card_is_authorized(int index, unsigned long code){
  for(int i=0; i<Nbr_of_AuthorizedCards; i++){
    if(authorized_Cards[index][i] == code){
      return true;
    }
  }
  return false;
}


void processRFIDData(int readerIndex) {
  unsigned char i;
 

  Serial.print("Reader ");
  Serial.print(readerIndex + 1);
  Serial.print(": Read ");
  Serial.print(bitCount[readerIndex]);
  Serial.print(" bits. ");

  // Decoding logic based on the number of bits received

  if (bitCount[readerIndex] == 34)
    {
      // 35 bit HID Corporate 1000 format
      // facility code = bits 2 to 14
      for (i=2; i<14; i++)
      {
         facilityCode[readerIndex] <<=1;
         facilityCode[readerIndex] |= databits[readerIndex][i];
      }
      
      // card code = bits 15 to 34
      for (i=13; i<34; i++)
      {
         cardCode[readerIndex] <<=1;
         cardCode[readerIndex] |= databits[readerIndex][i];
      }
      

    }
    else if (bitCount[readerIndex] == 26)
    {
      // standard 26 bit format
      // facility code = bits 2 to 9
      for (i=1; i<9; i++)
      {
         facilityCode[readerIndex] <<=1;
         facilityCode[readerIndex] |= databits[readerIndex][i];
      }
      
      // card code = bits 10 to 23
      for (i=9; i<25; i++)
      {
         cardCode[readerIndex] <<=1;
         cardCode[readerIndex] |= databits[readerIndex][i];
      }     
       
    }
    else {
      
      Serial.println("Unable to decode."); 
    }


      Serial.print("Reader ");
      Serial.print(readerIndex + 1);
      Serial.print("FC = ");
      Serial.print(facilityCode[readerIndex]);
      Serial.print(", CC = ");
      Serial.println(cardCode[readerIndex]);
      Serial.println();
    if ( card_is_authorized(readerIndex, cardCode[readerIndex]) )
      {
        if(readerIndex==0)
         main_reader(cardCode[0]);

        
        else if(readerIndex==1)
         reader1(cardCode[1]);
         
         else if(readerIndex==2)
         reader2(cardCode[2]);
         
         else if(readerIndex==3)
         reader3(cardCode[3]);

         
         else if(readerIndex==4)
         reader4(cardCode[4]);
         
         else if(readerIndex==5)
         reader5(cardCode[5]);

         else if(readerIndex==6)
         reader6(cardCode[6]);
         
         else if(readerIndex==7)
         reader7(cardCode[7]);

         
           // Cleanup and get ready for the next card
           bitCount[readerIndex] = 0;
           facilityCode[readerIndex] = 0;
           cardCode[readerIndex] = 0;
           for (int i = 0; i < MAX_BITS; i++) {
           databits[readerIndex][i] = 0;
          }
       } 
    else {

        Serial.println("Access denied!!!");
        Serial.println();
        // Cleanup and get ready for the next card
        bitCount[readerIndex] = 0;
        facilityCode[readerIndex] = 0;
        cardCode[readerIndex] = 0;
        for (int i = 0; i < MAX_BITS; i++) {
          databits[readerIndex][i] = 0;
        }
        }

} 


void setup()
{

  // Set up pins and interrupts for Reader 1

 
  pinMode(2, INPUT); // DATA0 Reader 1 (INT0)
  pinMode(3, INPUT); // DATA1 Reader 1 (INT1)
  attachInterrupt(0, ISR_INT0_Reader1, FALLING);
  attachInterrupt(1, ISR_INT1_Reader1, FALLING);

  pinMode(10,INPUT);
  pinMode(11,INPUT);
  attachPCINT(digitalPinToPCINT(10), ISR_ch1_0, FALLING);
  attachPCINT(digitalPinToPCINT(11), ISR_ch1_1, FALLING);


  pinMode(12,INPUT);
  pinMode(13,INPUT);
  attachPCINT(digitalPinToPCINT(12), ISR_ch2_0, FALLING);
  attachPCINT(digitalPinToPCINT(13), ISR_ch2_1, FALLING);

  pinMode(50,INPUT);
  pinMode(51,INPUT);
  attachPCINT(digitalPinToPCINT(50), ISR_ch3_0, FALLING);
  attachPCINT(digitalPinToPCINT(51), ISR_ch3_1, FALLING);

  pinMode(52,INPUT);
  pinMode(53,INPUT);
  attachPCINT(digitalPinToPCINT(52), ISR_ch4_0, FALLING);
  attachPCINT(digitalPinToPCINT(53), ISR_ch4_1, FALLING);
  
  pinMode(A8,INPUT);
  pinMode(A9,INPUT);
  attachPCINT(digitalPinToPCINT(A8), ISR_ch5_0, FALLING);
  attachPCINT(digitalPinToPCINT(A9), ISR_ch5_1, FALLING);

  pinMode(A10,INPUT);
  pinMode(A11,INPUT);
  attachPCINT(digitalPinToPCINT(A10), ISR_ch6_0, FALLING);
  attachPCINT(digitalPinToPCINT(A11), ISR_ch6_1, FALLING);

  pinMode(A12,INPUT);
  pinMode(A13,INPUT);
  attachPCINT(digitalPinToPCINT(A12), ISR_ch7_0, FALLING);
  attachPCINT(digitalPinToPCINT(A13), ISR_ch7_1, FALLING);

  for(int i=0;i<14;i++){
    pinMode(relays[i],OUTPUT);
    digitalWrite(relays[i],HIGH);
  }


  Serial.begin(9600);
  Serial.println("RFID Readers & Arduino Access Door");
  Serial.println();

 for (int i = 0; i < NUM_READERS; i++) {
    weigand_counter[i] = WEIGAND_WAIT_TIME;
    facilityCode[i] = 0;
    cardCode[i] = 0;
    
  }

  initialize_cards();

  
}




void loop()
{ 
 for (int i = 0; i < NUM_READERS; i++) {
    if (!flagDone[i]) {
      if (--weigand_counter[i] == 0)
        flagDone[i] = 1;       
    }
    
   if (bitCount[i] > 0 && flagDone[i]) {
        processRFIDData(i); // Call processRFIDData for the corresponding reader
      
    }
  }
  if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');

        if (command.startsWith("MODIFY_CARD:")) {
            // Parse the command and extract card ID and new data
            String data = command.substring(12); // Remove "MODIFY_CARD:" prefix
            int cardId = data.substring(0, data.indexOf(',')).toInt();
            String newData = data.substring(data.indexOf(',') + 1);

            // Modify authorized card data based on cardId and newData
            // Example: Update an array of authorized cards

            // Send a response back to the Node.js application if needed
            // Serial.println("Card data modified successfully");
        }
    }
}


void main_reader(unsigned long cardCode[NUM_READERS]){
             if (cardCode[readerIndex]==23138){
          digitalWrite(relays[6],LOW);
          delay(2000);
          digitalWrite(relays[6],HIGH);
           }         
          else if(cardCode[readerIndex]==45814){
          digitalWrite(relays[1],LOW);
          delay(2000);
          digitalWrite(relays[1],HIGH);
          }
          else if(cardCode[readerIndex]==45100){
          digitalWrite(relays[2],LOW);
          delay(2000);
          digitalWrite(relays[2],HIGH);
          }
          else if(cardCode[readerIndex]==38064){ 
          digitalWrite(relays[3],LOW);
          delay(2000);
          digitalWrite(relays[3],HIGH);
          }
          else if(cardCode[readerIndex]==34875){
          digitalWrite(relays[4],LOW);
          delay(2000);
          digitalWrite(relays[4],HIGH);
          }
          else if(cardCode[readerIndex]==62663){
          digitalWrite(relays[5],LOW);
          delay(2000);
          digitalWrite(relays[5],HIGH);
          }
          else if(cardCode[readerIndex]==23138){
          digitalWrite(relays[0],LOW);
          delay(2000);
          digitalWrite(relays[0],HIGH);
          }
}

void reader1(unsigned long cardCode[NUM_REDAERS]){
          digitalWrite(relays[7],LOW);
          delay(500);
          digitalWrite(relays[7],HIGH);
}
void reader2(unsigned long cardCode[NUM_REDAERS]){
          digitalWrite(relays[8],LOW);
          delay(500);
          digitalWrite(relays[8],HIGH);
}
void reader3(unsigned long cardCode[NUM_REDAERS]){
          digitalWrite(relays[9],LOW);
          delay(500);
          digitalWrite(relays[9],HIGH);
}
void reader4(unsigned long cardCode[NUM_REDAERS]){
          digitalWrite(relays[10],LOW);
          delay(500);
          digitalWrite(relays[10],HIGH);
}
void reader5(unsigned long cardCode[NUM_REDAERS]){
          digitalWrite(relays[11],LOW);
          delay(500);
          digitalWrite(relays[11],HIGH);
}
void reader6(unsigned long cardCode[NUM_REDAERS]){
          digitalWrite(relays[12],LOW);
          delay(500);
          digitalWrite(relays[13],HIGH);
}
void reader7(unsigned long cardCode[NUM_REDAERS]){
          digitalWrite(relays[13],LOW);
          delay(500);
          digitalWrite(relays[13],HIGH);
}
