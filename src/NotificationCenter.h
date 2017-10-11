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

 The idea behind this is to have a "Mailbox" anyone can refer to to send and receive messages.
 To receive notifications, one must first register itself to do so.
 There are different notification "channels" - one can "post to" and/or "subscribe for" to a certain
 channel/type of notifications. Notifications are distinguished by an ID(string) the developer supplies.
 This is pretty much a copy of NSNotification found in the Cocoa interfaces.

 For a Notification to happen, we need at least 2 entities:

 1 - the Notification Poster : someone who posts a notification on a certain channel
 2 - a Notification Observer: someone who is listenting for notifications in a certain channel

 Notifications include an ID:

 	struct Notification{
		string ID;
	};

 Developers who want to send extra data inside their notifications should make their own Notifications 
 by subclassing
 
 	struct MyNotification : publicNotificationCenter::Notification{
 		//your stuff here
 	};

 For example:

 We want to keep track of a device being plugged in / out across different components of our app.
 ofApp owns the pointer to the device, and it's the only component that gets the events from the device.
 We need that event to be forwarded to several components of our app that are buried under several levels of
 encapsulation.

 The workflow to solve this problem with NotificationCenter would be:

 1 - Define a custom Data Structure to attach in your notifications
 
 	 struct MyNotificationData {
		string myInfo;
		float myValue;
	 };

 2 - Make all the observers "tune in" for notfications of your desired ID ("MyNotificationType")

 	 //in the setup() of each of the objects that wants to be notified for this Notification ID
 	 NotificationCenter::one().addObserver(this, &MyClass::onMyNotification, "MyNotificationType");
 
 	 //implement a callback for the notification
 	 void MyClass::onMyNotification(NotificationCenter::Notification & n){
 		//obtain your custom notification data by downcasting
		try{
			MyNotificationData & n = static_cast<MyNotificationData&>(n);
		}catch(const std::bad_cast& e){
			ofLogError() << "type conversion error!";
		}
 	 }

 3 - Post notifications from whichever object needs to
 	
 	 //create custom data to send with the notification
	 MyNotificationData data;
	 NotificationCenter::one().postNotification("MyNotificationType", data);
 
 4 - When an object does not want to listen to more notifications (or it will be deleted) it must remove
 	 itself from the observer list

 	NotificationCenter::one().removeObserver(this, &MyClass::onMyNotification, "MyNotificationType");

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

	struct Notification{ //you may want to subclass this to attach custom content to your notifications...
		string ID;
		virtual ~Notification(){}; //make Notification a polymorphic object
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

	template <class ListenerClass>
	void removeObserver(ListenerClass * who,
						void (ListenerClass::*callback)(Notification &),
						const string & notificationID){

		auto it = eventsByID.find(notificationID);
		if(it != eventsByID.end()){
			auto & event = it->second;
			ofRemoveListener( event, who, callback );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////

	void postNotification(const string & notificationID){
		Notification notif;
		postNotification(notificationID, notif);
	}

	void postNotification(const string & notificationID, Notification & notif ){

		auto it = eventsByID.find(notificationID);
		if(it != eventsByID.end()){
			notif.ID = notificationID;
			ofNotifyEvent(it->second, notif, this);
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
