//
//  TextDetectionViewController.m
//  TextDetection
//
//  Created by Emiel Bon on 15-07-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#import "TextDetectionViewController.h"
#include "ContentLoader.h"
#include "RenderWindow.h"

@interface TextDetectionViewController ()
@property (strong, nonatomic) EAGLContext *context;
@end

@implementation TextDetectionViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    [EAGLContext setCurrentContext:self.context];
    
    NSString *filePath = [[NSBundle mainBundle] pathForResource:@"sign800x600" ofType:@"jpg"];
    NSString *bundlePath = [[filePath stringByDeletingLastPathComponent] stringByAppendingString:@"/"];
    
    GLKTextureInfo *textureInfo;
    NSError *error;
    
    textureInfo = [GLKTextureLoader textureWithContentsOfFile:filePath options:@{ GLKTextureLoaderOriginBottomLeft : @YES }error:&error];
    
    ContentLoader::ContentPath = bundlePath.UTF8String;
    window = new RenderWindow(textureInfo.name, textureInfo.width, textureInfo.height);
}

- (void)dealloc
{
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    // todo: maybe remove, we don't want it to be destroyed
    /*if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }*/

    // Dispose of any resources that can be recreated.
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    [view bindDrawable];
    window->Draw();
}

@end