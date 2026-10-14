# NsBackpack User Guide

## Purpose

NsBackpack is a slot inventory. An actor gets a backpack component with a fixed number of slots. Items are data assets built from fragments, such as durability, weight, or a use ability.

NsBackpack holds and stacks items. It has no inventory UI, no crafting logic, no trade logic, no save support, and no replication. The game builds those on top.

## Setup

1. Add `NsBackpack` to the `Plugins` list of the `.uproject` file.
2. For C++, add `"NsBackpack"` to the module dependencies in `Build.cs`.

## Quick Start

### 1. Make an item

1. Make a data asset of class `NsBackpackItemDefinition`.
2. Set `Name`, `Description`, `Icon2D`, and `WorldMesh`.
3. Set `StackSizeMax`. 1 means no stacking.
4. Add fragments in `Fragments`, for example `NsBackpackItemFragmentWeight`.

### 2. Add a backpack

1. Add a `NsBackpackComponent` to the actor.
2. Set `InventorySize`. The default is 6 slots.
3. Optional: add items to `DefaultInventory`. Each one is given one time on `BeginPlay`.

### 3. Give and remove items

```cpp
Backpack->GiveItemByClass(ChipStackItem, 5);
const bool bHasKey = Backpack->FindItemByClass(VipKeyItem, 1);
Backpack->RemoveItemByClass(ChipStackItem, 2);
```

## Common Tasks

### Show the inventory

1. Call `GetInventorySlots`.
2. For each slot, read `ItemInstance`. A null instance is an empty slot.
3. Read `GetItemDefinition()->Icon2D` and `GetQuatity()` for the slot widget.

Open and close the UI from `OpenInventory` and `CloseInventory`. They are empty events for a game to override.

### Use an item

Call `ActivateItem` on the item instance. Each fragment runs `OnActivation`.

### Read a fragment

```cpp
const UNsBackpackItemFragmentWeight* const Weight = UNsBackpackItemDefinition::GetFragment<UNsBackpackItemFragmentWeight>(Definition);
```

### Make a new fragment

1. Make a C++ or Blueprint child of `NsBackpackItemFragment`.
2. Override `OnInitialization`, `OnActivation`, `OnDeactivation`, or `CanStackWith`.
3. Add it to an item definition.

## Fragments

| Fragment | Data | Behavior |
|---|---|---|
| `Ability` | `Ability` class | On activation, runs `OnExecute` on the class default object of the ability. |
| `Consumable` | `MaxUses`, `CurrentUses` | On activation, can remove one item from the backpack. See Limits. |
| `Durability` | `MaxDurability`, `CurrentDurability` | Sets current to max on init. No wear logic. |
| `Weight` | `Weight` in kilograms | Data only. |
| `Tradeable` | `BuyValue`, `SellValue` | Data only. |
| `Craftable` | `Recipe`: item and count | Data only. |
| `Deconstruct` | `DeconstructList`: item and count | Data only. |
| `Socket` | `Sockets`, `MaxSockets` | Data only. |

## Rules

- Each item instance gets its own copy of the fragments. Change fragment values on the instance, not on the definition.
- Check the return value of `GiveItemByClass`. False means that not all items fit.
- Do not store per-player state in an ability class. The ability runs on the class default object.

## Settings

Project Settings, "NsBackpack Settings":

| Setting | Default | Effect |
|---|---|---|
| `DefaultItemWidget` | None | A widget class for game UI to read. The plugin does not use it. |
| `LayoutMode` | `Expanded` | `Expanded` or `Compact`, for game UI to read. The plugin does not use it. |

Component: `InventorySize`, `DefaultInventory`, and `InventoryWidget`. The plugin does not use `InventoryWidget`.

## Debug

NsBackpack has no console commands or log category. Read the slots with `GetInventorySlots` from a cheat or a debug widget.

## Limits

- `GiveItemByClass` and `RemoveItemByClass` do not roll back. If not all items fit, the ones that fit stay.
- Stacking uses the item definition only. `CanStackWith` on fragments is not called. Two items with different durability can stack.
- `Consumable` sets `CurrentUses` to `MaxUses` on init, then adds 1 on each use. The item is removed on the first use, whatever `MaxUses` is.
- `DeactivateItem` on a consumable also removes one item.
- No replication. In multiplayer, keep the backpack on the server and send the UI data to the owner client.
- No save support. Use `NsSaveSystem` from the game to save the slots.
- The item asset loads synchronously in `GiveItemByClass`.
- `ENsBackpackItemRarity` and `ENsBackpackItemCategory` exist, but the item definition has no field for them.
