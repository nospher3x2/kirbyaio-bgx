#include "DashDatabase.h"


namespace DashDatabase
{
	std::map<champion_id, std::vector<SpellDash>> dashs;
	
	std::map<uint32_t, std::map<spellslot, float>> spellCooldowns;
	std::map<uint32_t, std::pair<int, int>> takedowns; // Kills, Assists
	
	std::pair<bool, uint32_t> hasKatarina = { false, 0 };
	
	void Load()
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

		//for (const auto& hero : entitylist->get_all_heroes())
		//{
		//	if (!hero || !hero->is_valid())
		//		continue;
		//
		//	if (hero->get_champion() == champion_id::Katarina)
		//	{
		//		hasKatarina = std::make_pair(true, hero->get_network_id());
		//	}
		//}
	}

	std::vector<SpellDash> GetDashes(champion_id champion) {
		auto it = dashs.find(champion);
		if (it == dashs.end())
			return std::vector<SpellDash>();

		return it->second;
	}

	void OnProcessSpellCast(game_object_script sender, spell_instance_script spell)
	{
		if (!sender->is_ai_hero())
			return;

		const auto& champion = sender->get_champion();
		if (champion == champion_id::Katarina && spell->get_spellslot() == spellslot::e)
		{
			const auto& RAW_COOLDOWN = 15.5 - (1.5 * spell->get_level());
			spellCooldowns[sender->get_network_id()] = {
				{ spellslot::e, gametime->get_time() + CalculateSpellCooldown(sender, RAW_COOLDOWN) }
			};
			return;
		}
	}

	void OnBuffGain(game_object_script sender, buff_instance_script buff)
	{
		if (!sender->is_ai_hero())
			return;

		const auto& champion = sender->get_champion();
		if (champion == champion_id::Katarina && buff->get_hash_name() == buff_hash("KatarinaDaggers"))
		{
			const auto& CAPPED_LEVEL = sender->get_level() >= 16 ? 16 : sender->get_level();
			const auto& COOLDOWN_REDUCTION = 78.0 + (6.0 * std::floor(CAPPED_LEVEL / 5));
			const auto& ACTUAL_COOLDOWN = spellCooldowns[sender->get_network_id()][spellslot::e] - gametime->get_time();
			
			spellCooldowns[sender->get_network_id()] = {
				{ spellslot::e, gametime->get_time() + ACTUAL_COOLDOWN * (COOLDOWN_REDUCTION/100.f) }
			};
		}
	}

	void OnObjectDead(game_object_script sender, buff_instance_script buff)
	{
		if (!sender->is_ai_hero())
			return;

		const auto& katarina = entitylist->get_object_by_network_id(hasKatarina.second);
		
		auto& takedown = takedowns[hasKatarina.second];
		auto itsTakedown = false;

		const auto& cachedKills = takedown.first;
		const auto& katarinaKills = katarina->get_hero_stat(int_hero_stat::CHAMPIONS_KILLED);
		if (katarinaKills > cachedKills)
		{
			itsTakedown = true;
			takedown.first = katarinaKills;
		}

		if (!itsTakedown)
		{
			auto cachedAssists = takedown.second;
			auto katarinaAssists = myhero->get_hero_stat(int_hero_stat::ASSISTS);
			if (katarinaAssists > cachedAssists)
			{
				itsTakedown = true;
				takedown.second = katarinaAssists;
			}
		}

		if (itsTakedown)
		{
			spellCooldowns[katarina->get_network_id()] = {
				{ spellslot::e, 0 }
			};
		}
	}

	float CalculateSpellCooldown(game_object_script target, float rawCooldown)
	{
		// TO-DO: Calculate Spell Cooldown
		return 0.0f;
	}
}