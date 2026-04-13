
#pragma once

#include "CoreMinimal.h"
#include "NsBackpackItemFragment.h"
#include "NsBackpackItemFragmentSocket.generated.h"

/**
 * Fragment that defines item Socket
 */
UCLASS(DefaultToInstanced, EditInlineNew, BlueprintType, Blueprintable)
class NSBACKPACK_API UNsBackpackItemFragmentSocket : public UNsBackpackItemFragment
{
    GENERATED_BODY()

// Functions
public:

    /** Constructor */
    UNsBackpackItemFragmentSocket()
        : MaxSockets(1)
    {}

    //~ Begin UNsBackpackItemFragment Interface
    virtual void OnInitialization_Implementation(class UNsBackpackItemInstance* InOwningItem) override {}
    virtual void OnActivation_Implementation(class UNsBackpackItemInstance* InOwningItem) override {}
    virtual void OnDeactivation_Implementation(class UNsBackpackItemInstance* InOwningItem) override {}
    virtual bool CanStackWith_Implementation(const class UNsBackpackItemFragment* InOtherFragment) override
    {
        if (const UNsBackpackItemFragmentSocket* const SocketFragment = Cast<UNsBackpackItemFragmentSocket>(InOtherFragment))
        {
            return SocketFragment->MaxSockets == MaxSockets;
        }
        return true;
    }
    //~ End UNsBackpackItemFragment Interface

    /** Sockets */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sockets")
    TArray<TSoftClassPtr<class UNsBackpackItemDefinition>> Sockets;

    /** Max Sockets */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sockets")
    int32 MaxSockets;
};
