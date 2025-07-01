#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "NsBackpackSettings.generated.h"

class UNsBackpackItemTable;

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="NsBackpack Settings"))
class NSBACKPACK_API UNsBackpackSettings : public UDeveloperSettings
{
    GENERATED_BODY()
public:
    UNsBackpackSettings();

    UPROPERTY(Config, EditAnywhere, Category="Inventory")
    TSoftObjectPtr<UNsBackpackItemTable> ItemTable;
};

