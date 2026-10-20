// Copyright (C) 2025 nulled.softworks. All rights reserved.

#include "NsBackpackComponent.h"

#include "NsBackpackItemDefinition.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"

UNsBackpackComponent::UNsBackpackComponent()
    : SlotCount(8)
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UNsBackpackComponent::BeginPlay()
{
    Super::BeginPlay();
    if (!CanChange())
    {
        return;
    }

    ResetSlots(SlotCount);
    for (const FNsBackpackStartItem& StartItem : StartItems)
    {
        AddItem(StartItem.Item, StartItem.Quantity);
    }
}

void UNsBackpackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UNsBackpackComponent, Slots);
}

void UNsBackpackComponent::ResetSlots(const int32 Count)
{
    if (!CanChange())
    {
        return;
    }

    Slots.Reset();
    Slots.SetNum(FMath::Max(0, Count));
    NotifyChanged();
}

int32 UNsBackpackComponent::AddItem(UNsBackpackItemDefinition* const Item, const int32 Quantity)
{
    if (!CanChange() || Item == nullptr || Quantity <= 0)
    {
        return FMath::Max(0, Quantity);
    }

    // The stacks of the item fill first, so the items stay together
    int32 Remaining = Quantity;
    for (int32 Index = 0; Index < Slots.Num() && Remaining > 0; ++Index)
    {
        if (!Slots[Index].IsEmpty())
        {
            const int32 Added = FMath::Min(GetRoom(Index, Item), Remaining);
            Slots[Index].Quantity += Added;
            Remaining -= Added;
        }
    }

    for (int32 Index = 0; Index < Slots.Num() && Remaining > 0; ++Index)
    {
        if (Slots[Index].IsEmpty() && CanAccept(Item, Index))
        {
            const int32 Added = FMath::Min(FMath::Max(1, Item->MaxStack), Remaining);
            Slots[Index] = FNsBackpackSlot();
            Slots[Index].Item = Item;
            Slots[Index].Quantity = Added;
            Remaining -= Added;
        }
    }

    if (Remaining != Quantity)
    {
        NotifyChanged();
    }
    return Remaining;
}

bool UNsBackpackComponent::RemoveItem(UNsBackpackItemDefinition* const Item, const int32 Quantity)
{
    if (!CanChange() || Item == nullptr || Quantity <= 0 || CountItem(Item) < Quantity)
    {
        return false;
    }

    int32 Remaining = Quantity;
    for (FNsBackpackSlot& Slot : Slots)
    {
        if (Remaining > 0 && !Slot.IsEmpty() && Slot.Item == Item)
        {
            const int32 Removed = FMath::Min(Slot.Quantity, Remaining);
            Slot.Quantity -= Removed;
            Remaining -= Removed;
            if (Slot.Quantity <= 0)
            {
                Slot = FNsBackpackSlot();
            }
        }
    }

    NotifyChanged();
    return true;
}

bool UNsBackpackComponent::RemoveFromSlot(const int32 SlotIndex, const int32 Quantity)
{
    if (!CanChange() || !Slots.IsValidIndex(SlotIndex) || Quantity <= 0 || Slots[SlotIndex].IsEmpty() || Slots[SlotIndex].Quantity < Quantity)
    {
        return false;
    }

    FNsBackpackSlot& Slot = Slots[SlotIndex];
    Slot.Quantity -= Quantity;
    if (Slot.Quantity <= 0)
    {
        Slot = FNsBackpackSlot();
    }

    NotifyChanged();
    return true;
}

bool UNsBackpackComponent::MoveSlot(const int32 FromIndex, const int32 ToIndex)
{
    if (!CanChange() || !Slots.IsValidIndex(FromIndex) || !Slots.IsValidIndex(ToIndex) || FromIndex == ToIndex || Slots[FromIndex].IsEmpty())
    {
        return false;
    }

    FNsBackpackSlot& From = Slots[FromIndex];
    FNsBackpackSlot& To = Slots[ToIndex];

    // The same item joins the stack while it has room. The rest stays in the first slot
    const int32 Room = To.IsEmpty() ? 0 : GetRoom(ToIndex, From.Item);
    if (Room > 0 && !From.Data.IsValid())
    {
        const int32 Moved = FMath::Min(Room, From.Quantity);
        To.Quantity += Moved;
        From.Quantity -= Moved;
        if (From.Quantity <= 0)
        {
            From = FNsBackpackSlot();
        }
        NotifyChanged();
        return true;
    }

    // A swap needs each slot to accept the item of the other slot
    if (!CanAccept(From.Item, ToIndex) || (!To.IsEmpty() && !CanAccept(To.Item, FromIndex)))
    {
        return false;
    }

    Swap(From, To);
    NotifyChanged();
    return true;
}

bool UNsBackpackComponent::SetSlot(const int32 SlotIndex, const FNsBackpackSlot& Slot)
{
    if (!CanChange() || !Slots.IsValidIndex(SlotIndex) || (!Slot.IsEmpty() && !CanAccept(Slot.Item, SlotIndex)))
    {
        return false;
    }

    Slots[SlotIndex] = Slot.IsEmpty() ? FNsBackpackSlot() : Slot;
    NotifyChanged();
    return true;
}

int32 UNsBackpackComponent::CountItem(const UNsBackpackItemDefinition* const Item) const
{
    int32 Count = 0;
    for (const FNsBackpackSlot& Slot : Slots)
    {
        Count += !Slot.IsEmpty() && Slot.Item == Item ? Slot.Quantity : 0;
    }
    return Count;
}

int32 UNsBackpackComponent::FindItem(const UNsBackpackItemDefinition* const Item) const
{
    return Slots.IndexOfByPredicate([Item](const FNsBackpackSlot& Slot) { return !Slot.IsEmpty() && Slot.Item == Item; });
}

int32 UNsBackpackComponent::FindEmptySlot() const
{
    return Slots.IndexOfByPredicate([](const FNsBackpackSlot& Slot) { return Slot.IsEmpty(); });
}

const FNsBackpackSlot& UNsBackpackComponent::GetSlot(const int32 SlotIndex) const
{
    static const FNsBackpackSlot EmptySlot;
    return Slots.IsValidIndex(SlotIndex) ? Slots[SlotIndex] : EmptySlot;
}

bool UNsBackpackComponent::CanAccept_Implementation(const UNsBackpackItemDefinition* const Item, const int32 SlotIndex) const
{
    return true;
}

bool UNsBackpackComponent::CanStack_Implementation(const FNsBackpackSlot& Slot, const UNsBackpackItemDefinition* const Item) const
{
    return Slot.Item == Item && !Slot.Data.IsValid();
}

bool UNsBackpackComponent::CanChange() const
{
    const AActor* const Owner = GetOwner();
    return Owner == nullptr || Owner->HasAuthority();
}

void UNsBackpackComponent::NotifyChanged()
{
    OnChanged.Broadcast();
}

void UNsBackpackComponent::OnRep_Slots()
{
    NotifyChanged();
}

int32 UNsBackpackComponent::GetRoom(const int32 SlotIndex, const UNsBackpackItemDefinition* const Item) const
{
    const FNsBackpackSlot& Slot = GetSlot(SlotIndex);
    if (Item == nullptr || Slot.IsEmpty() || !CanStack(Slot, Item))
    {
        return 0;
    }
    return FMath::Max(0, FMath::Max(1, Item->MaxStack) - Slot.Quantity);
}
