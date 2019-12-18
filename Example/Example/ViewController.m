//
//  ViewController.m
//  Example
//
//  Created by 储强盛 on 2019/12/18.
//  Copyright © 2019 Sensors Data Inc. All rights reserved.
//

#import "ViewController.h"
#import <SensorsAnalyticsSDK/SensorsAnalyticsSDK.h>

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}


- (IBAction)trackAction1:(NSButton *)sender {
    [[SensorsAnalyticsSDK sharedInstance] track:@"TestMac" withProperties:@{@"name":@"测试中文呐"}];
}

- (IBAction)trackAction2:(NSButton *)sender {
    [[SensorsAnalyticsSDK sharedInstance] flush];
}


- (IBAction)trackAction3:(NSButton *)sender {
    [[SensorsAnalyticsSDK sharedInstance] track:@"TestMac" withProperties:@{@"name":@(1.01)}];
}


- (IBAction)trackAction4:(NSButton *)sender {
    [[SensorsAnalyticsSDK sharedInstance] track:@"TestMac" withProperties:@{@"name":@"cnia_nciar,mnckasd"}];
}

- (IBAction)trackAction5:(NSButton *)sender {
    [[SensorsAnalyticsSDK sharedInstance] login:@"test-mac-123654" withProperties:@{@"name":@"login 测试"}];
}

@end
