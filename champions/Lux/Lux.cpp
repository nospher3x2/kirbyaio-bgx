#include "Lux.h"
#include <math.h>

void Lux::Load()
{
	DashDatabase::load();
	
	this->main = menu->create_tab("kirby_lux", "[KirbyAIO] Lux");
	this->main->set_assigned_texture(myhero->get_square_icon_portrait());
	this->main->add_separator("header", "[KirbyAIO] Lux ~ 1.0");
	
	auto const combo = this->main->add_tab("combo", "Combo");
	{
		combo->add_separator("q_settings", "[Q] Settings");
		{
			this->combo.use_q = combo->add_checkbox("use_q", "Use Q", true);
			this->combo.use_invisible_q = combo->add_checkbox("use_invisible_q", "Use Invisible Q (W After)", true);
			this->combo.use_collision_q = combo->add_checkbox("q_collision", "Use Q Collision on Minion Logic", true);
			
			this->combo.wait_dash_for_use_q = combo->add_checkbox("wait_dash_for_use_q", "Dont use Q if enemy has dash", true);
			const auto& dash_database = combo->add_tab("dash_database", " ^~ Dash Database");
			{
				dash_database->add_separator("header", "Enemy Dash Database");
				const auto& enemies = entitylist->get_enemy_heroes();
				for (auto const& enemy : enemies)
				{
					const auto& dashs = DashDatabase::getDashes(enemy->get_champion());
					if (dashs.size() > 0)
					{
						const auto& tab = dash_database->add_tab(enemy->get_model(), enemy->get_model());
						tab->set_assigned_texture(enemy->get_square_icon_portrait());
						tab->add_separator("champ", enemy->get_model());
						{
							for (auto const& dash : dashs)
							{
								this->combo.q_dash_whitelist[enemy->get_champion()].emplace_back(dash, tab->add_checkbox(dash.name, dash.name, dash.enabledDefault));
								this->combo.q_dash_whitelist[enemy->get_champion()].back().second->set_texture(enemy->get_spell(dash.slot)->get_icon_texture());
							}
						}
					}
				}
				dash_database->add_separator("missing", "If you want to add a dash, please contact me on discord.");
			}
			this->combo.ignore_whitelist_if_key_pressed = combo->add_hotkey("ignore_whitelist_if_key_pressed", " ^~ Ignore Dash", TreeHotkeyMode::Hold, 0x1, false);
			this->combo.ignore_whitelist_if_slow = combo->add_checkbox("ignore_whitelist_if_slow", " ^~ Ignore Dash if Slowed", false);
		}

		combo->add_separator("w_settings", "[W] Settings");
		{
			this->combo.use_w = combo->add_checkbox("use_w", "Use W", true);
			this->combo.w_incoming_damage = combo->add_slider("w_damage", "Use W only if Incoming Damage >= X% HP", 10, 1, 99);
			const auto whitelist = combo->add_tab("whitelist", " ^~ Ally Whitelist");
			{
				for (const auto& ally : entitylist->get_ally_heroes())
				{
					this->combo.w_incoming_damage_whitelist[ally->get_network_id()] = whitelist->add_checkbox(ally->get_model(), ally->get_model(), true);
					this->combo.w_incoming_damage_whitelist[ally->get_network_id()]->set_texture(ally->get_square_icon_portrait());
				}
			}
		}

		combo->add_separator("e_settings", "[E] Settings");
		{
			this->combo.use_e = combo->add_checkbox("use_e", "Use E", true);
		}
	}
	
	auto const harass = this->main->add_tab("harass", "Harass");
	{
		
	}
	
	auto const farming = this->main->add_tab("farming", "Farming");
	{

	}
	
	auto const automatic = this->main->add_tab("automatic", "Automatic");
	{
		
	}

	const auto hitchance = this->main->add_tab("hitchance", "Hitchance");
	{
		hitchance->add_separator("spells", "Spells Hitchance");
		{
			this->hitchance.q = hitchance->add_combobox("q", "Q Hitchance", { {"Low",nullptr},{"Medium",nullptr },{"High", nullptr},{"Very High",nullptr} }, 2);
			this->hitchance.e = hitchance->add_combobox("e", "E Hitchance", { {"Low",nullptr},{"Medium",nullptr },{"High", nullptr},{"Very High",nullptr} }, 2);
			this->hitchance.r = hitchance->add_combobox("r", "R Hitchance", { {"Low",nullptr},{"Medium",nullptr },{"High", nullptr},{"Very High",nullptr} }, 2);
		}
		hitchance->add_separator("fow", "FOW Hitchance");
		{
			this->hitchance.q_fow = hitchance->add_combobox("q_fow", "Q Hitchance", { {"Low",nullptr},{"Medium",nullptr },{"High", nullptr},{"Very High",nullptr} }, 1);
			this->hitchance.e_fow = hitchance->add_combobox("e_fow", "E Hitchance", { {"Low",nullptr},{"Medium",nullptr },{"High", nullptr},{"Very High",nullptr} }, 1);
			this->hitchance.r_fow = hitchance->add_combobox("r_fow", "R Hitchance", { {"Low",nullptr},{"Medium",nullptr },{"High", nullptr},{"Very High",nullptr} }, 1);
		}
	}
	
	auto const renderer = this->main->add_tab("renderer", "Renderer");
	{
		auto const spells = renderer->add_tab("spells", "Spells Range");
		{
			float Q_DEFAULT_COLOR[] = { 255.f, 255.f, 255.f, 255.f };
			
			this->renderer.spells.q = spells->add_checkbox("q", "Should Draw Q Range", true);
			this->renderer.spells.q_color = spells->add_colorpick("q_color", " ^~ Color", Q_DEFAULT_COLOR);
			
			float W_DEFAULT_COLOR[] = { 255.f, 255.f, 255.f, 255.f };
			this->renderer.spells.w = spells->add_checkbox("w", "Should Draw W Range", false);
			this->renderer.spells.w_color = spells->add_colorpick("w_color", " ^~ Color", W_DEFAULT_COLOR);
			
			float E_DEFAULT_COLOR[] = { 255.f, 255.f, 255.f, 255.f };
			this->renderer.spells.e = spells->add_checkbox("e", "Should Draw E Range", true);
			this->renderer.spells.e_color = spells->add_colorpick("e_color", " ^~ Color", E_DEFAULT_COLOR);

			float R_DEFAULT_COLOR[] = { 255.f, 255.f, 255.f, 255.f };
			this->renderer.spells.r = spells->add_checkbox("r", "Should Draw R Range", false);
			this->renderer.spells.r_color = spells->add_colorpick("r_color", " ^~ Color", R_DEFAULT_COLOR);
		}

		auto const damage = renderer->add_tab("damage", "Damage Indicator");
		{
			float DEFAULT_COLOR[] = { 255.f, 255.f, 255.f, 255.f };
			this->renderer.damage.enabled = damage->add_checkbox("enabled", "Should Draw Damage Indicator", true);
			this->renderer.damage.color = damage->add_colorpick("color", " ^~ Color", DEFAULT_COLOR);
		}

		auto const misc = renderer->add_tab("misc", "Misc");
		{
			this->renderer.misc.draw_farm = misc->add_checkbox("draw_farm", "Draw Farm State", true);
			this->renderer.misc.draw_killable = misc->add_checkbox("draw_killable", "Draw Killable Warning", true);
		}
	}
	
	this->main->add_separator("hotkeys_header", "~ Hotkeys ~");
	{
		this->hotkeys.spell_farm = this->main->add_hotkey("farm", "Toggle Farm", TreeHotkeyMode::Toggle, 'A', true, false);
		this->hotkeys.semi_q = this->main->add_hotkey("semi_q", "Semi Q", TreeHotkeyMode::Hold, 'G', true, false);
		this->hotkeys.semi_e = this->main->add_hotkey("semi_e", "Semi E", TreeHotkeyMode::Hold, 'J', true, false);
		this->hotkeys.semi_r = this->main->add_hotkey("semi_r", "Semi R", TreeHotkeyMode::Hold, 'T', true, false);
	}

	this->spells.q = plugin_sdk->register_spell(spellslot::q, 1300.f);
	this->spells.w = plugin_sdk->register_spell(spellslot::w, 1175.f);
	this->spells.e = plugin_sdk->register_spell(spellslot::e, 1100.f);
	this->spells.r = plugin_sdk->register_spell(spellslot::r, 3400.f);

	this->spells.q->set_skillshot(
		0.25f,
		70.f,
		1200.f,
		{
			collisionable_objects::yasuo_wall,
			collisionable_objects::minions,
			collisionable_objects::heroes
		},
		skillshot_type::skillshot_line
	);

	this->spells.w->set_skillshot(
		0.25f,
		110.f,
		2400.f,
		{
			collisionable_objects::yasuo_wall
		},
		skillshot_type::skillshot_line
	);

	this->spells.e->set_skillshot(
		0.25f,
		310.f,
		1200.f,
		{
			collisionable_objects::yasuo_wall
		},
		skillshot_type::skillshot_circle
	);

	this->spells.r->set_skillshot(
		1.0f,
		100.f,
		FLT_MAX,
		{},
		skillshot_type::skillshot_line
	);
}

