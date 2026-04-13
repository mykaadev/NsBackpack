
#pragma once

#include "CoreMinimal.h"
#include "NsBackpackComponent.h"
#include "NsBackpackItemFragment.h"
#include "Item/NsBackpackItem.h"
#include "NsBackpackItemFragmentConsumable.generated.h"

/**
 * Fragment that defines item Consumable logic
 */
UCLASS(DefaultToInstanced, EditInlineNew, BlueprintType, Blueprintable)
class NSBACKPACK_API UNsBackpackItemFragmentConsumable : public UNsBackpackItemFragment
{
    GENERATED_BODY()

// Functions
public:

    /** Constructor */
    UNsBackpackItemFragmentConsumable()
        : MaxUses(1)
        , CurrentUses(0)
    {}

    //~ Begin UNsBackpackItemFragment Interface
    virtual void OnInitialization_Implementation(class UNsBackpackItemInstance* InOwningItem) override { CurrentUses = MaxUses; }
    virtual void OnActivation_Implementation(class UNsBackpackItemInstance* InOwningItem) override
    {
        ++CurrentUses;
        if (CurrentUses >= MaxUses)
        {
            OnDeactivation(InOwningItem);
        }
    }
    virtual void OnDeactivation_Implementation(class UNsBackpackItemInstance* InOwningItem) override
    {
        if (InOwningItem != nullptr)
        {
            if (UNsBackpackComponent* const BackpackComponent = InOwningItem->GetBackpackComponent())
            {
                BackpackComponent->RemoveItemByClass(InOwningItem->GetItemDefinition());
            }
        }
    }
    virtual bool CanStackWith_Implementation(const class UNsBackpackItemFragment* InOtherFragment) override { return true; }
    //~ End UNsBackpackItemFragment Interface

    /** Max number of uses */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Consumable")
    int32 MaxUses;

    /** Current number of uses */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Consumable")
    int32 CurrentUses;
};
