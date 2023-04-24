#include "../plugin_sdk/plugin_sdk.hpp"

struct SpecialDashData {
	
	vector startPosition;
	vector endPosition;
	float speed;
	float castTime;
	float startTime;
	game_object_script origin;

	SpecialDashData(vector startPosition, vector endPosition, float speed, float castTime, float startTime, game_object_script origin) {
		this->startPosition = startPosition;
		this->endPosition = endPosition;
		this->speed = speed;
		this->castTime = castTime;
		this->startTime = startTime;
		this->origin = origin;
	}
	
	virtual bool operator ==(const SpecialDashData* rhs)
	{
		return rhs->startTime == this->startTime && rhs->origin == this->origin;
	}
};


class PredictionHelper {
public:
	static PredictionHelper* GetInstance();
	
	PredictionHelper();

	SpecialDashData* GetSpecialDash(game_object_script enemy, float range = 0.0f);
	bool CanCastOnSpecialDash(script_spell* spell, SpecialDashData* dash, float extraDelay = 0.0f);
	std::pair<bool, vector> GetCastPositionOnSpecialDash(script_spell* spell, SpecialDashData* dash, int maxCollisions = 1);
	
	bool HasSpecialBuff(game_object_script target);
	bool CanCastOnSpecialBuff(script_spell* spell, game_object_script target, float extraDelay = 0.0f, int maxCollisions = 1);
	
	std::pair<bool, vector> GetExpectedPosition(game_object_script enemy, float delay = 0.f);
	
	// SDK EVENTS
	void OnUpdate();
	
	// SPECIAL DASH
	void OnCreateObject(game_object_script object);
	void OnDeleteObject(game_object_script object);
	void OnProcessSpellCast(game_object_script sendPositioner, spell_instance_script spell);
	void OnNetworkPacket(game_object_script sender, std::uint32_t network_id, pkttype_e type, void* args);
	
	// SPECIAL BUFFS
	void OnBuffGain(game_object_script object, buff_instance_script buff);
	void OnBuffLose(game_object_script object, buff_instance_script buff);
private:
	static std::unique_ptr<PredictionHelper> instance;
	std::set<uint32_t> SPECIAL_BUFFS_HASH = {
		buff_hash("ZhonyasRingShield"),
		buff_hash("ChronoRevive"),
		buff_hash("BardRStasis"),
		buff_hash("LissandraRSelf")
	};
	
	std::map<uint32_t, float> miaTracker; // networkId, time
	std::map<uint32_t, float> specialBuffs; // networkId, buffEndTime

	std::map<uint32_t, std::shared_ptr<SpecialDashData>> specialDashs; // nameHash, data
	game_object_script yoneShadowObject;
};