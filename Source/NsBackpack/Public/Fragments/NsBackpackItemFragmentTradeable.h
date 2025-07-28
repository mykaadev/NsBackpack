
#pragma once

#include "CoreMinimal.h"
#include "NsBackpackItemFragment.h"
#include "NsBackpackItemFragmentTradeable.generated.h"

/**
 * Fragment that defines item tradeable value
 */
UCLASS(DefaultToInstanced, EditInlineNew, BlueprintType, Blueprintable)
class UNsBackpackItemFragmentTradeable : public UNsBackpackItemFragment
{
    GENERATED_BODY()

// Functions
public:

    /** Constructor */
    UNsBackpackItemFragmentTradeable()
        : BuyValue(0)
        , SellValue(0)
    {}

    //~ Begin UNsBackpackItemFragment Interface
    virtual void OnInitialization_Implementation(class UNsBackpackItemInstance* InOwningItem) override {}
    virtual void OnActivation_Implementation(class UNsBackpackItemInstance* InOwningItem) override{}
    virtual void OnDeactivation_Implementation(class UNsBackpackItemInstance* InOwningItem) override{}
    virtual bool CanStackWith_Implementation(const class UNsBackpackItemFragment* InOtherFragment) override { return true; }
    //~ End UNsBackpackItemFragment Interface

    /** Buy Value */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trade")
    int32 BuyValue;

    /** Sell Value */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trade")
    int32 SellValue;
};
