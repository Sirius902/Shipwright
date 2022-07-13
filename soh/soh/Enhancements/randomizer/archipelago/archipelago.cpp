#include "archipelago.h"

#include <algorithm>
#include <fstream>
#include <filesystem>
#include <optional>
#include <cmath>
#include <string>
#include <vector>
#include <unordered_set>
#include <cstddef>

#include <yaml-cpp/yaml.h>
#include <Cvar.h>

#include "../libultraship/ImGuiImpl.h"
#include "soh/Enhancements/debugger/ImGuiHelpers.h"
#include "Lib/spdlog/include/spdlog/spdlog.h"

extern "C" {
    #include "z64.h"
    #include "macros.h"

    extern GlobalContext* gGlobalCtx;
}

namespace fs = std::filesystem;

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
        SPDLOG_ERROR("Archipelago Check Params not set!");
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

    SPDLOG_INFO("Archipelago Check Obtained ({})", checkName.c_str());

    currentCheck = {};
}

void DrawArchipelago(bool& open) {
    // ARCHIPELAGO_TODO: Add Archipelago window.
}

void InitArchipelago() {
    SohImGui::AddWindow("Randomizer", "Archipelago", DrawArchipelago);
}

const std::unordered_map<RandomizerSettingKey, const char*> settingNames{
    { RSK_FOREST, "open_forest" },
    { RSK_KAK_GATE, "open_kakariko" },
    { RSK_DOOR_OF_TIME, "open_door_of_time" },
    { RSK_ZORAS_FOUNTAIN, "zora_fountain" },
    { RSK_GERUDO_FORTRESS, "gerudo_fortress" },
    { RSK_RAINBOW_BRIDGE, "bridge" },
    { RSK_RAINBOW_BRIDGE_STONE_COUNT, "bridge_stones" },
    { RSK_RAINBOW_BRIDGE_MEDALLION_COUNT, "bridge_medallions" },
    { RSK_RAINBOW_BRIDGE_REWARD_COUNT, "bridge_rewards" },
    { RSK_RAINBOW_BRIDGE_DUNGEON_COUNT, "bridge_dungeons" },
    { RSK_RAINBOW_BRIDGE_TOKEN_COUNT, "bridge_tokens" },
    { RSK_RANDOM_TRIALS, "random_trials" },
    // ARCHIPELAGO_TODO: Change to trial_count once this setting is implemented correctly.
    { RSK_TRIAL_COUNT, "no_trials" },
    { RSK_STARTING_OCARINA, "starting_ocarina" },
    { RSK_SHUFFLE_OCARINA, "shuffle_ocarinas" },
    { RSK_STARTING_DEKU_SHIELD, "starting_deku_shield" },
    { RSK_STARTING_KOKIRI_SWORD, "starting_kokiri_sword" },
    { RSK_SHUFFLE_KOKIRI_SWORD, "shuffle_kokiri_sword" },
    { RSK_STARTING_MAPS_COMPASSES, "starting_maps_compasses" },
    { RSK_SHUFFLE_DUNGEON_REWARDS, "shuffle_dungeon_rewards" },
    { RSK_SHUFFLE_SONGS, "shuffle_songs" },
    { RSK_SHUFFLE_WEIRD_EGG, "shuffle_weird_egg" },
    { RSK_SHUFFLE_GERUDO_MEMBERSHIP_CARD, "shuffle_gerudo_card" },
    { RSK_ITEM_POOL, "item_pool" },
    { RSK_ICE_TRAPS, "ice_traps" },
    { RSK_GOSSIP_STONE_HINTS, "hints" },
    { RSK_HINT_CLARITY, "hint_clarity" },
    { RSK_HINT_DISTRIBUTION, "hint_distribution" },
    { RSK_GANONS_BOSS_KEY, "ganon_boss_key" },
    { RSK_SKIP_CHILD_STEALTH, "no_guard_stealth" },
    { RSK_SKIP_CHILD_ZELDA, "skip_child_zelda" },
    { RSK_STARTING_CONSUMABLES, "starting_consumables" },
    { RSK_FULL_WALLETS, "full_wallets" },
    { RSK_EXCLUDE_DEKU_THEATER_MASK_OF_TRUTH, "no_mask_of_truth" },
    { RSK_LANGUAGE, "language" },
    { RSK_EXCLUDE_KAK_10_GOLD_SKULLTULA_REWARD, "exclude_10_skulls" },
    { RSK_EXCLUDE_KAK_20_GOLD_SKULLTULA_REWARD, "exclude_20_skulls" },
    { RSK_EXCLUDE_KAK_30_GOLD_SKULLTULA_REWARD, "exclude_30_skulls" },
    { RSK_EXCLUDE_KAK_40_GOLD_SKULLTULA_REWARD, "exclude_40_skulls" },
    { RSK_EXCLUDE_KAK_50_GOLD_SKULLTULA_REWARD, "exclude_50_skulls" },
    { RSK_SHUFFLE_CHEST_MINIGAME, "shuffle_chest_minigame" },
    { RSK_CUCCO_COUNT, "chicken_count" },
    { RSK_BIG_POE_COUNT, "big_poe_count" },
    { RSK_SKIP_EPONA_RACE, "no_epona_race" },
    { RSK_SKIP_TOWER_ESCAPE, "no_escape_sequence" },
};

