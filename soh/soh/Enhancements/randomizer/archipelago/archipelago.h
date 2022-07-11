#pragma once

#include <string>

#include "randomizerTypes.h"

extern "C" {
    #include "z64item.h"
}

std::string ArchipelagoItemText();

void SetArchipelagoCurrentCheckImpl(RandomizerCheck check, GetItemID getItemId);
void ClearArchipelagoCurrentCheckImpl();
void GiveArchipelagoItemImpl();
