// Copyright (C) 2025 nulled.softworks. All rights reserved.

#include "NsBackpackGridComponent.h"
#include "NsBackpackGridShape.h"
#include "NsBackpackItemDefinition.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace NsBackpackGridTest
{
    /** Makes an item with a grid shape */
    UNsBackpackItemDefinition* MakeItem(const FIntPoint Size, const TArray<FIntPoint>& Cells = {}, const int32 MaxStack = 1)
    {
        UNsBackpackItemDefinition* const Item = NewObject<UNsBackpackItemDefinition>();
        Item->MaxStack = MaxStack;
        UNsBackpackGridShape* const Shape = NewObject<UNsBackpackGridShape>(Item);
        Shape->Size = Size;
        Shape->Cells = Cells;
        Item->Fragments.Add(Shape);
        return Item;
    }
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNsBackpackGridShapeTest, "NsBackpack.Grid.Shapes",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FNsBackpackGridShapeTest::RunTest(const FString& Parameters)
{
    UNsBackpackItemDefinition* const Plain = NewObject<UNsBackpackItemDefinition>();
    TestEqual(TEXT("An item without a shape takes one cell"), UNsBackpackGridShape::GetItemCells(Plain, 1).Num(), 1);

    UNsBackpackItemDefinition* const Pole = NsBackpackGridTest::MakeItem(FIntPoint(1, 3));
    const TArray<FIntPoint> Standing = UNsBackpackGridShape::GetItemCells(Pole, 0);
    TestTrue(TEXT("The pole stands in one column"), Standing.Contains(FIntPoint(0, 2)) && !Standing.Contains(FIntPoint(1, 0)));
    const TArray<FIntPoint> Lying = UNsBackpackGridShape::GetItemCells(Pole, 1);
    TestTrue(TEXT("A quarter turn lays the pole in one row"), Lying.Contains(FIntPoint(2, 0)) && !Lying.Contains(FIntPoint(0, 1)));

    // The L covers the top left, the bottom left, and the bottom right of a 2 x 2 box
    UNsBackpackItemDefinition* const Hook = NsBackpackGridTest::MakeItem(FIntPoint(2, 2), {FIntPoint(0, 0), FIntPoint(0, 1), FIntPoint(1, 1)});
    const TArray<FIntPoint> Turned = UNsBackpackGridShape::GetItemCells(Hook, 1);
    TestEqual(TEXT("The L keeps three cells"), Turned.Num(), 3);
    TestTrue(TEXT("A quarter turn moves the L to the top row and the bottom left"),
        Turned.Contains(FIntPoint(0, 0)) && Turned.Contains(FIntPoint(1, 0)) && Turned.Contains(FIntPoint(0, 1)) && !Turned.Contains(FIntPoint(1, 1)));
    TestEqual(TEXT("Four quarter turns give the first shape"), UNsBackpackGridShape::GetItemCells(Hook, 4), UNsBackpackGridShape::GetItemCells(Hook, 0));
    TestEqual(TEXT("A negative turn counts backward"), UNsBackpackGridShape::GetItemCells(Hook, -1), UNsBackpackGridShape::GetItemCells(Hook, 3));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNsBackpackGridComponentTest, "NsBackpack.Grid.Component",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FNsBackpackGridComponentTest::RunTest(const FString& Parameters)
{
    // A backpack without an owner can change, so the test needs no world
    UNsBackpackGridComponent* const Pack = NewObject<UNsBackpackGridComponent>();
    UNsBackpackItemDefinition* const Idol = NsBackpackGridTest::MakeItem(FIntPoint(2, 2));
    UNsBackpackItemDefinition* const Pole = NsBackpackGridTest::MakeItem(FIntPoint(1, 3));
    UNsBackpackItemDefinition* const Coin = NewObject<UNsBackpackItemDefinition>();
    Coin->MaxStack = 5;
    Pack->ResetGrid(4, 4);

    TestEqual(TEXT("The grid has 16 cells"), Pack->GetSlotCount(), 16);
    TestEqual(TEXT("The grid has 4 rows"), Pack->GetGridHeight(), 4);
    TestEqual(TEXT("The idol fits"), Pack->AddItem(Idol, 1), 0);
    TestEqual(TEXT("The idol covers the cell below its right half"), Pack->GetCellOwner(5), 0);
    TestEqual(TEXT("The first free cell is right of the idol"), Pack->FindEmptySlot(), 2);
    TestEqual(TEXT("Seven coins fill two stacks"), Pack->AddItem(Coin, 7), 0);
    TestEqual(TEXT("The second stack holds two"), Pack->GetSlot(3).Quantity, 2);

    // Slot 1 is empty but the idol covers it, so the pole goes to the first column that has 3 free cells
    TestEqual(TEXT("The pole fits"), Pack->AddItem(Pole, 1), 0);
    TestEqual(TEXT("The pole stands at the third cell of row 2"), Pack->FindItem(Pole), 6);
    TestEqual(TEXT("The pole covers the bottom row"), Pack->GetCellOwner(14), 6);
    TestEqual(TEXT("Nine cells are full"), Pack->GetFilledCellCount(), 9);

    TestTrue(TEXT("The pole moves one column right over its own cells"), Pack->MoveSlot(6, 7));
    TestEqual(TEXT("The old column is free"), Pack->GetCellOwner(14), INDEX_NONE);
    TestFalse(TEXT("A quarter turn on the last column does not fit"), Pack->RotateItem(7));
    TestTrue(TEXT("The pole moves and lies down in row 3"), Pack->MoveItem(7, 8, 1));
    TestEqual(TEXT("The pole keeps its rotation"), Pack->GetRotation(8), 1);
    TestEqual(TEXT("The pole covers the third cell of row 3"), Pack->GetCellOwner(10), 8);
    TestFalse(TEXT("A coin cannot replace a covered cell"), Pack->SetSlot(9, Pack->GetSlot(2)));

    TestTrue(TEXT("One coin leaves the full stack"), Pack->RemoveFromSlot(2, 1));
    TestTrue(TEXT("The small stack joins the open stack"), Pack->MoveSlot(3, 2));
    TestEqual(TEXT("The open stack is full"), Pack->GetSlot(2).Quantity, 5);
    TestEqual(TEXT("One coin stays"), Pack->GetSlot(3).Quantity, 1);
    TestFalse(TEXT("A coin cannot move onto the idol"), Pack->MoveSlot(3, 5));
    TestFalse(TEXT("A second idol cannot go where it leaves the grid"), Pack->PlaceItem(Idol, 1, 15, 0));
    TestFalse(TEXT("A place never replaces an item"), Pack->PlaceItem(Coin, 1, 2, 0));

    TestTrue(TEXT("The idol goes"), Pack->RemoveItem(Idol, 1));
    TestEqual(TEXT("The first cell is free again"), Pack->FindEmptySlot(), 0);
    TestTrue(TEXT("The rotation of an empty slot is 0"), Pack->GetRotation(0) == 0);

    // In a grid of one row, the pole fits only when it lies down
    Pack->ResetGrid(3, 1);
    TestEqual(TEXT("The pole fits with a quarter turn"), Pack->AddItem(Pole, 1), 0);
    TestEqual(TEXT("The pole lies down"), Pack->GetRotation(0), 1);
    TestEqual(TEXT("A second pole does not fit"), Pack->AddItem(Pole, 1), 1);
    return true;
}

#endif
