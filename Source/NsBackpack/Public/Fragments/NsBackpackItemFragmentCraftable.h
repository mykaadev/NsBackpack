
#pragma once

#include "CoreMinimal.h"
#include "NsBackpackItemFragment.h"
#include "NsBackpackItemFragmentCraftable.generated.h"

/**
 * Fragment that defines item Craftable
 */
UCLASS(DefaultToInstanced, EditInlineNew, BlueprintType, Blueprintable)
class UNsBackpackItemFragmentCraftable : public UNsBackpackItemFragment
{
    GENERATED_BODY()

// Functions
public:

    /** Constructor */
    UNsBackpackItemFragmentCraftable()
    {}

    //~ Begin UNsBackpackItemFragment Interface
    virtual void OnInitialization_Implementation(class UNsBackpackItemInstance* InOwningItem) override {}
    virtual void OnActivation_Implementation(class UNsBackpackItemInstance* InOwningItem) override{}
    virtual void OnDeactivation_Implementation(class UNsBackpackItemInstance* InOwningItem) override{}
    virtual bool CanStackWith_Implementation(const class UNsBackpackItemFragment* InOtherFragment) override { return true; }
    //~ End UNsBackpackItemFragment Interface

    /** Crafting Recipes */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crafting")
    TMap<TSoftClassPtr<class UNsBackpackItemDefinition>, int32> Recipe;
};
