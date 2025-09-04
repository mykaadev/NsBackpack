// Copyright (C) 2025 nulled.softworks. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NsBackpackTypeLibrary.generated.h"

/**
 * Defines the rarity of an inventory item.
 */
UENUM(BlueprintType)
enum class ENsBackpackItemRarity : uint8
{
    Common      UMETA(DisplayName = "Common"),
    Uncommon    UMETA(DisplayName = "Uncommon"),
    Rare        UMETA(DisplayName = "Rare"),
    Epic        UMETA(DisplayName = "Epic"),
    Legendary   UMETA(DisplayName = "Legendary"),
    Mythic      UMETA(DisplayName = "Mythic"),
    Unique      UMETA(DisplayName = "Unique")
};

/**
 * Defines the category/type of inventory item.
 */
UENUM(BlueprintType)
enum class ENsBackpackItemCategory : uint8
{
    Consumable      UMETA(DisplayName = "Consumable"),      // Potions, food, etc.
    Equipment       UMETA(DisplayName = "Equipment"),       // Armor, wearable gear
    Weapon          UMETA(DisplayName = "Weapon"),          // Swords, guns, etc.
    Resource        UMETA(DisplayName = "Resource"),        // Crafting materials
    QuestItem       UMETA(DisplayName = "Quest Item"),      // Items tied to quests
    KeyItem         UMETA(DisplayName = "Key Item"),        // Special progression items
    Currency        UMETA(DisplayName = "Currency"),        // Gold, credits, etc.
    Miscellaneous   UMETA(DisplayName = "Miscellaneous")    // Everything else
};

/**
 * Struct that represents a backpack slot
 */
USTRUCT(BlueprintType)
struct FNsBackpackSlot
{
    GENERATED_BODY()

// Variables
public:

    /** Item instance */
    UPROPERTY(BlueprintReadWrite, Category="Slot")
    TObjectPtr<class UNsBackpackItemInstance> ItemInstance;

    /** Slot Index */
    UPROPERTY(BlueprintReadOnly, Category="Slot")
    int32 SlotIndex;

// Functions
public:

    /** Constructor */
    FNsBackpackSlot()
        : SlotIndex(INDEX_NONE)
    {}
};

/**
 * Base inventory item object with overrideable behaviour.
 */
UCLASS(Blueprintable, BlueprintType)
class NSBACKPACK_API UNsBackpackTypeLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

// Functions
public:

    /** Constructor */
    UNsBackpackTypeLibrary() {};
};