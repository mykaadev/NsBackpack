# NsBackpack User Guide

## Purpose

NsBackpack is a slot inventory. An actor gets a backpack component with a fixed number of slots. Each slot holds an item, a quantity, and optional item data. An item is a data asset with a name, an icon, a mesh, a stack size, and fragments that the game makes.

The optional module `NsBackpackGrid` puts the slots in a grid. An item takes the cells of its shape and turns by quarter turns.

NsBackpack has no UI, no selected slot, no item use, no crafting, and no save. The game builds those on top. NsBackpack depends on no other plugin.

## Setup

1. Enable `NsBackpack` in the `.uproject` file.
2. Add `NsBackpack` to the module dependencies of the game. Add `NsBackpackGrid` too for a grid.
3. Add a `UNsBackpackComponent` to an actor, or make a child class of it for game features.
4. Set `SlotCount` and `StartItems` on the component.

The plugin has no project settings.

## Quick Start

1. Make a data asset of class `NsBackpackItemDefinition`. Set `DisplayName`, `Icon`, `WorldMesh`, and `MaxStack`.
2. Add the item to the backpack:

```cpp
const int32 Left = Backpack->AddItem(BoltItem, 15);
```

`AddItem` returns the quantity that did not fit.

Blueprint: call `Add Item`, `Remove Item`, `Count Item`, and `Get Slots`. Bind `On Changed` to refresh the UI.

## Common Tasks

### Add game data to an item

1. Make a C++ or Blueprint child class of `UNsBackpackItemFragment`. Add the fields that the game needs.
2. Add the fragment to the `Fragments` list of the item definition.
3. Read it with `Item->FindFragment<UMyFragment>()`, or with `Find Fragment` in Blueprint.

### Keep state for one item

Put a struct in `FNsBackpackSlot::Data`, for example the durability of one sword. A slot with data does not stack.

### Add a slot rule

Make a child class of `UNsBackpackComponent` and override `CanAccept`, for example for a weight limit or a slot for tools only. Override `CanStack` for a stack rule.

### Add game events

Override `NotifyChanged` in the child class. Call the parent first. The parent fires `OnChanged`.

### Move items between slots

Call `MoveSlot`. The items join when the target stack has room. Else the two slots swap.

### Load a save

Call `ResetSlots`, then `SetSlot` for each saved slot. For a grid, call `ResetGrid`, then `SetGridSlot` with the saved rotation.

### Use a grid

1. Add a `UNsBackpackGridComponent` in place of `UNsBackpackComponent`. Set `GridWidth` (columns) and `SlotCount` (cells). The default is 4 x 4.
2. Add a `Grid Shape` fragment to each item that takes more than one cell. Set `Size`, for example 2 x 2. For an L or another shape, list the `Cells` inside the box. An item without the fragment takes one cell.
3. `AddItem` fills the stacks, then the first place where the item fits. It tries a quarter turn when no place fits.
4. For a drag in the UI, call `MoveItem` with the target cell and the rotation, or `RotateItem`. `CanPlace` gives the preview.
5. `GetCellOwner` gives the item that covers a cell. `GetItemCells` gives the cells of an item. `GetFilledCellCount` gives the cells in use, for example for a weight rule.

A slot index is a cell index, row by row from the top left. An item sits in the slot of the top left cell of its box. A grid move never swaps two items. A move onto a stack of the same item joins it.

## Rules

- Change the slots on the server. On a client, the change functions do nothing.
- Do not keep a reference to a slot. Read it again after `OnChanged`.
- Do not put game rules in the item definition. Put them in fragments.
- In a grid, an empty slot can be under an item. Use `GetCellOwner` or `FindEmptySlot`, not `GetSlot(...).IsEmpty()`, to find a free cell.
- A child class of the grid component that overrides `CanAccept` calls the parent. The parent does the fit check.

## Settings

On `UNsBackpackComponent`:

| Setting | Default | Effect |
|---|---|---|
| `SlotCount` | 8 | Number of slots when play starts. |
| `StartItems` | empty | Items that the backpack gets when play starts. |

On `UNsBackpackItemDefinition`:

| Setting | Default | Effect |
|---|---|---|
| `DisplayName` | empty | Name that the UI shows. |
| `Description` | empty | One line about the item. |
| `Icon` | none | Picture in the UI. |
| `WorldMesh` | none | Mesh in the world. |
| `MaxStack` | 1 | Most items in one slot. |
| `Fragments` | empty | Game data of the item. |

On `UNsBackpackGridComponent`:

| Setting | Default | Effect |
|---|---|---|
| `GridWidth` | 4 | Number of columns. |
| `SlotCount` | 16 | Number of cells. |

On the `Grid Shape` fragment (`UNsBackpackGridShape`):

| Setting | Default | Effect |
|---|---|---|
| `Size` | 1 x 1 | Width and height of the box of the shape. |
| `Cells` | empty | Cells of the shape inside the box. Empty fills the box. |

## Debug

- Read `GetSlots` in a debugger or a Blueprint print.
- Run the tests `NsBackpack.Component.Slots`, `NsBackpack.Grid.Shapes`, and `NsBackpack.Grid.Component` in Session Frontend.
- Data validation reports an empty fragment entry and two fragments of one class.

## Limits

- The number of slots is fixed until the next `ResetSlots`.
- The whole slot list replicates on a change. A large backpack sends more data.
- A grid builds its cell map on each query. A grid of a few hundred cells is fine. A larger grid needs a cache.
