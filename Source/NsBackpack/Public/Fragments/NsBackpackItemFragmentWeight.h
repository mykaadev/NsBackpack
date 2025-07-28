
#pragma once

#include "CoreMinimal.h"
#include "NsBackpackItemFragment.h"
#include "NsBackpackItemFragmentWeight.generated.h"

/**
 * Fragment that defines item weight
 */
UCLASS(DefaultToInstanced, EditInlineNew, BlueprintType, Blueprintable)
class UNsBackpackItemFragmentWeight : public UNsBackpackItemFragment
{
    GENERATED_BODY()

// Functions
public:

    /** Constructor */
    UNsBackpackItemFragmentWeight()
        : Weight(1.f)
    {}

    //~ Begin UNsBackpackItemFragment Interface
    virtual void OnInitialization_Implementation(class UNsBackpackItemInstance* InOwningItem) override {}
    virtual void OnActivation_Implementation(class UNsBackpackItemInstance* InOwningItem) override{}
    virtual void OnDeactivation_Implementation(class UNsBackpackItemInstance* InOwningItem) override{}
    virtual bool CanStackWith_Implementation(const class UNsBackpackItemFragment* InOtherFragment) override { return true; }
    //~ End UNsBackpackItemFragment Interface

    /** Weight of the item in kilograms */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weight")
    float Weight;
};
