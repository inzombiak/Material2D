//
//  GameScene.m
//  BeatHeroes-iOS
//
//  Created by Zareh on 5/7/16.
//  Copyright (c) 2016 Zareh. All rights reserved.
//
#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif
#include <iostream>
// ----------------------------------------------------------------------------
// This makes relative paths work in C++ in Xcode by changing directory to the Resources folder inside the .app bundle

#import "GameScene.h"
//#import "CircularGestureRecognizer.h"

@implementation GameScene

//- (void)doSingleTap:(UITapGestureRecognizer * )recognizer
//{
//    g.Tap();
//}
//
//- (void)doDoubleTap:(UITapGestureRecognizer *)recognizer
//{
//    g.DoubleTap();
//}
//
//-(void)doSwipe:(UISwipeGestureRecognizer *)recognizer
//{
//    double direction;
//    if(recognizer.direction == UISwipeGestureRecognizerDirectionRight)
//        direction = 0;
//    else if(recognizer.direction == UISwipeGestureRecognizerDirectionDown)
//        direction = M_PI_2;
//    else if(recognizer.direction == UISwipeGestureRecognizerDirectionLeft)
//        direction = M_PI;
//    else if(recognizer.direction == UISwipeGestureRecognizerDirectionUp)
//        direction = 3*M_PI_2;
//    g.Swipe(direction);
//}
//
//-(void) doCircularSwipe:(CircularGestureRecognizer *) recognizer
//{
//    g.SwipeCurve(recognizer.isClockwise);
//}

-(void)didMoveToView:(SKView *)view
{
    
//    UITapGestureRecognizer *singleTap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(doSingleTap:)];
//    singleTap.numberOfTapsRequired = 1;
//    
//    UITapGestureRecognizer *doubleTap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(doDoubleTap:)];
//    doubleTap.numberOfTapsRequired = 2;
//    
//    UISwipeGestureRecognizer *swipeUp = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(doSwipe:)];
//    [swipeUp setDirection:UISwipeGestureRecognizerDirectionUp];
//    
//    UISwipeGestureRecognizer *swipeDown = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(doSwipe:)];
//    [swipeDown setDirection:UISwipeGestureRecognizerDirectionDown];
//    
//    UISwipeGestureRecognizer *swipeLeft = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(doSwipe:)];
//    [swipeLeft setDirection:UISwipeGestureRecognizerDirectionLeft];
//    
//    UISwipeGestureRecognizer *swipeRight = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(doSwipe:)];
//    [swipeRight setDirection:UISwipeGestureRecognizerDirectionRight];
//    
//    CircularGestureRecognizer *circle = [[CircularGestureRecognizer alloc] initWithTarget:self action:@selector(doCircularSwipe:)];
//    
//    [singleTap requireGestureRecognizerToFail:swipeUp];
//    [singleTap requireGestureRecognizerToFail:swipeDown];
//    [singleTap requireGestureRecognizerToFail:swipeLeft];
//    [singleTap requireGestureRecognizerToFail:swipeRight];
//    [singleTap requireGestureRecognizerToFail:circle];
//    [singleTap requireGestureRecognizerToFail:doubleTap];
//    [[self view] addGestureRecognizer:singleTap];
//    
//    [doubleTap requireGestureRecognizerToFail:swipeUp];
//    [doubleTap requireGestureRecognizerToFail:swipeDown];
//    [doubleTap requireGestureRecognizerToFail:swipeLeft];
//    [doubleTap requireGestureRecognizerToFail:swipeRight];
//    [doubleTap requireGestureRecognizerToFail:circle];
//    [[self view] addGestureRecognizer:doubleTap];
//    
//    [swipeUp requireGestureRecognizerToFail:circle];
//    [[self view] addGestureRecognizer:swipeUp];
//    
//    [swipeDown requireGestureRecognizerToFail:circle];
//    [[self view] addGestureRecognizer:swipeDown];
//    
//    [swipeLeft requireGestureRecognizerToFail:circle];
//    [[self view] addGestureRecognizer:swipeLeft];
//    
//    [swipeRight requireGestureRecognizerToFail:circle];
//    [[self view] addGestureRecognizer:swipeRight];
//    
//    [[self view] addGestureRecognizer:circle];
    
#ifdef __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
    }
    CFRelease(resourcesURL);
    
    chdir(path);
    std::cout << "Current Path: " << path << std::endl;