void Lux::Unload()
{
	menu->delete_tab(this->main);
}

void Lux::OnUpdate()
{
	if (this->hotkeys.semi_q->get_bool())
		this->CastQ();

	if (this->hotkeys.semi_r->get_bool())
		this->SemiCastR();

	if (this->e_data.object->is_valid())
	{
		this->AutomaticCastE2();
		this->AutomaticCastE2InPredict();
	}
	
	if (!myhero->is_recalling())
	{
		this->AutomaticCastUltimate();
		this->AutomaticCastComboOnSpecialDash();
	}

	if (orbwalker->combo_mode())
	{
		if (this->combo.use_q->get_bool())
			this->CastQ();
		
		if (this->combo.use_e->get_bool())
			this->CastE();
	}
}

void Lux::OnDraw()
{
	auto const position = myhero->get_position();

	if (this->renderer.misc.draw_farm->get_bool())
	{
		auto const spellFarmIsEnabled = this->hotkeys.spell_farm->get_bool();
		draw_manager->add_text(
			position,
			spellFarmIsEnabled ? D3DCOLOR_ARGB(200, 0, 255, 0) : D3DCOLOR_ARGB(200, 255, 0, 0),
			15,
			"Spell Farm: %s", spellFarmIsEnabled ? "[ON]" : "[OFF]"
		);
	}

	// Draw R Polygon
	if (this->r_data.object && this->r_data.object->is_valid())
	{
		auto& polygon = this->r_data.line;
		int pointsSize = polygon.points.size();
		pointsSize -= 1;

		for (int i = 0; i <= pointsSize; i++) {
			auto nextIndex = (polygon.points.size() - 1 == i) ? 0 : (i + 1);
			draw_manager->add_line(
				polygon.points[i],
				polygon.points[nextIndex],
				D3DCOLOR_ARGB(255, 255, 255, 255)
			);
		}
	}
}

