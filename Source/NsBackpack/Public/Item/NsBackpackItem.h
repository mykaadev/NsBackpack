// Copyright (C) 2025 nulled.softworks. All rights reserved.

#pragma once

#include "CoreMinimal.h"
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
    
// Functions
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

        
// Variables
public:

    /** The display name of the item */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item | Display")
    FText Name;

    /** Flavor text or a summary */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item | Display")
    FText Description;

    /** The 3D model for the item in the world */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item | Visuals")
    TSoftObjectPtr<UStaticMesh> WorldMesh;

    /** The 2D UI icon */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item | Visuals")
    TSoftObjectPtr<UTexture2D> Icon2D;

    /** Maximum stack count for this item type */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item | Stacking", meta=(ClampMin="1"))
    int32 StackSizeMax = 1;

    /** An array of fragments that build the item itself */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category="Item | Functionality")
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
class NSBACKPACK_API UNsBackpackItemInstance : public UObject
{
    GENERATED_BODY()

// Functions
public:

    /** Constructor */
    UNsBackpackItemInstance();

    /** Get Item Definition */
    UFUNCTION(BlueprintCallable, Category = "Item")
    class UNsBackpackItemDefinition* GetItemDefinition() const;

    /** Initializes the item and their fragments */
    UFUNCTION(BlueprintCallable, Category = "Item")
    void Initialize(class UNsBackpackItemDefinition* Def, const int32 InQuantity = 1);

    /** Activate Item */
    UFUNCTION(BlueprintCallable, Category = "Item")
    void ActivateItem();

    /** Deactivate Item */
    UFUNCTION(BlueprintCallable, Category = "Item")
    void DeactivateItem();

    /** Get the quantity of this item */
    UFUNCTION(BlueprintCallable, Category = "Item")
    int32 GetQuatity() const;

    /** Get the inventory that this item resides in */
    UFUNCTION(BlueprintCallable, Category = "Item")
    class UNsBackpackComponent* GetBackpackComponent() const;

    /** Get the inventory owner and owner of this item */
    UFUNCTION(BlueprintCallable, Category = "Item")
    class AActor* GetBackpackOwner() const;

    /** Set owning backpack */
    void SetBackpackComponent(class UNsBackpackComponent* InBackpack);

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

// Variables
protected:

    /** Original Item definition data asset */
    UPROPERTY(BlueprintReadOnly, Category="Item")
    TObjectPtr<class UNsBackpackItemDefinition> ItemDefinition;

    /** Item Fragments copied from the item definition */
    UPROPERTY(BlueprintReadOnly, Category="Item")
    TArray<TObjectPtr<class UNsBackpackItemFragment>> ItemFragments;

    /** Quantity of item */
    UPROPERTY(BlueprintReadOnly, Category="Item", meta=(ClampMin="1"))
    int32 Quantity;

    /** Backpack component */
    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<class UNsBackpackComponent> BackpackComponent;
};
