#include "../plugin_sdk/plugin_sdk.hpp"

// THANKS BREAKCODER FOR SPECIAL DASH DATABASE/LOGIC.
// THANKS BREAKCODER FOR SPECIAL DASH DATABASE/LOGIC.
// THANKS BREAKCODER FOR SPECIAL DASH DATABASE/LOGIC.
struct SpecialDashData {
	vector dash_start;
	vector dash_end;
	float dash_speed;
	float cast_time;
	float dash_start_time;
	game_object_script origin;
};


class PredictionHelper {
public:
	static PredictionHelper* GetInstance();
	
	PredictionHelper();

	SpecialDashData* GetSpecialDash(game_object_script enemy, float range = 0.0f);
	bool CanCastOnSpecialDash(script_spell* spell, SpecialDashData* dash, float extraDelay = 0.0f);
	std::pair<bool, vector> GetCastPositionOnSpecialDash(script_spell* spell, SpecialDashData* dash, int maxCollisions = 1);
	
	std::pair<bool, vector> GetExpectedPosition(game_object_script enemy, float delay = 0.f);
	std::pair<bool, vector> GetCastPositionOnSpecialBuff(script_spell* spell, game_object_script target, int maxCollisions = 1);
	
	// SDK EVENTS
	void OnUpdate();
	
	// SPECIAL DASH
	void OnCreateObject(game_object_script object);
	void OnDeleteObject(game_object_script object);
	void OnProcessSpellCast(game_object_script sender, spell_instance_script spell);
	
	// SPECIAL BUFFS
	void OnBuffGain(game_object_script object, buff_instance_script buff);
	void OnBuffLose(game_object_script object, buff_instance_script buff);
	
private:
	static std::unique_ptr<PredictionHelper> instance;
	
	std::map<uint32_t, std::shared_ptr<SpecialDashData>> specialDashs; // nameHash, data
	std::map<uint32_t, float> miaFor; // networkId, time
	std::map<uint32_t, float> specialBuffs; // networkId, buffEndTime
	game_object_script yoneShadowObject;
};