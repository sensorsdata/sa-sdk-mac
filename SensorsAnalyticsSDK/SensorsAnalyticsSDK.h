//  SensorsAnalyticsSDK.h
//  SensorsAnalyticsSDK
//
//  Created by 曹犟 on 15/7/1.
//  Copyright © 2015－2018 Sensors Data Inc. All rights reserved.

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN
/**
 * @abstract
 * Debug 模式，用于检验数据导入是否正确。该模式下，事件会逐条实时发送到 SensorsAnalytics，并根据返回值检查
 * 数据导入是否正确。
 *
 * @discussion
 * Debug 模式的具体使用方式，请参考:
 *  http://www.sensorsdata.cn/manual/debug_mode.html
 *
 * Debug模式有三种选项:
 *   SensorsAnalyticsDebugOff - 关闭 DEBUG 模式
 *   SensorsAnalyticsDebugOnly - 打开 DEBUG 模式，但该模式下发送的数据仅用于调试，不进行数据导入
 *   SensorsAnalyticsDebugAndTrack - 打开 DEBUG 模式，并将数据导入到 SensorsAnalytics 中
 */
typedef NS_ENUM(NSInteger, SensorsAnalyticsDebugMode) {
    SensorsAnalyticsDebugOff,
    SensorsAnalyticsDebugOnly,
    SensorsAnalyticsDebugAndTrack,
};

/**
 * @abstract
 * 网络类型
 *
 * @discussion
 *   SensorsAnalyticsNetworkTypeNONE - NULL
 *   SensorsAnalyticsNetworkType2G - 2G
 *   SensorsAnalyticsNetworkType3G - 3G
 *   SensorsAnalyticsNetworkType4G - 4G
 *   SensorsAnalyticsNetworkTypeWIFI - WIFI
 *   SensorsAnalyticsNetworkTypeALL - ALL
 */
typedef NS_OPTIONS(NSInteger, SensorsAnalyticsNetworkType) {
    SensorsAnalyticsNetworkTypeNONE      = 0,
    SensorsAnalyticsNetworkType2G       = 1 << 0,
    SensorsAnalyticsNetworkType3G       = 1 << 1,
    SensorsAnalyticsNetworkType4G       = 1 << 2,
    SensorsAnalyticsNetworkTypeWIFI     = 1 << 3,
    SensorsAnalyticsNetworkTypeALL      = 0xFF,
};

/**
 * @class
 * SensorsAnalyticsSDK类
 *
 * @abstract
 * 在SDK中嵌入SensorsAnalytics的SDK并进行使用的主要API
 *
 * @discussion
 * 使用SensorsAnalyticsSDK类来跟踪用户行为，并且把数据发给所指定的SensorsAnalytics的服务。
 */
@interface SensorsAnalyticsSDK : NSObject


/**
 * @property
 *
 * @abstract
 * 获取用户的唯一用户标识
 */
@property (atomic, readonly, copy) NSString *distinctId;

/**
 * @property
 *
 * @abstract
 * 用户登录唯一标识符
 */
@property (atomic, readonly, copy) NSString *loginId;

/**
 * @proeprty
 *
 * @abstract
 * 当App进入后台时，是否执行flush将数据发送到SensrosAnalytics
 *
 * @discussion
 * 默认值为 YES
 */
@property (atomic) BOOL flushBeforeEnterBackground;

/**
 * @property
 *
 * @abstract
 * 两次数据发送的最小时间间隔，单位毫秒
 *
 * @discussion
 * 默认值为 15 * 1000 毫秒， 在每次调用track、trackSignUp以及profileSet等接口的时候，
 * 都会检查如下条件，以判断是否向服务器上传数据:
 * 1. 是否WIFI/3G/4G网络
 * 2. 是否满足以下数据发送条件之一:
 *   1) 与上次发送的时间间隔是否大于 flushInterval
 *   2) 本地缓存日志数目是否达到 flushBulkSize
 * 如果同时满足这两个条件，则向服务器发送一次数据；如果不满足，则把数据加入到队列中，等待下次检查时把整个队列的内容一并发送。
 * 需要注意的是，为了避免占用过多存储，队列最多只缓存10000条数据。
 */
@property (atomic) UInt64 flushInterval;

