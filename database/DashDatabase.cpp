#include "DashDatabase.h"

namespace DashDatabase
{
	std::map<champion_id, std::vector<SpellDash>> dashs;
	
	void load()
	{
		// Katarina
		dashs.insert({ champion_id::Katarina, {
			{ "[E] Shumpo", spellslot::e, DashType::Blink, true, [](game_object_script target) {
					return std::any_of(
						entitylist->get_attackable_objects().begin(),
						entitylist->get_attackable_objects().end(),
						[&](game_object_script object) {
							return target->get_position().distance(object->get_position()) <= 725.f;
						});
				}
			}
		}});

		// Fizz
		dashs.insert({ champion_id::Fizz, {
			{ "[E] Playful/Trickster", spellslot::e, DashType::Dash, true }
		}});

		// Leblanc
		dashs.insert({ champion_id::Leblanc, {
			{ "[W] Distortion", spellslot::w, DashType::Dash, true },
			{ "[R] Mimic", spellslot::r, DashType::Dash, false, [](game_object_script target) {
					return true; // TO-DO: Check if R is W Clone
				}
			}
		} });

		// Yasuo
		dashs.insert({ champion_id::Yasuo, {
			{ "[E] Sweeping Blade", spellslot::e, DashType::Dash, false, [](game_object_script target) {
					return std::any_of(
						entitylist->get_attackable_objects().begin(),
						entitylist->get_attackable_objects().end(),
						[&](game_object_script object) {
							return target->get_position().distance(object->get_position()) <= 475.f;
						});
				}
			}
		} });

		// Akali
		dashs.insert({ champion_id::Akali, {
			{ "[E] Shuriken Flip", spellslot::e, DashType::Dash, false }
		} });

		// Riven
		dashs.insert({ champion_id::Riven, {
			{ "[Q] Broken Wings", spellslot::q, DashType::Dash, false },
			{ "[E] Valor", spellslot::e, DashType::Dash, true }
		} });

		// Kassadin
		dashs.insert({ champion_id::Kassadin, {
			{ "[R] Riftwalk", spellslot::r, DashType::Dash, false }
		} });

		// Zed
		dashs.insert({ champion_id::Zed, {
			{ "[R] Death Mark", spellslot::r, DashType::Blink, true, [](game_object_script target) {
					return std::any_of(
						entitylist->get_all_heroes().begin(),
						entitylist->get_all_heroes().end(),
						[&](game_object_script object) {
							return object->is_targetable_to_team(target->get_team())
								&& target->get_position().distance(object->get_position()) <= 625.f;
					});
				}
			}
		} });

		// Talon
		dashs.insert({ champion_id::Talon, {
			{ "[R] Shadow Assault", spellslot::r, DashType::Dash, false }
		} });

		// Irelia
		dashs.insert({ champion_id::Irelia, {
			{ "[Q] Bladesurge", spellslot::q, DashType::Dash, false, [](game_object_script target) {
					return std::any_of(
						entitylist->get_attackable_objects().begin(),
						entitylist->get_attackable_objects().end(),
						[&](game_object_script object) {
							return target->get_position().distance(object->get_position()) <= 600.f;
						});;
				}
			}
		} });

		// Kha'Zix
		dashs.insert({ champion_id::Khazix, {
			{ "[E] Leap", spellslot::e, DashType::Dash, false }
		} });

		// Lee Sin
		dashs.insert({ champion_id::LeeSin, {
			{ "[Q] Sonic Wave", spellslot::q, DashType::Dash, false, [](game_object_script target) {
					return target->get_spell(spellslot::q)->toogle_state() == 1;
				}
			}
		} });

		// Master Yi
		dashs.insert({ champion_id::MasterYi, {
			{ "[Q] Alpha Strike", spellslot::q, DashType::Dash, true, [](game_object_script target) {
					return std::any_of(
						entitylist->get_attackable_objects().begin(),
						entitylist->get_attackable_objects().end(),
						[&](game_object_script object) {
							return target->get_position().distance(object->get_position()) <= 600.f;
						});
				}
			}
		} });

		// Shaco
		dashs.insert({ champion_id::Shaco, {
			{ "[Q] Two-Shiv Poison", spellslot::q, DashType::Dash, true }
		} });

		// Tristana
		dashs.insert({ champion_id::Tristana, {
			{ "[W] Rocket Jump", spellslot::w, DashType::Dash, true }
		} });

		// Vayne
		dashs.insert({ champion_id::Vayne, {
			{ "[Q] Tumble", spellslot::q, DashType::Dash, true }
		} });

		// Fiora
		dashs.insert({ champion_id::Fiora, {
			{ "[Q] Lunge", spellslot::q, DashType::Dash, true }
		} });

		// Hecarim
		dashs.insert({ champion_id::Hecarim, {
			{ "[R] Onslaught of Shadows", spellslot::r, DashType::Dash, false }
		} });

		// Jax
		dashs.insert({ champion_id::Jax, {
			{ "[Q] Leap Strike", spellslot::q, DashType::Dash, true, [](game_object_script target) {
					return std::any_of(
						entitylist->get_attackable_objects().begin(),
						entitylist->get_attackable_objects().end(),
						[&](game_object_script object) {
							return target->get_position().distance(object->get_position()) <= 700.f;
						});
				}
			}
		} });

		// Kled
		dashs.insert({ champion_id::Kled, {
			{ "[E] Jousting", spellslot::e, DashType::Dash, true, [](game_object_script target) {
					return true; // TO-DO: Check if Kled is on Skarl
				}
			}
		} });

		// Lucian
		dashs.insert({ champion_id::Lucian, {
			{ "[E] Relentless Pursuit", spellslot::e, DashType::Dash, true }
		} });

		// Ezreal
		dashs.insert({ champion_id::Ezreal, {
			{ "[E] Arcane Shift", spellslot::e, DashType::Blink, true }
		} });

		// Vladimir
		dashs.insert({ champion_id::Vladimir, {
			{ "[W] Sanguine Pool", spellslot::w, DashType::Blink, true }
		} });
	}

	std::vector<SpellDash> getDashes(champion_id champion) {
		auto it = dashs.find(champion);
		if (it == dashs.end())
			return std::vector<SpellDash>();

		return it->second;
	}
}