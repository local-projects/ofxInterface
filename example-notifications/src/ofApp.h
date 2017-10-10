#pragma once

#include "ofMain.h"
#include "ofxInterface.h"

/// See "NotificationCenter.h" for an explanation on how NotificationCenter works

////////////////////////////////////////////////////////////////////////////////////////////////////

// Build my own notification data structure to attach to notifications

struct MyNotificationData {

	string myInfo;
	float myValue;

	MyNotificationData(){
		ofLogNotice("MyNotificationData") << "MyNotificationData()";
	}

	~MyNotificationData(){
		ofLogNotice("MyNotificationData") << "~MyNotificationData()";
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////


class MyNode : public ofxInterface::Node{
public:

	void setup(float x, float y, float w, float h){
		setPosition(x, y);
		setSize(w, h);
		/// Register myself (this) for Notifications of type "deviceConnectedNotification" and "deviceDisconnectedNotification"
		ofxInterface::NotificationCenter & notifCenter = ofxInterface::NotificationCenter::one();
		notifCenter.addObserver(this, &MyNode::onDeviceConnectedNotif, "deviceConnectedNotification");
		notifCenter.addObserver(this, &MyNode::onDeviceDisconnectedNotif, "deviceDisconnectedNotification");
	}

	void drawDebug(){
		ofxInterface::Node::drawDebug();
		ofDrawBitmapStringHighlight(deviceAvailable ? "Device Connected" : "Device Disconnected", 20, 20);
	}

	void onDeviceConnectedNotif(ofxInterface::NotificationCenter::Notification& n){
		ofLogNotice("MyNode") << "onDeviceConnectedNotif()";
		deviceAvailable = true;
		if (n.data){
			std::shared_ptr<MyNotificationData> notifData = std::static_pointer_cast<MyNotificationData>(n.data);
			ofLogNotice("MyNode") << "Notification data \"" << notifData.get()->myInfo << "\" " << notifData.get()->myValue;
		}else{
			ofLogNotice("MyNode") << "no Notification data.";
		}
	}

	void onDeviceDisconnectedNotif(ofxInterface::NotificationCenter::Notification& n){
		ofLogNotice("MyNode") << "onDeviceDisconnectedNotif()";
		deviceAvailable = false;
		if (n.data){
			std::shared_ptr<MyNotificationData> notifData = std::static_pointer_cast<MyNotificationData>(n.data);
			ofLogNotice("MyNode") << "Notification data \"" << notifData.get()->myInfo << "\" " << notifData.get()->myValue;
		}else{
			ofLogNotice("MyNode") << "no Notification data.";
		}
	}

	bool deviceAvailable = false;
};


////////////////////////////////////////////////////////////////////////////////////////////////////


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);

		ofxInterface::Node* scene;

		void populateScene();

		void connectDevice();
		void disconnectDevice();
};


////////////////////////////////////////////////////////////////////////////////////////////////////
