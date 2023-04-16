#pragma once
#include "../plugin_sdk/plugin_sdk.hpp"
#include "../helpers/PredictionHelper.h"

class Champion
{
public:
	virtual ~Champion() = default;

	virtual void Load();
	virtual void Unload();

	virtual void OnUpdate();

	virtual void OnDraw();
	virtual void OnEnvDraw();

	virtual void OnCreateObject(game_object_script object);
	virtual void OnDeleteObject(game_object_script object);

	virtual void OnBuffGain(game_object_script target, buff_instance_script buff);

	virtual void OnProcessSpellCast(game_object_script sender, spell_instance_script spell);
	virtual void OnCastSpell(spellslot slot, game_object_script target, vector& position, vector& position2, bool isCharge, bool* process);


protected:

	static inline uint32_t immuneSpells[]
	{
		spell_hash("EvelynnR"),
		spell_hash("ZedR"),
		spell_hash("EkkoR"),
		spell_hash("FizzE"),
		spell_hash("FizzETwo"),
		spell_hash("FizzEBuffer"),
		spell_hash("XayahR"),
		spell_hash("VladimirSanguinePool")
	};

	bool IsYuumiAttached(game_object_script target)
	{
		if (target->get_champion() != champion_id::Yuumi)
			return false;

		const auto& yuumiBuff = target->get_buff(buff_hash("YuumiWAttach"));
		return yuumiBuff && yuumiBuff->get_caster()->get_handle() == target->get_handle();
	}
	
	bool IsValidCustom(game_object_script target)
	{
		if (!target || !target->is_valid())
			return false;

		if (target->is_dead())
			return false;

		const auto& hasAuroraPrediction = menu->get_tab("aurora_prediction");
		if ((!hasAuroraPrediction || hasAuroraPrediction->is_hidden()) && !target->is_visible())
			return false;
		
		if (target->is_ai_hero())
		{
			if (this->IsYuumiAttached(target))
				return false;

			const auto& activeSpell = target->get_active_spell();
			const auto& isCastingImmortilitySpell = activeSpell ? std::any_of(
				std::begin(this->immuneSpells),
				std::end(this->immuneSpells),
				[&](uint32_t spellHash) {
					return activeSpell->get_spell_data()->get_name_hash() == spellHash;
				}) : false;

			if (isCastingImmortilitySpell || target_selector->is_invulnerable(target))
				return false;
			
		}
		
		return true;
	}

	bool IsStunned(game_object_script target)
	{
		return target->has_buff_type({ 
			buff_type::Stun, 
			buff_type::Taunt, 
			buff_type::Polymorph, 
			buff_type::Fear, 
			buff_type::Charm, 
			buff_type::Suppression, 
			buff_type::Knockup, 
			buff_type::Knockback, 
			buff_type::Asleep,
			buff_type::Snare
		});
	}

	hit_chance GetHitchanceFromConfig(TreeEntry* entry)
	{
		switch (entry->get_int())
		{
		case 0:
			return hit_chance::low;
		case 1:
			return hit_chance::medium;
		case 2:
			return hit_chance::high;
		case 3:
			return hit_chance::very_high;
		case 4:
			return hit_chance::dashing;
		default:
			return hit_chance::immobile;
		}
	}

	game_object_script GetSelectedTarget()
	{
		const auto& target = target_selector->get_selected_target();
		if (!this->IsValidCustom(target))
			return nullptr;

		auto output = prediction->get_prediction(target, 1.0f);
		if (output.hitchance == hit_chance::impossible) // FoW.
			return nullptr;

		return target;
	}
	
	game_object_script GetTarget(float range, damage_type type, std::function<bool(game_object_script target)> validation = [](game_object_script target) { return true;  })
	{
		const auto& selectedTarget = this->GetSelectedTarget();
		if (selectedTarget && validation(selectedTarget)) {
			return selectedTarget;
		}
		
		std::vector<game_object_script> targets = {};
		for (auto const& enemy : entitylist->get_enemy_heroes())
		{
			if (!this->IsValidCustom(enemy))
				continue;
			
			if (range != FLT_MAX && enemy->get_position().distance(myhero->get_position()) > range)
				continue;
		
			if (validation(enemy))
			{
				targets.push_back(enemy);
			}
		}

		if (targets.empty()) return nullptr;
		return target_selector->get_target(targets, type);
	}

	std::pair<game_object_script, prediction_output> GetTarget(script_spell* spell, damage_type type, hit_chance hitchance)
	{
		const auto& selectedTarget = this->GetSelectedTarget();
		if (selectedTarget) {
			return std::make_pair(selectedTarget, spell->get_prediction(selectedTarget));
		}
		
		std::vector<std::pair<game_object_script, prediction_output>> targets = {};
		std::vector<game_object_script> enemies = {};
		
		for (auto const& enemy : entitylist->get_enemy_heroes())
		{
			if (!this->IsValidCustom(enemy))
				continue;

			if (spell->range() != FLT_MAX && enemy->get_position().distance(myhero->get_position()) > spell->range())
				continue;
			
			auto output = spell->get_prediction(enemy);
			if (output.hitchance >= hitchance)
			{
				targets.push_back(std::make_pair(enemy, output));
				enemies.push_back(enemy);
			}
		}

		if (targets.empty()) return { nullptr, prediction_output() };

		const auto& target = target_selector->get_target(enemies, type);
		return *std::find_if(targets.begin(), targets.end(), [target](const std::pair<game_object_script, prediction_output>& pair) {
			return pair.first == target;
		});
	}
	
	std::vector<game_object_script> GetTargets(float range, std::function<bool(game_object_script target)> validation = [](game_object_script target) { return true;  })
	{
		std::vector<game_object_script> targets = {};
		for (auto const& enemy : entitylist->get_enemy_heroes())
		{
			if (!this->IsValidCustom(enemy))
				continue;

			if (range != FLT_MAX && enemy->get_position().distance(myhero->get_position()) > range)
				continue;

			if (validation(enemy))
			{
				targets.push_back(enemy);
			}
		}

		return targets;
	}
};	