void Lux::OnEnvDraw()
{
	if (this->renderer.spells.q->get_bool())
	{
		auto const color = this->renderer.spells.q_color->get_color();
		draw_manager->add_circle(myhero->get_position(), this->spells.q->range(), color);
	}

	if (this->renderer.spells.w->get_bool())
	{
		auto const color = this->renderer.spells.w_color->get_color();
		draw_manager->add_circle(myhero->get_position(), this->spells.w->range(), color);
	}

	if (this->renderer.spells.e->get_bool())
	{
		auto const color = this->renderer.spells.e_color->get_color();
		draw_manager->add_circle(myhero->get_position(), this->spells.e->range(), color, 2);
	}

	if (this->renderer.spells.r->get_bool())
	{
		auto const color = this->renderer.spells.r_color->get_color();
		draw_manager->add_circle(myhero->get_position(), this->spells.r->range(), color);
	}

}

void Lux::OnCreateObject(game_object_script object)
{
	if (object->get_emitter_resources_hash() == buff_hash("Lux_E_tar_aoe_green"))
	{
		this->e_data.object = object;
		this->e_data.circle = geometry::circle(object->get_position(), 300).to_polygon();
		return;
	}

	if (object->get_emitter_resources_hash() == buff_hash("Lux_R_cas"))
	{
		const auto& endPosition = myhero->get_position() + myhero->get_direction().normalized() * (this->spells.r->range() + myhero->get_bounding_radius() / 2.f);
		
		this->r_data.object = object;
		this->r_data.line = geometry::rectangle(myhero->get_position(), endPosition, 100).to_polygon();
		this->r_data.castedAt = gametime->get_time();
		return;
	}

	if (object->is_missile() && object->missile_is_targeted() && object->missile_get_sender_id() == myhero->get_id())
	{
		const auto& target = entitylist->get_object(object->missile_get_target_id());
		const auto& hash = object->get_missile_sdata()->get_name_hash();
		if (target->is_ai_hero() && (hash == spell_hash("LuxBasicAttack") || hash == spell_hash("LuxBasicAttack2")))
		{
			this->attack.missile = object;
			this->attack.target = target;
		}
		return;
	}
}

