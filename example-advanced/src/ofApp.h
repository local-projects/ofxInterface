#pragma once

#include "ofMain.h"
#include "ofxInterface.h"
#include "ScreenObject.h"
#include "AppManager.h"

#include "NotificationCenter.h"


struct MyData : public ofxInterface::NotificationCenter::Notification{
	public :
		MyData(){
			ofLogNotice("MyData") << "MyData()";

		}
		~MyData(){
			ofLogNotice("MyData") << "~MyData()";

		}
};

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    //ofxInterface nodes
   
    ofxInterface::Node* scene;
    AppManager * appMan;
    bool bShowDebug;

	void onTestNotification(ofxInterface::NotificationCenter::Notification& n);
};
