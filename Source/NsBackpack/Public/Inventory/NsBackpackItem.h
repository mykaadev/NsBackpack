// Copyright (C) 2025 mykaadev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/Object.h"
#include "NsBackpackItem.generated.h"

USTRUCT(BlueprintType)
struct FNsBackpackItemRow : public FTableRowBase
{
    GENERATED_BODY()

    /** Item */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<class UNsBackpackItem> ItemClass;

    //~ Begin FTableRowBase Interface
    virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;
    //~ End FTableRowBase Interface
};

/**
 * Base inventory item object with overrideable behaviour.
 */
UCLASS(Blueprintable, BlueprintType)
class NSBACKPACK_API UNsBackpackItem : public UObject
{
    GENERATED_BODY()
public:

    /** Constructor */
    UNsBackpackItem();

    /** Identifier assigned from the item table. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item")
    int32 ID;

    /** Name */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
    FText Name;

    /** Description */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
    FText Description;

    /** UI Image */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
    TSoftObjectPtr<class UTexture2D> Image;

    /** Can It Stack */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
    bool bStackable;

    /** Max Stack Size, 0 is infinite amount */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item", meta=(EditCondition="bStackable", ClampMin="0"))
    int32 MaxStackSize;

    /** In Game Mesh Component */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equipable")
    TSoftObjectPtr<class UStaticMesh> Mesh;

    /** In Game Actor to Spawn */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawnable")
    TSubclassOf<class AActor> ActorToSpawn;

    /** Use Item*/
    UFUNCTION(BlueprintNativeEvent, Category="Item")
    void Use(class AActor* InItemOwner);
    virtual void Use_Implementation(class AActor* const InItemOwner);
};

/** Struct representing a stack of items. */
USTRUCT(BlueprintType)
struct FNsBackpackItemStack
{
    GENERATED_BODY()

    /** Item */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<class UNsBackpackItem> Item;

    /** Quantity */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity;

    /** Constructor */
    FNsBackpackItemStack()
        : Item(nullptr)
        , Quantity(0)
    {}

    /** Constructor */
    FNsBackpackItemStack(class UNsBackpackItem* const InItem, const int32 InQuantity)
        : Item(InItem)
        , Quantity(InQuantity)
    {}
};