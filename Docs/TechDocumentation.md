# NsBackpack Technical Documentation

## Architecture

One runtime module, `NsBackpack`. Four public headers:

| Header | Content |
|---|---|
| `NsBackpackItemDefinition.h` | `UNsBackpackItemDefinition`: the data asset of one kind of item, and `FindFragment` |
| `NsBackpackItemFragment.h` | `UNsBackpackItemFragment`: the empty base of game data on an item |
| `NsBackpackTypes.h` | `FNsBackpackSlot` and `FNsBackpackStartItem` |
| `NsBackpackComponent.h` | `UNsBackpackComponent`: the slots and the change functions |

Data flow:

1. `BeginPlay` on the server calls `ResetSlots(SlotCount)`, then `AddItem` for each start item.
2. `AddItem` fills the stacks of the item first, then the empty slots that `CanAccept` allows.
3. `RemoveItem` checks the total first. It removes nothing when the backpack holds fewer.
4. `MoveSlot` joins the items when `CanStack` allows it and the stack has room. Else it swaps the slots when `CanAccept` allows both directions.
5. Each change calls `NotifyChanged`, which fires `OnChanged`. On a client, `OnRep_Slots` calls `NotifyChanged`.

## Lifetime

- The component does not tick.
- A slot holds a hard pointer to its item definition, so a loaded slot keeps its item asset loaded.

## Extension Points

- `UNsBackpackItemFragment`: the game makes the fragment classes.
- `FNsBackpackSlot::Data`: an `FInstancedStruct` for the state of one item.
- `CanAccept` and `CanStack`: Blueprint native events for slot rules.
- `NotifyChanged`: a virtual function for the events of a child class.
- A child class adds game features, such as a selected slot or an item use.

## Dependencies

`Core`, `CoreUObject`, `Engine`, and `NetCore`. No other plugin.

## Performance

- Each change function walks the slots one or two times.
- The slot list replicates as one property. A change sends the changed slots in the array delta.

## Networking

- The component replicates by default. `Slots` replicates to every machine.
- `CanChange` is true on the server, and for a component without an owner. Every change function checks it.
- A client asks the game for a change through its own server RPC.

## Testing

- `NsBackpack.Component.Slots`: the stack fill, the overflow, the remove checks, the slot removal, the move to an empty slot, and the swap. The test needs no world, because a component without an owner can change.

## Change Rules

- Keep the slot struct and the change functions stable. Every game inventory uses them.
- Add a new hook as a Blueprint native event or a virtual function with a default that keeps the current behavior.
- Keep the plugin free of other plugins and of game types.