void Lux::OnDeleteObject(game_object_script object)
{
	const auto& id = object->get_network_id();;

	if (this->e_data.object && this->e_data.object->get_network_id() == id)
	{
		this->e_data.object = nullptr;
		this->e_data.circle = geometry::circle(vector(0, 0, 0), 0).to_polygon();
		return;
	}

	if (this->r_data.object && this->r_data.object->get_network_id() == id)
	{
		this->r_data.object = nullptr;
		this->r_data.line = geometry::rectangle(vector(0, 0, 0), vector(0, 0, 0), 0).to_polygon();
		this->r_data.castedAt = 0;
		return;
	}

	if (this->attack.missile && this->attack.missile->get_network_id() == id)
	{
		this->attack.missile = nullptr;
		this->attack.target = nullptr;
		return;
	}
}

void Lux::OnBuffGain(game_object_script object, buff_instance_script buff)
{
	if (buff->get_hash_name() == buff_hash("LuxIlluminatingFraulein") && myhero->is_in_auto_attack_range(object) && orbwalker->can_attack()) {
		myhero->issue_order(object, true, false);
	}
}

void Lux::OnCastSpell(spellslot slot, game_object_script target, vector& position, vector& position2, bool isCharge, bool* process)
{
	if (slot != spellslot::q || !orbwalker->combo_mode())
		return;

	if (this->combo.use_invisible_q->get_bool() && this->spells.w->is_ready())
	{
		scheduler->delay_action(0.25f + ping->get_ping() / 1000 + (1.f/30.f), [this, position]() {
			this->spells.w->cast(position);
		});
		return;
	}
}

int Lux::GetSingularityState()
{
	return myhero->get_spell(spellslot::e)->get_name_hash() == spell_hash("LuxLightStrikeToggle");
}

bool Lux::HasPassive(game_object_script target)
{
	return target->has_buff(buff_hash("LuxIlluminatingFraulein"));
}

bool Lux::IsDashable(game_object_script target)
{
	if (this->combo.ignore_whitelist_if_key_pressed->get_bool())
		return false;
	
	auto it = this->combo.q_dash_whitelist.find(target->get_champion());
	if (it == this->combo.q_dash_whitelist.end())
		return false;

	for (auto& pair : it->second)
	{
		if (pair.second->get_bool() == false)
			continue;
		
		if (target->get_spell_state(pair.first.slot) != spell_state::Ready)
			continue;
		
		if (pair.first.canCast(target))
			return true;
	}

	return false;
}

