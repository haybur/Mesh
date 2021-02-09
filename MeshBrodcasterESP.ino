#include "painlessMesh.h"

#define   MESH_PREFIX     "meshtest"
#define   MESH_PASSWORD   "12345678"
#define   MESH_PORT       5555

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// User stub
void sendAudio() ; // Prototype so PlatformIO doesn't complain

Task taskSendAudio( TASK_MILLISECOND * 1 , TASK_FOREVER, &sendAudio );
//Task tasksendAudio2(&sendAudio2);

int spectrumValue[16];

void sendAudio() {
  String fftData = ".";
  for (int i = 0; i < 16; i++) {
    fftData += spectrumValue[i];
    fftData += ".";
  }
  mesh.sendBroadcast( fftData );
}

void sendAudio2() {
  String fftData = ".";
  for (int i = 0; i < 16; i++) {
    fftData += spectrumValue[i];
    fftData += ".";
  }
  mesh.sendBroadcast( fftData );
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  //userScheduler.addTask( taskSendAudio );
  taskSendAudio.enable();
}

void loop() {
  receiveAudioFromTeensy();
  
  // it will run the user scheduler as well
  mesh.update();

}

void receiveAudioFromTeensy() {
  if (Serial2.available()) {
    if (Serial2.read() == 'A') {
      for (int i = 0; i < 16; i++)
        spectrumValue[i] = Serial2.read();
    }
    sendAudio2();
  }

  printSpectrum();
}

void printSpectrum() {
  for (int i = 0; i < 16; i++) {
    Serial.print(spectrumValue[i]);
    Serial.print("\t");
  }
  Serial.println();
}
