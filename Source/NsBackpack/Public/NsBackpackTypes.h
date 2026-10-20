// Copyright (C) 2025 nulled.softworks. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "NsBackpackTypes.generated.h"

/** One slot of a backpack: an item, a quantity, and optional data of that item */
USTRUCT(BlueprintType)
struct NSBACKPACK_API FNsBackpackSlot
{
    GENERATED_BODY()

// Functions
public:
    /** Returns true when the slot holds no item */
    bool IsEmpty() const { return Item == nullptr || Quantity <= 0; }

// Variables
public:
    /** Item in the slot, or null */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Backpack")
    TObjectPtr<class UNsBackpackItemDefinition> Item;

    /** Number of items in the slot */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Backpack", meta = (ClampMin = "0"))
    int32 Quantity = 0;

    /** Optional state of the item in this slot, such as a durability. The game picks the struct. A slot with data does not stack */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Backpack")
    FInstancedStruct Data;
};

/** An item that a backpack gets when play starts */
USTRUCT(BlueprintType)
struct NSBACKPACK_API FNsBackpackStartItem
{
    GENERATED_BODY()

// Variables
public:
    /** Item to add */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Backpack")
    TObjectPtr<class UNsBackpackItemDefinition> Item;

    /** Number of items to add */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Backpack", meta = (ClampMin = "1"))
    int32 Quantity = 1;
};
