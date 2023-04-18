#pragma once
#include "../Champion.h"
#include "../../database/DashDatabase.h"
class Lux : public Champion
{
public:
	void Load() override;
	void Unload() override;

	void OnUpdate() override;
	void OnDraw() override;
	void OnEnvDraw() override;

	void OnCreateObject(game_object_script object) override;
	void OnDeleteObject(game_object_script object) override;

	void OnBuffGain(game_object_script object, buff_instance_script buff) override;
	void OnCastSpell(spellslot slot, game_object_script target, vector& position, vector& position2, bool isCharge, bool* process) override;

	/*
	* Definitions
	*/
	int GetSingularityState();
	bool HasPassive(game_object_script target);
	bool QWillHit(game_object_script target);

	/*
	* Logics
	*/
	bool CastQ();
	bool CastE();

	void SemiCastQ();
	void SemiCastR();
	
	void AutomaticCastQ(); // On Dashing, On Special Skills, Zhonyas etc.
	void AutomaticCastE2(); // On Killable or if is leaving out E.
	void AutomaticCastE2InPredict(); // Auto Attack and Ultimate
	void AutomaticCastUltimate(); // In CC, OnKillable if On SpecialSkills/Zhonyas

	void AutomaticCastComboOnSpecialDash();


	/*
	* Damages
	*/
	float GetPassiveDamage(game_object_script target);
	float GetQDamage(game_object_script target);
	float GetWDamage(game_object_script target);
	float GetEDamage(game_object_script target);
	float GetRDamage(game_object_script target);
	float GetFullComboDamage(game_object_script target);
	
		
protected:
	std::vector<game_object_script> particles;
	
	struct {
		game_object_script object;
		geometry::polygon circle;
	} e_data;

	struct {
		game_object_script object;
		geometry::polygon line;
		float castedAt;
	} r_data;
	
	struct {
		game_object_script missile = nullptr;
		game_object_script target = nullptr;
	} attack;
	
	TreeTab* main = nullptr;
	struct {
		script_spell* q = nullptr;
		script_spell* w = nullptr;
		script_spell* e = nullptr;
		script_spell* r = nullptr;
	} spells;

	struct {
		TreeEntry* use_q = nullptr;
		TreeEntry* use_invisible_q = nullptr;
		TreeEntry* use_collision_q = nullptr;
		
		TreeEntry* wait_dash_for_use_q = nullptr;
		std::map<champion_id, 
			std::vector<std::pair<spellslot, TreeEntry*>>
		> q_dash_whitelist = {};
		TreeEntry* ignore_whitelist_if_key_pressed = nullptr;
		TreeEntry* ignore_whitelist_if_slow = nullptr;

		TreeEntry* use_w = nullptr;
		TreeEntry* w_incoming_damage = nullptr;
		std::map<uint32_t, TreeEntry*> w_incoming_damage_whitelist;
		
		TreeEntry* use_e = nullptr;
		TreeEntry* use_r = nullptr;
	} combo;

	struct {
		TreeEntry* use_q_on_dash = nullptr;
		TreeEntry* use_q_on_special_skills = nullptr;
		TreeEntry* use_q_on_special_items = nullptr;
		TreeEntry* use_q_anti_melee = nullptr;
		TreeEntry* use_q_anti_gapclose = nullptr;

		TreeEntry* use_e_on_cc = nullptr;
		TreeEntry* use_e_on_slow = nullptr;
		
		TreeEntry* use_r_on_killable = nullptr;
		TreeEntry* use_r_on_cc = nullptr;
		TreeEntry* try_r_on_slow = nullptr;
		TreeEntry* use_r_on_special_skills = nullptr;
		TreeEntry* use_r_on_special_items = nullptr;
	} automatic;

	struct {
		TreeEntry* q = nullptr;
		TreeEntry* e = nullptr;
		TreeEntry* r = nullptr;
		
		TreeEntry* q_fow = nullptr;
		TreeEntry* e_fow = nullptr;
		TreeEntry* r_fow = nullptr;
	} hitchance;
	
	struct {
		struct {
			TreeEntry* q = nullptr;
			TreeEntry* q_color = nullptr;
			
			TreeEntry* w = nullptr;
			TreeEntry* w_color = nullptr;

			TreeEntry* e = nullptr;
			TreeEntry* e_color = nullptr;

			TreeEntry* r = nullptr;
			TreeEntry* r_color = nullptr;
		} spells;

		struct {
			TreeEntry* enabled = nullptr;
			TreeEntry* color = nullptr;
		} damage;

		struct {
			TreeEntry* draw_farm = nullptr;
			TreeEntry* draw_killable = nullptr;
		} misc;

	} renderer;

	struct {
		TreeEntry* spell_farm = nullptr;
		TreeEntry* semi_q = nullptr;
		TreeEntry* semi_e = nullptr;
		TreeEntry* semi_r = nullptr;
	} hotkeys;


};

