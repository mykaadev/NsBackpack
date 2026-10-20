// Copyright (C) 2025 nulled.softworks. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NsBackpackTypes.h"
#include "NsBackpackComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNsBackpackChangedDelegate);

/**
 * A fixed number of slots that hold items
 * The server, or a backpack that does not replicate, changes the slots. The slots replicate to every machine
 * Override Can Accept and Can Stack for game rules. Make a child class for game features, such as a selected slot
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (NsBackpack), meta = (BlueprintSpawnableComponent))
class NSBACKPACK_API UNsBackpackComponent : public UActorComponent
{
    GENERATED_BODY()

// Functions
public:
    /** Constructor */
    UNsBackpackComponent();

    //~ Begin UActorComponent Interface
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    //~ End UActorComponent Interface

    /** Empties the backpack and makes this number of slots */
    UFUNCTION(BlueprintCallable, Category = "Backpack")
    void ResetSlots(int32 Count);

    /** Adds items. The stacks of the item fill first, then the empty slots. Returns the quantity that did not fit */
    UFUNCTION(BlueprintCallable, Category = "Backpack")
    int32 AddItem(class UNsBackpackItemDefinition* Item, int32 Quantity = 1);

    /** Removes items, from the first slots first. Returns false and removes nothing when the backpack holds fewer */
    UFUNCTION(BlueprintCallable, Category = "Backpack")
    bool RemoveItem(class UNsBackpackItemDefinition* Item, int32 Quantity = 1);

    /** Removes items from one slot. Returns false and removes nothing when the slot holds fewer */
    UFUNCTION(BlueprintCallable, Category = "Backpack")
    bool RemoveFromSlot(int32 SlotIndex, int32 Quantity = 1);

    /** Moves a slot onto another slot. The items join when the stack has room, else the two slots swap */
    UFUNCTION(BlueprintCallable, Category = "Backpack")
    bool MoveSlot(int32 FromIndex, int32 ToIndex);

    /** Writes one slot, for example for a split or a load from a save. Returns false when the slot cannot take the item */
    UFUNCTION(BlueprintCallable, Category = "Backpack")
    bool SetSlot(int32 SlotIndex, const FNsBackpackSlot& Slot);

    /** Returns the quantity of an item in every slot */
    UFUNCTION(BlueprintPure, Category = "Backpack")
    int32 CountItem(const class UNsBackpackItemDefinition* Item) const;

    /** Returns the first slot that holds the item, or -1 */
    UFUNCTION(BlueprintPure, Category = "Backpack")
    int32 FindItem(const class UNsBackpackItemDefinition* Item) const;

    /** Returns the first empty slot, or -1 */
    UFUNCTION(BlueprintPure, Category = "Backpack")
    int32 FindEmptySlot() const;

    /** Returns one slot. An index out of range gives an empty slot */
    UFUNCTION(BlueprintPure, Category = "Backpack")
    const FNsBackpackSlot& GetSlot(int32 SlotIndex) const;

    /** Returns every slot */
    UFUNCTION(BlueprintPure, Category = "Backpack")
    const TArray<FNsBackpackSlot>& GetSlots() const { return Slots; }

    /** Returns the number of slots */
    UFUNCTION(BlueprintPure, Category = "Backpack")
    int32 GetSlotCount() const { return Slots.Num(); }

protected:
    /** Returns true when the slot can take the item. Override it for game rules, such as a weight limit or a slot for tools only */
    UFUNCTION(BlueprintNativeEvent, Category = "Backpack")
    bool CanAccept(const class UNsBackpackItemDefinition* Item, int32 SlotIndex) const;

    virtual bool CanAccept_Implementation(const class UNsBackpackItemDefinition* Item, int32 SlotIndex) const;

    /** Returns true when the item can join the stack in the slot. The default needs the same item and no slot data */
    UFUNCTION(BlueprintNativeEvent, Category = "Backpack")
    bool CanStack(const FNsBackpackSlot& Slot, const class UNsBackpackItemDefinition* Item) const;

    virtual bool CanStack_Implementation(const FNsBackpackSlot& Slot, const class UNsBackpackItemDefinition* Item) const;

    /** Returns true when this machine can change the slots: the server, or a backpack without an owner */
    bool CanChange() const;

    /** Fires On Changed on this machine. A child class overrides it to add its own events */
    virtual void NotifyChanged();

private:
    /** Tells the client about new slots */
    UFUNCTION()
    void OnRep_Slots();

    /** Returns the room left in the stack of a slot for the item */
    int32 GetRoom(int32 SlotIndex, const class UNsBackpackItemDefinition* Item) const;

// Variables
public:
    /** Fires on every machine after the slots change */
    UPROPERTY(BlueprintAssignable, Category = "Backpack")
    FNsBackpackChangedDelegate OnChanged;

    /** Number of slots when play starts */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Backpack", meta = (ClampMin = "0"))
    int32 SlotCount;

    /** Items that the backpack gets when play starts */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Backpack")
    TArray<FNsBackpackStartItem> StartItems;

private:
    /** The slots */
    UPROPERTY(ReplicatedUsing = OnRep_Slots)
    TArray<FNsBackpackSlot> Slots;
};
