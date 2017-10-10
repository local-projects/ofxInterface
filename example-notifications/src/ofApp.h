#pragma once

#include "ofMain.h"
#include "ofxInterface.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 
 NotificationCenter proposal
 
 The idea behind this is to have a "Mailbox" anyone can refer to to send and recieve messages.
 To receive notifications, one must first register itself to do so. 
 There are different notification "channels" - one can "post to" and/or "subscribe for" to a certain 
 channel/type of notifications. Notificatons are distinguished by an ID(string) the developer supplies.
 
 For a Notification to happen, we need at least 2 entities:
 
 	1 - the Notification Poster : someone who posts a notification on a certain channel
 	2 - a Notification Observer: someone who is listenting for notifications in a certain channel
 
 Notifications include and ID and a custom object:
 
 	shared_ptr<void> data;
 
 so that develeopers can attach any kind of object to a Notification.
 


 For example: 
 
 We want to keep track of a device being plugged in / out across different components of our app.
 ofApp owns the pointer to the device, and it's the only component that gets the events from the device.
 We need that event to be forwarded to several components of our app that are buried under several levels of
 encapsulation.

 The workflow to solve this problem with NotificationCenter would be:
 
 1 - make all the observers "tune in" for notfications of type "deviceConnected"
 
 	class MyObject{

 	};



 */

////////////////////////////////////////////////////////////////////////////////////////////////////


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
		ofxInterface::NotificationCenter::one().addObserver(this, &MyNode::onDeviceConnectedNotif, "deviceConnectedNotification");
		ofxInterface::NotificationCenter::one().addObserver(this, &MyNode::onDeviceDisconnectedNotif, "deviceDisconnectedNotification");
	}

	void drawDebug(){
		ofxInterface::Node::drawDebug();
		ofDrawBitmapStringHighlight(deviceAvailable ? "Device Connected" : "Device Disconnected", 20, 20);
	}

	void onDeviceConnectedNotif(ofxInterface::NotificationCenter::Notification& n){
		ofLogNotice("MyNode") << "onDeviceConnectedNotif";
		deviceAvailable = true;
		if (n.data.get()){
			std::shared_ptr<MyNotificationData> notifData = std::static_pointer_cast<MyNotificationData>(n.data);
			if (data){
				ofLogNotice("MyNode") << "Notification data " << notifData.get()->myInfo;
			}
		}
	}

	void onDeviceDisconnectedNotif(ofxInterface::NotificationCenter::Notification& n){
		ofLogNotice("MyNode") << "onDeviceDisconnectedNotif";
		deviceAvailable = false;
		if (n.data.get()){
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