const std::unordered_map<RandomizerSettingKey, std::vector<const char*>> enumSettingValues{
    { RSK_FOREST, { "closed", "open", "closed_deku" } },
    { RSK_KAK_GATE, { "closed", "open" } },
    { RSK_DOOR_OF_TIME, { "open", "closed", "intended" } },
    { RSK_ZORAS_FOUNTAIN, { "normal", "adult", "open" } },
    { RSK_GERUDO_FORTRESS, { "normal", "fast", "open" } },
    { RSK_RAINBOW_BRIDGE, { "open", "vanilla", "stones", "medallions", "rewards", "dungeons", "tokens" } },
    { RSK_SHUFFLE_DUNGEON_REWARDS, { "end_of_dungeon", "any_dungeon", "overworld", "anywhere" } },
    { RSK_SHUFFLE_SONGS, { "song_locations", "dungeon_rewards", "anywhere" } },
    { RSK_GANONS_BOSS_KEY, { "start_with", "vanilla", "own_dungeon" } },
    { RSK_GOSSIP_STONE_HINTS, { "no_hints", "need_nothing", "mask_of_truth", "stone_of_agony" } },
    { RSK_HINT_CLARITY, { "obscure", "ambiguous", "clear" } },
    { RSK_HINT_DISTRIBUTION, { "useless", "balanced", "strong", "very strong" } },
    { RSK_ITEM_POOL, { "plentiful", "balanced", "scarce", "minimal" } },
    { RSK_ICE_TRAPS, { "off", "normal", "extra", "mayhem", "onslaught" } },
    { RSK_LANGUAGE, { "english", "german", "french" } },
};

const std::unordered_set<RandomizerSettingKey> booleanSettings{
    RSK_RANDOM_TRIALS,
    // ARCHIPELAGO_TODO: Remove once this setting is implemented correctly.
    RSK_TRIAL_COUNT,
    RSK_STARTING_OCARINA,
    RSK_SHUFFLE_OCARINA,
    RSK_STARTING_DEKU_SHIELD,
    RSK_STARTING_KOKIRI_SWORD,
    RSK_SHUFFLE_KOKIRI_SWORD,
    RSK_STARTING_MAPS_COMPASSES,
    RSK_SHUFFLE_WEIRD_EGG,
    RSK_SHUFFLE_GERUDO_MEMBERSHIP_CARD,
    RSK_SKIP_CHILD_STEALTH,
    RSK_SKIP_CHILD_ZELDA,
    RSK_STARTING_CONSUMABLES,
    RSK_FULL_WALLETS,
    RSK_EXCLUDE_DEKU_THEATER_MASK_OF_TRUTH,
    RSK_EXCLUDE_KAK_10_GOLD_SKULLTULA_REWARD,
    RSK_EXCLUDE_KAK_20_GOLD_SKULLTULA_REWARD,
    RSK_EXCLUDE_KAK_30_GOLD_SKULLTULA_REWARD,
    RSK_EXCLUDE_KAK_40_GOLD_SKULLTULA_REWARD,
    RSK_EXCLUDE_KAK_50_GOLD_SKULLTULA_REWARD,
    RSK_SHUFFLE_CHEST_MINIGAME,
    RSK_SKIP_EPONA_RACE,
    RSK_SKIP_TOWER_ESCAPE,
};

static void EmitSettings(YAML::Emitter& out, std::unordered_map<RandomizerSettingKey, u8> cvarSettings) {
    for (const auto& [k, v] : cvarSettings) {
        // Skip because RSK_TRIAL_COUNT isn't properly implemented yet.
        if (k == RSK_RANDOM_TRIALS) {
            continue;
        }

        out << YAML::Key << settingNames.at(k) << YAML::Value;

        if (auto values = enumSettingValues.find(k); values != enumSettingValues.end()) {
            out << values->second.at(v);
        } else if (booleanSettings.find(k) != booleanSettings.end()) {
            out << YAML::SingleQuoted << (v != 0 ? "true" : "false");
        } else {
            out << static_cast<std::size_t>(v);
        }
    }
}

void GenerateArchipelagoConfig(std::unordered_map<RandomizerSettingKey, u8> cvarSettings) {
    YAML::Emitter out;

    const char* game = "Ship of Harkinian";

    out << YAML::BeginMap;
    out << YAML::Key << game;
    out << YAML::BeginMap;
    {
        out << YAML::Key << "settings";
        out << YAML::BeginMap;
        EmitSettings(out, cvarSettings);
        out << YAML::EndMap;
    }
    out << YAML::EndMap;

    out << YAML::Key << "description";
    out << YAML::Value << YAML::SingleQuoted << "Generated by https://github.com/Sirius902/Shipwright/tree/archipelago";
    out << YAML::Key << "game";
    out << YAML::Value << game;
    {
        out << YAML::Key << "name";
        // ARCHIPELAGO_TODO: Reject name if empty and display error to user.
        // ARCHIPELAGO_TOOD: Remove carriage return filter once controller ui pr gets in.
        const char* emptyName = "EmptyNameError";
        std::string playerName = CVar_GetString("gArchipelagoName", emptyName);
        if (playerName.empty()) {
            playerName = emptyName;
        }
        playerName.erase(std::remove(playerName.begin(), playerName.end(), '\r'), playerName.end());
        out << YAML::Value << playerName;
    }
    out << YAML::EndMap;

    fs::path configPath = "Archipelago/APsoh.yaml";
    fs::create_directory(configPath.parent_path());

    std::ofstream config{configPath};
    if (!config.is_open()) {
        SPDLOG_ERROR("({}) Failed to create Archipelago config.", configPath.string());
    }

    config << out.c_str();
}
