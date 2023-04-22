#include "Lux.h"
#include <math.h>

void Lux::Load()
{
	DashDatabase::Load();
	
	this->main = menu->create_tab("kirby_lux", "[KirbyAIO] Lux");
	this->main->set_assigned_texture(myhero->get_square_icon_portrait());
	this->main->add_separator("header", "[KirbyAIO] Lux ~ 1.0");
	
	auto const combo = this->main->add_tab("combo", "Combo");
	{
		combo->add_separator("q_settings", "[Q] Settings");
		{
			this->combo.use_q = combo->add_checkbox("use_q", "Use Q", true);
			this->combo.use_invisible_q = combo->add_checkbox("use_invisible_q", "Use Invisible Q (W After)", true);
			this->combo.invisible_q_mana = combo->add_slider("invisible_q_mana", " ^~ Min Q Mana %", 80, 100, 0, 100);
			this->combo.use_collision_q = combo->add_checkbox("q_collision", "Use Q Collision on Minion Logic", true);
			
			this->combo.wait_dash_for_use_q = combo->add_checkbox("wait_dash_for_use_q", "Dont use Q if enemy has dash", true);
			const auto& dash_database = combo->add_tab("dash_database", " ^~ Dash Database");
			{
				dash_database->add_separator("header", "Enemy Dash Database");
				const auto& enemies = entitylist->get_enemy_heroes();
				for (auto const& enemy : enemies)
				{
					const auto& dashs = DashDatabase::GetDashes(enemy->get_champion());
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
	
	auto const automatic = this->main->add_tab("automatic", "Automatic");
	{
		automatic->add_separator("q_settings", "[Q] Settings");
		{
			this->automatic.use_q_on_dash = automatic->add_checkbox("use_q_on_dash", "Use Q on Dash", true);
			this->automatic.use_q_on_special_skills = automatic->add_checkbox("use_q_on_special_skills", "Use Q on Special Dashs (YoneR etc)", true);
			this->automatic.use_q_on_special_items = automatic->add_checkbox("use_q_on_special_items", "Use Q on Special Items (Zhonyas etc)", true);
			this->automatic.use_q_anti_melee = automatic->add_checkbox("use_q_anti_melee", "Use Q on Anti Melee", true);
			this->automatic.use_q_anti_gapclose = automatic->add_checkbox("use_q_anti_gapclose", "Use Q on Anti Gapclose", true);
			const auto& antigapclose_whitelist = automatic->add_tab("q_gaplose", "Q Gapclose Whitelist");
			{
				antigapclose_whitelist->add_separator("x", "Enemy Whitelist");
				const auto& enemies = entitylist->get_enemy_heroes();
				for (const auto& enemy : enemies)
				{
					this->automatic.use_q_anti_gapclose_whitelist[enemy->get_network_id()] = antigapclose_whitelist->add_checkbox(enemy->get_model(), enemy->get_model(), true);
					this->automatic.use_q_anti_gapclose_whitelist[enemy->get_network_id()]->set_texture(enemy->get_square_icon_portrait());
				}
				
			}
		}

		automatic->add_separator("e_settings", "[E] Settings");
		{
			this->automatic.use_e_on_cc = automatic->add_checkbox("use_e_on_cc", "Use E on CC", true);
			this->automatic.use_e_on_special_skills = automatic->add_checkbox("use_e_on_special_skills", "Use E on Special Dashs (YoneR etc)", true);
			this->automatic.use_e_on_special_items = automatic->add_checkbox("use_e_on_special_items", "Use E on Special Items (Zhonyas etc)", true);
			this->automatic.use_e_anti_gapclose = automatic->add_checkbox("use_e_anti_gapclose", "Use E on Anti Gapclose", true);
			const auto& antigapclose_whitelist = automatic->add_tab("e_gaplose", "E Gapclose Whitelist");
			{
				antigapclose_whitelist->add_separator("x", "Enemy Whitelist");
				const auto& enemies = entitylist->get_enemy_heroes();
				for (const auto& enemy : enemies)
				{
					this->automatic.use_q_anti_gapclose_whitelist[enemy->get_network_id()] = antigapclose_whitelist->add_checkbox(enemy->get_model(), enemy->get_model(), true);
					this->automatic.use_q_anti_gapclose_whitelist[enemy->get_network_id()]->set_texture(enemy->get_square_icon_portrait());
				}
			}

			automatic->add_separator("e2_settings", "[E2] Settings");
			{
				this->automatic.use_e2 = automatic->add_checkbox("use_e2", "Use E2", true);
				this->automatic.use_e2_mode = automatic->add_combobox("use_e2_mode", "Use E2 Mode", { {"Enemy is Leaving",nullptr},{"Enemy Inside",nullptr },{"Always", nullptr} }, 0);
				this->automatic.use_e2_in_predict = automatic->add_checkbox("use_e2_in_predict", "Use E2 in A.A/Ultimate Predict", true);
				this->automatic.use_e2_if_killable = automatic->add_checkbox("use_e2_if_killable", "Use E2 if Killable", true);
			}
		}

		automatic->add_separator("r_settings", "[R] Settings");
		{
			this->automatic.use_r_if_killable = automatic->add_checkbox("use_r_if_killable", "Use R if Killable", true);
			this->automatic.use_r_on_cc = automatic->add_checkbox("use_r_on_cc", "Use R on CC", true);
			this->automatic.use_r_on_special_skills = automatic->add_checkbox("use_r_on_special_skills", "Use R on Special Dashs (YoneR etc)", true);
			this->automatic.use_r_on_special_items = automatic->add_checkbox("use_r_on_special_items", "Use R on Special Items (Zhonyas etc)", true);
		}
	}

	const auto hitchance = this->main->add_tab("hitchance", "Hitchance");
	{
		hitchance->add_separator("spells", "Spells Hitchance");
		{
			this->hitchance.q = hitchance->add_combobox("q", "Q Hitchance", { {"Low",nullptr},{"Medium",nullptr },{"High", nullptr},{"Very High",nullptr} }, 2);
			this->hitchance.e = hitchance->add_combobox("e", "E Hitchance", { {"Low",nullptr},{"Medium",nullptr },{"High", nullptr},{"Very High",nullptr} }, 2);
			this->hitchance.r = hitchance->add_combobox("r", "R Hitchance", { {"Low",nullptr},{"Medium",nullptr },{"High", nullptr},{"Very High",nullptr} }, 2);
		}
		hitchance->add_separator("fow", "FoW Hitchance");
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

			float R2_DEFAULT_COLOR[] = { 255.f, 255.f, 255.f, 255.f };
			this->renderer.spells.r_minimap = spells->add_checkbox("r2", "Should Draw R Range in Minimap", true);
			this->renderer.spells.r_minimap_color = spells->add_colorpick("r2_color", " ^~ Color", R2_DEFAULT_COLOR);
		}

		auto const damage = renderer->add_tab("damage", "Damage Indicator");
		{
			float DEFAULT_COLOR[] = { 255.f, 255.f, 255.f, 255.f };
			this->renderer.damage.enabled = damage->add_checkbox("enabled", "Should Draw Damage Indicator", true);
			this->renderer.damage.color = damage->add_colorpick("color", " ^~ Color", DEFAULT_COLOR);
		}

		auto const misc = renderer->add_tab("misc", "Misc");
		{
			this->renderer.misc.draw_killable = misc->add_checkbox("draw_killable", "Draw Killable Warning", true);
			this->renderer.misc.draw_ignoring_dash = misc->add_checkbox("draw_ignoring_dash", "Draw Ignoring Dash", true);
			this->renderer.misc.draw_e_duration = misc->add_checkbox("draw_e_duration", "Draw E Circle Duration", true);
		}
	}
	
	this->main->add_separator("hotkeys_header", "~ Hotkeys ~");
	{
		this->hotkeys.semi_q = this->main->add_hotkey("semi_q", "Semi Q", TreeHotkeyMode::Hold, 'G', true, false);
		this->hotkeys.semi_e = this->main->add_hotkey("semi_e", "Semi E", TreeHotkeyMode::Hold, 'J', true, false);
		this->hotkeys.semi_r = this->main->add_hotkey("semi_r", "Semi R", TreeHotkeyMode::Hold, 'T', true, false);
	}

	this->main->add_separator("credits", "<3 Nospher#9995 & vi#3992");

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
		300.f,
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
		this->AutomaticCastQ();
		this->AutomaticCastE();
		this->AutomaticCastUltimate();
		this->AutomaticCastOnSpecialDash();
	}

	if (orbwalker->combo_mode())
	{
		if (this->combo.use_q->get_bool())
			this->CastQ();
		
		if (this->combo.use_e->get_bool())
			this->CastE();
	}

	if (orbwalker->harass())
	{
		//if (this->harass.use_e->get_bool())
			this->CastE();
	}
}

void Lux::OnDraw()
{	
	for (const auto& enemy : entitylist->get_enemy_heroes())
	{
		if (!enemy->is_valid() || !enemy->is_visible_on_screen())
			continue;

		const auto& damage = this->GetFullComboDamage(enemy);
		if (this->renderer.misc.draw_killable->get_bool() && damage >= enemy->get_real_health(false, true))
		{
			draw_manager->add_text(
				enemy->get_position(),
				D3DCOLOR_ARGB(200, 255, 0, 0),
				18,
				"KILLABLE!"
			);
		}

		if (this->renderer.damage.enabled->get_bool())
		{
			this->DrawDamageRl(
				enemy,
				damage,
				this->renderer.damage.color->get_color()
			);
		}
	}

	const auto& position = myhero->get_position();
	if (this->renderer.spells.r_minimap->get_bool())
	{
		draw_manager->draw_circle_on_minimap(
			position,
			this->spells.r->range(),
			this->renderer.spells.r_minimap_color->get_color()
		);
	}

	if (this->renderer.misc.draw_e_duration->get_bool() && e_data.object && e_data.object->is_valid())
	{
		this->DrawSemiCircle(
			e_data.position,
			300,
			5.f,
			360,
			(360 * (1 - ((gametime->get_time() - e_data.createdAt) / 5))),
			D3DCOLOR_ARGB(145, 255, 255, 255)
		);
	}

	if (this->renderer.misc.draw_ignoring_dash->get_bool() && this->combo.ignore_whitelist_if_key_pressed->get_bool())
	{
		draw_manager->add_text(
			position,
			D3DCOLOR_ARGB(255, 255, 255, 255),
			12,
			"IGNORING DASH"
		);
	}

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
	const auto& position = myhero->get_position();
	if (this->renderer.spells.q->get_bool())
	{
		draw_manager->add_circle_with_glow(
			position,
			this->renderer.spells.q_color->get_color(),
			this->spells.q->range(),
			1,
			{ .12,.75,0,0 }
		);
	}

	if (this->renderer.spells.w->get_bool())
	{
		draw_manager->add_circle_with_glow(
			position,
			this->renderer.spells.w_color->get_color(),
			this->spells.w->range(),
			1,
			{ .12,.75,0,0 }
		);
		
	}

	if (this->renderer.spells.e->get_bool())
	{
		draw_manager->add_circle_with_glow(
			myhero->get_position(),
			this->renderer.spells.e_color->get_color(),
			this->spells.e->range(),
			1,
			{ .12,.75,0,0 }
		);
	}

	if (this->renderer.spells.r->get_bool())
	{
		draw_manager->add_circle_with_glow(
			myhero->get_position(),
			this->renderer.spells.r_color->get_color(),
			this->spells.r->range(),
			1,
			{ .12,.75,0,0 }
		);
	}
}

void Lux::OnCreateObject(game_object_script object)
{
	if (object->get_emitter_resources_hash() == buff_hash("Lux_E_tar_aoe_green"))
	{
		this->e_data.object = object;
		this->e_data.createdAt = gametime->get_time();
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
	const auto& id = object->get_network_id();
	if (this->e_data.object && this->e_data.object->get_network_id() == id)
	{
		this->e_data.object = nullptr;
		this->e_data.circle = geometry::circle(vector(0, 0, 0), 0).to_polygon();
		this->e_data.position = vector(0, 0, 0);
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

void Lux::OnProcessSpellCast(game_object_script sender, spell_instance_script spell)
{
	if (!sender->is_me())
		return;

	const auto& slot = spell->get_spellslot();
	if (slot == spellslot::q && orbwalker->combo_mode())
	{
		if (this->combo.use_invisible_q->get_bool() && this->spells.w->is_ready())
		{
			const auto& position = spell->get_end_position();
			scheduler->delay_action(0.1f + ping->get_ping() / 1000 + (1.f / 30.f), [this, position]() {
				if (myhero->get_mana_percent() >= this->combo.invisible_q_mana->get_int())
				{
					this->spells.w->cast(position);
				}
			});
		}
		return;
	}
	
	if (slot == spellslot::e)
	{
		this->e_data.position = spell->get_end_position();
		this->e_data.circle = geometry::circle(spell->get_end_position(), 300).to_polygon();
	}
}

void Lux::OnGapcloser(game_object_script sender, antigapcloser::antigapcloser_args* args)
{
	if (!sender || !sender ->is_valid() || !args)
		return;

	const auto& E_RANGE = this->spells.e->range() + this->spells.e->get_radius();
	if (this->automatic.use_e_anti_gapclose->get_bool() && args->end_position.distance(myhero->get_position()) <= E_RANGE)
	{
		const auto& senderEnabled = this->automatic.use_e_anti_gapclose_whitelist.find(sender->get_network_id());
		if (senderEnabled != this->automatic.use_e_anti_gapclose_whitelist.end() && senderEnabled->second->get_bool())
		{
			this->spells.e->cast(sender);
		}
	}

	if (this->automatic.use_q_anti_gapclose->get_bool() && !args->is_unstoppable)
	{
		const auto& senderEnabled = this->automatic.use_q_anti_gapclose_whitelist.find(sender->get_network_id());
		if (senderEnabled != this->automatic.use_q_anti_gapclose_whitelist.end() && senderEnabled->second->get_bool())
		{
			const auto& willReachAt = args->start_time + args->end_position.distance(myhero->get_position()) / args->speed;
			const auto& travelTime = gametime->get_time() +
				myhero->get_position().distance(args->end_position) / this->spells.q->get_speed()
				+ this->spells.q->get_delay();

			const auto& diff = travelTime - willReachAt;
			if (diff > 0.25f)
			{
				if (this->spells.q->cast(args->end_position))
				{
					myhero->print_chat(0x1, "Q on Gapcloser");
				}
			}
		}
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
	if (!this->combo.wait_dash_for_use_q->get_bool())
		return false;
	
	if (this->combo.ignore_whitelist_if_key_pressed->get_bool())
		return false;
	
	auto it = this->combo.q_dash_whitelist.find(target->get_champion());
	if (it == this->combo.q_dash_whitelist.end())
		return false;

	for (const auto& [dash, menu] : it->second)
	{
		if (!menu->get_bool())
			continue;

		if (!dash.canCast(target))
			continue;
		
		const auto& spell = target->get_spell(dash.slot);
		const auto& isReady = spell->is_learned() 
			&& target->get_mana_for_spell(dash.slot) <= target->get_mana()
			&& spell->cooldown() <= 0.f;
		
		if (isReady)
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

void Lux::AutomaticCastQ()
{
	if (!this->spells.q->is_ready())
		return;
	
	const auto& enemies = this->GetTargets(this->spells.q->range() + 200.f);
	if (enemies.size() <= 0)
		return;

	for (const auto& enemy : enemies)
	{
		prediction_output output = this->spells.q->get_prediction(enemy);
		if (output.hitchance < hit_chance::high)
			continue;

		if (!output.get_cast_position().is_valid())
			continue;
		
		if (this->automatic.use_q_anti_melee->get_bool() && enemy->is_melee() && enemy->is_in_auto_attack_range(myhero))
		{
			this->spells.q->cast(output.get_cast_position());
			return;
		}
		
		if (this->automatic.use_q_on_dash->get_bool() && output.hitchance >= hit_chance::dashing)
		{
			this->spells.q->cast(output.get_cast_position());
			return;
		}
	}
}

void Lux::AutomaticCastE()
{
	if (!this->spells.e->is_ready() || this->GetSingularityState() == 1 || (this->e_data.object && this->e_data.object->is_valid()))
		return;

	const auto& enemies = this->GetTargets(this->spells.e->range() + 200.f);
	if (enemies.size() <= 0)
		return;

	for (const auto& enemy : enemies)
	{
		prediction_output output = this->spells.e->get_prediction(enemy);
		if (output.hitchance < hit_chance::high)
			continue;
		
		if (!output.get_cast_position().is_valid())
			continue;

		if (this->automatic.use_e_on_cc->get_bool() && output.hitchance >= hit_chance::immobile)
		{
			this->spells.e->cast(output.get_cast_position());
			return;
		}
	}
}

void Lux::AutomaticCastE2()
{
	if (!this->spells.e->is_ready() || this->GetSingularityState() == 0)
		return;
	
	if (this->automatic.use_e2_mode->get_int() == 2 && this->spells.e->cast())
		return;

	const auto& enemies = this->GetTargets(FLT_MAX, [&](game_object_script target) {
		return !target_selector->is_invulnerable(target) && target->has_buff(buff_hash(("LuxESlow")));
	});

	if (enemies.size() <= 0)
		return;

	for (const auto& enemy : enemies)
	{
		if (this->automatic.use_e2_if_killable->get_bool())
		{
			// Auto Cast if enemy will die; Refactor to use my own damage calculation
			if (this->GetEDamage(enemy) >= enemy->get_real_health(false, true) && this->spells.e->cast())
				return;
		}
		
		switch (this->automatic.use_e2_mode->get_int())
		{
		case 0:
		{
			// Auto Cast if enemy is leaving
			prediction_output output = prediction->get_prediction(enemy, ping->get_ping() / 1000 + 0.25f);
			if (!output.get_unit_position().is_valid())
				return;
			
			if (this->e_data.circle.is_outside(output.get_unit_position()) && this->spells.e->cast())
			{
				console->print("castou");
				return;
			}
			break;
		}
		case 1:
		{
			if (this->spells.e->cast())
				return;
			break;
		}
		}
	}
}

void Lux::AutomaticCastE2InPredict()
{
	if (!this->automatic.use_e2_in_predict->get_bool() || !this->spells.e->is_ready())
		return;
	
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

	const auto& E_RANGE = this->spells.e->range() + this->spells.e->get_radius();
	const auto& enemies = this->GetTargets(this->spells.r->range(), [&](game_object_script target) {
		// OVERKILL CHECK IF KILLABLE WITH A.A
		if (myhero->is_in_auto_attack_range(target))
		{
			const auto& EXTRA_DAMAGE = this->HasPassive(target) ? this->GetPassiveDamage(target) : 0;
			const auto& TOTAL_DAMAGE = myhero->get_auto_attack_damage(target) + EXTRA_DAMAGE;
			if (TOTAL_DAMAGE >= target->get_real_health(false, true))
				return false;
		}
		 
		// OVERKILL cHECK IF KILLABLE WITH E
		const auto& IS_KILLABLE = this->GetEDamage(target) >= target->get_real_health(false, true);;
		if (this->spells.e->is_ready() && this->GetSingularityState() == 0 && target->get_distance(myhero) <= E_RANGE)
		{
			return !IS_KILLABLE;
		}

		// OVERKILL CHECK IF WILL KILLED WITH E
		if (this->e_data.circle.is_inside(target->get_position()))
		{
			return !IS_KILLABLE;
		}

		return true;
	});
	
	if (enemies.size() <= 0)
		return;

	for (const auto& enemy : enemies)
	{
		auto prediction = this->spells.r->get_prediction(enemy);
		if (prediction.hitchance == hit_chance::immobile)
		{
			const std::set<buff_type>& immobileBuffs = this->GetImmobileBuffTypes();
			for (const auto& buff : enemy->get_bufflist())
			{
				if (!buff || !buff->is_valid() || !buff->is_alive())
					continue;
				
				if (immobileBuffs.find(buff->get_type()) == immobileBuffs.end())
					continue;

				if (buff->get_remaining_time() >= 0.8)
				{
					this->spells.r->cast(prediction.get_cast_position());
					return;
				}
			}
		}
	}
}

void Lux::AutomaticCastOnSpecialDash()
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

	if (this->automatic.use_e_on_special_skills->get_bool())
	{
		bool CanCastEBeforeQ = this->automatic.use_q_on_special_skills->get_bool() && this->spells.q->is_ready() ?
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
	}
	
	if (this->automatic.use_q_on_special_skills->get_bool() && this->spells.q->is_ready())
	{
		const auto& qPrediction = PredictionHelper::GetInstance()->GetCastPositionOnSpecialDash(this->spells.q, dash, 2);
		if (qPrediction.first)
		{
			this->spells.q->cast(qPrediction.second);
		}
	}

	if (this->automatic.use_r_on_special_skills->get_bool() && !this->spells.q->is_ready() && !this->spells.e->is_ready() && this->spells.r->is_ready())
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

float Lux::GetPassiveDamage(game_object_script target)
{
	const auto& damage = 10 + (10 * myhero->get_level()) + (myhero->get_total_ability_power() * 0.20);
	return damagelib->calculate_damage_on_unit(myhero, target, damage_type::magical, damage);
}

float Lux::GetQDamage(game_object_script target)
{
	const auto& level = myhero->get_spell(spellslot::q)->level();
	if (level == 0)
		return 0;
	
	const auto& damage = 40 + (40 * level) + (myhero->get_total_ability_power() * 0.6);
	return damagelib->calculate_damage_on_unit(myhero, target, damage_type::magical, damage);
}

float Lux::GetEDamage(game_object_script target)
{
	const auto& level = myhero->get_spell(spellslot::e)->level();
	if (level == 0)
		return 0;

	const auto& damage = 20 + (50 * level) + (myhero->get_total_ability_power() * 0.8);
	return damagelib->calculate_damage_on_unit(myhero, target, damage_type::magical, damage);
}

float Lux::GetRDamage(game_object_script target)
{
	const auto& level = myhero->get_spell(spellslot::r)->level();
	if (level == 0)
		return 0;

	const auto& damage = 200 + (100 * level) + (myhero->get_total_ability_power() * 1.2);
	return damagelib->calculate_damage_on_unit(myhero, target, damage_type::magical, damage);
}

float Lux::GetFullComboDamage(game_object_script target)
{
	float actualMana = myhero->get_mana();
	float totalDamage = 0;
	bool hasPassive = this->HasPassive(target);
	
	const auto& qManaCost = myhero->get_mana_for_spell(spellslot::q);
	if (this->spells.q->is_ready() && actualMana >= qManaCost) {
		totalDamage += this->GetQDamage(target);
		actualMana -= qManaCost;
		totalDamage += hasPassive ? GetPassiveDamage(target) : 0;
		hasPassive = !hasPassive;
	}

	const auto& eManaCost = myhero->get_mana_for_spell(spellslot::e);
	if (this->spells.e->is_ready() && actualMana >= eManaCost) {
		totalDamage += this->GetEDamage(target);
		actualMana -= eManaCost;
		totalDamage += hasPassive ? GetPassiveDamage(target) : 0;
		hasPassive = !hasPassive;
	}

	const auto& rManaCost = myhero->get_mana_for_spell(spellslot::r);
	if (this->spells.r->is_ready() && actualMana >= rManaCost) {
		totalDamage += this->GetRDamage(target);
		totalDamage += hasPassive ? GetPassiveDamage(target) : 0;
		hasPassive = !hasPassive;
	}

	if (myhero->is_in_auto_attack_range(target))
	{
		totalDamage += myhero->get_auto_attack_damage(target);
		totalDamage += hasPassive ? GetPassiveDamage(target) : 0;
	}

	return totalDamage;
}