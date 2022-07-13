#pragma once

#include <string>
#include <unordered_map>

#include "randomizerTypes.h"

extern "C" {
    #include "ultra64/types.h"
    #include "z64item.h"
}

std::string ArchipelagoItemText();

void SetArchipelagoCurrentCheckImpl(RandomizerCheck check, GetItemID getItemId);
void GiveArchipelagoItemImpl();

void InitArchipelago();
void GenerateArchipelagoConfig(std::unordered_map<RandomizerSettingKey, u8> cvarSettings);
