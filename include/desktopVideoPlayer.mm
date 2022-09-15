//
//  DesktopVideoPlayer.cpp
//  explottens
//
//  Created by Bilal Mirza on 31/07/2018.
//
//

#include "DesktopVideoPlayer.h"
#include <AVKit/AVKit.h>

//@interface UIDesktopViewWrapper : NSObject
//
//@property (strong, nonatomic) AVPlayer *avPlayer;
//@property (strong, nonatomic) NSView *containerView;
//-(void) play;
//-(void) pause;
//-(void) resume;
//-(void) stop;
//
//-(id) init:(void*) videoPlayer;
//
//@end
//
//@implementation UIDesktopViewWrapper
//-(id)init:(void*)videoPlayer
//{
//    if (self = [super init]) {
//
//
//    }
//
//    return self;
//}
//
//@end

NSView *containerView;
AVPlayer *avPlayer;

USING_NS_CC;


DesktopVideoPlayer::~DesktopVideoPlayer()
{
    [avPlayer pause];
    [containerView removeFromSuperview];
    avPlayer = nil;
  
}

DesktopVideoPlayer* DesktopVideoPlayer::create()
{
    DesktopVideoPlayer *node = new (std::nothrow)DesktopVideoPlayer();
    if(node && node->init())
    {
        node->autorelease();
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}


bool DesktopVideoPlayer::init()
{

    NSWindow* nsWindow = Director::getInstance()->getOpenGLView()->getCocoaWindow();
    NSView* view = nsWindow.contentView;
    containerView = [[NSView alloc] initWithFrame:view.frame];
    
    this->scheduleUpdate();
    
    return true;
}

void DesktopVideoPlayer::setFileName(const std::string &videoPath)
{
    
    _fileName = videoPath;
    NSWindow* nsWindow = Director::getInstance()->getOpenGLView()->getCocoaWindow();
    NSView* view = nsWindow.contentView;
    
    NSString *str = [NSString stringWithCString:videoPath.c_str()
                                       encoding:[NSString defaultCStringEncoding]];
    NSURL *videoFileURL = [NSURL fileURLWithPath: str];
    avPlayer = [[AVPlayer alloc] initWithURL:videoFileURL];
    AVPlayerLayer *playerLayer = [AVPlayerLayer playerLayerWithPlayer: avPlayer];
    [containerView setWantsLayer:YES];
    
    playerLayer.frame = view.frame;
    [containerView.layer addSublayer: playerLayer];
    [playerLayer setVideoGravity: AVLayerVideoGravityResizeAspect];
    [playerLayer setNeedsDisplay];
    
    [containerView needsDisplay];
    [view addSubview:containerView];
    
}

void DesktopVideoPlayer::playVideo()
{
    if(_fileName != "")
    {
        [avPlayer play];
    }
}

void DesktopVideoPlayer::stopVideo()
{
    if(_fileName != "")
    {
        [avPlayer seekToTime:CMTimeMake(0, 1)];
        [avPlayer pause];
    }
}
void DesktopVideoPlayer::pauseVideo()
{
    if(_fileName != "")
    {
        [avPlayer pause];
    }
}
void DesktopVideoPlayer::resumeVideo()
{
    if(_fileName != "")
    {
        [avPlayer play];
    }
}

void DesktopVideoPlayer::seekTo(float sec)
{
    [avPlayer seekToTime:CMTimeMake(sec, 1)];

}

void DesktopVideoPlayer::setPlayerVisible(const bool show)
{
    if(show){
        [containerView setHidden:NO];
    }
    else{
        [containerView setHidden:YES];
    }
    
    
}

void DesktopVideoPlayer::update(float)
{
    if(_fileName != "")
    {

        if(CMTimeGetSeconds(avPlayer.currentTime) == CMTimeGetSeconds(avPlayer.currentItem.duration))
        {
            this->unscheduleUpdate();
            if(callback) {
                callback();
            }
        }
        
    }
    
}

void DesktopVideoPlayer::RemoveSelf()
{
    
}
