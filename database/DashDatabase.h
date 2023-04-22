#pragma once
#include "../plugin_sdk/plugin_sdk.hpp"

enum DashType {
	Blink,
	Dash
};

struct SpellDash {
	std::string name;
	spellslot slot;
	DashType type;
	bool enabledDefault;
	std::function<bool(game_object_script)> canCast;

	SpellDash(std::string name, spellslot slot, DashType type, bool enabledDefault, std::function<bool(game_object_script)> canCast = [](game_object_script target) { return true; })
	{
		this->name = name;
		this->slot = slot;
		this->type = type;
		this->enabledDefault = enabledDefault;
		this->canCast = canCast;
	}
};

namespace DashDatabase
{
	void Load();
	std::vector<SpellDash> GetDashes(champion_id champion);

	float CalculateSpellCooldown(game_object_script target, float rawCooldown);
	
	void OnProcessSpellCast(game_object_script sender, spell_instance_script spell);
	void OnBuffGain(game_object_script sender, buff_instance_script buff);
	void OnObjectDead(game_object_script object);
}