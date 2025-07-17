// Copyright (c) 2025 mykaa

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

    /** Increase available backpack slots. */
    void ExpandBackpack(const int32 InAdditionalSlots);

    /** Give items using the item class. */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool GiveItemByClass(const TSubclassOf<class UNsBackpackItem> InItemClass, const int32 InQuantity, const bool InPersistent);

    /** Remove items using the item class. */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool RemoveItemByClass(const TSubclassOf<class UNsBackpackItem> InItemClass, const int32 InQuantity, const bool InPersistent);

    /** Does the owner has this item*/
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool FindItemByClass(const TSubclassOf<class UNsBackpackItem> InItemClass, const int32 InQuantity, const bool InPersistent);

    /** Give items by identifier. */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool GiveItemByID(const int32 InItemID, const int32 InQuantity, const bool InPersistent);

    /** Remove items by identifier. */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool RemoveItemByID(const int32 InItemID, const int32 InQuantity, const bool InPersistent);

    /** Does the owner has this item */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool FindItemByID(const int32 InItemID, const int32 InQuantity, const bool InPersistent);

    /** Give items by object. */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool GiveItem(class UNsBackpackItem* InItem, const int32 InQuantity, const bool InPersistent);

    /** Remove items by object. */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool RemoveItem(class UNsBackpackItem* InItem, const int32 InQuantity, const bool InPersistent);

    /** Does the owner have  the specified item in any stash */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool FindItem(class UNsBackpackItem* InItem, const int32 InQuantity, const bool InPersistent);

private:

    /** Give To List */
    bool GiveToList(TArray<struct FNsBackpackItemStack>& InList, class UNsBackpackItem* const InItemData, const int32 InQuantity, const int32 InMaxSlots);

    /**  Remove from list */
    bool RemoveFromList(TArray<struct FNsBackpackItemStack>& InList, class UNsBackpackItem* const InItem, int32 InQuantity);

    /** Finds a item in a given list, returns true if the quantity is bigger or equal to the specified */
    bool FindInList(TArray<struct FNsBackpackItemStack>& InList, class UNsBackpackItem* InItem, const int32 InQuantity);

    /** Load Item database */
    void LoadItemDatabase();

// Variables
public:

    /** Maximum number of backpack item stacks. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    int32 BackpackSlots;

    /** Backpack item stacks. */
    UPROPERTY(BlueprintReadOnly, Category="Inventory")
    TArray<struct FNsBackpackItemStack> BackpackItems;

    /** Persistent item stacks that cannot be dropped. */
    UPROPERTY(BlueprintReadOnly, Category="Inventory")
    TArray<struct FNsBackpackItemStack> PersistentItems;

    /** Database used to resolve item IDs. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    TMap<int32, class UNsBackpackItem*> ItemDatabase;
};

