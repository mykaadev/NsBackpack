// Copyright (c) 2025 mykaa

#include "NsBackpackComponent.h"
#include "Item/NsBackpackItem.h"
#include "NsBackpackSettings.h"

UNsBackpackComponent::UNsBackpackComponent()
    : InventorySize(9)
{
    PrimaryComponentTick.bCanEverTick = false;
    InitializeSlots(InventorySize);
}

void UNsBackpackComponent::InitializeSlots(const int32 InSlots)
{
    Slots.SetNum(InSlots);
}

bool UNsBackpackComponent::GiveItemByClass(const TSubclassOf<class UNsBackpackItemDefinition> InItemClass, const int32 InQuantity)
{
    return false;
}

bool UNsBackpackComponent::RemoveItemByClass(const TSubclassOf<class UNsBackpackItemDefinition> InItemClass, const int32 InQuantity)
{
    return false;
}

bool UNsBackpackComponent::FindItemByClass(const TSubclassOf<class UNsBackpackItemDefinition> InItemClass, const int32 InQuantity)
{
    return false;
}

bool UNsBackpackComponent::GiveItemByID(const int32 InItemID, const int32 InQuantity, const bool InPersistent)
{
    return false;
}

bool UNsBackpackComponent::RemoveItemByID(const int32 InItemID, const int32 InQuantity, const bool InPersistent)
{
    return false;
}

bool UNsBackpackComponent::FindItemByID(const int32 InItemID, const int32 InQuantity, const bool InPersistent)
{
    return false;
}