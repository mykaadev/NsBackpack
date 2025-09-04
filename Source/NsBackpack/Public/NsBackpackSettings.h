// Copyright (C) 2025 nulled.softworks. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "NsBackpackSettings.generated.h"

/**
 * Layout modes for inventory packing
 */
UENUM(BlueprintType)
enum class ENsBackpackLayoutMode : uint8
{
    Expanded UMETA(DisplayName = "Expanded"),
    Compact  UMETA(DisplayName = "Compact")
};

/**
 * Ns Backpack Settings
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="NsBackpack Settings"))
class NSBACKPACK_API UNsBackpackSettings : public UDeveloperSettings
{
    GENERATED_BODY()

// Functions
public:

    /** Constructor */
    UNsBackpackSettings()
        : LayoutMode(ENsBackpackLayoutMode::Expanded)
    {};

    /** Get Game Settings */
    UFUNCTION(BlueprintCallable, Category = "Backpack Settings")
    static UNsBackpackSettings* GetSettings()
    {
        static UNsBackpackSettings* Instance;
        if(Instance != nullptr)
        {
            return Instance;
        }

        for (const TObjectIterator<UNsBackpackSettings> SettingsIt(RF_NoFlags); SettingsIt;)
        {
            Instance = *SettingsIt;
            break;
        }
        return Instance;
    }

// Variables
public:

    /** Default Item widget */
    UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category="Inventory")
    TSoftClassPtr<class UUserWidget> DefaultItemWidget;

    /** Layout mode to handle the inventory creation */
    UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category="Inventory")
    ENsBackpackLayoutMode LayoutMode;
};