bool Lux::CastQ()
{
	if (!this->spells.q->is_ready())
		return false;

	std::pair<game_object_script, prediction_output> target_prediction = this->GetTarget(this->spells.q, damage_type::magical, hit_chance::low, [&](game_object_script target)
		{
			return !this->IsDashable(target);
		});
	if (!target_prediction.first || !target_prediction.first->is_valid())
		return false;
	
	const auto& target = target_prediction.first;
	auto& prediction = target_prediction.second;

	const auto& hitchance = target->is_visible() ?
		this->hitchance.q : this->hitchance.q_fow;

	if (prediction.hitchance >= this->GetHitchanceFromConfig(hitchance))
	{
		return this->spells.q->cast(prediction.get_cast_position());
	}

	// Minion Collision
	if (prediction.hitchance == hit_chance::collision && this->combo.use_collision_q->get_bool())
	{
		auto& collisions = prediction.collision_objects;
		const auto& position = myhero->get_position();
		std::sort(collisions.begin(), collisions.end(), [&](const game_object_script& a, const game_object_script& b) {
			return a->get_distance(position) < b->get_distance(position);
		});

		// i need check if first object is minion because yasuo wall.
		if (collisions.size() >= 2 && collisions[0]->is_minion() && collisions[1]->is_ai_hero())
		{	
			auto predictionWithoutCollision = this->spells.q->get_prediction_no_collision(target);
			if (predictionWithoutCollision.hitchance >= this->GetHitchanceFromConfig(this->hitchance.q))
			{
				return this->spells.q->cast(predictionWithoutCollision.get_cast_position());
			}
		}	
	}
	
	return false;
}                      

bool Lux::CastE()	
{
	if (!this->spells.e->is_ready())
		return false;

	if (this->GetSingularityState() == 0)
	{
		const auto& prediction_target = this->GetTarget(this->spells.e, damage_type::magical, hit_chance::low);
		if (!prediction_target.first || !prediction_target.first->is_valid())
			return false;

		const auto& target = prediction_target.first;
		prediction_output prediction = prediction_target.second;

		const auto& hitchance = target->is_visible() ?
			this->hitchance.e : this->hitchance.e_fow;

		if (prediction.hitchance >= this->GetHitchanceFromConfig(hitchance))
		{
			return spells.e->cast(prediction.get_cast_position());
		}
	}
	return false;	
}

void Lux::SemiCastR()
{
	if (!this->spells.r->is_ready())
		return;

	const auto& prediction_target = this->GetTarget(this->spells.r, damage_type::magical, hit_chance::low);
	if (!prediction_target.first || !prediction_target.first->is_valid())
		return;

	const auto& target = prediction_target.first;
	prediction_output prediction = prediction_target.second;

	const auto& hitchance = target->is_visible() ?
		this->hitchance.r : this->hitchance.r_fow;

	if (prediction.hitchance >= this->GetHitchanceFromConfig(hitchance))
	{
		this->spells.r->cast(prediction.get_cast_position());
		return;
	}
	
	return;
}

void Lux::AutomaticCastE2()
{
	const auto& enemies = this->GetTargets(FLT_MAX, [&](game_object_script target) {
		return !target_selector->is_invulnerable(target) && target->has_buff(buff_hash(("LuxESlow")));
		});

	if (enemies.size() <= 0)
		return;

	for (const auto& enemy : enemies)
	{
		// Auto Cast if enemy is leaving
		prediction_output output = prediction->get_prediction(enemy, ping->get_ping() / 1000 + (1.f / 15.f));
		if (this->e_data.circle.is_outside(output.get_unit_position()) && this->spells.e->cast())
			return;

		// Auto Cast if enemy will die; Refactor to use my own damage calculation
		if (this->spells.e->get_damage(enemy) >= enemy->get_real_health(false, true) && this->spells.e->cast())
			return;
	}
}

