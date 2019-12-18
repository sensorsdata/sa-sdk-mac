//
//  AppDelegate.m
//  Example
//
//  Created by 储强盛 on 2019/12/18.
//  Copyright © 2019 Sensors Data Inc. All rights reserved.
//

#import "AppDelegate.h"
#import <SensorsAnalyticsSDK/SensorsAnalyticsSDK.h>

static NSString *const SADefaultServerURL = @"http://sdk-test.cloud.sensorsdata.cn:8006/sa?project=default&token=95c73ae661f85aa0";

@interface AppDelegate ()

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application

    [SensorsAnalyticsSDK sharedInstanceWithServerURL:SADefaultServerURL andDebugMode:SensorsAnalyticsDebugOff];

    [[SensorsAnalyticsSDK sharedInstance] setFlushNetworkPolicy:SensorsAnalyticsNetworkTypeALL];

    [[SensorsAnalyticsSDK sharedInstance] registerDynamicSuperProperties:^NSDictionary * _Nonnull{
        __block NSApplicationOcclusionState appState;
        if (NSThread.isMainThread) {
            appState = NSApplication.sharedApplication.occlusionState;
        }else {
            dispatch_sync(dispatch_get_main_queue(), ^{
                appState = NSApplication.sharedApplication.occlusionState;
            });
        }
        return @{@"__APPState__":@(appState)};
    }];

    [[SensorsAnalyticsSDK sharedInstance] enableLog:YES];
    [[SensorsAnalyticsSDK sharedInstance] setMaxCacheSize:20000];
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


@end
