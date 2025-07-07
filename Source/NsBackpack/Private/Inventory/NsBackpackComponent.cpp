// Copyright (c) 2025 mykaa

#include "Inventory/NsBackpackComponent.h"
#include "Inventory/NsBackpackItem.h"
#include "Inventory/NsBackpackSettings.h"

UNsBackpackComponent::UNsBackpackComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    LoadItemDatabase();
}

void UNsBackpackComponent::ExpandBackpack(const int32 InAdditionalSlots)
{
    BackpackSlots += FMath::Max(0, InAdditionalSlots);
}

bool UNsBackpackComponent::GiveItem(UNsBackpackItem* InItem, const int32 InQuantity, const bool InPersistent)
{
    TArray<FNsBackpackItemStack>& Target = InPersistent ? PersistentItems : BackpackItems;
    const int32 MaxSlots = InPersistent ? INDEX_NONE : BackpackSlots;
    return GiveToList(Target, InItem, InQuantity, MaxSlots);
}

bool UNsBackpackComponent::RemoveItem(UNsBackpackItem* InItem, const int32 InQuantity, const bool InPersistent)
{
    TArray<FNsBackpackItemStack>& Target = InPersistent ? PersistentItems : BackpackItems;
    return RemoveFromList(Target, InItem, InQuantity);
}

bool UNsBackpackComponent::GiveItemByID(const int32 InItemID, const int32 InQuantity, const bool InPersistent)
{
    if (UNsBackpackItem* Found = *ItemDatabase.Find(InItemID))
    {
        return GiveItem(Found, InQuantity, InPersistent);
    }
    return false;
}

bool UNsBackpackComponent::RemoveItemByID(const int32 InItemID, const int32 InQuantity, const bool InPersistent)
{
    if (UNsBackpackItem* Found = *ItemDatabase.Find(InItemID))
    {
        return RemoveItem(Found, InQuantity, InPersistent);
    }
    return false;
}

bool UNsBackpackComponent::GiveItemByClass(const TSubclassOf<UNsBackpackItem> InItemClass, const int32 InQuantity, const bool InPersistent)
{
    return InItemClass ? GiveItem(InItemClass->GetDefaultObject<UNsBackpackItem>(), InQuantity, InPersistent) : false;
}

bool UNsBackpackComponent::RemoveItemByClass(const TSubclassOf<UNsBackpackItem> InItemClass, const int32 InQuantity, const bool InPersistent)
{
    return InItemClass ? RemoveItem(InItemClass->GetDefaultObject<UNsBackpackItem>(), InQuantity, InPersistent) : false;
}

bool UNsBackpackComponent::GiveToList(TArray<FNsBackpackItemStack>& InList, UNsBackpackItem* const InItemData, const int32 InQuantity, const int32 InMaxSlots)
{
    if (!InItemData || InQuantity <= 0)
    {
        return false;
    }

    int32 Remaining = InQuantity;

    if (InItemData->bStackable)
    {
        for (FNsBackpackItemStack& Stack : InList)
        {
            if (Stack.Item == InItemData && Stack.Quantity < InItemData->MaxStackSize)
            {
                const int32 Addable = FMath::Min(Remaining, InItemData->MaxStackSize - Stack.Quantity);
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
        if (InMaxSlots != INDEX_NONE && InList.Num() >= InMaxSlots)
        {
            return false; // no space
        }

        const int32 AddAmount = InItemData->bStackable ? FMath::Min(Remaining, InItemData->MaxStackSize) : 1;
        InList.Add(FNsBackpackItemStack(InItemData, AddAmount));
        Remaining -= AddAmount;
        if (!InItemData->bStackable)
        {
            Remaining -= 0; // explicit clarity
        }
    }

    return true;
}

bool UNsBackpackComponent::RemoveFromList(TArray<FNsBackpackItemStack>& InList, UNsBackpackItem* InItem, const int32 InQuantity)
{
    if (!InItem || InQuantity <= 0)
    {
        return false;
    }

    int32 Remaining = InQuantity;

    for (int32 i = 0; i < InList.Num(); ++i)
    {
        FNsBackpackItemStack& Stack = InList[i];
        if (Stack.Item == InItem)
        {
            const int32 RemoveAmount = FMath::Min(Remaining, Stack.Quantity);
            Stack.Quantity -= RemoveAmount;
            Remaining -= RemoveAmount;

            if (Stack.Quantity <= 0)
            {
                InList.RemoveAt(i);
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

void UNsBackpackComponent::LoadItemDatabase()
{
    ItemDatabase.Empty();
    const UNsBackpackSettings* const Settings = UNsBackpackSettings::GetSettings();
    if (!Settings || !Settings->ItemDataTable.IsValid())
    {
        return;
    }

    const UDataTable* const Table = Settings->ItemDataTable.LoadSynchronous();
    if (!Table)
    {
        return;
    }

    TArray<FName> RowNames = Table->GetRowNames();
    for (int32 Index = 0; Index < RowNames.Num(); ++Index)
    {
        if (const FNsBackpackItemRow* const Row = Table->FindRow<FNsBackpackItemRow>(RowNames[Index], TEXT("")))
        {
            if (Row->ItemClass)
            {
                if (UNsBackpackItem* const Item = Row->ItemClass->GetDefaultObject<UNsBackpackItem>())
                {
                    ItemDatabase.Emplace(Item->ID, Item);
                }
            }
        }
    }
}