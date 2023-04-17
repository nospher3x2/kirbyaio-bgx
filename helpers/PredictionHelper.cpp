#include "PredictionHelper.h"

std::unique_ptr<PredictionHelper> PredictionHelper::instance = nullptr;

PredictionHelper* PredictionHelper::GetInstance()
{
	if (!PredictionHelper::instance)
	{
		PredictionHelper::instance = std::make_unique<PredictionHelper>();
	}

	return PredictionHelper::instance.get();
}

PredictionHelper::PredictionHelper()
{

};

SpecialDashData* PredictionHelper::GetSpecialDash(game_object_script enemy, float range)
{
	for (const auto& [hash, data] : this->specialDashs)
	{
		if (!data->origin || !data->origin->is_valid())
			continue;
		
		if (data->origin->get_network_id() == enemy->get_network_id())
		{
			if (range > 0 && data->dash_start.distance(data->dash_end) > range)
				continue;
			
			return data.get();
		}
	}

	return nullptr;
}

std::pair<bool, vector> PredictionHelper::GetCastPositionOnSpecialDash(script_spell* spell, SpecialDashData* dash, int maxCollisions)
{
	std::pair<bool, vector> output = std::make_pair(false, vector{});
	bool hit_other = false;
	bool isZoe = dash->origin->get_champion() == champion_id::Zoe;

	float travelTime = dash->cast_time + dash->dash_start.distance(dash->dash_end) / dash->dash_speed;
	float maxDistance = !isZoe ? (dash->origin->get_bounding_radius() + spell->get_radius()) / dash->origin->get_move_speed() : 0.75f;
	float endTime = dash->dash_start_time + travelTime;

	float timeToHit = myhero->get_position().distance(dash->dash_end) / spell->get_speed() + (ping->get_ping() / 1000.f) / 2 + spell->get_delay() + gametime->get_time();

	float waitTime = endTime - timeToHit;
	float waitLimit = !isZoe ? waitTime + maxDistance / 1.5f : waitTime + maxDistance;
	if (waitLimit > 0 && waitTime <= 0) {
		std::vector<game_object_script> colisions = spell->get_collision(myhero->get_position(), { dash->dash_end });
		//if (!colisions.empty()) {
		//	for (auto& colision : colisions) {
		//		if (colision != data.origin) {
		//			hit_other = true;
		//			break;
		//		}
		//	}
		//}

		if (!hit_other) {
			output.first = true;
			output.second = dash->dash_end;
			return output;
		}
	}
	else if (endTime <= gametime->get_time()) {
		console->print("deletado pq mt old");
		for (auto it = this->specialDashs.begin(); it != this->specialDashs.end(); ++it) {
			if (it->second->dash_start_time == dash->dash_start_time
				&& it->second->origin->get_network_id() == dash->origin->get_network_id()) {
				this->specialDashs.erase(it);
				break;
			}
		};
	}
	
	return output;
}

bool PredictionHelper::CanCastOnSpecialDash(script_spell* spell, SpecialDashData* dash, float extraDelay)
{
	float travelTime = dash->cast_time + dash->dash_start.distance(dash->dash_end) / dash->dash_speed;
	float endTime = dash->dash_start_time + travelTime;
	float timeToHit = myhero->get_position().distance(dash->dash_end) / spell->get_speed() + (ping->get_ping() / 2000.f) + spell->get_delay() + (extraDelay > 0 ? extraDelay + ping->get_ping()/2000.f : 0.f) + gametime->get_time();
	
	return endTime > timeToHit;
}

std::pair<bool, vector> PredictionHelper::GetExpectedPosition(game_object_script object, float delay)
{
	//auto tracker = this->miaTracker.find(object->get_network_id());
	//if (tracker == miaTracker.end())
	//{
	//	miaTracker[object->get_network_id()] = {
	//		gametime->get_time(),
	//		object->get_position()
	//	};
	//	
	//	tracker = miaTracker.find(object->get_network_id());
	//}
	//
	//if (object->is_visible())
	//{
	//	tracker->second.lastSeen = gametime->get_time();
	//	tracker->second.lastSeenPosition = object->get_position();
	//}
	//
	//auto const& lastSeenTime = gametime->get_time() - tracker->second.lastSeen;
	//if (lastSeenTime > 0.3f) {
	//	return std::make_pair(false, vector(0, 0, 0));
	//}
	//
	//const auto& pathController = object->get_path_controller();
	//if (pathController->get_path().empty()) {
	//	const auto& positionExpected = object->get_position() + (object->get_velocity() * (delay + lastSeenTime));
	//	return std::make_pair(true, positionExpected);
	//};
	//
	//const auto& path = pathController->get_path();
	//const auto& lastPath = path[path.size() - 1];
	//
	//auto const& lastSeenPosition = tracker->second.lastSeenPosition;
	//
	//auto const& timeToLastPath = (lastPath - lastSeenPosition) / lastSeenTime;
	//auto const& positionExpected = lastSeenPosition + (timeToLastPath * (delay + lastSeenTime));
	//
	//return std::make_pair(true, positionExpected);
}

