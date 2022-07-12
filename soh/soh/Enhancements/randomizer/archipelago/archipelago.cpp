#include "archipelago.h"

#include <unordered_map>
#include <optional>
#include <cmath>

#include "../libultraship/libultraship/luslog.h"

extern "C" {
    #include "z64.h"
    #include "macros.h"

    extern GlobalContext* gGlobalCtx;
}

extern std::unordered_map<std::string, RandomizerCheck> SpoilerfileCheckNameToEnum;

static std::optional<RandomizerCheck> currentCheck;

std::string ArchipelagoItemText() {
    return "\x1A\x13\x23\x08" "You got " "\x05\x41" "Aly's " "\x05\x42" "Kokiri Sword" "\x05\x40" "!" "\x02";
}

void SetArchipelagoCurrentCheckImpl(RandomizerCheck check, GetItemID getItemId) {
    if (std::abs(getItemId) == GI_ARCHIPELAGO_ITEM) {
        currentCheck = check;
    }
}

void GiveArchipelagoItemImpl() {
    if (!currentCheck) {
        lusprintf(__FILE__, __LINE__, 0, "Archipelago Check Params not set!\n");
        return;
    }

    RandomizerCheck check = *currentCheck;

    std::string checkName;
    for (const auto& [name, ch] : SpoilerfileCheckNameToEnum) {
        if (ch == check) {
            checkName = name;
            break;
        }
    }

    lusprintf(__FILE__, __LINE__, 0, "Archipelago Check Obtained: \"%s\"\n", checkName.c_str());

    currentCheck = {};
}
