#include "ofApp.h"

using namespace ofxInterface;


void ofApp::setup(){

	ofBackground(22);

	scene = new Node();
	scene->setSize(ofGetWidth(), ofGetHeight());
	scene->setName("Scene");

	TouchManager::one().setup(scene);

	populateScene();
}


void ofApp::populateScene(){

	float sizeX = 260;
	float sizeY = 50;
	float pad = 30;
	int perRow = 3;

	for (int i=0; i < perRow * 3; i++) {

		// setup them in a grid
		float x = 40 + (i%perRow)*(sizeX+pad);
		float y = 80 + (i/perRow)*(sizeY+pad);

		// create a ButtonExample node
		MyNode *btn = new MyNode();
		btn->setup(x, y, sizeX, sizeY);
		btn->setName("MyNode" + ofToString(i));

		// add it to the scene
		scene->addChild(btn);
	}
}


void ofApp::update(){

	float dt = 1.0 / 60.0;
	TouchManager::one().update(dt);
	scene->updateSubtree(dt);
}


void ofApp::draw(){
	scene->render();
	scene->renderDebug();

	ofDrawBitmapString("Press '1' to connect device. Press '2' to disconnect device.", 20, ofGetHeight() - 20);
}


void ofApp::connectDevice(){

	ofLogNotice("ofApp") << "## Device Connected! ######################################################";

	//create custom data to send with the notification
	MyNotificationData data;
	data.myInfo = "device #";
	data.myValue = ofRandom(1);

	//post a notification of type "deviceConnectedNotification", attach custom data to it
	ofxInterface::NotificationCenter::one().postNotification("deviceConnectedNotification", data);
}


void ofApp::disconnectDevice(){

	ofLogNotice("ofApp") << "## Device Disconnected! ###################################################";

	//post a notification of type "deviceConnectedNotification", with no data attached.
	ofxInterface::NotificationCenter::one().postNotification("deviceDisconnectedNotification");
}


void ofApp::keyPressed(int key){

	switch (key) {
		case '1':
			connectDevice(); // pretend the device was connected
			break;
		case '2':
			disconnectDevice(); // pretend the device was disconnected
			break;
	}
}


void ofApp::keyReleased(int key){
}


void ofApp::mouseMoved(int x, int y ){
}


void ofApp::mouseDragged(int x, int y, int button){
	TouchManager::one().touchMove(button, ofVec2f(x, y));
}


void ofApp::mousePressed(int x, int y, int button){
	TouchManager::one().touchDown(button, ofVec2f(x, y));
}


void ofApp::mouseReleased(int x, int y, int button){
	TouchManager::one().touchUp(button, ofVec2f(x, y));
}
