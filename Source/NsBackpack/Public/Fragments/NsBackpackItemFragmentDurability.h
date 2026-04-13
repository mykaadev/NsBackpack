
#pragma once

#include "CoreMinimal.h"
#include "NsBackpackItemFragment.h"
#include "NsBackpackItemFragmentDurability.generated.h"

/**
 * Fragment that defines item Durability logic
 */
UCLASS(DefaultToInstanced, EditInlineNew, BlueprintType, Blueprintable)
class NSBACKPACK_API UNsBackpackItemFragmentDurability : public UNsBackpackItemFragment
{
    GENERATED_BODY()

// Functions
public:

    /** Constructor */
    UNsBackpackItemFragmentDurability()
        : MaxDurability(100)
        , CurrentDurability(0)
    {}

    //~ Begin UNsBackpackItemFragment Interface
    virtual void OnInitialization_Implementation(class UNsBackpackItemInstance* InOwningItem) override { CurrentDurability = MaxDurability; }
    virtual void OnActivation_Implementation(class UNsBackpackItemInstance* InOwningItem) override {}
    virtual void OnDeactivation_Implementation(class UNsBackpackItemInstance* InOwningItem) override {}
    virtual bool CanStackWith_Implementation(const class UNsBackpackItemFragment* InOtherFragment) override
    {
        if (const UNsBackpackItemFragmentDurability* const DurabilityFragment = Cast<UNsBackpackItemFragmentDurability>(InOtherFragment))
        {
            // Only allow stacking if max durability matches and both are at full durability
            return MaxDurability == DurabilityFragment->MaxDurability
                && CurrentDurability == MaxDurability
                && DurabilityFragment->CurrentDurability == DurabilityFragment->MaxDurability;
        }

        // Different fragment type – assume stackable
        return true;
    }
    //~ End UNsBackpackItemFragment Interface

    /** Max number of durability */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Durability")
    int32 MaxDurability;

    /** Current number of durability */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Durability")
    int32 CurrentDurability;
};
