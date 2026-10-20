// Copyright (C) 2025 nulled.softworks. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NsBackpackItemDefinition.generated.h"

/**
 * One kind of item: its name, its look, its stack size, and its fragments
 * Every slot that holds this item points at this asset. Game rules live in fragments that the game makes
 */
UCLASS(BlueprintType)
class NSBACKPACK_API UNsBackpackItemDefinition : public UPrimaryDataAsset
{
    GENERATED_BODY()

// Functions
public:
    //~ Begin UObject Interface
#if WITH_EDITOR
    virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
    //~ End UObject Interface

    /** Returns the first fragment of this class, or null */
    UFUNCTION(BlueprintPure, Category = "Item", meta = (DeterminesOutputType = "FragmentClass"))
    class UNsBackpackItemFragment* FindFragment(TSubclassOf<class UNsBackpackItemFragment> FragmentClass) const;

    /** Returns the first fragment of this class, or null */
    template <typename T>
    const T* FindFragment() const
    {
        return Cast<T>(FindFragment(T::StaticClass()));
    }

// Variables
public:
    /** Name that the UI shows */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FText DisplayName;

    /** One line about the item */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (MultiLine = true))
    FText Description;

    /** Picture of the item in the UI */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    TSoftObjectPtr<class UTexture2D> Icon;

    /** Mesh of the item in the world */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    TSoftObjectPtr<class UStaticMesh> WorldMesh;

    /** Most items in one slot */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (ClampMin = "1"))
    int32 MaxStack = 1;

    /** Game data of the item, one fragment for each feature */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Item")
    TArray<TObjectPtr<class UNsBackpackItemFragment>> Fragments;
};
