// Copyright (C) 2025 nulled.softworks. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NsBackpackItemFragment.generated.h"

/**
 * Data of one item feature, such as a tool type, a weight, or a durability
 * The game makes the fragment classes and reads them. The plugin only stores them on the item definition
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced, CollapseCategories)
class NSBACKPACK_API UNsBackpackItemFragment : public UObject
{
    GENERATED_BODY()
};