std::pair<bool, vector> PredictionHelper::GetCastPositionOnSpecialBuff(script_spell* spell, game_object_script target, int maxCollisions)
{
	
}

void PredictionHelper::OnUpdate()
{
	// MIA TRACKER
	//for (const auto& [networkId, data] : this->miaTracker)
	//{
	//	auto const& object = entitylist->get_object_by_network_id(networkId);
	//	if (object == nullptr)
	//	{
	//		this->miaTracker.erase(networkId);
	//		continue;
	//	}
	//
	//	if (object->is_visible())
	//	{
	//		this->miaTracker[networkId] = {
	//			gametime->get_time(),
	//			object->get_position()
	//		);
	//	}
	//}
}

void PredictionHelper::OnCreateObject(game_object_script object)
{
	auto emitter = object->get_emitter();
	if (!emitter || !emitter->is_ai_hero())
		return;

	auto emitterHash = object->get_emitter_resources_hash();
	auto emitterPosition = emitter->get_position();
	auto particlePosition = object->get_position();
	auto target = object->get_particle_attachment_object();

	switch (emitterHash)
	{
	case buff_hash("Yone_E_Beam"):
	{
		this->yoneShadowObject = object;
		break;
	}
	case buff_hash("Ekko_R_ChargeIndicator"):
	{
		this->specialDashs[emitterHash] = std::make_unique<SpecialDashData>(
			emitterPosition,
			particlePosition,
			FLT_MAX,
			0.5f-(ping->get_ping()/1000.f),
			gametime->get_time(),
			emitter
		);
		break;
	}
	case buff_hash("Evelynn_R_Landing"):
	{
		this->specialDashs[emitterHash] = std::make_unique<SpecialDashData>(
			emitterPosition,
			particlePosition,
			FLT_MAX,
			0.85f - (ping->get_ping() / 1000.f),
			gametime->get_time(),
			emitter
		);
		break;
	}
	case buff_hash("Pantheon_R_Update_Indicator_Enemy"):
	{
		const auto& castPosition = object->get_position() + object->get_particle_rotation_forward() * 1350;
		this->specialDashs[emitterHash] = std::make_unique<SpecialDashData>(
			emitterPosition,
			castPosition,
			FLT_MAX,
			2.2f - (ping->get_ping() / 1000.f),
			gametime->get_time(),
			emitter
		);
		break;
	}
	case buff_hash("Galio_R_Tar_Ground_Enemy"):
	{
		this->specialDashs[emitterHash] = std::make_unique<SpecialDashData>(
			emitterPosition,
			particlePosition,
			FLT_MAX,
			2.75f - (ping->get_ping() / 1000.f),
			gametime->get_time(),
			emitter
		);
		break;
	}
	case buff_hash("TahmKench_W_ImpactWarning_Enemy"):
	{
		this->specialDashs[emitterHash] = std::make_unique<SpecialDashData>(
			emitterPosition,
			particlePosition,
			FLT_MAX,
			0.8f - (ping->get_ping() / 1000.f),
			gametime->get_time(),
			emitter
		);
		break;
	}
	case 463291848: // Yone R
	{
		const auto& realDelay = 0.75f - (ping->get_ping() / 1000.f);
		prediction_input input;
		input._from = particlePosition;
		input.speed = FLT_MAX;
		input.delay = realDelay - (ping->get_ping() / 1000.f) / 2; // AVOID PRED PING CALC
		input.collision_objects = { collisionable_objects::allies };
		input.range = 1000.f;
		input.use_bounding_radius = true;
		input.type = skillshot_type::skillshot_line;
		input.radius = 112.5f;

		vector endPosition = particlePosition + object->get_particle_rotation_forward().normalized() * (1000);
		const auto& result = prediction->get_collision({ endPosition }, &input);

		if (!result.empty())
		{
			std::vector<std::pair<game_object_script, float>> alliesHitted;

			for (const auto& collision : result)
			{
				if (collision->is_me()) continue;
				alliesHitted.push_back(std::make_pair(collision, collision->get_position().distance(particlePosition)));
			}

			if (!alliesHitted.empty())
			{
				int i = 0;
				for (auto& [ally, distance] : alliesHitted) {
					vector ally_pos = prediction->get_prediction(ally, realDelay).get_unit_position();
					if (!ally_pos.is_valid()) {
						alliesHitted.erase(alliesHitted.begin() + i);
						continue;

					}

					distance = particlePosition.distance(ally_pos);
					i++;
				}

				std::sort(alliesHitted.begin(), alliesHitted.end(), [](const auto& a, const auto& b) {
					return a.second > b.second;
					});

				const auto& distanceToLastTarget = alliesHitted.front().second;

				endPosition = particlePosition + object->get_particle_rotation_forward().normalized() * (distanceToLastTarget + 200.f);
			}
		}

		this->specialDashs[emitterHash] = std::make_unique<SpecialDashData>(
			emitterPosition,
			endPosition,
			FLT_MAX,
			0.75f,
			gametime->get_time(),
			emitter
		);
		break;
	}
	case buff_hash("TwistedFate_R_Gatemarker_Red"):
	{
		this->specialDashs[emitterHash] = std::make_unique<SpecialDashData>(
			emitterPosition,
			particlePosition,
			FLT_MAX,
			1.5f - (ping->get_ping() / 1000.f),
			gametime->get_time(),
			emitter
		);
		break;
	}
	case buff_hash("Viego_R_Tell"):
	{
		this->specialDashs[emitterHash] = std::make_unique<SpecialDashData>(
			emitterPosition,
			particlePosition,
			FLT_MAX,
			0.25f - (ping->get_ping() / 1000.f),
			gametime->get_time(),
			emitter
		);
		break;
	}
	case buff_hash("Zed_R_tar_TargetMarker"):
	{
		if (target && target->is_me())
		{
			const auto& castPosition = target->get_position() + (emitter->get_direction() * (target->get_bounding_radius() + emitter->get_bounding_radius()));
			this->specialDashs[emitterHash] = std::make_unique<SpecialDashData>(
				emitterPosition,
				castPosition,
				FLT_MAX,
				0.95f - (ping->get_ping() / 1000.f),
				gametime->get_time(),
				emitter
			);
		}
		break;
	}
	case buff_hash("Zoe_R_portal_exit"):
	{
		if (emitter->is_visible() && emitter->has_buff(buff_hash("zoepassivesheenbuff")))
		{
			buff_instance_script passive = emitter->get_buff(buff_hash("zoepassivesheenbuff"));
			float remainingTime = passive->get_remaining_time();
			float startTime = gametime->get_time() - (5.f - remainingTime);

			this->specialDashs[emitterHash] = std::make_unique<SpecialDashData>(
				emitterPosition,
				particlePosition,
				FLT_MAX,
				0.25f - (ping->get_ping() / 1000.f),
				startTime,
				emitter
			);
		}
		break;
	}
	default:
		break;
	}
}

