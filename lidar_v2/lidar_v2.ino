uint8_t inByte = 0;
void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
  Serial.begin(115200);
}

int start_count = 0;
uint8_t raw[1980] = {};
void loop() {

  bool got_scan = false;

  int motor_speed = 0;
  // put your main code here, to run repeatedly:

  // get incoming byte:
  Serial1.readBytes(&raw[start_count], 1);
  if (start_count == 0) {
    if (raw[start_count] == 0xFA) {
      start_count = 1;
    }
  } else if (start_count == 1) {
    if (raw[start_count] == 0xA0) {
      start_count = 0;

      // Now that entire start sequence has been found, read in the rest of the message
      got_scan = true;
      Serial1.readBytes(&raw[2], 1978);




      Serial1.readBytes(&raw[2], 1978);
      int index;
      for (uint16_t i = 0; i < 1980;  i ++) {
        if (raw[i] == 0xFA && raw[i + 1] == (0xA0 + i / 22)) { //&& CRC check
          //Serial.println("-----------------------------------");
          motor_speed += (raw[i + 3] << 8) + raw[i + 2]; //accumulate count for avg. time increment
          int rpms = (raw[i + 3] << 8 | raw[i + 2]) / 64;
          for (uint16_t j = i + 4; j < i + 20; j = j + 4) {

            // Four bytes per reading
            index = (4 * i) / 22 + (j - 4 - i) / 4;
            uint8_t byte0 = raw[j];
            uint8_t byte1 = raw[j + 1];
            uint8_t byte2 = raw[j + 2];
            uint8_t byte3 = raw[j + 3];
            // First two bits of byte1 are status flags
            // uint8_t flag1 = (byte1 & 0x80) >> 7;  // No return/max range/too low of reflectivity
            // uint8_t flag2 = (byte1 & 0x40) >> 6;  // Object too close, possible poor reading due to proximity kicks in at < 0.6m
            // Remaining bits are the range in mm
            uint16_t range = ((byte1 & 0x3F) << 8) + byte0;
            uint16_t intensity = (byte3 << 8) + byte2;
            /*
              Serial.print(byte0, HEX);
              Serial.print('\t');
              Serial.print(byte1, HEX);
              Serial.print('\t');
              Serial.print(byte2, HEX);
              Serial.print('\t');
              Serial.print(byte3, HEX);
              Serial.print('\t');
            */
            //Serial.print(rpms);
            //Serial.print('\t');
            Serial.println(String(index) + ' ' + String(range / 10.0));
            //Serial.print('\t');
            //Serial.println(range);
            //Serial.print('\t');
            //Serial.println(intensity);
          }
        }
        if ( i == 1979) {
          uint8_t raw[1980] = {};
        }
      }
    }
  }
}
