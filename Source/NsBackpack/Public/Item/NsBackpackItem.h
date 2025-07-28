// Copyright (C) 2025 mykaadev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "NsBackpackTypeLibrary.h"
#include "UObject/Object.h"
#include "NsBackpackItem.generated.h"

/**
 * Base inventory item object with overrideable behaviour.
 */
UCLASS(Blueprintable, BlueprintType)
class NSBACKPACK_API UNsBackpackItemDefinition : public UPrimaryDataAsset
{
    GENERATED_BODY()
public:

    /** Constructor */
    UNsBackpackItemDefinition();

    //~ Begin UPrimaryDataAsset Interface
#if WITH_EDITOR
    virtual void PostLoad() override;
    virtual void PreSave(FObjectPreSaveContext SaveContext) override;
    virtual EDataValidationResult IsDataValid(class FDataValidationContext &Context) const override;
#endif
    //~ End UPrimaryDataAsset Interface

    /** The display name of the item */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Display")
    FText Name;

    /** Flavor text or a summary */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Display")
    FText Description;

    /** A gameplay tag categorizing the item */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Categorization")
    ENsBackpackItemCategory ItemType;

    /** How common or rare the item is */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Categorization")
    ENsBackpackItemRarity Rarity;

    /** The 3D model for the item in the world */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visuals")
    TSoftObjectPtr<UStaticMesh> WorldMesh;

    /** The 2D UI icon */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visuals")
    TSoftObjectPtr<UTexture2D> Icon2D;

    /** Maximum stack count for this item type */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stacking", meta=(ClampMin="1"))
    int32 StackSizeMax = 1;

    /** An array of fragments that build the item itself */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category="Functionality")
    TArray<TObjectPtr<class UNsBackpackItemFragment>> Fragments;

    /** Get Fragment */
    template <typename T>
    static const T* GetFragment(UNsBackpackItemDefinition* InItemDefinition)
    {
        for (auto Fragment : InItemDefinition->Fragments)
        {
            if (auto Talent = Cast<T>(Fragment))
            {
                return Talent;
            }
        }

        return nullptr;
    }
};

/**
 * Definition of an item instance
 */
UCLASS(Blueprintable, BlueprintType)
class UNsBackpackItemInstance : public UObject
{
    GENERATED_BODY()

public:

    /** Constructor */
    UNsBackpackItemInstance()
        : Backpack(nullptr)
    {}

    /** Get Item Definition */
    class UNsBackpackItemDefinition* GetItemDefinition() const { return ItemDefinition; }

    /** Initializes the item and their fragments */
    void Initialize(class UNsBackpackItemDefinition* Def, const int32 InQuantity = 1);

    // Get the quantity of the item
    UFUNCTION(BlueprintCallable, Category = "Item")
    int32 GetQuatity() const { return Quantity; }

    /** Activate Item */
    void ActivateItem();

    /** Deactivate Item */
    void DeactivateItem();

    /** Get the inventory that this item resides in */
    UFUNCTION(BlueprintCallable, Category = "Item")
    class UNsBackpackComponent* GetBackpackComponent() const;

    /** Get the inventory owner and owner of this item */
    UFUNCTION(BlueprintCallable, Category = "Item")
    class AActor* GetBackpackOwner() const;

    /** Set owning backpack */
    void SetBackpackComponent(class UNsBackpackComponent* InBackpack) { Backpack = InBackpack; }

    /** Add to the stack quantity */
    void AddQuantity(int32 Amount);

    /** Remove from the stack quantity */
    void RemoveQuantity(int32 Amount);

    /** Whether the stack has reached the max quantity */
    bool IsStackFull() const;

private:

    /** Weather it can stack with other item */
    bool CanStackWith(const UNsBackpackItemInstance* InOther) const;

    /** Get Fragment */
    template <typename T>
    const T* GetFragment()
    {
        for (const UNsBackpackItemFragment* Frag : ItemFragments)
        {
            if (auto Found = Cast<T>(Frag))
            {
                return Found;
            }
        }

        return nullptr;
    }

protected:

    /** Original Item definition data asset */
    UPROPERTY(BlueprintReadOnly, Category="Item")
    TObjectPtr<class UNsBackpackItemDefinition> ItemDefinition;

    /** Item Fragments copied from the item definition */
    UPROPERTY(BlueprintReadOnly, Category="Item")
    TArray<TObjectPtr<class UNsBackpackItemFragment>> ItemFragments;

    /** Quantity of item */
    UPROPERTY(BlueprintReadOnly, Category="Item", meta=(ClampMin="1"))
    int32 Quantity = 1;

    /** Backpack component */
    UPROPERTY(BlueprintReadOnly)
    class UNsBackpackComponent* Backpack;
};
