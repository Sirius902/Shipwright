#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "randomizerTypes.h"

extern "C" {
    #include "ultra64/types.h"
    #include "z64item.h"
}

class Archipelago {
private:
    std::optional<RandomizerCheck> currentCheck;

public:
    static constexpr std::size_t nameMaxLength = 16;

    std::string getItemText();
    void setCurrentCheck(RandomizerCheck check, GetItemID getItemId);
    void obtainCheck();

    static std::shared_ptr<Archipelago> getInstance();
    static void generateConfig(const std::unordered_map<RandomizerSettingKey, u8>& cvarSettings);
};

void Archipelago_Init();
