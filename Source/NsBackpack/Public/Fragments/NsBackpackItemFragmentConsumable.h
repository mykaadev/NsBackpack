
#pragma once

#include "CoreMinimal.h"
#include "NsBackpackItemFragment.h"
#include "NsBackpackItemFragmentConsumable.generated.h"

/**
 * Fragment that defines item Consumable logic
 */
UCLASS(DefaultToInstanced, EditInlineNew, BlueprintType, Blueprintable)
class UNsBackpackItemFragmentConsumable : public UNsBackpackItemFragment
{
    GENERATED_BODY()

// Functions
public:

    /** Constructor */
    UNsBackpackItemFragmentConsumable()
        : MaxUses(1)
        , CurrentUses(1)
    {}

    //~ Begin UNsBackpackItemFragment Interface
    virtual void OnInitialization_Implementation(class UNsBackpackItemInstance* InOwningItem) override { CurrentUses = MaxUses; }
    virtual void OnActivation_Implementation(class UNsBackpackItemInstance* InOwningItem) override {}
    virtual void OnDeactivation_Implementation(class UNsBackpackItemInstance* InOwningItem) override {}
    virtual bool CanStackWith_Implementation(const class UNsBackpackItemFragment* InOtherFragment) override { return true; }
    //~ End UNsBackpackItemFragment Interface

    /** Max number of uses */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Consumable")
    int32 MaxUses;

    /** Current number of uses */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Consumable")
    int32 CurrentUses;
};
