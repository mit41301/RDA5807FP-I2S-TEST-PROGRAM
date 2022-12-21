// RDA5807FP FM Tuner with I2S Output
// TARGET : ESP32-WROOM-32D
 #include <Wire.h>

 unsigned int freq, freqB;
 unsigned char freqH = 0, freqL = 0;
 
 void setup()
 {
   Wire.begin();

   freq  = 919;                  //  91.9 MHz
// freq  = 1040;                 // 104.0 MHz

   freqB = freq - 870;           // BAND = 0 : 87–108 MHz (US/Europe)
   freqH = freqB>>2;             // Frequency = Channel Spacing(kHz) x CHAN + 87.0 MHz
   freqL = (freqB&3)<<6;         // Shift channel selection for matching register 0x03
   
   Serial.begin(115200);         // 

   Wire.beginTransmission(0x11); // Device address 0x11 (Random access)
   Wire.write(0x02);             // Register address 0x02
   Wire.write(0xC0);             // write 0xC003 into REG 02H (soft reset, enable)
   Wire.write(0x03);             // write 0xC003 into REG 02H (soft reset, enable)
   Wire.endTransmission();       // stop condition
   
   delay(500);                   // wait 500ms

   Wire.beginTransmission(0x10); // Device address 0x10 (Sequential access)
// REG(02H)
   Wire.write(0xC0);             // write 0xC00D (RDS on etc.) into REG 02H
   Wire.write(0x0D);             // write 0xC00D (RDS on etc.) into REG 02H
// REG(03H)
   Wire.write(freqH);            // write frequency into bits 15:6, set tune bit
   Wire.write(freqL + 0x10);     // write frequency into bits 15:6, set tune bit
// REG(04H)
   Wire.write(0B00001000);       // [11] DE - [De-emphasis] 0 = 75 μs; 1 = 50 μs
   Wire.write(0B01000000);       // [06] [I2S_ENABLED] 1- Enabled
// REG(05H)
   Wire.write(0B10001000);       // bit 15 & 11 Default values
   Wire.write(0B10111011);       // [7:6] LNAP | [5:4] LNA_ICSEL 11=3.0mA VOLUME[3:0] = 1011
// REG(06H)
   Wire.write(0B00000010);       // [09] [DATA_SIGNED] 1- I2S output signed 16-bit audio data
   Wire.write(0B10000000);       // [7:4] I2S_SW_CNT[4:0] 48ksps
   
   Wire.endTransmission();
 }
 
// CAUTION: Going to SLEEP!

 void loop()
 {
   Serial.println("Going to Sleep Now");
   delay(5000);
   Serial.println("Bye!");
   Serial.flush();
   esp_deep_sleep_start();
   Serial.println("This will never be printed");
 }
