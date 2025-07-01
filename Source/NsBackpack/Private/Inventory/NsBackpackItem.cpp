// Copyright (C) 2025 mykaadev. All rights reserved.

#include "Inventory/NsBackpackItem.h"

void FNsBackpackItemRow::OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName)
{
    FTableRowBase::OnDataTableChanged(InDataTable, InRowName);

    UDataTable* const ItemDataTable = const_cast<UDataTable*>(InDataTable);
    if (InDataTable != nullptr)
    {
        TArray<FNsBackpackItemRow*> ItemDataArray;
        InDataTable->GetAllRows<FNsBackpackItemRow>(TEXT(""), ItemDataArray);

        for (int32 Index = 0; Index < ItemDataArray.Num(); ++Index)
        {
            if (ItemDataArray[Index]->ItemClass != nullptr)
            {
                if (UNsBackpackItem* const Item = ItemDataArray[Index]->ItemClass->GetDefaultObject<UNsBackpackItem>())
                {
                    if (Item->ID != Index)
                    {
                        Item->ID = Index;
                        Item->Modify(); // Modify the item itself
                    }
                }
            }
        }

        ItemDataTable->Modify();
    }
}

UNsBackpackItem::UNsBackpackItem()
    : ID(0)
    , Image(nullptr)
    , bStackable(true)
    , MaxStackSize(0)
    , Mesh(nullptr)
    , ActorToSpawn(nullptr)
{}

void UNsBackpackItem::Use_Implementation(AActor* const InItemOwner)
{
    // Default does nothing
}
