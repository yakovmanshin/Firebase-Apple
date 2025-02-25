// Copyright 2019 Google
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#import <Foundation/Foundation.h>

#if __has_include(<FBLPromises/FBLPromises.h>)
#import <FBLPromises/FBLPromises.h>
#else
#import "FBLPromises.h"
#endif

@class FIRCLSApplicationIdentifierModel;
@class FIRCLSFileManager;

NS_ASSUME_NONNULL_BEGIN

@interface FIRCLSSettings : NSObject

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)initWithFileManager:(FIRCLSFileManager *)fileManager
                         appIDModel:(FIRCLSApplicationIdentifierModel *)appIDModel
    NS_DESIGNATED_INITIALIZER;

/**
 * Recreates the settings dictionary by re-reading the settings file from persistent storage. This
 * should be called before any settings values are read, as it will populate the underlying
 * settingsDictionary. If the Google App ID has changed or there is an error, delete the cache file
 * and settingsDictionary. If the cache has expired, set `isCacheExpired` to true so that settings
 * are re-fetched, but do not delete any values.
 */
- (void)reloadFromCacheWithGoogleAppID:(NSString *)googleAppID
                      currentTimestamp:(NSTimeInterval)currentTimestamp;

/**
 * Stores a separate file with the settings expiration and Google App ID it was saved with
 * so that we can later determine that the settings have expired.
 *
 * This should be called in a background thread right after the settings.json file has been
 * downloaded.
 */
- (void)cacheSettingsWithGoogleAppID:(NSString *)googleAppID
                    currentTimestamp:(NSTimeInterval)currentTimestamp;

/**
 * Returns true when Settings should be fetched from the server again
 */
@property(nonatomic, readonly) BOOL isCacheExpired;

/**
 * Determines how long these Settings should be respected until the SDK should fetch again
 */
@property(nonatomic, readonly) uint32_t cacheDurationSeconds;

/**
 * When this is false, Crashlytics will not start up
 */
@property(nonatomic, readonly) BOOL collectReportsEnabled;

/**
 * When this is false, Crashlytics will not collect non-fatal errors and errors
 * from the custom exception / record error APIs
 */
@property(nonatomic, readonly) BOOL errorReportingEnabled;

/**
 * When this is false, Crashlytics will not collect custom exceptions from the API
 */
@property(nonatomic, readonly) BOOL customExceptionsEnabled;

/**
 * When this is true, Crashlytics will collect data from MetricKit
 */
@property(nonatomic, readonly) BOOL metricKitCollectionEnabled;

/**
 * Returns the maximum number of custom exception events that will be
 * recorded in a session.
 */
@property(nonatomic, readonly) uint32_t errorLogBufferSize;

/**
 * Returns the maximum size of the log buffer in bytes
 */
@property(nonatomic, readonly) uint32_t logBufferSize;

/**
 * Returns the maximum number of custom exceptions that will be collected
 * in a session.
 */
@property(nonatomic, readonly) uint32_t maxCustomExceptions;

/**
 * Returns the maximum number of custom key-value pair keys (not bytes).
 */
@property(nonatomic, readonly) uint32_t maxCustomKeys;

/**
 * Returns the initial upload rate for on-demand exception reporting.
 */
@property(nonatomic, readonly) double onDemandUploadRate;

/**
 * Base exponent used when exponential backoff is triggered for on-demand reporting.
 */
@property(nonatomic, readonly) double onDemandBackoffBase;

/**
 * Step duration to use with exponential backoff for on-demand reporting.
 */
@property(nonatomic, readonly) uint32_t onDemandBackoffStepDuration;

/**
 * When this is true, Crashlytics will suspend all threads to do on-demand fatal recording.
 */
@property(nonatomic, readonly) BOOL onDemandThreadSuspensionEnabled;

@end

NS_ASSUME_NONNULL_END
