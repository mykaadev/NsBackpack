// Copyright (C) 2025 mykaadev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "NsBackpackSettings.generated.h"

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
    {
        CategoryName = TEXT("Plugins");
        SectionName = TEXT("NsBackpack");
    }

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

    UPROPERTY(Config, EditAnywhere, Category="Inventory")
    TSoftObjectPtr<class UDataTable> ItemDataTable;
};

