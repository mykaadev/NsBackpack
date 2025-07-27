
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NsBackpackItemFragment.generated.h"

/**
 * Item Fragment class
 */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class UNsBackpackItemFragment : public UObject
{
    GENERATED_BODY()

// Functions
public:

    /** Constructor */
    UNsBackpackItemFragment() {}

    /** On Initialization */
    virtual void OnInitialization(class UNsBackpackItemInstance* InOwningItem) {}

    /** On Activate */
    virtual void OnActivation(class UNsBackpackItemInstance* InOwningItem) {}

    /** On Deactivate */
    virtual void OnDeactivation(class UNsBackpackItemInstance* InOwningItem) {}

    /** Can stack with */
    virtual bool CanStackWith(const UNsBackpackItemFragment* InOtherFragment) const { return true; }
};
