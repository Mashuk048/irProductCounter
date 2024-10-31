
void initOled(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);

}



void showText(){

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 5);
  // Display static text
  display.println("DID: KC101");

  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print("A: ");
  display.println(cA);
  display.print("B: ");
  display.println(cB);
  display.display();
//  delay(1000);
}
