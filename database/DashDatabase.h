#pragma once
#include "../plugin_sdk/plugin_sdk.hpp"

enum DashType {
	Blink,
	Dash
};

typedef struct {
	std::string name;
	spellslot slot;
	DashType type;
	bool enabledDefault;
	std::function<bool(game_object_script)> canCast;
} SpellDash;

struct DashDatabase {
	static inline std::map<champion_id, std::vector<SpellDash>> dashes = {
		{// Katarina
			champion_id::Katarina,
			{
				{ "[E] Shumpo", spellslot::e, DashType::Blink, true, [](game_object_script target) {
						return std::any_of(
							entitylist->get_attackable_objects().begin(),
							entitylist->get_attackable_objects().end(),
							[&](game_object_script object) {
								return target->get_position().distance(object->get_position()) <= 725.f;
							});
					}
				}
			},
		},
		{
			champion_id::Fizz,
			{
				{ "[E] Playful/Trickster", spellslot::e, DashType::Dash, true, [](game_object_script target) {
						return true;
					}
				}
			},
		},
		{
			champion_id::Leblanc, {
				{ "[W] Distortion", spellslot::w, DashType::Dash, false, [](game_object_script target) {
						return true;
					}
				}
			},
		},
		{
			champion_id::Akali, {
				{ "[E] Shuriken Flip", spellslot::e, DashType::Dash, false, [](game_object_script target) {
						return true;
					}
				}
			},
		},
		{
			champion_id::Yasuo, {
				{ "[E] Sweeping Blade", spellslot::e, DashType::Dash, false, [](game_object_script target) {
						return std::any_of(
							entitylist->get_attackable_objects().begin(),
							entitylist->get_attackable_objects().end(),
							[&](game_object_script object) {
								return object->is_targetable_to_team(target->get_team())
									&& target->get_position().distance(object->get_position()) <= 475.f;
							});
					}
				}
			},
		},
		{
			champion_id::Zed, {
				{ "[R] Living Shadow", spellslot::r, DashType::Blink, false, [](game_object_script target) {
						return std::any_of(
							entitylist->get_all_heroes().begin(),
							entitylist->get_all_heroes().end(),
							[&](game_object_script object) {
								return object->is_targetable_to_team(target->get_team())
									&& target->get_position().distance(object->get_position()) <= 625.f;
							});
					}
				}
			},
		},
		{
			champion_id::Qiyana, {
				{ "[W] Terrashape", spellslot::w, DashType::Dash, true, [](game_object_script target) {
						return true;
					}
				},
				{ "[E] Bladecaller", spellslot::e, DashType::Dash, false, [](game_object_script target) {
						return std::any_of(
							entitylist->get_attackable_objects().begin(),
							entitylist->get_attackable_objects().end(),
							[&](game_object_script object) {
								return object->is_targetable_to_team(target->get_team())
									&& target->get_position().distance(object->get_position()) <= 650.f;
							});
					}
				}
			},
		},
		{
			champion_id::Kled, {
				{ "[E] Jousting", spellslot::e, DashType::Dash, false, [](game_object_script target) {
						return true;
					}
				}
			}
		},
		{
			champion_id::Riven, {
				{ "[Q] Broken Wings", spellslot::q, DashType::Dash, false, [](game_object_script target) {
						return true;
					}
				},
				{ "[E] Valor", spellslot::e, DashType::Dash, true, [](game_object_script target) {
						return true;
					}
				}
			},
		},
		{
			champion_id::Shen, {
				{ "[E] Shadow Dash", spellslot::e, DashType::Dash, false, [](game_object_script target) {
						return true;
					}
				}
			},
		},
		{
			champion_id::Kassadin, {
				{ "[R] Riftwalk", spellslot::r, DashType::Blink, false, [](game_object_script target) {
						return true;
					}
				}
			},
		},
		{
			champion_id::Lucian, {
				{ "[E] Relentless Pursuit", spellslot::e, DashType::Dash, true, [](game_object_script target) {
						return true;
					}
				}
			},
		},
		{
			champion_id::Vayne, {
				{ "[Q] Tumble", spellslot::q, DashType::Dash, false, [](game_object_script target) {
						return true;
					}
				}
			},
		}
	};

	static inline bool isDashable(game_object_script target) {
		const auto& dashes = DashDatabase::dashes.find(target->get_champion());
		if (dashes == DashDatabase::dashes.end()) {
			return false;
		}

		for (const auto& dash : dashes->second) {
			if (target->get_spell_state(dash.slot) == spell_state::Ready && dash.canCast(target)) {
				return true;
			}
		}

		return false;
	}
};