// Copyright (C) 2025 nulled.softworks. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "NsBackpackItemFragment.h"
#include "NsBackpackGridShape.generated.h"

/**
 * The cells that an item takes in a grid backpack, such as 1 x 1, 2 x 2, or an L
 * An item without this fragment takes one cell. A rotation turns the shape by quarter turns, clockwise
 */
UCLASS(meta = (DisplayName = "Grid Shape"))
class NSBACKPACKGRID_API UNsBackpackGridShape : public UNsBackpackItemFragment
{
    GENERATED_BODY()

// Functions
public:
    /** Returns the cells of the shape after a number of quarter turns, from the top left of its box */
    TArray<FIntPoint> GetCells(int32 Rotation) const;

    /** Returns the cells of an item after a number of quarter turns. An item without a grid shape takes one cell */
    static TArray<FIntPoint> GetItemCells(const class UNsBackpackItemDefinition* Item, int32 Rotation);

    /** Returns a rotation in the range 0 to 3 */
    static int32 NormalizeRotation(int32 Rotation) { return ((Rotation % 4) + 4) % 4; }

// Variables
public:
    /** Width and height of the box of the shape, in cells */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid", meta = (ClampMin = "1"))
    FIntPoint Size = FIntPoint(1, 1);

    /** Cells of the shape inside the box, from the top left. Leave it empty to fill the whole box */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid")
    TArray<FIntPoint> Cells;
};
