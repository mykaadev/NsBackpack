// Copyright (C) 2025 nulled.softworks. All rights reserved.

#include "NsBackpackItemDefinition.h"

#include "NsBackpackItemFragment.h"
#include "Misc/DataValidation.h"

#define LOCTEXT_NAMESPACE "NsBackpackItemDefinition"

#if WITH_EDITOR
EDataValidationResult UNsBackpackItemDefinition::IsDataValid(FDataValidationContext& Context) const
{
    EDataValidationResult Result = Super::IsDataValid(Context);
    TSet<const UClass*> SeenClasses;
    for (const UNsBackpackItemFragment* const Fragment : Fragments)
    {
        if (Fragment == nullptr)
        {
            Context.AddError(LOCTEXT("EmptyFragment", "A fragment entry is empty. Pick a fragment class or remove the entry."));
            Result = EDataValidationResult::Invalid;
            continue;
        }

        // Find Fragment returns the first match, so a second fragment of one class never counts
        if (SeenClasses.Contains(Fragment->GetClass()))
        {
            Context.AddWarning(FText::Format(LOCTEXT("DuplicateFragment", "Two fragments are of class {0}. Only the first one counts."),
                FText::FromString(Fragment->GetClass()->GetName())));
        }
        SeenClasses.Add(Fragment->GetClass());
    }
    return Result;
}
#endif

UNsBackpackItemFragment* UNsBackpackItemDefinition::FindFragment(const TSubclassOf<UNsBackpackItemFragment> FragmentClass) const
{
    for (UNsBackpackItemFragment* const Fragment : Fragments)
    {
        if (Fragment != nullptr && Fragment->IsA(FragmentClass))
        {
            return Fragment;
        }
    }
    return nullptr;
}

#undef LOCTEXT_NAMESPACE
