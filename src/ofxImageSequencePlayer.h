//
//  ofxImageSequencePlayer.h
//  Created by Lukasz Karluk on 21/06/13.
//  http://julapy.com/blog
//

#pragma once

#include "ofMain.h"

class ofxImageSequencePlayer : public ofBaseVideoPlayer {

public:
    
    ofxImageSequencePlayer();
    ~ofxImageSequencePlayer();
    
    void setFrameRate(float value);
    
    bool loadMovie(string name);
    bool loadMovie(const vector<string> & imagePaths);
    void close();
    void update();
	
	bool setPixelFormat(ofPixelFormat pixelFormat);
	ofPixelFormat getPixelFormat();
	
    void play();
    void stop();
	
    bool isFrameNew();
    unsigned char * getPixels();
    ofPixelsRef	getPixelsRef();
    ofTexture *	getTexture();
	
    float getWidth();
    float getHeight();
	
    bool isPaused();
    bool isLoaded();
    bool isPlaying();
	
    float getPosition();
    float getSpeed();
    float getDuration();
    bool getIsMovieDone();
	
    void setPaused(bool bPause);
    void setPosition(float pct);
    void setVolume(float volume); // 0..1
    void setLoopState(ofLoopType state);
    void setSpeed(float speed);
    void setFrame(int frame);  // frame 0 = first frame...
	
    int	getCurrentFrame();
    int	getTotalNumFrames();
    ofLoopType getLoopState();
	
    void firstFrame();
    void nextFrame();
    void previousFrame();
    
protected:
    
    vector<string> imageSequencePaths;
    vector<ofTexture *> imageSequenceTextures;
    
    bool bLoaded;
    bool bPlaying;
    bool bPaused;
    bool bNewFrame;
    
    int frameIndex;
    int frameLastIndex;
    int framesTotal;
    float position;
    float duration;
    float time;
    
    float fps;
    float speed;
    ofLoopType loopType;
};