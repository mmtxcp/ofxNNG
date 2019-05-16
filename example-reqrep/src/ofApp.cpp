#include "ofApp.h"

using namespace ofx::nng;
//--------------------------------------------------------------
void ofApp::setup(){
	Rep::Settings reps;
	reps.url = "inproc://test";
	rep_.setup(reps);

	Req::Settings reqs;
	reqs.url = "inproc://test";
	req_.setup(reqs);
	ofAddListener(req_.onReply, this, &ofApp::onReply);
}

void ofApp::onReply(nng_msg &msg)
{
	auto body = nng_msg_body(&msg);
	auto len = nng_msg_len(&msg);
	ofLogNotice() << std::string((char*)body, len);
}


//--------------------------------------------------------------
void ofApp::update(){
	while(rep_.hasWaitingRequest()) {
		ofBuffer buffer;
		auto ctx = rep_.getNextRequest(buffer);
		buffer.append(":reply");
		rep_.reply(ctx, buffer);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	std::string msg = "pressed:" + ofToString((char)key);
	req_.send(const_cast<char*>(msg.data()), msg.length());
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
