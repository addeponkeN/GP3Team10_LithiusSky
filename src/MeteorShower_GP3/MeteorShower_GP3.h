// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

//	## ############# ##
//	## QUICK LOGGING ##
//	## ############# ##

/**
 * @brief your everyday logging macro
 */
#define LOG(x) MeteorLog(FColor::White, x)

// #define LOG(x, ...) \
// GEngine->AddOnScreenDebugMessage(-1, LogTime, FColor::White, \
// sizeof(__VA_ARGS__) > 0 ? FString::Printf(TEXT(x), __VA_ARGS__) : x)

// #define LOG(x, ...) \
// GEngine->AddOnScreenDebugMessage(-1, LogTime, FColor::White, \
// sizeof(__VA_ARGS__) > 0 ? FString::Printf(TEXT(x), __VA_ARGS__) : x)


/**
 * @brief log info(blue)
 */
#define LOGI(x) MeteorLog(FColor::Cyan, x);

/**
 * @brief log warnings (yellow)
 */
#define LOGW(x) MeteorLog(FColor::Yellow, x);

/**
 * @brief log errors (red)
 */
#define LOGE(x) MeteorLog(FColor::Red, x);

/**
 * @brief your every day logging macro with formatting
 */
#define LOGF(x, ...) GEngine->AddOnScreenDebugMessage(-1, LogTime, FColor::White, FString::Printf(TEXT(x), __VA_ARGS__));

/**
 * @brief log info with formatting
 */
#define LOGIF(x, ...) GEngine->AddOnScreenDebugMessage(-1, LogTime, FColor::Cyan, FString::Printf(TEXT(x), __VA_ARGS__));

/**
 * @brief log warnings with formatting
 */
#define LOGWF(x, ...) GEngine->AddOnScreenDebugMessage(-1, LogTime, FColor::Yellow, FString::Printf(TEXT(x), __VA_ARGS__));

/**
 * @brief log errors with formatting 
 */
#define LOGEF(x, ...) GEngine->AddOnScreenDebugMessage(-1, LogTime, FColor::Red, FString::Printf(TEXT(x), __VA_ARGS__));

constexpr float LogTime = 6.f;

static void MeteorLog(FColor color, const FString& msg)
{
	GEngine->AddOnScreenDebugMessage(-1, LogTime, color, msg);
}

//	## ####### ##
//	## ####### ##
