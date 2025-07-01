#include "Inventory/NsBackpackItemTable.h"
#include "Inventory/NsBackpackItem.h"

#if WITH_EDITOR
void UNsBackpackItemTable::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    TArray<FName> RowNames = GetRowNames();
    RowNames.Sort();
    for (int32 Index = 0; Index < RowNames.Num(); ++Index)
    {
        if (FNsBackpackItemRow* Row = FindRow<FNsBackpackItemRow>(RowNames[Index], TEXT("")))
        {
            Row->ID = Index;
            if (Row->ItemClass)
            {
                UNsBackpackItem* DefaultObj = Row->ItemClass->GetDefaultObject<UNsBackpackItem>();
                if (DefaultObj)
                {
                    DefaultObj->ID = Index;
                }
            }
        }
    }
}
#endif

