
#pragma once

#include "CoreMinimal.h"
#include "NsBackpackItemFragment.h"
#include "NsBackpackItemFragmentDeconstruct.generated.h"

/**
 * Fragment that defines item Deconstruct
 */
UCLASS(DefaultToInstanced, EditInlineNew, BlueprintType, Blueprintable)
class UNsBackpackItemFragmentDeconstruct : public UNsBackpackItemFragment
{
    GENERATED_BODY()

// Functions
public:

    /** Constructor */
    UNsBackpackItemFragmentDeconstruct()
    {}

    //~ Begin UNsBackpackItemFragment Interface
    virtual void OnInitialization_Implementation(class UNsBackpackItemInstance* InOwningItem) override {}
    virtual void OnActivation_Implementation(class UNsBackpackItemInstance* InOwningItem) override {}
    virtual void OnDeactivation_Implementation(class UNsBackpackItemInstance* InOwningItem) override {}
    virtual bool CanStackWith_Implementation(const class UNsBackpackItemFragment* InOtherFragment) override
    {
        if (const UNsBackpackItemFragmentDeconstruct* const DeconstructFragment = Cast<UNsBackpackItemFragmentDeconstruct>(InOtherFragment))
        {
            const TMap<TSoftClassPtr<UNsBackpackItemDefinition>, int32>& OtherMap = DeconstructFragment->DeconstructList;

            // Step 1: Check map sizes
            if (DeconstructList.Num() != OtherMap.Num())
            {
                return false;
            }

            // Step 2: Check each key-value pair
            for (const TPair<TSoftClassPtr<UNsBackpackItemDefinition>, int32>& Pair : DeconstructList)
            {
                const TSoftClassPtr<UNsBackpackItemDefinition>& Key = Pair.Key;
                const int32* const FoundValue = OtherMap.Find(Key);

                // If key not found or value doesn't match, maps aren't equal
                if (!FoundValue || *FoundValue != Pair.Value)
                {
                    return false;
                }
            }

            // All checks passed, maps are equal
            return true;
        }

        // Default fallback: consider stackable if other fragment isn't of the same type
        return true;
    }
    //~ End UNsBackpackItemFragment Interface

    /** Deconstructs */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Deconstruct")
    TMap<TSoftClassPtr<class UNsBackpackItemDefinition>, int32> DeconstructList;
};
