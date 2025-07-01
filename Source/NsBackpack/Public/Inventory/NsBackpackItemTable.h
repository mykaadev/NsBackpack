#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NsBackpackItemTable.generated.h"

UCLASS(BlueprintType)
class NSBACKPACK_API UNsBackpackItemTable : public UDataTable
{
    GENERATED_BODY()
public:
#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
