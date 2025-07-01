// Copyright (c) 2025 mykaa
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NsBackpackItemData.generated.h"

class UStaticMesh;

/** Data for an inventory item. */
UCLASS(BlueprintType)
class NSBACKPACK_API UNsBackpackItemData : public UDataAsset
{
    GENERATED_BODY()
public:
    /** Unique numeric identifier. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
    int32 ID = 0;

    /** Display name. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
    FText Name;

    /** Description text. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
    FText Description;

    /** Thumbnail image. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
    UTexture2D* Image = nullptr;

    /** If true this item can stack with items of the same type. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
    bool bStackable = false;

    /** Maximum items that can be stacked together. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item", meta=(EditCondition="bStackable", ClampMin="1"))
    int32 MaxStackSize = 1;

    /** Optional mesh if item can be equipped. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equipable")
    UStaticMesh* Mesh = nullptr;

    /** Optional actor class if item can be spawned into the world. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawnable")
    TSubclassOf<AActor> ActorToSpawn;

    /** Override to perform item specific behaviour. */
    UFUNCTION(BlueprintNativeEvent, Category="Item")
    void Use(AActor* Target);
};

/** Struct representing a stack of items. */
USTRUCT(BlueprintType)
struct FNsBackpackItemStack
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UNsBackpackItemData* Item = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity = 0;

    FNsBackpackItemStack() {}
    FNsBackpackItemStack(UNsBackpackItemData* InItem, int32 InQuantity)
        : Item(InItem), Quantity(InQuantity) {}
};

