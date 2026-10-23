// Copyright (C) 2025 nulled.softworks. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "NsBackpackComponent.h"
#include "NsBackpackGridComponent.generated.h"

/**
 * A backpack with its slots in a grid. Each slot is one cell, row by row from the top left
 * An item sits in the slot of its top left cell and covers the cells of its Grid Shape fragment. It can turn by quarter turns
 * Slot Count is the number of cells. Grid Width is the number of columns
 * A grid move never swaps two items. A move onto a stack of the same item joins the stack
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (NsBackpack), meta = (BlueprintSpawnableComponent))
class NSBACKPACKGRID_API UNsBackpackGridComponent : public UNsBackpackComponent
{
    GENERATED_BODY()

// Functions
public:
    /** Constructor */
    UNsBackpackGridComponent();

    //~ Begin UActorComponent Interface
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    //~ End UActorComponent Interface

    //~ Begin UNsBackpackComponent Interface
    virtual int32 AddItem(class UNsBackpackItemDefinition* Item, int32 Quantity = 1) override;
    virtual bool MoveSlot(int32 FromIndex, int32 ToIndex) override;
    virtual int32 FindEmptySlot() const override;
    //~ End UNsBackpackComponent Interface

    /** Empties the backpack and makes a grid of this size */
    UFUNCTION(BlueprintCallable, Category = "Backpack|Grid")
    void ResetGrid(int32 Width, int32 Height);

    /** Puts a new stack in an empty slot with a rotation. Returns false when the item does not fit or the quantity is more than one stack */
    UFUNCTION(BlueprintCallable, Category = "Backpack|Grid")
    bool PlaceItem(class UNsBackpackItemDefinition* Item, int32 Quantity, int32 SlotIndex, int32 Rotation);

    /** Moves the item of a slot to another cell with a rotation. Returns false when it does not fit there */
    UFUNCTION(BlueprintCallable, Category = "Backpack|Grid")
    bool MoveItem(int32 FromIndex, int32 ToIndex, int32 Rotation);

    /** Turns the item of a slot one quarter turn clockwise on the same top left cell. Returns false when it does not fit */
    UFUNCTION(BlueprintCallable, Category = "Backpack|Grid")
    bool RotateItem(int32 SlotIndex);

    /** Writes one slot with a rotation, for example for a load from a save. Returns false when the item does not fit */
    UFUNCTION(BlueprintCallable, Category = "Backpack|Grid")
    bool SetGridSlot(int32 SlotIndex, const FNsBackpackSlot& Slot, int32 Rotation);

    /** Returns true when the item fits with its top left cell on the slot. The item of Ignore Slot does not block it */
    UFUNCTION(BlueprintPure, Category = "Backpack|Grid", meta = (AdvancedDisplay = "IgnoreSlot"))
    bool CanPlace(const class UNsBackpackItemDefinition* Item, int32 SlotIndex, int32 Rotation, int32 IgnoreSlot = -1) const;

    /** Finds the first slot where the item fits. It tries every slot with no rotation first, then the other rotations */
    UFUNCTION(BlueprintPure, Category = "Backpack|Grid")
    bool FindPlacement(const class UNsBackpackItemDefinition* Item, int32& OutSlotIndex, int32& OutRotation) const;

    /** Returns the slot of the item that covers a cell, or -1 */
    UFUNCTION(BlueprintPure, Category = "Backpack|Grid")
    int32 GetCellOwner(int32 CellIndex) const;

    /** Returns the cells that the item of a slot covers */
    UFUNCTION(BlueprintPure, Category = "Backpack|Grid")
    TArray<int32> GetItemCells(int32 SlotIndex) const;

    /** Returns the number of cells that items cover */
    UFUNCTION(BlueprintPure, Category = "Backpack|Grid")
    int32 GetFilledCellCount() const;

    /** Returns the quarter turns of the item in a slot, 0 to 3 */
    UFUNCTION(BlueprintPure, Category = "Backpack|Grid")
    int32 GetRotation(int32 SlotIndex) const;

    /** Returns the number of columns */
    UFUNCTION(BlueprintPure, Category = "Backpack|Grid")
    int32 GetGridWidth() const { return FMath::Max(1, GridWidth); }

    /** Returns the number of full rows */
    UFUNCTION(BlueprintPure, Category = "Backpack|Grid")
    int32 GetGridHeight() const { return GetSlotCount() / GetGridWidth(); }

    /** Returns the slot of a cell position, or -1 outside the grid */
    UFUNCTION(BlueprintPure, Category = "Backpack|Grid")
    int32 GetCellIndex(FIntPoint Cell) const;

    /** Returns the column and the row of a slot */
    UFUNCTION(BlueprintPure, Category = "Backpack|Grid")
    FIntPoint GetCellPosition(int32 SlotIndex) const;

protected:
    /** Adds the fit check to the slot rules. A child class that overrides it calls the parent */
    virtual bool CanAccept_Implementation(const class UNsBackpackItemDefinition* Item, int32 SlotIndex) const override;

    /** Keeps one rotation for each slot, and 0 for each empty slot, then fires On Changed */
    virtual void NotifyChanged() override;

private:
    /** Tells the client about new rotations */
    UFUNCTION()
    void OnRep_Rotations();

    /** Returns the slot of the item that covers each cell, or -1 */
    TArray<int32> BuildCellOwners() const;

    /** Returns true when a stack in a slot can take the items of another slot */
    bool CanJoin(int32 FromIndex, int32 ToIndex) const;

    /** Writes a rotation and keeps the rotation list as long as the slot list */
    void SetRotation(int32 SlotIndex, int32 Rotation);

// Variables
public:
    /** Number of columns. Slot Count sets the number of cells */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Backpack|Grid", meta = (ClampMin = "1"))
    int32 GridWidth;

private:
    /** Quarter turns of the item in each slot */
    UPROPERTY(ReplicatedUsing = OnRep_Rotations)
    TArray<uint8> Rotations;

    /** Rotation that Can Accept checks in place of the stored one, or -1 */
    mutable int32 CheckRotation = INDEX_NONE;

    /** Slot whose item Can Accept ignores, or -1 */
    mutable int32 CheckIgnoreSlot = INDEX_NONE;
};
