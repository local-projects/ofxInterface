//
//  NotificationCenter.h
//  ofxInterface_exampleAdvanced
//
//  Created by Oriol Ferrer Mesià on 09/10/2017.
//
//

#pragma once
#include "ofMain.h"

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

 	struct Notification{
		string ID;
		shared_ptr<void> data;
	};

 so that develeopers can attach any kind of object to a Notification.


 For example:

 We want to keep track of a device being plugged in / out across different components of our app.
 ofApp owns the pointer to the device, and it's the only component that gets the events from the device.
 We need that event to be forwarded to several components of our app that are buried under several levels of
 encapsulation.

 The workflow to solve this problem with NotificationCenter would be:

 1 - Make all the observers "tune in" for notfications of type "deviceConnected"

 	//in the setup() of each of the objects that wants to be notified
 	NotificationCenter::one().addObserver(this, &MyClass::onMyNotification, "MyNotificationType");
 
 	//implement a callback for the notification
 	void MyClass::onMyNotification(NotificationCenter::Notification& n){
		if (n.data){ //access the notification data if available
			std::shared_ptr<MyNotificationData> notifData = std::static_pointer_cast<MyNotificationData>(n.data);
			if (notifData){
 			}
 		}
 	}
 
 2 - Define a custom Data Structure to attach in your notifications
 
 	struct MyNotificationData {
		string myInfo;
		float myValue;
	};

 3 - Post notifications from whichever object needs to
 	
 	//create custom data to send with the notification
	shared_ptr<MyNotificationData> data = std::make_shared<MyNotificationData>();
	data->myInfo = "My custom info";
	NotificationCenter::one().postNotification("MyNotificationType", data);

 */

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ofxInterface{

class NotificationCenter{

public:

	static NotificationCenter& one(){
		static NotificationCenter instance;
		return instance;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////

	struct Notification{
		string ID;
		shared_ptr<void> data;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////

	template <class ListenerClass>
	void addObserver(ListenerClass * who,
					 void (ListenerClass::*callback)(Notification &),
					 const string & notificationID
					 ){

		auto it = eventsByID.find(notificationID);
		if(it == eventsByID.end()){
			eventsByID[notificationID] = ofEvent<Notification>();
		}

		auto & event = eventsByID[notificationID];
		ofAddListener( event, who, callback );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////

	void postNotification(const string & notificationID){
		postNotification(notificationID, nullptr);
	}

	void postNotification(const string & notificationID, shared_ptr<void> data ){

		auto it = eventsByID.find(notificationID);
		if(it != eventsByID.end()){
			Notification n;
			n.ID = notificationID;
			n.data = data;
			ofNotifyEvent(it->second, n, this);
		}else{
			ofLogError("NotificationCenter") << " can't post notification with ID \"" << notificationID << "\" because nobody is listening!";
		}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////

protected:

	NotificationCenter(){};

	map<string, ofEvent<Notification> > eventsByID;

};

}

//#include "NotificationCenter.inl"
