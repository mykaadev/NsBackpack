// Copyright (c) 2025 mykaa

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NsBackpackTypeLibrary.h"
#include "NsBackpackComponent.generated.h"

/**
 * Component handling backpack and persistent item storage
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NSBACKPACK_API UNsBackpackComponent : public UActorComponent
{
    GENERATED_BODY()

// Functions
public:

    /** Constructor */
    UNsBackpackComponent();

    /** Initialize slots */
    void InitializeSlots(int32 NumSlots);

    /** Give items using the item class */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool GiveItemByClass(const TSoftObjectPtr<class UNsBackpackItemDefinition> InItemClass, const int32 InQuantity);

    /** Remove items using the item class */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool RemoveItemByClass(const TSoftObjectPtr<class UNsBackpackItemDefinition> InItemClass, const int32 InQuantity);

    /** Does the owner has this item */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool FindItemByClass(const TSoftObjectPtr<class UNsBackpackItemDefinition> InItemClass, const int32 InQuantity);

// Variables
public:

    /** Inventory Size */
    UPROPERTY(BlueprintReadWrite, Category="Inventory")
    int32 InventorySize;

    /** Backpack Inventory */
    UPROPERTY(BlueprintReadOnly, Category="Inventory")
    TArray<FNsBackpackSlot> Slots;
};

