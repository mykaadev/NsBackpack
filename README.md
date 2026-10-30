<!-- GH_ONLY_START -->
<h1 align="center">
  <br>
  <a href="https://github.com/mykaadev/NsBackpack"><img src="https://raw.githubusercontent.com/mykaadev/NsBackpack/refs/heads/main/Resources/Banner.png" alt="NsBackpack" width="400"></a>
</h1>

<h4 align="center">A slot inventory with stacks, item fragments, and an optional shape grid for Unreal Engine.</h4>

<div align="center">
    <a href="https://github.com/mykaadev/NsBackpack/commits/main"><img src="https://img.shields.io/github/last-commit/mykaadev/NsBackpack?style=plastic&logo=github&logoColor=white" alt="GitHub Last Commit"></a>
    <a href="https://github.com/mykaadev/NsBackpack/issues"><img src="https://img.shields.io/github/issues-raw/mykaadev/NsBackpack?style=plastic&logo=github&logoColor=white" alt="GitHub Issues"></a>
    <a href="https://github.com/mykaadev/NsBackpack/pulls"><img src="https://img.shields.io/github/issues-pr-raw/mykaadev/NsBackpack?style=plastic&logo=github&logoColor=white" alt="GitHub Pull Requests"></a>
    <a href="https://github.com/mykaadev/NsBackpack"><img src="https://img.shields.io/github/stars/mykaadev/NsBackpack?style=plastic&logo=github" alt="GitHub Stars"></a>
    <a href="https://twitter.com/mykaadev/"><img src="https://img.shields.io/twitter/follow/mykaadev?style=plastic&logo=x" alt="Twitter Follow"></a>

<p style="display:none;">
  <a href="#-summary">👀 Summary</a> •
  <a href="#-features">📦 Features</a> •
  <a href="#-requirements">⚙️ Requirements</a> •
  <a href="#-installation">🛠️ Installation</a> •
  <a href="#-getting-started">🚀 Getting Started</a> •
  <a href="#-modules">🧩 Modules</a> •
  <a href="#-api">🔧 API</a> •
  <a href="#-limits">🚧 Limits</a> •
  <a href="#-documentation">📚 Documentation</a> •
  <a href="#-credits">❤️ Credits</a> •
  <a href="#-support">📞 Support</a> •
  <a href="#-license">📃 License</a>
</p>
<a href="https://buymeacoffee.com/mykaadev"><img src="https://www.svgrepo.com/show/476855/coffee-to-go.svg" alt="Coffee" width="50px"></a>
<p><b>Buy me a coffee!</b></p>
</div>
<!-- GH_ONLY_END -->

## 👀 Summary
NsBackpack is a slot inventory for gameplay programmers. An actor gets a backpack component with a fixed number of slots. Each slot holds an item, a quantity, and optional item data. An item is a data asset with a name, an icon, a mesh, a stack size, and fragments that your game makes. The optional `NsBackpackGrid` module puts the slots in a grid, where an item covers the cells of its shape.

NsBackpack has no UI, no selected slot, no item use, no crafting, and no save system. It has no server RPCs, so your game sends the change requests to the server. NsBackpack depends on no other plugin.

## 📦 Features
- **Slots and stacks**: Add, remove, count, find, and move items. Stacks fill before empty slots.
- **Item assets**: One data asset for each kind of item, with a display name, an icon, a world mesh, and a stack size.
- **Fragments**: Put game data on an item as instanced fragment objects, and find them by class.
- **Item state**: Keep the state of one item, such as durability, in an instanced struct on its slot.
- **Slot rules**: Override `CanAccept` and `CanStack` in C++ or Blueprint.
- **Start items**: Give a backpack its items when play starts.
- **Replication**: The server changes the slots, and the slots replicate to every machine.
- **Shape grid**: Items with a `Grid Shape` fragment cover a box or any set of cells, and turn by quarter turns.

## ⚙️ Requirements
Unreal Engine 5.7+

## 🛠️ Installation
1. **Clone** or download this repository.
2. Copy the `NsBackpack` folder into the `Plugins` folder of your project.
3. Add `"NsBackpack"` to the dependencies of your game module in `Build.cs`. Add `"NsBackpackGrid"` too for a grid.
4. Generate the project files and enable the plugin.

## 🚀 Getting Started
1. Make an item: Content Browser > **Miscellaneous** > **Data Asset** > `NsBackpackItemDefinition`.
2. Set **Display Name** and **Max Stack** on the item.
3. Add a `UNsBackpackComponent` to your character.
4. On the server, call `Add Item` with the item and a quantity.
5. Bind `On Changed` to update your UI.

In C++:

```cpp
#include "NsBackpackComponent.h"

// In the constructor of the character
Backpack = CreateDefaultSubobject<UNsBackpackComponent>(TEXT("Backpack"));

// On the server. The return value is the quantity that did not fit.
const int32 Left = Backpack->AddItem(BoltItem, 15);
```

## 🧩 Modules
| Module | Content |
|---|---|
| `NsBackpack` | The slot component, the item definition, the fragment base class, and the slot types |
| `NsBackpackGrid` | The grid component and the `Grid Shape` fragment. It depends only on `NsBackpack` |

## 🔧 API
- `UNsBackpackComponent` – The slots. `AddItem`, `RemoveItem`, `RemoveFromSlot`, `MoveSlot`, `SetSlot`, `CountItem`, `FindItem`, `FindEmptySlot`, and `OnChanged`.
- `UNsBackpackItemDefinition` – The data asset of one kind of item. `FindFragment` returns its first fragment of a class.
- `UNsBackpackItemFragment` – The abstract base class of game data on an item.
- `FNsBackpackSlot` – An item, a quantity, and an `FInstancedStruct` of item data.
- `UNsBackpackGridComponent` – A backpack with its slots in a grid. `PlaceItem`, `MoveItem`, `RotateItem`, `CanPlace`, `FindPlacement`, and `GetCellOwner`.
- `UNsBackpackGridShape` – The `Grid Shape` fragment: a box size and the cells inside the box.

## 🚧 Limits
- Only the server, or a backpack without an owner, can change the slots. The change functions do nothing on a client.
- The number of slots stays fixed until the next `ResetSlots` or `ResetGrid` call.
- A slot with item data does not stack.
- A grid move never swaps two items.
- The grid builds its cell map again on each query.

## 📚 Documentation
- [User Guide](https://mykaadev.github.io/plugins/nsbackpack/user-guide/) – Setup, common tasks, settings, and debug tools.
- [Technical Documentation](https://mykaadev.github.io/plugins/nsbackpack/tech-documentation/) – Architecture, lifetime, and extension points.

<!-- GH_ONLY_START -->
## ❤️ Credits
<a href="https://github.com/mykaadev/NsBackpack/graphs/contributors"><img src="https://contrib.rocks/image?repo=mykaadev/NsBackpack"/></a>

## 📞 Support
Reach out via the **[profile page](https://github.com/mykaadev)**.

## 📃 License
[![License](https://img.shields.io/badge/license-MIT-green)](https://www.tldrlegal.com/license/mit-license)
<!-- GH_ONLY_END -->
