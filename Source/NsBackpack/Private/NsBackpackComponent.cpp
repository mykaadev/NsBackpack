// Copyright (c) 2025 mykaa

#include "NsBackpackComponent.h"
#include "Item/NsBackpackItem.h"
#include "NsBackpackSettings.h"
#include "Engine/DataTable.h"

UNsBackpackComponent::UNsBackpackComponent()
    : InventorySize(9)
{
    PrimaryComponentTick.bCanEverTick = false;
    InitializeSlots(InventorySize);
}

void UNsBackpackComponent::InitializeSlots(const int32 InSlots)
{
    Slots.SetNum(InSlots);
    for (int32 Index = 0; Index < Slots.Num(); ++Index)
    {
        Slots[Index].SlotIndex = Index;
        Slots[Index].ItemInstance = nullptr;
    }
}

bool UNsBackpackComponent::GiveItemByClass(const TSubclassOf<class UNsBackpackItemDefinition> InItemClass, const int32 InQuantity)
{
    if (!InItemClass || InQuantity <= 0)
    {
        return false;
    }

    UNsBackpackItemDefinition* Def = InItemClass->GetDefaultObject<UNsBackpackItemDefinition>();
    if (!Def)
    {
        return false;
    }

    int32 Remaining = InQuantity;

    // Try to stack into existing stacks first
    for (const FNsBackpackSlot& Slot : Slots)
    {
        if (Remaining <= 0)
        {
            break;
        }

        if (Slot.ItemInstance != nullptr && Slot.ItemInstance->GetItemDefinition() == Def)
        {
            const int32 SpaceLeft = Def->StackSizeMax - Slot.ItemInstance->GetQuatity();
            if (SpaceLeft > 0)
            {
                const int32 ToAdd = FMath::Min(SpaceLeft, Remaining);
                Slot.ItemInstance->AddQuantity(ToAdd);
                Remaining -= ToAdd;
            }
        }
    }

    // Place into empty slots
    for (FNsBackpackSlot& Slot : Slots)
    {
        if (Remaining <= 0)
        {
            break;
        }

        if (Slot.ItemInstance == nullptr)
        {
            const int32 ToAdd = FMath::Min(Remaining, Def->StackSizeMax);
            UNsBackpackItemInstance* NewInstance = NewObject<UNsBackpackItemInstance>(this);
            NewInstance->Initialize(Def, ToAdd);
            NewInstance->SetBackpackComponent(this);
            Slot.ItemInstance = NewInstance;
            Remaining -= ToAdd;
        }
    }

    return Remaining == 0;
}

bool UNsBackpackComponent::RemoveItemByClass(const TSubclassOf<class UNsBackpackItemDefinition> InItemClass, const int32 InQuantity)
{
    if (!InItemClass || InQuantity <= 0)
    {
        return false;
    }

    const UNsBackpackItemDefinition* Def = InItemClass->GetDefaultObject<UNsBackpackItemDefinition>();
    if (!Def)
    {
        return false;
    }

    int32 Remaining = InQuantity;

    for (FNsBackpackSlot& Slot : Slots)
    {
        if (Remaining <= 0)
        {
            break;
        }

        if (Slot.ItemInstance && Slot.ItemInstance->GetItemDefinition() == Def)
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

bool UNsBackpackComponent::FindItemByClass(const TSubclassOf<class UNsBackpackItemDefinition> InItemClass, const int32 InQuantity)
{
    if (!InItemClass || InQuantity <= 0)
    {
        return false;
    }

    UNsBackpackItemDefinition* Def = InItemClass->GetDefaultObject<UNsBackpackItemDefinition>();
    if (!Def)
    {
        return false;
    }

    int32 Count = 0;
    for (const FNsBackpackSlot& Slot : Slots)
    {
        if (Slot.ItemInstance && Slot.ItemInstance->GetItemDefinition() == Def)
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