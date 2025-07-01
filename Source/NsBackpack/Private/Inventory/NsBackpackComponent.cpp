// Copyright (c) 2025 mykaa
#include "Inventory/NsBackpackComponent.h"

UNsBackpackComponent::UNsBackpackComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UNsBackpackComponent::ExpandBackpack(int32 AdditionalSlots)
{
    BackpackSlots += FMath::Max(0, AdditionalSlots);
}

bool UNsBackpackComponent::GiveItem(UNsBackpackItemData* ItemData, int32 Quantity, bool bPersistent)
{
    TArray<FNsBackpackItemStack>& Target = bPersistent ? PersistentItems : BackpackItems;
    int32 MaxSlots = bPersistent ? INDEX_NONE : BackpackSlots;
    return GiveToList(Target, ItemData, Quantity, MaxSlots);
}

bool UNsBackpackComponent::RemoveItem(UNsBackpackItemData* ItemData, int32 Quantity, bool bPersistent)
{
    TArray<FNsBackpackItemStack>& Target = bPersistent ? PersistentItems : BackpackItems;
    return RemoveFromList(Target, ItemData, Quantity);
}

bool UNsBackpackComponent::GiveItemByID(int32 ItemID, int32 Quantity, bool bPersistent)
{
    if (UNsBackpackItemData** Found = ItemDatabase.Find(ItemID))
    {
        return GiveItem(*Found, Quantity, bPersistent);
    }
    return false;
}

bool UNsBackpackComponent::RemoveItemByID(int32 ItemID, int32 Quantity, bool bPersistent)
{
    if (UNsBackpackItemData** Found = ItemDatabase.Find(ItemID))
    {
        return RemoveItem(*Found, Quantity, bPersistent);
    }
    return false;
}

bool UNsBackpackComponent::GiveToList(TArray<FNsBackpackItemStack>& List, UNsBackpackItemData* ItemData, int32 Quantity, int32 MaxSlots)
{
    if (!ItemData || Quantity <= 0)
    {
        return false;
    }

    int32 Remaining = Quantity;

    if (ItemData->bStackable)
    {
        for (FNsBackpackItemStack& Stack : List)
        {
            if (Stack.Item == ItemData && Stack.Quantity < ItemData->MaxStackSize)
            {
                int32 Addable = FMath::Min(Remaining, ItemData->MaxStackSize - Stack.Quantity);
                Stack.Quantity += Addable;
                Remaining -= Addable;
                if (Remaining <= 0)
                {
                    return true;
                }
            }
        }
    }

    while (Remaining > 0)
    {
        if (MaxSlots != INDEX_NONE && List.Num() >= MaxSlots)
        {
            return false; // no space
        }

        int32 AddAmount = ItemData->bStackable ? FMath::Min(Remaining, ItemData->MaxStackSize) : 1;
        List.Add(FNsBackpackItemStack(ItemData, AddAmount));
        Remaining -= AddAmount;
        if (!ItemData->bStackable)
        {
            Remaining -= 0; // explicit clarity
        }
    }

    return true;
}

bool UNsBackpackComponent::RemoveFromList(TArray<FNsBackpackItemStack>& List, UNsBackpackItemData* ItemData, int32 Quantity)
{
    if (!ItemData || Quantity <= 0)
    {
        return false;
    }

    int32 Remaining = Quantity;

    for (int32 i = 0; i < List.Num(); ++i)
    {
        FNsBackpackItemStack& Stack = List[i];
        if (Stack.Item == ItemData)
        {
            int32 RemoveAmount = FMath::Min(Remaining, Stack.Quantity);
            Stack.Quantity -= RemoveAmount;
            Remaining -= RemoveAmount;

            if (Stack.Quantity <= 0)
            {
                List.RemoveAt(i);
                --i;
            }

            if (Remaining <= 0)
            {
                return true;
            }
        }
    }

    return Remaining <= 0;
}