void PredictionHelper::OnDeleteObject(game_object_script object)
{
	auto emitterHash = object->get_emitter_resources_hash();

	switch (emitterHash) {
	case buff_hash("Yone_E_Beam"): {
		this->specialDashs.erase(spell_hash("YoneE"));
		this->yoneShadowObject = nullptr;
		break;
	}
	case buff_hash("Yone_R_Cas_Residual"): {
		this->specialDashs.erase(spell_hash("YoneR"));
		break;
	}
	}
}

void PredictionHelper::OnProcessSpellCast(game_object_script sender, spell_instance_script spell)
{
	if (!sender->is_enemy())
		return;

	const auto& spellHash = spell->get_spell_data()->get_name_hash();

	switch (spellHash)
	{
	case spell_hash("FiddleSticksR"):
	{
		const auto& endPosition = sender->get_position().extend(
			spell->get_end_position(), 
			fmin(800, sender->get_position().distance(spell->get_end_position()))
		);

		this->specialDashs[spellHash] = std::make_unique<SpecialDashData>(
			sender->get_position(),
			endPosition,
			FLT_MAX,
			1.5f - (ping->get_ping()/1000.f),
			gametime->get_time(),
			sender
		);
		break;
	}
	case spell_hash("YoneE"):
	{
		myhero->print_chat(0x1, "dhasuhsa");
		if (this->yoneShadowObject && this->yoneShadowObject->is_valid())
		{
			myhero->print_chat(0x1, "registrado");

			this->specialDashs[spellHash] = std::make_unique<SpecialDashData>(
				sender->get_position(),
				this->yoneShadowObject->get_position(),
				3000.f,
				0.25f - (ping->get_ping() / 1000.f),
				gametime->get_time(),
				sender
			);
		}
		break;
	}
	}
}