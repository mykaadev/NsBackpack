// Copyright (c) 2025 mykaa

#include "NsBackpackComponent.h"
#include "Engine/DataTable.h"
#include "Item/NsBackpackItem.h"

UNsBackpackComponent::UNsBackpackComponent()
    : InventorySize(6)
{
    PrimaryComponentTick.bCanEverTick = false;
    InitializeSlots(InventorySize);

    for (const auto& DefaultItem : DefaultInventory)
    {
        if (!DefaultItem.IsNull())
        {
            GiveItemByClass(DefaultItem, 1);
        }
    }
}

void UNsBackpackComponent::InitializeSlots(const int32 InNumberOfSlots)
{
    InventorySlots.SetNum(InNumberOfSlots);
    for (int32 Index = 0; Index < InventorySlots.Num(); ++Index)
    {
        InventorySlots[Index].SlotIndex = Index;
        InventorySlots[Index].ItemInstance = nullptr;
    }
}

bool UNsBackpackComponent::GiveItemByClass(const TSoftObjectPtr<class UNsBackpackItemDefinition> InItemClass, const int32 InQuantity /**= 1*/)
{
    if (!InItemClass || InQuantity <= 0)
    {
        return false;
    }

    UNsBackpackItemDefinition* const ItemDefinition = InItemClass.LoadSynchronous();
    if (!ItemDefinition)
    {
        return false;
    }

    int32 Remaining = InQuantity;

    // Try to stack into existing stacks first
    for (const FNsBackpackSlot& Slot : InventorySlots)
    {
        if (Remaining <= 0)
        {
            break;
        }

        if (Slot.ItemInstance != nullptr && Slot.ItemInstance->GetItemDefinition() == ItemDefinition)
        {
            const int32 SpaceLeft = ItemDefinition->StackSizeMax - Slot.ItemInstance->GetQuatity();
            if (SpaceLeft > 0)
            {
                const int32 ToAdd = FMath::Min(SpaceLeft, Remaining);
                Slot.ItemInstance->AddQuantity(ToAdd);
                Remaining -= ToAdd;
            }
        }
    }

    // Place into empty slots
    for (FNsBackpackSlot& Slot : InventorySlots)
    {
        if (Remaining <= 0)
        {
            break;
        }

        if (Slot.ItemInstance == nullptr)
        {
            const int32 ToAdd = FMath::Min(Remaining, ItemDefinition->StackSizeMax);
            UNsBackpackItemInstance* const NewInstance = NewObject<UNsBackpackItemInstance>(this);
            NewInstance->Initialize(ItemDefinition, ToAdd);
            NewInstance->SetBackpackComponent(this);
            Slot.ItemInstance = NewInstance;
            Remaining -= ToAdd;
        }
    }

    return Remaining == 0;
}

bool UNsBackpackComponent::RemoveItemByClass(const TSoftObjectPtr<class UNsBackpackItemDefinition> InItemClass, const int32 InQuantity /**= 1*/)
{
    if (!InItemClass || InQuantity <= 0)
    {
        return false;
    }

    const UNsBackpackItemDefinition* const ItemDefinition = InItemClass.LoadSynchronous();
    if (!ItemDefinition)
    {
        return false;
    }

    int32 Remaining = InQuantity;

    for (FNsBackpackSlot& Slot : InventorySlots)
    {
        if (Remaining <= 0)
        {
            break;
        }

        if (Slot.ItemInstance && Slot.ItemInstance->GetItemDefinition() == ItemDefinition)
        {
            const int32 Available = Slot.ItemInstance->GetQuatity();
            const int32 ToRemove = FMath::Min(Available, Remaining);
            Slot.ItemInstance->RemoveQuantity(ToRemove);
            Remaining -= ToRemove;

            if (Slot.ItemInstance->GetQuatity() <= 0)
            {
                Slot.ItemInstance->ConditionalBeginDestroy();
                Slot.ItemInstance = nullptr;
            }
        }
    }

    return Remaining == 0;
}

bool UNsBackpackComponent::FindItemByClass(const TSoftObjectPtr<class UNsBackpackItemDefinition> InItemClass, const int32 InQuantity /**= 1*/)
{
    if (!InItemClass || InQuantity <= 0)
    {
        return false;
    }

    const UNsBackpackItemDefinition* const ItemDefinition = InItemClass.LoadSynchronous();
    if (!ItemDefinition)
    {
        return false;
    }

    int32 Count = 0;
    for (const FNsBackpackSlot& Slot : InventorySlots)
    {
        if (Slot.ItemInstance && Slot.ItemInstance->GetItemDefinition() == ItemDefinition)
        {
            Count += Slot.ItemInstance->GetQuatity();
            if (Count >= InQuantity)
            {
                return true;
            }
        }
    }

    return false;
}

TArray<FNsBackpackSlot> UNsBackpackComponent::GetInventorySlots() const
{
    return InventorySlots;
}

TArray<FNsBackpackSlot>& UNsBackpackComponent::GetMutableInventorySlots()
{
    return InventorySlots;
}