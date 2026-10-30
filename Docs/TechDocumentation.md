# NsBackpack Technical Documentation

## Architecture

### Modules

| Module | Type | Loading phase | Content |
|---|---|---|---|
| `NsBackpack` | Runtime | Default | The slot component, the item definition, the fragment base class, and the slot types |
| `NsBackpackGrid` | Runtime | Default | The grid component and the grid shape fragment |

`NsBackpackGrid` depends on `NsBackpack`. `NsBackpack` does not know about the grid. The `.uplugin` file lists no plugin dependencies.

### Folders

- `Source/NsBackpack/Public` – `NsBackpack.h`, `NsBackpackComponent.h`, `NsBackpackItemDefinition.h`, `NsBackpackItemFragment.h`, `NsBackpackTypes.h`.
- `Source/NsBackpack/Private` – the `.cpp` files and `Tests/NsBackpackComponentTest.cpp`.
- `Source/NsBackpackGrid/Public` – `NsBackpackGridComponent.h`, `NsBackpackGridShape.h`.
- `Source/NsBackpackGrid/Private` – the `.cpp` files and `Tests/NsBackpackGridTest.cpp`.

### Main Classes

| Type | Responsibility |
|---|---|
| `UNsBackpackComponent` | Holds `Slots` and runs every change. Replicates the slots. |
| `UNsBackpackItemDefinition` | A `UPrimaryDataAsset` for one kind of item. `FindFragment` returns the first fragment of a class. |
| `UNsBackpackItemFragment` | An abstract, `EditInlineNew`, `DefaultToInstanced` base class for game data on an item. |
| `FNsBackpackSlot` | `Item`, `Quantity`, and `Data` (`FInstancedStruct`). `IsEmpty()` is true when `Item` is null or `Quantity` is 0 or less. |
| `FNsBackpackStartItem` | `Item` and `Quantity` for `StartItems`. |
| `UNsBackpackGridComponent` | A child of `UNsBackpackComponent`. Keeps a rotation for each slot and adds the fit check. |
| `UNsBackpackGridShape` | A fragment with display name "Grid Shape". Holds `Size` and `Cells`, and turns the cells. |
| `FNsBackpackModule` | The module class of `NsBackpack`. It has no startup code. `NsBackpackGrid` uses `FDefaultModuleImpl`. |

### Data Flow

Slot changes:

1. Each change function checks `CanChange()` first. It is true when the component has no owner or the owner has authority.
2. `AddItem` fills the non-empty slots where `CanStack` allows the item, up to `MaxStack`. Then it fills empty slots where `CanAccept` allows the item, one stack at a time.
3. `RemoveItem` calls `CountItem` first. It removes nothing when the total is too small. Then it removes from the first slots first.
4. `MoveSlot` joins the source into the target when the target is not empty, has room, and the source has no `Data`. Else it swaps the slots when `CanAccept` allows both items in their new slots.
5. `SetSlot` writes one slot when `CanAccept` allows it. It does not clamp `Quantity` to `MaxStack`.
6. Each change that does something calls `NotifyChanged`, which broadcasts `OnChanged`. On a client, `OnRep_Slots` calls `NotifyChanged`.

`GetRoom` gives the room in a slot: 0 when the slot is empty or `CanStack` is false, else `MaxStack` minus `Quantity`. A `MaxStack` under 1 counts as 1.

Grid:

1. Each slot is one cell, row by row from the top left. `GetGridWidth()` is `GridWidth`, at least 1. `GetGridHeight()` is the number of full rows.
2. `Rotations` holds 0 to 3 quarter turns for each slot. `NotifyChanged` sets its length to the slot count and sets 0 on each empty slot. This runs only where `CanChange()` is true.
3. `BuildCellOwners` makes the cell map on each call. Each item covers the cells of its shape from its slot, with its rotation.
4. `CanAccept_Implementation` calls the parent, then makes sure that every cell of the shape is in the grid and that no other item covers it. The item in the target slot does not block. The item in `CheckIgnoreSlot` does not block.
5. `CanPlace` sets `CheckRotation` and `CheckIgnoreSlot` with `TGuardValue`, then calls `CanAccept`.
6. `AddItem` calls the parent first (stacks, then fits with the stored rotation, which is 0 for an empty slot). Then it loops `FindPlacement` and `PlaceItem` for the rest. `FindPlacement` tries every slot with rotation 0, then rotation 1, 2, and 3.
7. `MoveSlot` looks up the owner of the target cell. When another item covers it, the move joins that stack through the parent `MoveSlot`, or fails. Else it calls `MoveItem` with the current rotation.
8. `MoveItem` writes the target with `SetGridSlot` while `CheckIgnoreSlot` is the source. Then it clears the source with `SetSlot`.
9. `SetGridSlot` stores the new rotation before `SetSlot`, because `CanAccept` reads the stored rotation. It restores the old rotation when `SetSlot` fails.

