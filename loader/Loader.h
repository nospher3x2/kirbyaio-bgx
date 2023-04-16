#pragma once
#include "../plugin_sdk/plugin_sdk.hpp"
#include "../champions/Champion.h"

namespace Loader
{

	bool Load();
	void Unload();

	namespace Callbacks
	{
		void Register();
		void Remove();
		
		void OnUpdate();
		void OnDraw();
		void OnEnvDraw();
		void OnCreateObject(game_object_script object);
		void OnDeleteObject(game_object_script object);
		void OnBuffGain(game_object_script object, buff_instance_script buff);
		void OnProcessSpellCast(game_object_script sender, spell_instance_script spell);
		void OnCastSpell(spellslot slot, game_object_script target, vector& position, vector& position2, bool isCharge, bool* process);

	}
}