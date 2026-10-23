// Copyright (C) 2025 nulled.softworks. All rights reserved.

#include "NsBackpackGridShape.h"

#include "NsBackpackItemDefinition.h"

TArray<FIntPoint> UNsBackpackGridShape::GetCells(const int32 Rotation) const
{
    int32 Width = FMath::Max(1, Size.X);
    int32 Height = FMath::Max(1, Size.Y);

    // Cells outside the box and second copies of a cell do not count
    TArray<FIntPoint> Result;
    for (const FIntPoint& Cell : Cells)
    {
        if (Cell.X >= 0 && Cell.Y >= 0 && Cell.X < Width && Cell.Y < Height)
        {
            Result.AddUnique(Cell);
        }
    }
    if (Result.IsEmpty())
    {
        for (int32 Y = 0; Y < Height; ++Y)
        {
            for (int32 X = 0; X < Width; ++X)
            {
                Result.Add(FIntPoint(X, Y));
            }
        }
    }

    // A clockwise quarter turn moves the cell (X, Y) of a box of height H to (H - 1 - Y, X)
    for (int32 Turn = 0; Turn < NormalizeRotation(Rotation); ++Turn)
    {
        for (FIntPoint& Cell : Result)
        {
            Cell = FIntPoint(Height - 1 - Cell.Y, Cell.X);
        }
        Swap(Width, Height);
    }
    return Result;
}

TArray<FIntPoint> UNsBackpackGridShape::GetItemCells(const UNsBackpackItemDefinition* const Item, const int32 Rotation)
{
    const UNsBackpackGridShape* const Shape = Item != nullptr ? Item->FindFragment<UNsBackpackGridShape>() : nullptr;
    return Shape != nullptr ? Shape->GetCells(Rotation) : TArray<FIntPoint>{FIntPoint(0, 0)};
}
