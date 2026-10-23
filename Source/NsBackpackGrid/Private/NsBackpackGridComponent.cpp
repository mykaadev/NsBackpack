// Copyright (C) 2025 nulled.softworks. All rights reserved.

#include "NsBackpackGridComponent.h"

#include "NsBackpackGridShape.h"
#include "NsBackpackItemDefinition.h"
#include "Net/UnrealNetwork.h"

UNsBackpackGridComponent::UNsBackpackGridComponent()
    : GridWidth(4)
{
    SlotCount = 16;
}

void UNsBackpackGridComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UNsBackpackGridComponent, GridWidth);
    DOREPLIFETIME(UNsBackpackGridComponent, Rotations);
}

int32 UNsBackpackGridComponent::AddItem(UNsBackpackItemDefinition* const Item, const int32 Quantity)
{
    // The parent fills the stacks, then the slots where the item fits with no rotation
    int32 Remaining = Super::AddItem(Item, Quantity);
    while (Remaining > 0 && CanChange() && Item != nullptr)
    {
        int32 SlotIndex = INDEX_NONE;
        int32 Rotation = 0;
        const int32 Stack = FMath::Min(FMath::Max(1, Item->MaxStack), Remaining);
        if (!FindPlacement(Item, SlotIndex, Rotation) || !PlaceItem(Item, Stack, SlotIndex, Rotation))
        {
            break;
        }
        Remaining -= Stack;
    }
    return Remaining;
}

bool UNsBackpackGridComponent::MoveSlot(const int32 FromIndex, const int32 ToIndex)
{
    if (FromIndex == ToIndex)
    {
        return false;
    }

    // A move onto a covered cell can only join the stack that covers it
    const int32 Owner = GetCellOwner(ToIndex);
    if (Owner != INDEX_NONE && Owner != FromIndex)
    {
        return CanJoin(FromIndex, Owner) && Super::MoveSlot(FromIndex, Owner);
    }
    return MoveItem(FromIndex, ToIndex, GetRotation(FromIndex));
}

int32 UNsBackpackGridComponent::FindEmptySlot() const
{
    // A shape can leave its own top left cell open, so the slot must be empty too
    const TArray<int32> Owners = BuildCellOwners();
    for (int32 SlotIndex = 0; SlotIndex < Owners.Num(); ++SlotIndex)
    {
        if (Owners[SlotIndex] == INDEX_NONE && GetSlot(SlotIndex).IsEmpty())
        {
            return SlotIndex;
        }
    }
    return INDEX_NONE;
}

void UNsBackpackGridComponent::ResetGrid(const int32 Width, const int32 Height)
{
    if (!CanChange())
    {
        return;
    }

    GridWidth = FMath::Max(1, Width);
    ResetSlots(GridWidth * FMath::Max(0, Height));
}

bool UNsBackpackGridComponent::PlaceItem(UNsBackpackItemDefinition* const Item, const int32 Quantity, const int32 SlotIndex, const int32 Rotation)
{
    if (!CanChange() || Item == nullptr || Quantity <= 0 || Quantity > FMath::Max(1, Item->MaxStack) || !GetSlot(SlotIndex).IsEmpty())
    {
        return false;
    }

    FNsBackpackSlot Slot;
    Slot.Item = Item;
    Slot.Quantity = Quantity;
    return SetGridSlot(SlotIndex, Slot, Rotation);
}

bool UNsBackpackGridComponent::MoveItem(const int32 FromIndex, const int32 ToIndex, const int32 Rotation)
{
    const FNsBackpackSlot Moved = GetSlot(FromIndex);
    if (!CanChange() || Moved.IsEmpty() || (ToIndex != FromIndex && !GetSlot(ToIndex).IsEmpty()) || !CanPlace(Moved.Item, ToIndex, Rotation, FromIndex))
    {
        return false;
    }

    if (ToIndex == FromIndex)
    {
        SetRotation(FromIndex, Rotation);
        NotifyChanged();
        return true;
    }

    // The item covers its old cells and its new cells until the old slot clears. Can Accept ignores the old slot
    TGuardValue<int32> IgnoreGuard(CheckIgnoreSlot, FromIndex);
    if (!SetGridSlot(ToIndex, Moved, Rotation))
    {
        return false;
    }
    return SetSlot(FromIndex, FNsBackpackSlot());
}

bool UNsBackpackGridComponent::RotateItem(const int32 SlotIndex)
{
    return MoveItem(SlotIndex, SlotIndex, GetRotation(SlotIndex) + 1);
}

bool UNsBackpackGridComponent::SetGridSlot(const int32 SlotIndex, const FNsBackpackSlot& Slot, const int32 Rotation)
{
    if (!CanChange() || SlotIndex < 0 || SlotIndex >= GetSlotCount())
    {
        return false;
    }

    // Set Slot checks the fit with the stored rotation, so the rotation goes in first
    const int32 OldRotation = GetRotation(SlotIndex);
    SetRotation(SlotIndex, Rotation);
    if (!SetSlot(SlotIndex, Slot))
    {
        SetRotation(SlotIndex, OldRotation);
        return false;
    }
    return true;
}

bool UNsBackpackGridComponent::CanPlace(const UNsBackpackItemDefinition* const Item, const int32 SlotIndex, const int32 Rotation, const int32 IgnoreSlot) const
{
    TGuardValue<int32> RotationGuard(CheckRotation, UNsBackpackGridShape::NormalizeRotation(Rotation));
    TGuardValue<int32> IgnoreGuard(CheckIgnoreSlot, IgnoreSlot != INDEX_NONE ? IgnoreSlot : CheckIgnoreSlot);
    return CanAccept(Item, SlotIndex);
}