/**
 * @property
 *
 * @abstract
 * 本地缓存的最大事件数目，当累积日志量达到阈值时发送数据
 *
 * @discussion
 * 默认值为 100，在每次调用track、trackSignUp以及profileSet等接口的时候，都会检查如下条件，以判断是否向服务器上传数据:
 * 1. 是否WIFI/3G/4G网络
 * 2. 是否满足以下数据发送条件之一:
 *   1) 与上次发送的时间间隔是否大于 flushInterval
 *   2) 本地缓存日志数目是否达到 flushBulkSize
 * 如果同时满足这两个条件，则向服务器发送一次数据；如果不满足，则把数据加入到队列中，等待下次检查时把整个队列的内容一并发送。
 * 需要注意的是，为了避免占用过多存储，队列最多只缓存10000条数据。
 */
@property (atomic) UInt64 flushBulkSize;
#pragma mark- init instance

/**
 * @abstract
 * 根据传入的配置，初始化并返回一个SensorsAnalyticsSDK的单例
 *
 * @param serverURL 收集事件的URL
 * @param debugMode Sensors Analytics 的 Debug 模式
 *
 * @return 返回的单例
 */
+ (SensorsAnalyticsSDK *)sharedInstanceWithServerURL:(nullable NSString *)serverURL
                                        andDebugMode:(SensorsAnalyticsDebugMode)debugMode;

/**
 * @abstract
 * 返回之前所初始化好的单例
 *
 * @discussion
 * 调用这个方法之前，必须先调用 sharedInstanceWithServerURL 这个方法
 *
 * @return 返回的单例
 */
+ (SensorsAnalyticsSDK *)sharedInstance;

/**
 * @abstract
 * 返回预置的属性
 *
 * @return NSDictionary 返回预置的属性
 */
- (NSDictionary *)getPresetProperties;

/**
 * @abstract
 * 设置当前 serverUrl
 *
 */
- (void)setServerUrl:(NSString *)serverUrl;

#pragma mark--cache and flush
/**
 * @abstract
 * 设置本地缓存最多事件条数
 *
 * @discussion
 * 默认为 10000 条事件
 *
 * @param maxCacheSize 本地缓存最多事件条数
 */
- (void)setMaxCacheSize:(UInt64)maxCacheSize;

- (UInt64)getMaxCacheSize;

/**
 * @abstract
 * 设置 flush 时网络发送策略
 *
 * @discussion
 * 默认 3G、4G、WI-FI 环境下都会尝试 flush
 *
 * @param networkType SensorsAnalyticsNetworkType
 */
- (void)setFlushNetworkPolicy:(SensorsAnalyticsNetworkType)networkType;

/**
 * @abstract
 * 登录，设置当前用户的 loginId
 *
 * @param loginId 当前用户的 loginId
 */
- (void)login:(NSString *)loginId;
- (void)login:(NSString *)loginId withProperties:(NSDictionary * _Nullable )properties ;

/**
 * @abstract
 * 注销，清空当前用户的 loginId
 *
 */
- (void)logout;

/**
 * @abstract
 * 获取匿名id
 *
 * @return anonymousId 匿名 id
 */
- (NSString *)anonymousId;

/**
 * @abstract
 * 重置默认匿名id
 */
- (void)resetAnonymousId;

/**
 * @abstract
 * 自动收集 App Crash 日志，该功能默认是关闭的
 */
- (void)trackAppCrash;

/**
 * @abstract
 * 设置当前用户的distinctId
 *
 * @discussion
 * 一般情况下， 选择一个不会重复的匿名ID，如设备ID等
 * 如果没有设置identify，则使用SDK自动生成的匿名ID
 * SDK会自动将设置的distinctId保存到keychain中，下次启动时会从中读取
 * 注意： 如果是注册用户的 uid 请使用- (void)login:(NSString *)loginId; 方法
 * @param distinctId 当前用户的distinctId
 */
- (void)identify:(NSString *)distinctId;
#pragma mark - track event
/**
 * @abstract
 * 调用 track 接口，追踪一个带有属性的 event
 *
 * @discussion
 * propertyDict 是一个 Map。
 * 其中的 key 是 Property 的名称，必须是 NSString
 * value 则是 Property 的内容，只支持 NSString、NSNumber、NSSet、NSArray、NSDate 这些类型
 * 特别的，NSSet 或者 NSArray 类型的 value 中目前只支持其中的元素是 NSString
 *
 * @param event             event的名称
 * @param propertyDict     event的属性
 */
- (void)track:(NSString *)event withProperties:(nullable NSDictionary *)propertyDict;

/**
 * @abstract
 * 调用 track 接口，追踪一个无私有属性的 event
 *
 * @param event event 的名称
 */
- (void)track:(NSString *)event;

