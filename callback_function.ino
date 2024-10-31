//-----------------------Callback function-------------------------------------//

void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  
//-------------------------------------Getting config data---------------------//
StaticJsonDocument<128> myJsonDoc;
deserializeJson( myJsonDoc,payload,length );
const char* newdid =  myJsonDoc["did"];



if(strcmp(did,newdid) == 0){

//WRITE CODE
  
}

else{
    myJsonDoc["status"] = "failed";   
    char buffer[128];
    size_t n = serializeJson(myJsonDoc, buffer);
    Serial.print("Published:");
    P_R_I_N_T(client.publish(resTopic, buffer, n));
}

     
}//Callback ends
