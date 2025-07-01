#include "Inventory/NsBackpackItem.h"
#include "Inventory/NsBackpackSettings.h"

void UNsBackpackItem::Use_Implementation(AActor* Target)
{
    // Default does nothing
}

#if WITH_EDITOR
void UNsBackpackItem::PostInitProperties()
{
    Super::PostInitProperties();
    if (!HasAnyFlags(RF_ClassDefaultObject))
    {
        const UNsBackpackSettings* Settings = GetDefault<UNsBackpackSettings>();
        if (Settings && Settings->ItemTable.IsValid())
        {
            UNsBackpackItemTable* Table = Settings->ItemTable.LoadSynchronous();
            if (Table && Table->RowStruct == FNsBackpackItemRow::StaticStruct())
            {
                TArray<FName> RowNames = Table->GetRowNames();
                for (FName RowName : RowNames)
                {
                    if (FNsBackpackItemRow* Row = Table->FindRow<FNsBackpackItemRow>(RowName, TEXT("")))
                    {
                        if (Row->ItemClass == GetClass())
                        {
                            ID = Row->ID;
                            return;
                        }
                    }
                }

                FNsBackpackItemRow NewRow;
                NewRow.ItemClass = GetClass();
                Table->AddRow(*Name.ToString(), NewRow);
                Table->MarkPackageDirty();
            }
        }
    }
}
#endif