#endif
    CGSize screenSize = [UIScreen mainScreen].bounds.size;

    CGRect pos;
    pos.size.width = 48;
    pos.size.height = 48;
    
    pos.origin.x = 24;
    pos.origin.y =  95;
    SKShapeNode* mute = [SKShapeNode shapeNodeWithRect:pos];
    [mute setFillColor:[UIColor blueColor]];
    mute.alpha = 1;
    mute.name = @"iceButton";//how the node is identified later0
    [self addChild:mute];
    
    pos.origin.x =  96;
    pos.origin.y = 95;
    SKShapeNode* controls = [SKShapeNode shapeNodeWithRect:pos];
    [controls setFillColor:[UIColor greenColor]];
    controls.name = @"stickyButton";//how the node is identified later0
    controls.alpha = 1;
    [self addChild:controls];
    
    pos.origin.x =  168;
    pos.origin.y = 95;
    SKShapeNode* beat = [SKShapeNode shapeNodeWithRect:pos];
    [beat setFillColor:[UIColor redColor]];
    beat.name = @"fireButton";//how the node is identified later0
    beat.alpha = 1;
    [self addChild:beat];
    
    pos.origin.x =  368;
    pos.origin.y = 95;
    SKShapeNode* reset = [SKShapeNode shapeNodeWithRect:pos];
    [reset setFillColor:[UIColor blackColor]];
    reset.name = @"resetButton";//how the node is identified later0
    reset.alpha = 1;
    [self addChild:reset];
    
    g.SetScreenSize(screenSize.width,screenSize.height);
    g.Start(self);
}


-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch* touch = [touches anyObject];
    CGPoint location = [touch locationInNode:self];
    SKNode *node = [self nodeAtPoint:location];
    if([node.name isEqualToString:@"fireButton"])
    {
        g.SetMaterial(MaterialType::Fire);
        [NSObject cancelPreviousPerformRequestsWithTarget:self];
        return;
    }
    else if ([node.name isEqualToString:@"iceButton"])
    {
        g.SetMaterial(MaterialType::Ice);
        [NSObject cancelPreviousPerformRequestsWithTarget:self];
        return;
    }
    else if ([node.name isEqualToString:@"stickyButton"])
    {
        g.SetMaterial(MaterialType::Sticky);
        [NSObject cancelPreviousPerformRequestsWithTarget:self];
        return;
    }
    else if ([node.name isEqualToString:@"resetButton"])
    {
        g.Restart();
        [NSObject cancelPreviousPerformRequestsWithTarget:self];
        CGRect pos;
        pos.size.width = 48;
        pos.size.height = 48;
        
        pos.origin.x = 24;
        pos.origin.y =  95;
        SKShapeNode* mute = [SKShapeNode shapeNodeWithRect:pos];
        [mute setFillColor:[UIColor blueColor]];
        mute.alpha = 1;
        mute.name = @"iceButton";//how the node is identified later0
        [self addChild:mute];
        
        pos.origin.x =  96;
        pos.origin.y = 95;
        SKShapeNode* controls = [SKShapeNode shapeNodeWithRect:pos];
        [controls setFillColor:[UIColor greenColor]];
        controls.name = @"stickyButton";//how the node is identified later0
        controls.alpha = 1;
        [self addChild:controls];
        
        pos.origin.x =  168;
        pos.origin.y = 95;
        SKShapeNode* beat = [SKShapeNode shapeNodeWithRect:pos];
        [beat setFillColor:[UIColor redColor]];
        beat.name = @"fireButton";//how the node is identified later0
        beat.alpha = 1;
        [self addChild:beat];
        
        pos.origin.x =  368;
        pos.origin.y = 95;
        SKShapeNode* reset = [SKShapeNode shapeNodeWithRect:pos];
        [reset setFillColor:[UIColor blackColor]];
        reset.name = @"resetButton";//how the node is identified later0
        reset.alpha = 1;
        [self addChild:reset];
        return;
    }
   // location = [touch locationInView:touch.view];
    if (touch.tapCount == 2)
    {
        //This will cancel the singleTap action
        [NSObject cancelPreviousPerformRequestsWithTarget:self];
    }
 
    glm::vec2 pos = glm::vec2(location.x, location.y);
    
    g.MousePressEvent(pos);
}

-(void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    UITouch* touch = [touches anyObject];
    CGPoint location = [touch locationInView:self.view];
    glm::vec2 pos = glm::vec2(location.x, location.y);
    
    g.MouseMoveEvent(pos);
}

-(void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    UITouch* touch = [touches anyObject];
    CGPoint location = [touch locationInView:self.view];
    glm::vec2 pos = glm::vec2(location.x, location.y);
    
    g.MouseReleaseEvent(pos, touch.tapCount == 1);
}
    
-(void)update:(CFTimeInterval)currentTime
{
    double dt = currentTime;
    g.Update(dt);
    /* Called before each frame is rendered */
}

@end
