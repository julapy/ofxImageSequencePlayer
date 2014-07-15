//
//  ofxImageSequencePlayer.cpp
//  Created by Lukasz Karluk on 21/06/13.
//  http://julapy.com/blog
//

#include "ofxImageSequencePlayer.h"

ofxImageSequencePlayer::ofxImageSequencePlayer() {
    bLoaded = false;
    bPlaying = false;
    bPaused = false;
    bNewFrame = false;
    
    frameIndex = 0;
    frameLastIndex = 0;
    framesTotal = 0;
    position = 0;
    duration = 0;
    time = 0;
    
    fps = 30;
    speed = 1;
    loopType = OF_LOOP_NONE;
}

ofxImageSequencePlayer::~ofxImageSequencePlayer() {
    close();
}

void ofxImageSequencePlayer::setFrameRate(float value) {
    fps = value;
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
        bool bLoaded = imageLoader.loadImage(imageSequencePaths[i]);
        if(bLoaded == false) {
            continue;
        }

        ofTexture * texture = new ofTexture();
        texture->allocate(imageLoader.getPixelsRef());
        texture->loadData(imageLoader.getPixelsRef());
        imageSequenceTextures.push_back(texture);
        
        imageLoader.clear();
    }
    
    bLoaded = imageSequenceTextures.size() > 0;
    if(bLoaded == false) {
        return false;
    }
    
    frameIndex = 0;
    framesTotal = imageSequenceTextures.size();
    frameLastIndex = framesTotal - 1;
    
    duration = framesTotal / fps;
    
    bNewFrame = true;
    
    return bLoaded;
}

void ofxImageSequencePlayer::close() {
    bLoaded = false;
    bPlaying = false;
    bPaused = false;
    bNewFrame = false;
    
    frameIndex = 0;
    frameLastIndex = 0;
    framesTotal = 0;
    position = 0;
    duration = 0;
    time = 0;
    
    imageSequencePaths.clear();
    
    for(int i=0; i<imageSequenceTextures.size(); i++) {
        delete imageSequenceTextures[i];
        imageSequenceTextures[i] = NULL;
    }
    imageSequenceTextures.clear();
}

void ofxImageSequencePlayer::update() {
    bNewFrame = false;
    
    bool bUpdate = true;
    bUpdate = bUpdate && (isLoaded() == true);
    bUpdate = bUpdate && (isPlaying() == true);
    bUpdate = bUpdate && (isPaused() == false);
    
    if(bUpdate == false) {
        return;
    }
    
    // not sure adding the last frame duration is the most accurate approach here.
    // might need to rethink this.
    
    time += ofGetLastFrameTime();
    float p = ofMap(time, 0.0, duration, 0.0, 1.0, true);
    
    setPosition(p);
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

    if(getIsMovieDone()) {
        setFrame(0);
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

    time = 0;
    setPosition(0);
}

bool ofxImageSequencePlayer::isFrameNew() {
    if(isLoaded() == false) {
        return false;
    }
    
    return bNewFrame;
}

unsigned char * ofxImageSequencePlayer::getPixels() {
    if(isLoaded() == false) {
        return NULL;
    }
    
    // TODO.
    return NULL;
}

ofPixelsRef	ofxImageSequencePlayer::getPixelsRef() {
    static ofPixels dummy;
    return dummy;
}

ofTexture * ofxImageSequencePlayer::getTexture() {
    if(isLoaded() == false) {
        return NULL;
    }
    
    ofTexture * texture = imageSequenceTextures[getCurrentFrame()];
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
    bool bFinished = (bPlaying == false) && (getCurrentFrame() == frameLastIndex);
    return bFinished;
}

void ofxImageSequencePlayer::setPaused(bool bPause) {
    bPaused = bPause;
}

void ofxImageSequencePlayer::setPosition(float value) {
    int index = value * frameLastIndex;
    setFrame(index);
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
    
    int index = ofClamp(value, 0, frameLastIndex);
    if(frameIndex == index) {
        return;
    }
    frameIndex = index;
    bNewFrame = true;
    
    position = frameIndex / (float)frameLastIndex;
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
    setFrame(0);
}

void ofxImageSequencePlayer::nextFrame() {
    if(isLoaded() == false) {
        return;
    }

    int index = getCurrentFrame() + 1;
    if(index > frameLastIndex) {
        if(loopType == OF_LOOP_NONE) {
            index = frameLastIndex;
            if(isPlaying()) {
                stop();
            }
        } else if(loopType == OF_LOOP_NORMAL) {
            index = 0;
        } else if(loopType == OF_LOOP_PALINDROME) {
            // TODO.
        }
    }
    
    setFrame(index);
}

void ofxImageSequencePlayer::previousFrame() {
    if(isLoaded() == false) {
        return;
    }
    
    int index = getCurrentFrame() - 1;
    if(index < 0) {
        if(loopType == OF_LOOP_NONE) {
            index = 0;
        } else if(loopType == OF_LOOP_NORMAL) {
            index = frameLastIndex;
        } else if(loopType == OF_LOOP_PALINDROME) {
            // TODO.
        }
    }
    
    setFrame(index);
}
