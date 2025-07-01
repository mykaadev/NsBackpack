// Copyright (c) 2025 mykaa
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/NsBackpackItem.h"
#include "NsBackpackComponent.generated.h"

/** Component handling backpack and persistent item storage. */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NSBACKPACK_API UNsBackpackComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UNsBackpackComponent();

    /** Maximum number of backpack item stacks. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    int32 BackpackSlots = 20;

    /** Backpack item stacks. */
    UPROPERTY(BlueprintReadOnly, Category="Inventory")
    TArray<FNsBackpackItemStack> BackpackItems;

    /** Persistent item stacks that cannot be dropped. */
    UPROPERTY(BlueprintReadOnly, Category="Inventory")
    TArray<FNsBackpackItemStack> PersistentItems;

    /** Database used to resolve item IDs. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    TMap<int32, UNsBackpackItem*> ItemDatabase;

    /** Increase available backpack slots. */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    void ExpandBackpack(int32 AdditionalSlots);

    /** Give items by data asset. */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool GiveItem(UNsBackpackItem* ItemData, int32 Quantity, bool bPersistent);

    /** Remove items by data asset. */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool RemoveItem(UNsBackpackItem* ItemData, int32 Quantity, bool bPersistent);

    /** Give items using the item class. */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool GiveItemByClass(TSubclassOf<UNsBackpackItem> ItemClass, int32 Quantity, bool bPersistent);

    /** Remove items using the item class. */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool RemoveItemByClass(TSubclassOf<UNsBackpackItem> ItemClass, int32 Quantity, bool bPersistent);

    /** Give items by identifier. */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool GiveItemByID(int32 ItemID, int32 Quantity, bool bPersistent);

    /** Remove items by identifier. */
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool RemoveItemByID(int32 ItemID, int32 Quantity, bool bPersistent);

private:
    void LoadItemDatabase();
    bool GiveToList(TArray<FNsBackpackItemStack>& List, UNsBackpackItem* ItemData, int32 Quantity, int32 MaxSlots);
    bool RemoveFromList(TArray<FNsBackpackItemStack>& List, UNsBackpackItem* ItemData, int32 Quantity);
};