void Lux::AutomaticCastE2InPredict()
{
	// Close E when attack missile is about to hit to explode passive
	if (this->attack.missile && this->attack.target)
	{
		const auto& attackTarget = this->attack.target;
		if (!this->HasPassive(attackTarget) && attackTarget->has_buff(buff_hash("LuxESlow")))
		{
			const auto& willReachAt =
				this->attack.missile->get_distance(attackTarget->get_position()) / orbwalker->get_my_projectile_speed();

			if (willReachAt <= ping->get_ping() / 1000.f + (1.f / 30.f) && this->spells.e->cast())
				return;
		}
	}

	// Close E when R is about to hit to explode passive
	if (this->r_data.object && this->r_data.object->is_valid())
	{
		const auto& enemy = this->GetTarget(FLT_MAX, damage_type::magical, [&](game_object_script target) {
			return !target_selector->is_invulnerable(target) && target->has_buff(buff_hash("LuxESlow")) && this->r_data.line.is_inside(target->get_position());
		});

		if (enemy) 
		{
			const auto& canCloseE =
				this->r_data.castedAt + 0.80 + (ping->get_ping() / 1000.f + (1.f / 30.f)) <= gametime->get_time();

			if (canCloseE && this->spells.e->cast())
				return;
		}
	}
}

void Lux::AutomaticCastUltimate()
{
	if (!this->spells.r->is_ready())
		return; 

	const auto& enemies = this->GetTargets(this->spells.r->range());
	if (enemies.size() <= 0)
		return;

	for (const auto& enemy : enemies)
	{
		auto prediction = this->spells.r->get_prediction(enemy);
		if (prediction.hitchance == hit_chance::immobile)
		{
			this->spells.r->cast(prediction.get_cast_position());
			return;
		}
	}
}

void Lux::AutomaticCastComboOnSpecialDash()
{
	const auto& range = this->spells.q->is_ready() ? this->spells.q->range() : this->spells.e->range() + this->spells.e->get_radius();
	
	const auto& enemy = this->GetTarget(range, damage_type::physical, [&](game_object_script target) {
		const auto& dash = PredictionHelper::GetInstance()->GetSpecialDash(target, range + target->get_bounding_radius());
		return dash;
	});

	if (!enemy || !enemy->is_valid())
		return;
	
	const auto& dash = PredictionHelper::GetInstance()->GetSpecialDash(enemy);
	if (!dash) 
		return;
	
	bool CanCastEBeforeQ = this->spells.q->is_ready() ?
		PredictionHelper::GetInstance()->CanCastOnSpecialDash(this->spells.q, dash, this->spells.e->get_delay())
		: true;
	
	const auto& eRange = this->spells.e->range() + this->spells.e->get_radius();
	if (CanCastEBeforeQ && this->spells.e->is_ready() && this->GetSingularityState() == 0)
	{
		if (dash->endPosition.distance(myhero) <= eRange)
		{
			myhero->cast_spell(spellslot::e, dash->endPosition);
		}
	}

	if (this->spells.q->is_ready())
	{
		const auto& qPrediction = PredictionHelper::GetInstance()->GetCastPositionOnSpecialDash(this->spells.q, dash, 2);
		if (qPrediction.first)
		{
			this->spells.q->cast(qPrediction.second);
		}
	}

	
	if (!this->spells.q->is_ready() && !this->spells.e->is_ready() && this->spells.r->is_ready())
	{
		const auto& rPrediction = PredictionHelper::GetInstance()->GetCastPositionOnSpecialDash(this->spells.r, dash, 0);
		if (rPrediction.first)
		{
			if (this->spells.r->cast(rPrediction.second))
			{
				myhero->print_chat(0x1, "R1");
			}
		}
	}
}