/**
 * @abstract
 * 设置 Cookie
 *
 * @param cookie NSString cookie
 * @param encode BOOL 是否 encode
 */
- (void)setCookie:(NSString *)cookie withEncode:(BOOL)encode;

/**
 * @abstract
 * 返回已设置的 Cookie
 *
 * @param decode BOOL 是否 decode
 * @return NSString cookie
 */
- (NSString *)getCookieWithDecode:(BOOL)decode;

/**
 * @abstract
 * 初始化事件的计时器。
 *
 * @discussion
 * 若需要统计某个事件的持续时间，先在事件开始时调用 trackTimerStart:"Event" 记录事件开始时间，该方法并不会真正发
 * 送事件；随后在事件结束时，调用 track:"Event" withProperties:properties，SDK 会追踪 "Event" 事件，并自动将事件持续时间记录
 *
 * 时间单位为秒,可保留三位小数
 *
 * 多次调用 trackTimerStart:"Event" 时，事件 "Event" 的开始时间以最后一次调用时为准。
 *
 * @param event             event的名称
*/
- (void)trackTimerStart:(NSString *)event;


- (void)trackTimerEnd:(NSString *)event withProperties:(nullable NSDictionary *)propertyDict;

- (void)trackTimerEnd:(NSString *)event;

/**
 * @abstract
 * 清除所有事件计时器
 */
- (void)clearTrackTimer;

- (SensorsAnalyticsDebugMode)debugMode;

/**
 * @abstract
 * 用来设置每个事件都带有的一些公共属性
 *
 * @discussion
 * 当 track 的 Properties，superProperties 和 SDK 自动生成的 automaticProperties 有相同的 key 时，遵循如下的优先级：
 *    track.properties > superProperties > automaticProperties
 * 另外，当这个接口被多次调用时，是用新传入的数据去 merge 先前的数据，并在必要时进行 merge
 * 例如，在调用接口前，dict 是 @{@"a":1, @"b": "bbb"}，传入的 dict 是 @{@"b": 123, @"c": @"asd"}，则 merge 后的结果是
 * @{"a":1, @"b": 123, @"c": @"asd"}，同时，SDK 会自动将 superProperties 保存到文件中，下次启动时也会从中读取
 *
 * @param propertyDict 传入 merge 到公共属性的 dict
 */
- (void)registerSuperProperties:(NSDictionary *)propertyDict;

/**
 * @abstract
 * 用来设置事件的动态公共属性
 *
 * @discussion
 * 当 track 的 Properties，superProperties 和 SDK 自动生成的 automaticProperties 有相同的 key 时，遵循如下的优先级：
 *    track.properties > dynamicSuperProperties > superProperties > automaticProperties
 *
 * 例如，track.properties 是 @{@"a":1, @"b": "bbb"}，返回的 eventCommonProperty 是 @{@"b": 123, @"c": @"asd"}，
 * superProperties 是  @{@"a":1, @"b": "bbb",@"c":@"ccc"}，automaticProperties 是 @{@"a":1, @"b": "bbb",@"d":@"ddd"},
 * 则 merge 后的结果是 @{"a":1, @"b": "bbb", @"c": @"asd",@"d":@"ddd"}
 * 返回的 NSDictionary 需满足以下要求
 * 重要：1,key 必须是 NSString
 *          2,key 的名称必须符合要求
 *          3,value 的类型必须是 NSString、NSNumber、NSSet、NSArray、NSDate
 *          4,value 类型为 NSSet、NSArray 时，NSSet、NSArray 中的所有元素必须为 NSString
 * @param dynamicSuperProperties block 用来返回事件的动态公共属性
 */
-(void)registerDynamicSuperProperties:(NSDictionary<NSString *,id> *(^)(void)) dynamicSuperProperties;

/**
 * @abstract
 * 从 superProperty 中删除某个 property
 *
 * @param property 待删除的 property 的名称
 */
- (void)unregisterSuperProperty:(NSString *)property;

/**
 * @abstract
 * 删除当前所有的 superProperty
 */
- (void)clearSuperProperties;

/**
 * @abstract
 * 拿到当前的 superProperty 的副本
 *
 * @return 当前的 superProperty 的副本
 */
- (NSDictionary *)currentSuperProperties;

/**
 * @abstract
 * 得到 SDK 的版本
 *
 * @return SDK 的版本
 */
- (NSString *)libVersion;

