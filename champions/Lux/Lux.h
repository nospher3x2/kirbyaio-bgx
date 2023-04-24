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
	void OnProcessSpellCast(game_object_script sender, spell_instance_script spell) override;
	void OnNewPath(game_object_script sender, const std::vector<vector>& path, bool isDash, float dashSpeed) override;

	void OnGapcloser(game_object_script sender, antigapcloser::antigapcloser_args* args) override;
	/*
	* Definitions
	*/
	int GetSingularityState();
	bool HasPassive(game_object_script target);
	bool IsInsideE(game_object_script target);
	bool HasDashAvailable(game_object_script target, bool onlyBlink = false);

	/*
	* Logics
	*/
	bool CastQ();
	bool CastE();

	void SemiCastQ();
	void SemiCastR();
	
	void AutomaticCastQ(); // On Dashing, On Special Skills, Zhonyas etc.
	void AutomaticCastE(); 
	void AutomaticCastE2(); // On Killable or if is leaving out E.
	void AutomaticCastE2InPredict(); // Auto Attack and Ultimate
	void AutomaticCastUltimate(); // In CC, OnKillable if On SpecialSkills/Zhonyas
	void AutomaticCastOnSpecialDash(); // Full Combo on YoneR, YoneE etc
	void AutomaticCastOnSpecialBuff(); // Full Combo on Statis


	/*
	* Damages
	*/
	float GetPassiveDamage(game_object_script target);
	float GetQDamage(game_object_script target);
	float GetEDamage(game_object_script target);
	float GetRDamage(game_object_script target);
	float GetFullComboDamage(game_object_script target);
	
		
protected:
	struct {
		game_object_script object;
		vector position;
		geometry::polygon circle;
		float createdAt;
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
		TreeEntry* invisible_q_mana = nullptr;
		TreeEntry* use_collision_q = nullptr;
		
		TreeEntry* wait_dash_for_use_q = nullptr;
		std::map<champion_id, 
			std::vector<std::pair<SpellDash, TreeEntry*>>
		> q_dash_whitelist = {};
		TreeEntry* ignore_whitelist_if_key_pressed = nullptr;

		TreeEntry* use_w = nullptr;
		TreeEntry* w_incoming_damage = nullptr;
		std::map<uint32_t, TreeEntry*> w_incoming_damage_whitelist;
		
		TreeEntry* use_e = nullptr;
	} combo;

	struct {
		TreeEntry* use_q = nullptr;
		TreeEntry* use_e = nullptr;
	} harass;

	struct {
		TreeEntry* use_q_on_dash = nullptr;
		TreeEntry* use_q_on_special_skills = nullptr;
		TreeEntry* use_q_on_special_items = nullptr;
		TreeEntry* use_q_anti_melee = nullptr;
		TreeEntry* use_q_anti_gapclose = nullptr;
		std::map<uint32_t, TreeEntry*> use_q_anti_gapclose_whitelist = {};

		TreeEntry* use_e_on_cc = nullptr;
		TreeEntry* use_e_on_special_skills = nullptr;
		TreeEntry* use_e_on_special_items = nullptr;
		TreeEntry* use_e_anti_gapclose = nullptr;
		std::map<uint32_t, TreeEntry*> use_e_anti_gapclose_whitelist = {};
		TreeEntry* use_e2 = nullptr;
		TreeEntry* use_e2_mode = nullptr;
		TreeEntry* use_e2_in_predict = nullptr;
		TreeEntry* use_e2_if_killable = nullptr;
		TreeEntry* use_e2_if_no_enemy_inside = nullptr;
		TreeEntry* dont_e2_if_aa_range = nullptr;
		TreeEntry* dont_e2_if_has_passive = nullptr;
		TreeEntry* ignore_checks_if_leaving = nullptr;
		
		TreeEntry* use_r_if_killable = nullptr;
		TreeEntry* use_r_on_cc = nullptr;
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

			TreeEntry* r_minimap = nullptr;
			TreeEntry* r_minimap_color = nullptr;
		} spells;

		struct {
			TreeEntry* enabled = nullptr;
			TreeEntry* color = nullptr;
		} damage;

		struct {
			TreeEntry* draw_ignoring_dash = nullptr;
			TreeEntry* draw_killable = nullptr;
			TreeEntry* draw_e_duration = nullptr;
		} misc;

	} renderer;

	struct {
		TreeEntry* semi_q = nullptr;
		TreeEntry* semi_e = nullptr;
		TreeEntry* semi_r = nullptr;
	} hotkeys;


};

