Pod::Spec.new do |s|
  s.name         = "SensorsAnalyticsSDK-mac"
  s.version      = "1.0.2"
  s.summary      = "The official macOS SDK of Sensors Analytics."
  s.homepage     = "http://www.sensorsdata.cn"
  s.source       = { :git => 'https://github.com/sensorsdata/sa-sdk-mac.git', :tag => "v#{s.version}" } 
  s.module_name  = "SensorsAnalyticsSDK"
  s.license      = { :type => "Apache License, Version 2.0" }
  s.author       = { "QiangSheng Chu" => "chuqiangsheng@sensorsdata.cn" }
  s.platform     = :osx, '10.10'
  s.frameworks   = 'Cocoa', 'Foundation', 'SystemConfiguration'
  s.libraries    = 'sqlite3', 'z'
  s.source_files =  "SensorsAnalyticsSDK/**/*.{h,m}"
  s.public_header_files = "SensorsAnalyticsSDK/SensorsAnalyticsSDK.h"
  s.resource = 'SensorsAnalyticsSDK/SensorsAnalyticsSDK.bundle'

end
