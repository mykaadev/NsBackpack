#ifndef NSBACKPACK_ITEM_H
#define NSBACKPACK_ITEM_H

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/DataTable.h"
#include "NsBackpackItem.generated.h"

class UStaticMesh;
class AActor;

class UNsBackpackItem;

USTRUCT(BlueprintType)
struct FNsBackpackItemRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UNsBackpackItem> ItemClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 ID = 0;
};

/** Base inventory item object with overrideable behaviour. */
UCLASS(Blueprintable, BlueprintType)
class NSBACKPACK_API UNsBackpackItem : public UObject
{
    GENERATED_BODY()
public:
    /** Identifier assigned from the item table. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item")
    int32 ID = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
    FText Name;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
    FText Description;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
    UTexture2D* Image = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
    bool bStackable = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item", meta=(EditCondition="bStackable", ClampMin="1"))
    int32 MaxStackSize = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equipable")
    UStaticMesh* Mesh = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawnable")
    TSubclassOf<AActor> ActorToSpawn;

    UFUNCTION(BlueprintNativeEvent, Category="Item")
    void Use(AActor* Target);

#if WITH_EDITOR
    virtual void PostInitProperties() override;
#endif
};

/** Struct representing a stack of items. */
USTRUCT(BlueprintType)
struct FNsBackpackItemStack
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UNsBackpackItem* Item = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity = 0;

    FNsBackpackItemStack() {}
    FNsBackpackItemStack(UNsBackpackItem* InItem, int32 InQuantity) : Item(InItem), Quantity(InQuantity) {}
};

#endif // NSBACKPACK_ITEM_H