bool UNsBackpackGridComponent::FindPlacement(const UNsBackpackItemDefinition* const Item, int32& OutSlotIndex, int32& OutRotation) const
{
    for (int32 Rotation = 0; Rotation < 4; ++Rotation)
    {
        for (int32 SlotIndex = 0; SlotIndex < GetSlotCount(); ++SlotIndex)
        {
            if (GetSlot(SlotIndex).IsEmpty() && CanPlace(Item, SlotIndex, Rotation))
            {
                OutSlotIndex = SlotIndex;
                OutRotation = Rotation;
                return true;
            }
        }
    }
    OutSlotIndex = INDEX_NONE;
    OutRotation = 0;
    return false;
}

int32 UNsBackpackGridComponent::GetCellOwner(const int32 CellIndex) const
{
    const TArray<int32> Owners = BuildCellOwners();
    return Owners.IsValidIndex(CellIndex) ? Owners[CellIndex] : INDEX_NONE;
}

TArray<int32> UNsBackpackGridComponent::GetItemCells(const int32 SlotIndex) const
{
    TArray<int32> Result;
    const FNsBackpackSlot& Slot = GetSlot(SlotIndex);
    if (Slot.IsEmpty())
    {
        return Result;
    }

    const FIntPoint Anchor = GetCellPosition(SlotIndex);
    for (const FIntPoint& Offset : UNsBackpackGridShape::GetItemCells(Slot.Item, GetRotation(SlotIndex)))
    {
        const int32 CellIndex = GetCellIndex(Anchor + Offset);
        if (CellIndex != INDEX_NONE)
        {
            Result.Add(CellIndex);
        }
    }
    return Result;
}

int32 UNsBackpackGridComponent::GetFilledCellCount() const
{
    int32 Count = 0;
    for (const int32 Owner : BuildCellOwners())
    {
        Count += Owner != INDEX_NONE ? 1 : 0;
    }
    return Count;
}

int32 UNsBackpackGridComponent::GetRotation(const int32 SlotIndex) const
{
    return Rotations.IsValidIndex(SlotIndex) ? Rotations[SlotIndex] : 0;
}

int32 UNsBackpackGridComponent::GetCellIndex(const FIntPoint Cell) const
{
    if (Cell.X < 0 || Cell.Y < 0 || Cell.X >= GetGridWidth() || Cell.Y >= GetGridHeight())
    {
        return INDEX_NONE;
    }
    return Cell.Y * GetGridWidth() + Cell.X;
}

FIntPoint UNsBackpackGridComponent::GetCellPosition(const int32 SlotIndex) const
{
    return FIntPoint(SlotIndex % GetGridWidth(), SlotIndex / GetGridWidth());
}

bool UNsBackpackGridComponent::CanAccept_Implementation(const UNsBackpackItemDefinition* const Item, const int32 SlotIndex) const
{
    if (!Super::CanAccept_Implementation(Item, SlotIndex) || Item == nullptr || GetCellIndex(GetCellPosition(SlotIndex)) != SlotIndex)
    {
        return false;
    }

    // The item in the slot itself does not block, because the new item replaces it
    const TArray<int32> Owners = BuildCellOwners();
    const int32 Rotation = CheckRotation != INDEX_NONE ? CheckRotation : GetRotation(SlotIndex);
    const FIntPoint Anchor = GetCellPosition(SlotIndex);
    for (const FIntPoint& Offset : UNsBackpackGridShape::GetItemCells(Item, Rotation))
    {
        const int32 CellIndex = GetCellIndex(Anchor + Offset);
        if (CellIndex == INDEX_NONE)
        {
            return false;
        }
        const int32 Owner = Owners[CellIndex];
        if (Owner != INDEX_NONE && Owner != SlotIndex && Owner != CheckIgnoreSlot)
        {
            return false;
        }
    }
    return true;
}

void UNsBackpackGridComponent::NotifyChanged()
{
    if (CanChange())
    {
        const TArray<FNsBackpackSlot>& AllSlots = GetSlots();
        Rotations.SetNum(AllSlots.Num());
        for (int32 SlotIndex = 0; SlotIndex < AllSlots.Num(); ++SlotIndex)
        {
            if (AllSlots[SlotIndex].IsEmpty())
            {
                Rotations[SlotIndex] = 0;
            }
        }
    }
    Super::NotifyChanged();
}

void UNsBackpackGridComponent::OnRep_Rotations()
{
    Super::NotifyChanged();
}

TArray<int32> UNsBackpackGridComponent::BuildCellOwners() const
{
    TArray<int32> Owners;
    Owners.Init(INDEX_NONE, GetSlotCount());
    for (int32 SlotIndex = 0; SlotIndex < GetSlotCount(); ++SlotIndex)
    {
        for (const int32 CellIndex : GetItemCells(SlotIndex))
        {
            if (Owners[CellIndex] == INDEX_NONE)
            {
                Owners[CellIndex] = SlotIndex;
            }
        }
    }
    return Owners;
}

bool UNsBackpackGridComponent::CanJoin(const int32 FromIndex, const int32 ToIndex) const
{
    const FNsBackpackSlot& From = GetSlot(FromIndex);
    const FNsBackpackSlot& To = GetSlot(ToIndex);
    return !From.IsEmpty() && !To.IsEmpty() && !From.Data.IsValid() && CanStack(To, From.Item) && To.Quantity < FMath::Max(1, From.Item->MaxStack);
}

void UNsBackpackGridComponent::SetRotation(const int32 SlotIndex, const int32 Rotation)
{
    if (Rotations.Num() < GetSlotCount())
    {
        Rotations.SetNum(GetSlotCount());
    }
    if (Rotations.IsValidIndex(SlotIndex))
    {
        Rotations[SlotIndex] = static_cast<uint8>(UNsBackpackGridShape::NormalizeRotation(Rotation));
    }
}
