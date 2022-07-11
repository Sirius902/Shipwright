#include "archipelago.h"

#include <unordered_map>
#include <optional>

extern "C" {
    #include "functions.h"
}

#ifdef osSyncPrintf
#undef osSyncPrintf
#endif
#define osSyncPrintf(fmt, ...) lusprintf(__FILE__, __LINE__, 0, fmt, __VA_ARGS__)

extern std::unordered_map<std::string, RandomizerCheck> SpoilerfileCheckNameToEnum;

static std::optional<RandomizerCheck> currentCheck;

std::string ArchipelagoItemText() {
    return "\x1A\x13\x23\x08" "You got " "\x05\x41" "Aly's " "\x05\x42" "Kokiri Sword" "\x05\x40" "!" "\x02";
}

void SetArchipelagoCurrentCheckImpl(RandomizerCheck check, GetItemID getItemId) {
    if (getItemId == GI_ARCHIPELAGO_ITEM) {
        currentCheck = check;
    }
}

void ClearArchipelagoCurrentCheckImpl() {
    currentCheck = {};
}

void GiveArchipelagoItemImpl() {
    RandomizerCheck check = currentCheck ? *currentCheck : RC_UNKNOWN_CHECK;

    std::string checkName;
    for (const auto& [name, ch] : SpoilerfileCheckNameToEnum) {
        if (ch == check) {
            checkName = name;
            break;
        }
    }

    osSyncPrintf("Archipelago Check Obtained: \"%s\"\n", checkName.c_str());
}
