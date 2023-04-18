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
	void load();
	std::vector<SpellDash> getDashes(champion_id champion);
}