#include "Champion.h"

void Champion::Load()
{
}

void Champion::Unload()
{
}

void Champion::OnUpdate()
{
}

void Champion::OnDraw()
{
}

void Champion::OnEnvDraw()
{
}

void Champion::OnCreateObject(game_object_script object)
{
}

void Champion::OnDeleteObject(game_object_script object)
{
}

void Champion::OnBuffGain(game_object_script target, buff_instance_script buff)
{
}

void Champion::OnProcessSpellCast(game_object_script sender, spell_instance_script spell)
{
}

void Champion::OnCastSpell(spellslot slot, game_object_script target, vector& position, vector& position2, bool isCharge, bool* process)
{
	
}

void Champion::OnNewPath(game_object_script sender, const std::vector<vector>& path, bool isDash, float dashSpeed)
{
}

void Champion::OnGapcloser(game_object_script sender, antigapcloser::antigapcloser_args* args)
{
}