/**
 * @abstract
 * 强制试图把数据传到对应的 SensorsAnalytics 服务器上
 *
 * @discussion
 * 主动调用 flush 接口，则不论 flushInterval 和网络类型的限制条件是否满足，都尝试向服务器上传一次数据
 */
- (void)flush;

/**
 * @abstract
 * 删除本地缓存的全部事件
 *
 * @discussion
 * 一旦调用该接口，将会删除本地缓存的全部事件，请慎用！
 */
- (void)deleteAll;

#pragma mark- profile
/**
 * @abstract
 * 直接设置用户的一个或者几个 Profiles
 *
 * @discussion
 * 这些Profile的内容用一个<code>NSDictionary</code>来存储
 * 其中的key是Profile的名称，必须是<code>NSString</code>
 * Value则是Profile的内容，只支持 <code>NSString</code>,<code>NSNumber</code>,<code>NSSet</code>,<code>NSArray</code>,
 *                              <code>NSDate</code>这些类型
 * 特别的，<code>NSSet</code>或者<code>NSArray</code>类型的value中目前只支持其中的元素是<code>NSString</code>
 * 如果某个Profile之前已经存在了，则这次会被覆盖掉；不存在，则会创建
 *
 * @param profileDict 要替换的那些 Profile 的内容
 */
- (void)set:(NSDictionary *)profileDict;

/**
 * @abstract
 * 首次设置用户的一个或者几个 Profiles
 *
 * @discussion
 * 与 set 接口不同的是，如果该用户的某个 Profile 之前已经存在了，会被忽略；不存在，则会创建
 *
 * @param profileDict 要替换的那些 Profile 的内容
 */
- (void)setOnce:(NSDictionary *)profileDict;

/**
 * @abstract
 * 设置用户的单个 Profile 的内容
 *
 * @discussion
 * 如果这个 Profile 之前已经存在了，则这次会被覆盖掉；不存在，则会创建
 *
 * @param profile Profile 的名称
 * @param content Profile 的内容
 */
- (void)set:(NSString *) profile to:(id)content;

/**
 * @abstract
 * 首次设置用户的单个 Profile 的内容
 *
 * @discussion
 * 与 set 类接口不同的是，如果这个 Profile 之前已经存在了，则这次会被忽略；不存在，则会创建
 *
 * @param profile Profile 的名称
 * @param content Profile 的内容
 */
- (void)setOnce:(NSString *) profile to:(id)content;

/**
 * @abstract
 * 删除某个 Profile 的全部内容
 *
 * @discussion
 * 如果这个 Profile 之前不存在，则直接忽略
 *
 * @param profile Profile 的名称
 */
- (void)unset:(NSString *) profile;

/**
 * @abstract
 * 给一个数值类型的 Profile 增加一个数值
 *
 * @discussion
 * 只能对 NSNumber 类型的 Profile 调用这个接口，否则会被忽略
 * 如果这个 Profile 之前不存在，则初始值当做 0 来处理
 *
 * @param profile  待增加数值的 Profile 的名称
 * @param amount   要增加的数值
 */
- (void)increment:(NSString *)profile by:(NSNumber *)amount;

/**
 * @abstract
 * 给多个数值类型的 Profile 增加数值
 *
 * @discussion
 * profileDict 中，key 是 NSString ，value 是 NSNumber
 * 其它与 -(void)increment:by: 相同
 *
 * @param profileDict 多个
 */
- (void)increment:(NSDictionary *)profileDict;

/**
 * @abstract
 * 向一个<code>NSSet</code>或者<code>NSArray</code>类型的value添加一些值
 *
 * @discussion
 * 如前面所述，这个<code>NSSet</code>或者<code>NSArray</code>的元素必须是<code>NSString</code>，否则，会忽略
 * 同时，如果要append的Profile之前不存在，会初始化一个空的<code>NSSet</code>或者<code>NSArray</code>
 *
 * @param profile profile
 * @param content description
 */
- (void)append:(NSString *)profile by:(NSObject<NSFastEnumeration> *)content;

/**
 * @abstract
 * 删除当前这个用户的所有记录
 */
- (void)deleteUser;
/**
 * @abstract
 * log 功能开关
 *
 * @discussion
 * 根据需要决定是否开启 SDK log , SensorsAnalyticsDebugOff 模式默认关闭 log
 * SensorsAnalyticsDebugOnly  SensorsAnalyticsDebugAndTrack 模式默认开启log
 *
 * @param enabelLog YES/NO
 */
- (void)enableLog:(BOOL)enabelLog;
@end
NS_ASSUME_NONNULL_END
