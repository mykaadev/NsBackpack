// Copyright (C) 2025 mykaadev. All rights reserved.

#include "Item/NsBackpackItem.h"
#include "Fragments/NsBackpackItemFragment.h"
#include "Misc/DataValidation.h"
#include "NsBackpackComponent.h"
#include "NsBackpackTypeLibrary.h"
#if WITH_EDITOR
#include "UObject/ObjectSaveContext.h"
#endif

UNsBackpackItemDefinition::UNsBackpackItemDefinition()
    : ID(-1)
    , ItemType(ENsBackpackItemCategory::Miscellaneous)
    , Rarity(ENsBackpackItemRarity::Common)
{
}

#if WITH_EDITOR
void UNsBackpackItemDefinition::PostLoad()
{
    Super::PostLoad();

    // Remove any null fragments
    Fragments.RemoveAll([](const TObjectPtr<UNsBackpackItemFragment>& Fragment)
    {
        return Fragment == nullptr;
    });
}

void UNsBackpackItemDefinition::PreSave(FObjectPreSaveContext SaveContext)
{
    Super::PreSave(SaveContext);

    // Remove any null fragments before saving
    Fragments.RemoveAll([](const TObjectPtr<UNsBackpackItemFragment>& Fragment)
    {
        return Fragment == nullptr;
    });
}

EDataValidationResult UNsBackpackItemDefinition::IsDataValid(FDataValidationContext &Context) const
{
    const EDataValidationResult Result = Super::IsDataValid(Context);

    bool bHasNullFragments = false;
    for (int32 Index = 0; Index < Fragments.Num(); ++Index)
    {
        if (!Fragments[Index])
        {
            Context.AddError(FText::Format( NSLOCTEXT("ItemDefinition", "NullFragmentError", "Null fragment at index {0}"), FText::AsNumber(Index)));
            bHasNullFragments = true;
        }
    }

    return bHasNullFragments ? EDataValidationResult::Invalid : Result;
}

/***********************************************************************************************
 *                                        Item Instance
 ***********************************************************************************************/

void UNsBackpackItemInstance::Initialize(class UNsBackpackItemDefinition* Def, const int32 InQuantity)
{
    ItemDefinition = Def;
    Quantity = FMath::Clamp(InQuantity, 1, Def->StackSizeMax);

    // Instance fragments from Item Definition
    ItemFragments.Empty();
    for (const UNsBackpackItemFragment* const Fragment : Def->Fragments)
    {
        if (Fragment)
        {
            UNsBackpackItemFragment* InstancedFragment = DuplicateObject(Fragment, this);
            InstancedFragment->OnInitialization(this);
            ItemFragments.Emplace(InstancedFragment);
        }
    }
}

void UNsBackpackItemInstance::ActivateItem()
{
    for (int32 Index = 0; Index < ItemFragments.Num(); ++Index)
    {
        if (ItemFragments[Index] != nullptr)
        {
            ItemFragments[Index]->OnActivation(this);
        }
    }
}

void UNsBackpackItemInstance::DeactivateItem()
{
    for (int32 Index = 0; Index < ItemFragments.Num(); ++Index)
    {
        if (ItemFragments[Index] != nullptr)
        {
            ItemFragments[Index]->OnDeactivation(this);
        }
    }
}

bool UNsBackpackItemInstance::CanStackWith(const UNsBackpackItemInstance* InOtherItemInstance) const
{
    if (ItemDefinition != nullptr && ItemDefinition->StackSizeMax <= 0)
    {
        return false;
    }

    for (int32 Index = 0; Index < ItemFragments.Num(); ++Index)
    {
        if (ItemFragments[Index] != nullptr)
        {
            if (!ItemFragments[Index]->CanStackWith(InOtherItemInstance->ItemFragments[Index]))
            {
                return false;
            }
        }
    }

    return false;
}

class UNsBackpackComponent* UNsBackpackItemInstance::GetBackpackComponent() const
{
    return Backpack;
}

class AActor* UNsBackpackItemInstance::GetBackpackOwner() const
{
    if (Backpack != nullptr)
    {
        if (AActor* const Owner = Backpack->GetOwner())
        {
            return Owner;
        }
    }

    return nullptr;
}

#endif
