
#pragma once

#include "CoreMinimal.h"
#include "NsBackpackItemFragment.h"
#include "NsBackpackItemFragmentAbility.generated.h"

/**
 * Simple class that holds item abilities
 */
UCLASS(Blueprintable, BlueprintType)
class UNsBackpackItemAbility : public UObject
{
    GENERATED_BODY()

// Functions
public:

    /** Constructor */
    UNsBackpackItemAbility() {}

    /** On Execute */
    UFUNCTION(BlueprintNativeEvent)
    void OnExecute(class UNsBackpackItemInstance* InOwningItem);
    virtual void OnExecute_Implementation(class UNsBackpackItemInstance* InOwningItem) {}
};

/**
 * Fragment that defines item Ability logic
 */
UCLASS(DefaultToInstanced, EditInlineNew, BlueprintType, Blueprintable)
class UNsBackpackItemFragmentAbility : public UNsBackpackItemFragment
{
    GENERATED_BODY()

// Functions
public:

    /** Constructor */
    UNsBackpackItemFragmentAbility()
    {}

    //~ Begin UNsBackpackItemFragment Interface
    virtual void OnInitialization_Implementation(class UNsBackpackItemInstance* InOwningItem) override {}
    virtual void OnActivation_Implementation(class UNsBackpackItemInstance* InOwningItem) override
    {
        if (const UClass* const AbilityClass = Ability.LoadSynchronous())
        {
            if (UNsBackpackItemAbility* const DefaultAbility = AbilityClass->GetDefaultObject<UNsBackpackItemAbility>())
            {
                DefaultAbility->OnExecute(InOwningItem);
            }
        }
    }
    virtual void OnDeactivation_Implementation(class UNsBackpackItemInstance* InOwningItem) override {}
    virtual bool CanStackWith_Implementation(const class UNsBackpackItemFragment* InOtherFragment) override { return true; }
    //~ End UNsBackpackItemFragment Interface

    /** Ability to execute */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
    TSoftClassPtr<class UNsBackpackItemAbility> Ability;
};