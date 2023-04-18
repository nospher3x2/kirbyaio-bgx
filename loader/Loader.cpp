#include "Loader.h"

#include "../champions/Lux/Lux.h"

std::unique_ptr<Champion> champion = nullptr;
bool Loader::Load()
{

	// Load the champion
		champion = std::make_unique<Lux>();
	switch (myhero->get_champion())
	{
	case champion_id::Lux:
		break;
	default:
		console->print("[YessAIO] Champion not supported.");
		break;
	}

	if (champion) {
		champion->Load();
		Callbacks::Register();
		return true;
	}

	return false;
}

void Loader::Unload()
{
	if (champion) {
		Callbacks::Remove();
		champion->Unload();
	}
}

void Loader::Callbacks::Register()
{
	event_handler<events::on_update>::add_callback(OnUpdate);
	event_handler<events::on_draw>::add_callback(OnDraw);
	event_handler<events::on_env_draw>::add_callback(OnEnvDraw);
	event_handler<events::on_create_object>::add_callback(OnCreateObject);
	event_handler<events::on_delete_object>::add_callback(OnDeleteObject);
	event_handler<events::on_buff_gain>::add_callback(OnBuffGain);
	event_handler<events::on_process_spell_cast>::add_callback(OnProcessSpellCast);
	event_handler<events::on_cast_spell>::add_callback(OnCastSpell);
	event_handler<events::on_network_packet>::add_callback(OnNetworkPacket);
}

void Loader::Callbacks::Remove()
{
	event_handler<events::on_update>::remove_handler(OnUpdate);
	event_handler<events::on_draw>::remove_handler(OnDraw);
	event_handler<events::on_env_draw>::remove_handler(OnEnvDraw);
	event_handler<events::on_create_object>::remove_handler(OnCreateObject);
	event_handler<events::on_delete_object>::remove_handler(OnDeleteObject);
	event_handler<events::on_buff_gain>::remove_handler(OnBuffGain);
	event_handler<events::on_process_spell_cast>::remove_handler(OnProcessSpellCast);
	event_handler<events::on_cast_spell>::remove_handler(OnCastSpell);
	event_handler<events::on_network_packet>::remove_handler(OnNetworkPacket);
}

void Loader::Callbacks::OnUpdate()
{
	champion->OnUpdate();
}

void Loader::Callbacks::OnDraw()
{
	champion->OnDraw();
}

void Loader::Callbacks::OnEnvDraw()
{
	champion->OnEnvDraw();
}

void Loader::Callbacks::OnCreateObject(game_object_script object)
{
	champion->OnCreateObject(object);
	PredictionHelper::GetInstance()->OnCreateObject(object);
}

void Loader::Callbacks::OnDeleteObject(game_object_script object)
{
	champion->OnDeleteObject(object);
	PredictionHelper::GetInstance()->OnDeleteObject(object);
}

void Loader::Callbacks::OnBuffGain(game_object_script target, buff_instance_script buff)
{
	champion->OnBuffGain(target, buff);
	//PredictionHelper::GetInstance()->OnBuffGain(target, buff);
}

void Loader::Callbacks::OnProcessSpellCast(game_object_script sender, spell_instance_script spell)
{
	champion->OnProcessSpellCast(sender, spell);
	PredictionHelper::GetInstance()->OnProcessSpellCast(sender, spell);
}

void Loader::Callbacks::OnCastSpell(spellslot slot, game_object_script target, vector& position, vector& position2, bool isCharge, bool* process)
{
	champion->OnCastSpell(slot, target, position, position2, isCharge, process);
}

void Loader::Callbacks::OnNetworkPacket(game_object_script sender, std::uint32_t network_id, pkttype_e type, void* args)
{
	PredictionHelper::GetInstance()->OnNetworkPacket(sender, network_id, type, args);
}