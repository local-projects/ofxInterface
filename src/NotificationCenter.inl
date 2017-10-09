//
//  NotificationCenter.cpp
//  ofxInterface_exampleAdvanced
//
//  Created by Oriol Ferrer Mesià on 09/10/2017.
//
//

#include "NotificationCenter.h"

NotificationCenter::NotificationCenter(){}


template <class ListenerClass>
void NotificationCenter::addObserver(	ListenerClass * who,
				 						void (ListenerClass::*callback)(Notification&),
				 						const string & notificationID
									 ){

	auto it = eventsByID.find(notificationID);
	if(it == eventsByID.end()){
		eventsByID[notificationID] = ofEvent<Notification>();
	}

	auto & event = eventsByID[notificationID];
	ofAddListener( event, who, callback );
}
