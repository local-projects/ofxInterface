//
//  NotificationCenter.h
//  ofxInterface_exampleAdvanced
//
//  Created by Oriol Ferrer Mesià on 09/10/2017.
//
//

#pragma once
#include "ofMain.h"

namespace ofxInterface{

class NotificationCenter{

public:

	static NotificationCenter& one(){
		static NotificationCenter instance;
		return instance;
	}

	struct NotificationData{

	};

	struct Notification{
		string ID;
		shared_ptr<void> data;
		~Notification(){
		}
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
