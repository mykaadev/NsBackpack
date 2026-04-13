
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NsBackpackItemFragment.generated.h"

/**
 * Item Fragment class
 */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class NSBACKPACK_API UNsBackpackItemFragment : public UObject
{
    GENERATED_BODY()

// Functions
public:

    /** Constructor */
    UNsBackpackItemFragment() {}

    /** On Initialization */
    UFUNCTION(BlueprintNativeEvent)
    void OnInitialization(class UNsBackpackItemInstance* InOwningItem);
    virtual void OnInitialization_Implementation(class UNsBackpackItemInstance* InOwningItem) {}

    /** On Activate */
    UFUNCTION(BlueprintNativeEvent)
    void OnActivation(class UNsBackpackItemInstance* InOwningItem);
    virtual void OnActivation_Implementation(class UNsBackpackItemInstance* InOwningItem) {}

    /** On Deactivate */
    UFUNCTION(BlueprintNativeEvent)
    void OnDeactivation(class UNsBackpackItemInstance* InOwningItem);
    virtual void OnDeactivation_Implementation(class UNsBackpackItemInstance* InOwningItem) {}

    /** Can stack with */
    UFUNCTION(BlueprintNativeEvent)
    bool CanStackWith(const class UNsBackpackItemFragment* InOtherFragment);
    virtual bool CanStackWith_Implementation(const UNsBackpackItemFragment* InOtherFragment) { return true; }
};
