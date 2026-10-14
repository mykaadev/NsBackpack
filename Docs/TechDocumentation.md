# NsBackpack Technical Documentation

## Architecture

One runtime module, `NsBackpack`.

| Type | Role |
|---|---|
| `UNsBackpackComponent` | Actor component. It owns `InventorySlots` and the give, remove, and find functions. |
| `FNsBackpackSlot` | One slot: `ItemInstance` and `SlotIndex`. |
| `UNsBackpackItemDefinition` | `UPrimaryDataAsset`. Display data, `StackSizeMax`, and instanced `Fragments`. |
| `UNsBackpackItemInstance` | Runtime item. It holds the definition, a quantity, the owning component, and duplicated fragments. |
| `UNsBackpackItemFragment` | Abstract, `EditInlineNew` base class with four events. |
| `UNsBackpackItemAbility` | `Blueprintable` object with `OnExecute`. The ability fragment runs it. |
| `UNsBackpackSettings` | Developer settings: `DefaultItemWidget`, `LayoutMode`. |
| `ENsBackpackItemRarity`, `ENsBackpackItemCategory` | Enums for game use. |

### Give flow

1. Load the definition synchronously.
2. Fill existing stacks of the same definition up to `StackSizeMax`.
3. Fill empty slots with new instances. Each new instance duplicates the fragments of the definition and calls `OnInitialization`.
4. Return true only if all items fit.

### Remove flow

1. Take from slots in order until the count is met.
2. Destroy an instance when its quantity reaches 0, and clear the slot.
3. Return true only if the full count was removed.

## Lifetime

- `BeginPlay` sizes the slots and gives each `DefaultInventory` item one time.
- Instances are outered to the component. Fragments are outered to their instance.
- `RemoveItemByClass` calls `ConditionalBeginDestroy` on empty instances.
- In the editor, the definition removes null fragments on load and save. `IsDataValid` reports null fragments.

## Extension Points

| Point | Use |
|---|---|
| `UNsBackpackItemFragment` | Add new item behavior without new item classes. |
| `UNsBackpackItemAbility` | Add a use effect for the ability fragment. |
| `OpenInventory`, `CloseInventory` | `BlueprintNativeEvent` hooks on the component for game UI. |
| `GetMutableInventorySlots` | Direct slot access for sort or move features in the game. |

## Dependencies

`Core`, `CoreUObject`, `Engine`, `DeveloperSettings`, `Slate`, `SlateCore`, `UMG`. No other studio plugin. The UI modules are not used by code today.

## Performance

- Give, remove, and find walk all slots. This is fine for tens of slots.
- `GiveItemByClass`, `RemoveItemByClass`, and `FindItemByClass` call `LoadSynchronous` on the soft definition.
- `GetInventorySlots` returns a copy of the array.

## Networking

None. `InventorySlots` is not replicated, and item instances are not subobjects for replication.

## Testing

NsBackpack has no automated tests. Manual test path:

1. Make an item with `StackSizeMax` 5. Give 7 to a backpack with 2 slots. Make sure that the slots hold 5 and 2.
2. Remove 6. Make sure that one slot is empty and the other holds 1.
3. Give 20 to a full backpack. Make sure that the call returns false.

## Change Rules

- `Config/DefaultNsBackpack.ini` holds core redirects for old names: `Slots`, `Inventory`, `GetInventory`, and `Backpack`. Keep them while old assets can exist.
- `GetQuatity` has a typo. A rename needs a function redirect.
- Known issues to fix with Plugin Feature Fit:
  - `CanStackWith` on the instance is private and not used.
  - The consumable fragment removes the item on the first use.
  - Give and remove have no rollback.
- Replication and a save payload are candidate features. A save payload fits an optional integration module that depends on `NsSaveSystem`.
