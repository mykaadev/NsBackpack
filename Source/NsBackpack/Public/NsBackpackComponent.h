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

    /** Give items using the item class */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool GiveItemByClass(const TSubclassOf<class UNsBackpackItemDefinition> InItemClass, const int32 InQuantity);

    /** Remove items using the item class */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool RemoveItemByClass(const TSubclassOf<class UNsBackpackItemDefinition> InItemClass, const int32 InQuantity);

    /** Does the owner has this item */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool FindItemByClass(const TSubclassOf<class UNsBackpackItemDefinition> InItemClass, const int32 InQuantity);

    /** Give items by identifier */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool GiveItemByID(const int32 InItemID, const int32 InQuantity, const bool InPersistent);

    /** Remove items by identifier */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool RemoveItemByID(const int32 InItemID, const int32 InQuantity, const bool InPersistent);

    /** Does the owner has this item */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool FindItemByID(const int32 InItemID, const int32 InQuantity, const bool InPersistent);

    /** Initialize slots */
    void InitializeSlots(int32 NumSlots);

// Variables
public:

    /** Inventory Size */
    UPROPERTY(BlueprintReadWrite, Category="Inventory")
    int32 InventorySize;

    /** Backpack Inventory */
    UPROPERTY(BlueprintReadOnly, Category="Inventory")
    TArray<FNsBackpackSlot> Slots;
};

