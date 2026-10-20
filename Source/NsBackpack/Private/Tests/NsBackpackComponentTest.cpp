// Copyright (C) 2025 nulled.softworks. All rights reserved.

#include "NsBackpackComponent.h"
#include "NsBackpackItemDefinition.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNsBackpackComponentTest, "NsBackpack.Component.Slots",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FNsBackpackComponentTest::RunTest(const FString& Parameters)
{
    // A backpack without an owner can change, so the test needs no world
    UNsBackpackComponent* const Backpack = NewObject<UNsBackpackComponent>();
    UNsBackpackItemDefinition* const Bolt = NewObject<UNsBackpackItemDefinition>();
    Bolt->MaxStack = 10;
    UNsBackpackItemDefinition* const Wrench = NewObject<UNsBackpackItemDefinition>();
    Backpack->ResetSlots(3);

    TestEqual(TEXT("Three slots exist"), Backpack->GetSlotCount(), 3);
    TestEqual(TEXT("Fifteen bolts fit in two slots"), Backpack->AddItem(Bolt, 15), 0);
    TestEqual(TEXT("The first slot holds a full stack"), Backpack->GetSlot(0).Quantity, 10);
    TestEqual(TEXT("The second slot holds the rest"), Backpack->GetSlot(1).Quantity, 5);
    TestEqual(TEXT("The count is fifteen"), Backpack->CountItem(Bolt), 15);

    TestEqual(TEXT("Three more bolts join the open stack"), Backpack->AddItem(Bolt, 3), 0);
    TestEqual(TEXT("The open stack holds eight"), Backpack->GetSlot(1).Quantity, 8);

    TestEqual(TEXT("The wrench takes the last slot"), Backpack->AddItem(Wrench, 1), 0);
    TestEqual(TEXT("A second wrench does not fit"), Backpack->AddItem(Wrench, 1), 1);
    TestEqual(TEXT("The empty slot is gone"), Backpack->FindEmptySlot(), INDEX_NONE);

    TestFalse(TEXT("A remove of too many bolts does nothing"), Backpack->RemoveItem(Bolt, 30));
    TestEqual(TEXT("The bolts stay"), Backpack->CountItem(Bolt), 18);
    TestTrue(TEXT("Eleven bolts go"), Backpack->RemoveItem(Bolt, 11));
    TestTrue(TEXT("The first slot is empty"), Backpack->GetSlot(0).IsEmpty());
    TestEqual(TEXT("Seven bolts stay"), Backpack->CountItem(Bolt), 7);

    TestTrue(TEXT("The wrench moves to the empty slot"), Backpack->MoveSlot(2, 0));
    TestEqual(TEXT("The wrench is in the first slot"), Backpack->FindItem(Wrench), 0);
    TestTrue(TEXT("The bolts swap with the wrench"), Backpack->MoveSlot(1, 0));
    TestEqual(TEXT("The bolts are in the first slot"), Backpack->FindItem(Bolt), 0);

    TestTrue(TEXT("Two bolts leave the first slot"), Backpack->RemoveFromSlot(0, 2));
    TestEqual(TEXT("Five bolts stay"), Backpack->GetSlot(0).Quantity, 5);
    TestFalse(TEXT("A slot out of range does nothing"), Backpack->RemoveFromSlot(7, 1));
    return true;
}

#endif
