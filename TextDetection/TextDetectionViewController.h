//
//  TextDetectionViewController.h
//  TextDetection
//
//  Created by Emiel Bon on 15-07-14.
//  Copyright (c) 2014 Emiel Bon. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#include "RenderWindow.h"

@interface TextDetectionViewController : GLKViewController
{
    RenderWindow *window;
}
@property (weak, nonatomic) IBOutlet UILabel *visibleTextureNameLabel;

@end
