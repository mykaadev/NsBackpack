// Copyright (C) 2025 nulled.softworks. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NsBackpackTypeLibrary.h"
#include "NsBackpackComponent.generated.h"

/**
 * Component handling backpack and persistent item storage
 */
UCLASS(BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NSBACKPACK_API UNsBackpackComponent : public UActorComponent
{
    GENERATED_BODY()

// Functions
public:

    /** Constructor */
    UNsBackpackComponent();
    
    //~ Begin UActorComponent Interface
    virtual void BeginPlay() override;
    //~ End UActorComponent Interface

    /** Initialize slots */
    void InitializeSlots(int32 NumSlots);

    /** Give items using the item class */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool GiveItemByClass(const TSoftObjectPtr<class UNsBackpackItemDefinition> InItemClass, const int32 InQuantity = 1);

    /** Remove items using the item class */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool RemoveItemByClass(const TSoftObjectPtr<class UNsBackpackItemDefinition> InItemClass, const int32 InQuantity = 1);

    /** Does the owner has this item */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool FindItemByClass(const TSoftObjectPtr<class UNsBackpackItemDefinition> InItemClass, const int32 InQuantity = 1);

    /** Get inventory */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    TArray<FNsBackpackSlot> GetInventorySlots() const;

    /** Get mutable inventory */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    TArray<FNsBackpackSlot>& GetMutableInventorySlots();
    
    /** Open inventory */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void OpenInventory(class AActor* InActor);
    virtual void OpenInventory_Implementation(class AActor* const InActor);

    /** Close inventory */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void CloseInventory(class AActor* InActor);
    virtual void CloseInventory_Implementation(class AActor* const InActor);

// Variables
public:

    /** Inventory Size */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    int32 InventorySize;

    /** Default Inventory */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    TArray<TSoftObjectPtr<class UNsBackpackItemDefinition>> DefaultInventory;

    /** Inventory widget */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    TSoftClassPtr<class UUserWidget> InventoryWidget;

private:

    /** Backpack Inventory */
    UPROPERTY()
    TArray<FNsBackpackSlot> InventorySlots;
};

