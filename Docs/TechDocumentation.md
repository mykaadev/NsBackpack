# NsBackpack Technical Documentation

## Architecture

Two runtime modules. `NsBackpack` holds the slot list. `NsBackpackGrid` is optional and depends only on `NsBackpack`.

`NsBackpack` has four public headers:

| Header | Content |
|---|---|
| `NsBackpackItemDefinition.h` | `UNsBackpackItemDefinition`: the data asset of one kind of item, and `FindFragment` |
| `NsBackpackItemFragment.h` | `UNsBackpackItemFragment`: the empty base of game data on an item |
| `NsBackpackTypes.h` | `FNsBackpackSlot` and `FNsBackpackStartItem` |
| `NsBackpackComponent.h` | `UNsBackpackComponent`: the slots and the change functions |

`NsBackpackGrid` has two public headers:

| Header | Content |
|---|---|
| `NsBackpackGridShape.h` | `UNsBackpackGridShape`: the fragment with the box size and the cells of an item, and the quarter turn math |
| `NsBackpackGridComponent.h` | `UNsBackpackGridComponent`: a child class of the backpack with the slots in a grid |

Data flow:

1. `BeginPlay` on the server calls `ResetSlots(SlotCount)`, then `AddItem` for each start item.
2. `AddItem` fills the stacks of the item first, then the empty slots that `CanAccept` allows.
3. `RemoveItem` checks the total first. It removes nothing when the backpack holds fewer.
4. `MoveSlot` joins the items when `CanStack` allows it and the stack has room. Else it swaps the slots when `CanAccept` allows both directions.
5. Each change calls `NotifyChanged`, which fires `OnChanged`. On a client, `OnRep_Slots` calls `NotifyChanged`.

Grid data flow:

1. Each slot is one cell. The grid keeps a rotation (0 to 3) for each slot in `Rotations`, next to the slot list of the parent. `NotifyChanged` keeps the list as long as the slots and sets 0 on each empty slot.
2. The cell map comes from the slots on each query: each item covers the cells of its shape from its slot, with its rotation.
3. `CanAccept` adds the fit check to the slot rules: every cell of the shape is in the grid, and no other item covers it. The item in the target slot does not block, because `SetSlot` replaces it. `CanPlace` checks another rotation, and can ignore one more slot (the item that moves).
4. `AddItem` calls the parent first (stacks, then fits with no rotation), then places the rest with `FindPlacement`, which tries the other rotations.
5. `MoveSlot` joins a stack when the target cell is under a stack of the same item. Else it calls `MoveItem` with the same rotation. A grid never swaps.

## Lifetime

- The component does not tick.
- A slot holds a hard pointer to its item definition, so a loaded slot keeps its item asset loaded.

## Extension Points

- `UNsBackpackItemFragment`: the game makes the fragment classes.
- `FNsBackpackSlot::Data`: an `FInstancedStruct` for the state of one item.
- `CanAccept` and `CanStack`: Blueprint native events for slot rules.
- `NotifyChanged`: a virtual function for the events of a child class.
- `AddItem`, `MoveSlot`, and `FindEmptySlot`: virtual, so a child class can change the placement. The grid uses this.
- A child class adds game features, such as a selected slot or an item use.

## Dependencies

`Core`, `CoreUObject`, `Engine`, and `NetCore`. No other plugin. `NsBackpackGrid` adds `NsBackpack` only.

## Performance

- Each change function walks the slots one or two times.
- The slot list replicates as one property. A change sends the changed slots in the array delta.
- A grid query builds the cell map: slots times shape cells. The fit check of `AddItem` builds it once for each slot.

## Networking

- The component replicates by default. `Slots` replicates to every machine.
- `CanChange` is true on the server, and for a component without an owner. Every change function checks it.
- A client asks the game for a change through its own server RPC.
- The grid replicates `GridWidth` and `Rotations`. The slots and the rotations can arrive in two updates. Each update fires `OnChanged`.

## Testing

- `NsBackpack.Component.Slots`: the stack fill, the overflow, the remove checks, the slot removal, the move to an empty slot, and the swap. The test needs no world, because a component without an owner can change.
- `NsBackpack.Grid.Shapes`: one cell with no fragment, the quarter turns of a bar and of an L, and the turn count range.
- `NsBackpack.Grid.Component`: the fit with covered cells, the stacks, the moves over own cells, the rotation, the join, the blocked moves, and the fit that needs a quarter turn.

## Change Rules

- Keep the slot struct and the change functions stable. Every game inventory uses them.
- Add a new hook as a Blueprint native event or a virtual function with a default that keeps the current behavior.
- Keep the plugin free of other plugins and of game types.
