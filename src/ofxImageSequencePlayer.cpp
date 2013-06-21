//
//  ofxImageSequencePlayer.cpp
//  Created by Lukasz Karluk on 21/06/13.
//  http://julapy.com/blog
//

#include "ofxImageSequencePlayer.h"

ofxImageSequencePlayer::ofxImageSequencePlayer() {
    close();
}

ofxImageSequencePlayer::~ofxImageSequencePlayer() {
    close();
}

bool ofxImageSequencePlayer::loadMovie(string name) {
    close();
    
    // curretly this is as basic as it gets.
    // it loads all files from a directory.
    // TODO :: check if files in directory are a sequence.
    // TODO :: if a single file is passed, work out if its part of a sequence and load that sequence.
    
    ofDirectory dir(name);
    int numOfFiles = dir.listDir();
    for(int i=0; i<numOfFiles; i++) {
        imageSequencePaths.push_back(dir.getPath(i));
    }
    
    bLoaded = imageSequencePaths.size() > 0;
    if(bLoaded == false) {
        return false;
    }
    
    ofImage imageLoader;
    for(int i=0; i<imageSequencePaths.size(); i++) {
        imageLoader.loadImage(imageSequencePaths[i]);

        ofTexture * texture = new ofTexture();
        texture->loadData(imageLoader.getPixelsRef());
        imageSequenceTextures.push_back(texture);
    }
    
    frameIndex = 0;
    framesTotal = imageSequenceTextures.size();
    
    return bLoaded;
}

void ofxImageSequencePlayer::close() {
    bLoaded = false;
    bPlaying = false;
    bPaused = false;
    frameIndex = 0;
    framesTotal = 0;
    position = 0;
    speed = 1;
    duration = 0;
    loopType = OF_LOOP_NONE;
    
    imageSequencePaths.clear();
    
    for(int i=0; i<imageSequenceTextures.size(); i++) {
        delete imageSequenceTextures[i];
        imageSequenceTextures[i] = NULL;
    }
    imageSequenceTextures.clear();
}

void ofxImageSequencePlayer::update() {
    if(isLoaded() == false) {
        return;
    }

    nextFrame();
}

bool ofxImageSequencePlayer::setPixelFormat(ofPixelFormat pixelFormat) {
    if(isLoaded() == false) {
        return false;
    }
    
    // TODO.
    return false;
}

ofPixelFormat ofxImageSequencePlayer::getPixelFormat() {
    if(isLoaded() == false) {
        return OF_PIXELS_RGBA;
    }
    
    // TODO.
    return OF_PIXELS_RGBA;
}

void ofxImageSequencePlayer::play() {
    if(isLoaded() == false) {
        return;
    }

    bPlaying = true;
    bPaused = false;
}

void ofxImageSequencePlayer::stop() {
    if(isLoaded() == false) {
        return;
    }
    
    bPlaying = false;
    bPaused = false;
}

bool ofxImageSequencePlayer::isFrameNew() {
    if(isLoaded() == false) {
        return false;
    }
    
    // TODO.
    return false;
}

unsigned char * ofxImageSequencePlayer::getPixels() {
    if(isLoaded() == false) {
        return NULL;
    }
    
    // TODO.
    return NULL;
}

ofPixelsRef	ofxImageSequencePlayer::getPixelsRef() {
    if(isLoaded() == false) {
        return NULL;
    }

    // TODO.
    return NULL;
}

ofTexture * ofxImageSequencePlayer::getTexture() {
    if(isLoaded() == false) {
        return NULL;
    }
    
    ofTexture * texture = imageSequenceTextures[frameIndex];
    return texture;
}

float ofxImageSequencePlayer::getWidth() {
    if(isLoaded() == false) {
        return 0;
    }
    
    int w = getTexture()->getWidth();
    return w;
}

float ofxImageSequencePlayer::getHeight() {
    if(isLoaded() == false) {
        return 0;
    }
    
    int h = getTexture()->getHeight();
    return h;
}

bool ofxImageSequencePlayer::isPaused() {
    return bPaused;
}

bool ofxImageSequencePlayer::isLoaded() {
    return bLoaded;
}

bool ofxImageSequencePlayer::isPlaying() {
    return bPlaying;
}

float ofxImageSequencePlayer::getPosition() {
    return position;
}

float ofxImageSequencePlayer::getSpeed() {
    return speed;
}

float ofxImageSequencePlayer::getDuration() {
    return duration;
}

bool ofxImageSequencePlayer::getIsMovieDone() {
    // TODO
    return false;
}

void ofxImageSequencePlayer::setPaused(bool bPause) {
    bPaused = bPause;
}

void ofxImageSequencePlayer::setPosition(float value) {
    position = value;
    frameIndex = position * (framesTotal - 1);
}

void ofxImageSequencePlayer::setVolume(float volume) {
    // not supported.
}

void ofxImageSequencePlayer::setLoopState(ofLoopType value) {
    loopType = value;
}

void ofxImageSequencePlayer::setSpeed(float value) {
    speed = value;
}

void ofxImageSequencePlayer::setFrame(int value) {
    if(isLoaded() == false) {
        return;
    }
    
    frameIndex = ofClamp(value, 0, framesTotal-1);
}

int	ofxImageSequencePlayer::getCurrentFrame() {
    return frameIndex;
}

int	ofxImageSequencePlayer::getTotalNumFrames() {
    return framesTotal;
}

ofLoopType ofxImageSequencePlayer::getLoopState() {
    return loopType;
}

void ofxImageSequencePlayer::firstFrame() {
    frameIndex = 0;
}

void ofxImageSequencePlayer::nextFrame() {
    if(isLoaded() == false) {
        return;
    }

    if(++frameIndex > framesTotal - 1) {
        if(loopType == OF_LOOP_NONE) {
            frameIndex = framesTotal - 1;
        } else if(loopType == OF_LOOP_NORMAL) {
            frameIndex = 0;
        } else if(loopType == OF_LOOP_PALINDROME) {
            // TODO.
        }
    }
}

void ofxImageSequencePlayer::previousFrame() {
    if(--frameIndex < 0) {
        if(loopType == OF_LOOP_NONE) {
            frameIndex = 0;
        } else if(loopType == OF_LOOP_NORMAL) {
            frameIndex = framesTotal - 1;
        } else if(loopType == OF_LOOP_PALINDROME) {
            // TODO.
        }
    }
}
