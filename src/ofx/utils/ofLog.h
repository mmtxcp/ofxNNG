#pragma once
#include <iostream>
#include "ofx/utils/ofUtils.h"
namespace ofxNNG {
#define ofLogFatalError(module) std::cerr << std::endl << ofGetTimestampString() <<" "<<  module <<" "
#define ofLogError(module) std::cerr << std::endl << ofGetTimestampString() <<" "<< module <<" "
#define ofLogWarning(module) std::cout<< std::endl <<   ofGetTimestampString() <<" "<<module <<" "
#define ofLogNotice(module) std::cout << std::endl << ofGetTimestampString() <<" "<< module <<" "
#define ofLogVerbose(module) std::cout << std::endl <<  ofGetTimestampString() <<" "<<module <<" "
}