`UNsBackpackGridShape::GetCells` keeps the cells inside the box and drops repeats. An empty list gives the full box. A clockwise quarter turn moves cell (X, Y) of a box of height H to (H - 1 - Y, X). An item without the fragment covers cell (0, 0).

## Lifetime

1. The constructor turns off the tick and calls `SetIsReplicatedByDefault(true)`. The grid constructor sets `GridWidth` to 4 and `SlotCount` to 16.
2. `BeginPlay`, where `CanChange()` is true, calls `ResetSlots(SlotCount)`, then `AddItem` for each entry of `StartItems`. A client gets its slots by replication.
3. The component has no `EndPlay` code.

A slot holds a hard `TObjectPtr` to its item definition, so a slot keeps its item asset loaded. Fragments are instanced subobjects of the item asset.

## Extension Points

- `CanAccept(const UNsBackpackItemDefinition* Item, int32 SlotIndex) const` – `BlueprintNativeEvent`. Default: true. A grid subclass that overrides `CanAccept_Implementation` must call the parent, because the parent does the fit check.
- `CanStack(const FNsBackpackSlot& Slot, const UNsBackpackItemDefinition* Item) const` – `BlueprintNativeEvent`. Default: the same item and no `Data`.
- `virtual int32 AddItem(UNsBackpackItemDefinition* Item, int32 Quantity = 1)` – Change the placement. The grid overrides it.
- `virtual bool MoveSlot(int32 FromIndex, int32 ToIndex)` – Change the move. The grid overrides it.
- `virtual int32 FindEmptySlot() const` – Change what counts as empty. The grid overrides it.
- `virtual void NotifyChanged()` – C++ only. Call `Super` to keep `OnChanged`. The grid overrides it to keep `Rotations` in step.
- `OnChanged` – `BlueprintAssignable`, no parameters.
- `UNsBackpackItemFragment` – Subclass it in C++ or Blueprint for game data.
- `FNsBackpackSlot::Data` – Any `FInstancedStruct` for the state of one item.
- `CanChange()` – Protected, not virtual.

## Dependencies

| Dependency | Kind | Why |
|---|---|---|
| `Core`, `CoreUObject`, `Engine` | Public module (both modules) | Base types, the actor component, `FInstancedStruct`, and the data asset |
| `NetCore` | Public module (both modules) | Replication of the slots |
| `NsBackpack` | Public module of `NsBackpackGrid` | The parent component and the fragment base class |

The `.uplugin` file lists no plugins.

## Performance

- The components do not tick.
- Each base change function walks the slots one or two times. `RemoveItem` also calls `CountItem`.
- `BuildCellOwners` allocates a new array and walks every slot and every shape cell. `GetCellOwner`, `GetFilledCellCount`, `FindEmptySlot`, and each `CanAccept` call on the grid build it again.
- `FindPlacement` can call `CanPlace` up to 4 times for each slot, and each call builds the cell map. The cost grows with the square of the slot count.
- `Slots` replicates as a plain `TArray` property, not as a fast array serializer.

## Networking

- The components replicate by default.
- `Slots` uses `ReplicatedUsing = OnRep_Slots`. The grid adds `GridWidth` (`Replicated`) and `Rotations` (`ReplicatedUsing = OnRep_Rotations`).
- Each OnRep fires `OnChanged`. On a grid client, one change can fire `OnChanged` two times.
- Every change function returns early when `CanChange()` is false. The plugin has no RPCs. The game sends change requests to the server with its own RPCs.

## Testing

| Test | What it checks |
|---|---|
| `NsBackpack.Component.Slots` | Stack fill, overflow, the remove checks, `RemoveFromSlot`, the move to an empty slot, and the swap |
| `NsBackpack.Grid.Shapes` | One cell without a fragment, the quarter turns of a bar and an L, four turns, and a negative turn |
| `NsBackpack.Grid.Component` | Fits with covered cells, stacks, moves over own cells, rotation, the join, blocked moves, and a fit that needs a quarter turn |

The tests use `EditorContext` and `EngineFilter`. They need no world, because a component without an owner can change.

Run them in **Session Frontend > Automation**, filter `NsBackpack`. Or start the editor with `-ExecCmds="Automation RunTests NsBackpack"`.

## Change Rules

- Do not rename `Slots`, `SlotCount`, `StartItems`, `GridWidth`, `Rotations`, or the fields of `FNsBackpackSlot` and the item definition. Saved Blueprints, assets, and replication use them by name.
- Keep `Rotations` the same length as `Slots`. `SetRotation` and `NotifyChanged` depend on it.
- Keep the fit check in `UNsBackpackGridComponent::CanAccept_Implementation`. `SetSlot`, `AddItem`, and `MoveSlot` of the parent call `CanAccept`, so the grid rules apply to them too.
