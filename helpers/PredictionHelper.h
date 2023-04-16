#include "../plugin_sdk/plugin_sdk.hpp"

// THANKS BREAKCODER FOR SPECIAL DASH DATABASE/LOGIC.
// THANKS BREAKCODER FOR SPECIAL DASH DATABASE/LOGIC.
// THANKS BREAKCODER FOR SPECIAL DASH DATABASE/LOGIC.
struct SpecialDashData {
	//vector start;
	//vector end;
	//
	//float speed;
	//float castTime;	 
	//float castStartTime;
	//
	//game_object_script origin;

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

	std::vector<SpecialDashData> GetSpecialDash(game_object_script enemy);
	std::pair<bool, vector> GetCastPositionOnSpecialDash(script_spell* spell, SpecialDashData data, int maxCollisions = 1);
	bool CanCastOnSpecialDash(script_spell* spell, SpecialDashData data, float extraDelay = 0.0f);
	
	std::pair<bool, vector> GetExpectedPosition(game_object_script enemy, float delay = 0.f);
	std::pair<bool, vector> GetCastPositionOnSpecialBuff(script_spell* spell, game_object_script target, int maxCollisions = 1);
	
	// SDK EVENTS
	void OnUpdate();
	void OnCreateObject(game_object_script object);
	void OnDeleteObject(game_object_script object);
	
	void OnBuffGain(game_object_script object, buff_instance_script buff);
	void OnBuffLose(game_object_script object, buff_instance_script buff);
	
	void OnProcessSpellCast(game_object_script sender, spell_instance_script spell);
	std::map<uint32_t, SpecialDashData> specialDashs; // nameHash, data
private:
	static std::unique_ptr<PredictionHelper> instance;
	
	std::map<uint32_t, float> miaFor; // networkId, time
	std::map<uint32_t, float> specialBuffs; // networkId, buffEndTime
	game_object_script yoneShadowObject;

};