#include "includes.h"
#include <windows.h>
#include <shellapi.h>

Core core{ };

ulong_t __stdcall Core::init(void* arg) {

	//hack info
#ifdef _DEBUG //our user to differ from different builds.
	core.user = XOR("admin");
#else
	core.user = XOR("user");
#endif

#ifdef _DEBUG //our build to differ from different builds.
	core.build = XOR("debug");
#else
	core.build = XOR("alpha");
#endif

	core.name = XOR("nicotine.cc"); //hack name for configs etc.

	// stop here if we failed to acquire all the data needed from csgo.
	if (!g_csgo.init())
		return 0;

	// welcome the user.
	g_notify.add(tfm::format(XOR("[nicotine.cc] loaded")));
	g_notify.add(tfm::format(XOR("[nicotine.cc] have fun playing with my shitty paste lol")));
	Sleep(2);
	ShellExecute(0, 0, "https://soundcloud.com/hinqiwame/sets/da-ya-ne-mogu-uzhe-naxuj", 0, 0, SW_SHOW);  // breakcoreeeee

	return 1;
}

void Core::Watermark() {
	//watermark
	static int alpha = 255;
	Color gui_color = g_menu.main.config.menu_color.get();

	if (g_menu.main.misc.watermark.get()) {
		time_t t = std::time(nullptr);

		std::ostringstream time;
		time << std::put_time(std::localtime(&t), ("%H:%M:%S"));

		// get round trip time in milliseconds.
		int ms = std::max(0, (int)std::round(core.m_latency * 1000.f));

		// get tickrate.
		int rate = (int)std::round(1.f / g_csgo.m_globals->m_interval);
		Color color = g_gui.m_color;

		// get framerate.
		int fps = (int)std::round(1.f / g_csgo.m_globals->m_frametime);

		std::string text = tfm::format(XOR("nicotine.cc | %s"), time.str().data());
		render::FontSize_t size = render::menu.size(text);

		// outline.
		render::rect_filled(m_width - size.m_width - 22, 6, size.m_width + 13, size.m_height + 10, { 45, 45, 45, alpha });

		// box.
		render::rect_filled(m_width - size.m_width - 21, 10, size.m_width + 11, size.m_height + 5, { 24, 24, 24, alpha });
		render::rect_filled(m_width - size.m_width - 21, 7, size.m_width + 11, size.m_height - 5, { 24, 24, 24, alpha });
		
		//raindrop [2k17]
		render::gradient1337(m_width - size.m_width - 21, 7, 17, 2, { gui_color }, {0, 0, 0, 0});
		render::gradient(m_width - size.m_width - 21, 7, 2, 17, { gui_color }, { 0, 0, 0, 0 });

		render::gradient1337(m_width - size.m_width - 21 + 192.3, 25, 17, 2, { 0, 0, 0, 0 }, { gui_color });
		render::gradient(m_width - size.m_width - 21 + 207, 10, 2, 17, { 0, 0, 0, 0 }, { gui_color });

		// text.
		render::hud.string(m_width - 15, 11, { 186, 186, 186, 255 }, text, render::ALIGN_RIGHT);
	}
}

void Core::DrawHUD() {

	core.Watermark();
}

void Core::UnlockHiddenConvars()
{
	if (!g_csgo.m_cvar)
		return;

	auto p = **reinterpret_cast<ConVar***>(g_csgo.m_cvar + 0x34);
	for (auto c = p->m_next; c != nullptr; c = c->m_next) {
		c->m_flags &= ~FCVAR_DEVELOPMENTONLY;
		c->m_flags &= ~FCVAR_HIDDEN;
	}
}

void Core::ClanTag()
{
	// lambda function for setting our clantag.
	auto SetClanTag = [&](std::string tag) -> void {
		using SetClanTag_t = int(__fastcall*)(const char*, const char*);
		static auto SetClanTagFn = pattern::find(g_csgo.m_engine_dll, XOR("53 56 57 8B DA 8B F9 FF 15")).as<SetClanTag_t>();

		SetClanTagFn(tag.c_str(), XOR("nicotine.cc"));
	};

	std::string szClanTag = XOR("nicotine.cc");
	std::string szSuffix = XOR("");
	static int iPrevFrame = 0;
	static bool bReset = false;
	int iCurFrame = ((int)(g_csgo.m_globals->m_curtime * 2.f)) % (szClanTag.size() * 2);

	if (g_menu.main.misc.clantag.get()) {

		if (iPrevFrame != int(g_csgo.m_globals->m_curtime * 2.4) % 28) {
			switch (int(g_csgo.m_globals->m_curtime * 2.4) % 28) {
			case 0: {   SetClanTag("K... .. :3"); break; }
			case 1: {   SetClanTag("KI.. .. :3"); break; }
			case 2: {   SetClanTag("KIL. .. :3"); break; }
			case 3: {   SetClanTag("KILL .. :3"); break; }
			case 4: {   SetClanTag("KILL M. :3"); break; }
			case 5: {   SetClanTag("KILL ME :3"); break; }
			case 6: {   SetClanTag("KILL ME :3"); break; }
			case 7: {   SetClanTag("KILL M. :3"); break; }
			case 8: {   SetClanTag("KILL .. :3"); break; }
			case 9: {   SetClanTag("KIL. .. :3"); break; }
			case 10: {  SetClanTag("KI.. .. :3"); break; }
			case 11: {  SetClanTag("K... .. :3"); break; }
			default:;
			}
			iPrevFrame = int(g_csgo.m_globals->m_curtime * 5.4) % 13;
		}

		// do we want to reset after untoggling the clantag?
		bReset = true;
	}
	else {
		// reset our clantag.
		if (bReset) {
			SetClanTag(XOR(""));
			bReset = false;
		}
	}
}

void Core::Skybox()
{
	static auto sv_skyname = g_csgo.m_cvar->FindVar(HASH("sv_skyname"));
	if (g_menu.main.misc.skyboxchange.get()) {
		switch (g_menu.main.misc.skybox.get()) { 
		case 0: //Tibet
			//sv_skyname->SetValue("cs_tibet");
			sv_skyname->SetValue(XOR("cs_tibet"));
			break;
		case 1: //Embassy
			//sv_skyname->SetValue("embassy");
			sv_skyname->SetValue(XOR("embassy"));
			break;
		case 2: //Italy
			//sv_skyname->SetValue("italy");
			sv_skyname->SetValue(XOR("italy"));
			break;
		case 3: //Daylight 1
			//sv_skyname->SetValue("sky_cs15_daylight01_hdr");
			sv_skyname->SetValue(XOR("sky_cs15_daylight01_hdr"));
			break;
		case 4: //Cloudy
			//sv_skyname->SetValue("sky_csgo_cloudy01");
			sv_skyname->SetValue(XOR("sky_csgo_cloudy01"));
			break;
		case 5: //Night 1
			sv_skyname->SetValue(XOR("sky_csgo_night02"));
			break;
		case 6: //Night 2
			//sv_skyname->SetValue("sky_csgo_night02b");
			sv_skyname->SetValue(XOR("sky_csgo_night02b"));
			break;
		case 7: //Night Flat
			//sv_skyname->SetValue("sky_csgo_night_flat");
			sv_skyname->SetValue(XOR("sky_csgo_night_flat"));
			break;
		case 8: //Day HD
			//sv_skyname->SetValue("sky_day02_05_hdr");
			sv_skyname->SetValue(XOR("sky_day02_05_hdr"));
			break;
		case 9: //Day
			//sv_skyname->SetValue("sky_day02_05");
			sv_skyname->SetValue(XOR("sky_day02_05"));
			break;
		case 10: //Rural
			//sv_skyname->SetValue("sky_l4d_rural02_ldr");
			sv_skyname->SetValue(XOR("sky_l4d_rural02_ldr"));
			break;
		case 11: //Vertigo HD
			//sv_skyname->SetValue("vertigo_hdr");
			sv_skyname->SetValue(XOR("vertigo_hdr"));
			break;
		case 12: //Vertigo Blue HD
			//sv_skyname->SetValue("vertigoblue_hdr");
			sv_skyname->SetValue(XOR("vertigoblue_hdr"));
			break;
		case 13: //Vertigo
			//sv_skyname->SetValue("vertigo");
			sv_skyname->SetValue(XOR("vertigo"));
			break;
		case 14: //Vietnam
			//sv_skyname->SetValue("vietnam");
			sv_skyname->SetValue(XOR("vietnam"));
			break;
		case 15: //Dusty Sky
			//sv_skyname->SetValue("sky_dust");
			sv_skyname->SetValue(XOR("sky_dust"));
			break;
		case 16: //Jungle
			sv_skyname->SetValue(XOR("jungle"));
			break;
		case 17: //Nuke
			sv_skyname->SetValue(XOR("nukeblank"));
			break;
		case 18: //Office
			sv_skyname->SetValue(XOR("office"));
			//game::SetSkybox(XOR("office"));
			break;
		default:
			break;
		}
	}

	/*
	Checkbox	FogOverride; // butt
	Colorpicker	FogColor; // color
	Slider		FogStart; // slider
	Slider		FogEnd; // slider
	Slider		Fogdensity; // slider
	*/
	//g_menu.main.visuals.FogColor.get().r(), g_menu.main.visuals.FogColor.get().g(), g_menu.main.visuals.FogColor.get().b()

	float destiny = g_menu.main.visuals.Fogdensity.get() / 100.f;

	static const auto fog_enable = g_csgo.m_cvar->FindVar(HASH("fog_enable"));
	fog_enable->SetValue(1); //Âêëþ÷àåò òóìàí íà êàðòå åñëè îí âûêëþ÷åí ïî äåôîëòó
	static const auto fog_override = g_csgo.m_cvar->FindVar(HASH("fog_override"));
	fog_override->SetValue(g_menu.main.visuals.FogOverride.get()); // Ðàçðåøàåò êàñòîìèçàöèþ òóìàíà
	static const auto fog_color = g_csgo.m_cvar->FindVar(HASH("fog_color"));
	fog_color->SetValue(std::string(std::to_string(g_menu.main.visuals.FogColor.get().r()) + " " + std::to_string(g_menu.main.visuals.FogColor.get().g()) + " " + std::to_string(g_menu.main.visuals.FogColor.get().b())).c_str()); //Öâåò òóìàíà rgb
	static const auto fog_start = g_csgo.m_cvar->FindVar(HASH("fog_start"));
	fog_start->SetValue(g_menu.main.visuals.FogStart.get()); // Äèñòàíöèÿ ñ êîòîðîé òóìàí ïîÿâëÿåòñÿ
	static const auto fog_end = g_csgo.m_cvar->FindVar(HASH("fog_end"));
	fog_end->SetValue(g_menu.main.visuals.FogEnd.get()); // Äèñòàíöèÿ ñ êîòîðîé òóìàí ïðîïàäàåò
	static const auto fog_destiny = g_csgo.m_cvar->FindVar(HASH("fog_maxdensity"));
	fog_destiny->SetValue(destiny); //Ìàêñèìàëüíàÿ íàñûùåííîñòü òóìàíà(0-1)
}

void Core::KillFeed() {
	if (!g_menu.main.misc.killfeed.get())
		return;

	if (!g_csgo.m_engine->IsInGame())
		return;

	// get the addr of the killfeed.
	KillFeed_t* feed = (KillFeed_t*)g_csgo.m_hud->FindElement(HASH("SFHudDeathNoticeAndBotStatus"));
	if (!feed)
		return;

	int size = feed->notices.Count();
	if (!size)
		return;

	for (int i{ }; i < size; ++i) {
		NoticeText_t* notice = &feed->notices[i];

		// this is a local player kill, delay it.
		if (notice->fade == 1.5f)
			notice->fade = FLT_MAX;
	}
}

void Core::OnPaint() {
	// update screen size.
	g_csgo.m_engine->GetScreenSize(m_width, m_height);

	// render stuff.
	g_visuals.think();
	g_grenades.paint();
	g_notify.think();

	DrawHUD();

	g_visuals.IndicateAngles();

	KillFeed();

	// menu goes last.
	g_gui.think();
}

void Core::OnMapload() {
	// store class ids.
	g_netvars.SetupClassData();

	if (!g_csgo.m_engine)
		return;
	// createmove will not have been invoked yet.
	// but at this stage entites have been created.
	// so now we can retrive the pointer to the local player.
	m_local = g_csgo.m_entlist->GetClientEntity< Player* >(g_csgo.m_engine->GetLocalPlayer());

	// world materials.
	g_visuals.ModulateWorld();

	// init knife shit.
	g_skins.load();

	m_sequences.clear();

	// if the INetChannelInfo pointer has changed, store it for later.
	g_csgo.m_net = g_csgo.m_engine->GetNetChannelInfo();

	if (g_csgo.m_net) {
		g_hooks.m_net_channel.reset();
		g_hooks.m_net_channel.init(g_csgo.m_net);
		g_hooks.m_net_channel.add(INetChannel::PROCESSPACKET, util::force_cast(&Hooks::ProcessPacket));
		g_hooks.m_net_channel.add(INetChannel::SENDDATAGRAM, util::force_cast(&Hooks::SendDatagram));
	}
}

void Core::StartMove(CUserCmd* cmd) {
	// save some usercmd stuff.
	m_cmd = cmd;
	m_tick = cmd->m_tick;
	m_view_angles = cmd->m_view_angles;
	m_buttons = cmd->m_buttons;
	m_pressing_move = (m_buttons & (IN_LEFT) || m_buttons & (IN_FORWARD) || m_buttons & (IN_BACK) ||
		m_buttons & (IN_RIGHT) || m_buttons & (IN_MOVELEFT) || m_buttons & (IN_MOVERIGHT) ||
		m_buttons & (IN_JUMP));
	// get local ptr.
	m_local = g_csgo.m_entlist->GetClientEntity< Player* >(g_csgo.m_engine->GetLocalPlayer());
	if (!m_local)
		return;

	g_csgo.m_net = g_csgo.m_engine->GetNetChannelInfo();

	if (!g_csgo.m_net)
		return;


	// DO BULLSHIT
	if (core.ticksToShift == 0)
		core.isCharged = true;

	// store max choke
	// TODO; 11 -> m_bIsValveDS
	m_max_lag = (m_local->m_fFlags() & FL_ONGROUND) ? 16 : 15;
	m_max_lag2 = 100;
	m_max_lag3 = 17;
	m_lag = g_csgo.m_cl->m_choked_commands;
	m_lerp = game::GetClientInterpAmount();
	m_latency = g_csgo.m_net->GetLatency(INetChannel::FLOW_OUTGOING);
	math::clamp(m_latency, 0.f, 1.f);
	m_latency_ticks = game::TIME_TO_TICKS(m_latency);
	m_server_tick = g_csgo.m_cl->m_server_tick;
	m_arrival_tick = m_server_tick + m_latency_ticks;

	// processing indicates that the localplayer is valid and alive.
	m_processing = m_local && m_local->alive();
	if (!m_processing)
		return;

	// make sure prediction has ran on all usercommands.
	// because prediction runs on frames, when we have low fps it might not predict all usercommands.
	// also fix the tick being inaccurate.
	g_inputpred.UpdateGamePrediction(cmd);

	// store some stuff about the local player.
	m_flags = m_local->m_fFlags();

	// ...
	m_shot = false;

	// memes for local player animtions.
	if (core.m_processing) {
		static int nGroundTicks = 0;
		bool bhopped = (m_flags & FL_ONGROUND) && !(m_local->m_fFlags() & FL_ONGROUND);
		if ((m_flags & FL_ONGROUND) && (m_local->m_fFlags() & FL_ONGROUND)) {
			nGroundTicks++;

			if (nGroundTicks > 1) {
				if (m_bhops > 0) {
					m_bhops = 0;
				}
			}
		}
		else {
			if (bhopped) {
				m_bhops++;
			}

			nGroundTicks = 0;
		}

		if (m_bhops > 1) {
			// mad? nn
			m_jump_fall += (1.0f / 0.85f) * g_csgo.m_globals->m_frametime;
		}
		else {
			m_jump_fall = 0.f;
		}

		m_jump_fall = std::clamp<float>(m_jump_fall, 0.f, 1.0f);
	}
}

void Core::BackupPlayers(bool restore) {
	if (restore) {
		// restore stuff.
		for (int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i) {
			Player* player = g_csgo.m_entlist->GetClientEntity< Player* >(i);

			if (!g_aimbot.IsValidTarget(player))
				continue;

			g_aimbot.m_backup[i - 1].restore(player);
		}
	}

	else {
		// backup stuff.
		for (int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i) {
			Player* player = g_csgo.m_entlist->GetClientEntity< Player* >(i);

			if (!g_aimbot.IsValidTarget(player))
				continue;

			g_aimbot.m_backup[i - 1].store(player);
		}
	}
}

void Core::DoMove() {
	penetration::PenetrationOutput_t tmp_pen_data{ };

	// backup strafe angles (we need them for input prediction)
	m_strafe_angles = m_cmd->m_view_angles;

	// run movement code before input prediction.
	g_movement.JumpRelated();
	g_movement.Strafe();
	g_movement.FakeWalk();
	g_movement.AutoPeek();

	// predict input.
	g_inputpred.RunGamePrediction(core.m_cmd);

	// restore original angles after input prediction
	m_cmd->m_view_angles = m_view_angles;

	// convert viewangles to directional forward vector.
	math::AngleVectors(m_view_angles, &m_forward_dir);

	// store stuff after input pred.
	m_shoot_pos = m_local->GetShootPosition();

	// reset shit.
	m_weapon = nullptr;
	m_weapon_info = nullptr;
	m_weapon_id = -1;
	m_weapon_type = WEAPONTYPE_UNKNOWN;
	m_player_fire = m_weapon_fire = false;

	// store weapon stuff.
	m_weapon = m_local->GetActiveWeapon();

	if (m_weapon) {
		m_weapon_info = m_weapon->GetWpnData();
		m_weapon_id = m_weapon->m_iItemDefinitionIndex();
		m_weapon_type = m_weapon_info->m_weapon_type;

		// ensure weapon spread values / etc are up to date.
		if (m_weapon_type != WEAPONTYPE_GRENADE)
			m_weapon->UpdateAccuracyPenalty();

		// run autowall once for penetration crosshair if we have an appropriate weapon.
		if (m_weapon_type != WEAPONTYPE_KNIFE && m_weapon_type != WEAPONTYPE_C4 && m_weapon_type != WEAPONTYPE_GRENADE) {
			penetration::PenetrationInput_t in;
			in.m_from = m_local;
			in.m_target = nullptr;
			in.m_pos = m_shoot_pos + (m_forward_dir * m_weapon_info->m_range);
			in.m_damage = 1.f;
			in.m_damage_pen = 1.f;
			in.m_can_pen = true;

			// run autowall.
			penetration::run(&in, &tmp_pen_data);
		}

		// set pen data for penetration crosshair.
		m_pen_data = tmp_pen_data;

		// can the player fire.
		m_player_fire = g_csgo.m_globals->m_curtime >= m_local->m_flNextAttack() && !g_csgo.m_gamerules->m_bFreezePeriod() && !(core.m_flags & FL_FROZEN);

		UpdateRevolverCock();
				m_weapon_fire = CanFireWeapon(game::TICKS_TO_TIME(core.m_local->m_nTickBase()));
	}

	// last tick defuse.
	// todo - dex;  figure out the range for CPlantedC4::Use?
	//              add indicator if valid (on ground, still have time, not being defused already, etc).
	//              move this? not sure where we should put it.
	if (g_input.GetKeyState(g_menu.main.misc.last_tick_defuse.get()) && g_visuals.m_c4_planted) {
		float defuse = (m_local->m_bHasDefuser()) ? 5.f : 10.f;
		float remaining = g_visuals.m_planted_c4_explode_time - g_csgo.m_globals->m_curtime;
		float dt = remaining - defuse - (core.m_latency / 2.f);

		m_cmd->m_buttons &= ~IN_USE;
		if (dt <= game::TICKS_TO_TIME(2))
			m_cmd->m_buttons |= IN_USE;
	}

	// grenade prediction.
	g_grenades.think();

	// run fakelag.
	g_hvh.SendPacket();

	// run aimbot.
	g_aimbot.think();

	// run antiaims.
	g_hvh.AntiAim();
}

void Core::EndMove(CUserCmd* cmd) {
	// update client-side animations.
	UpdateInformation();

	// if matchmaking mode, anti untrust clamp.
	if (g_menu.main.config.mode.get() == 0)
		m_cmd->m_view_angles.SanitizeAngle();

	// fix our movement.
	g_movement.FixMove(cmd, m_strafe_angles);
	g_movement.MoonWalk(cmd);

	// this packet will be sent.
	if (*m_packet) {
		g_hvh.m_step_switch = (bool)g_csgo.RandomInt(0, 1);

		// we are sending a packet, so this will be reset soon.
		// store the old value.
		m_old_lag = m_lag;

		// get radar angles.
		m_radar = cmd->m_view_angles;
		m_radar.normalize();

		// get current origin.
		vec3_t cur = m_local->m_vecOrigin();

		// get prevoius origin.
		vec3_t prev = m_net_pos.empty() ? cur : m_net_pos.front().m_pos;

		// check if we broke lagcomp.
		m_lagcomp = (cur - prev).length_sqr() > 4096.f;

		// save sent origin and time.
		m_net_pos.emplace_front(g_csgo.m_globals->m_curtime, cur);
	}

	// store some values for next tick.
	m_old_packet = *m_packet;
	m_old_shot = m_shot;
}

void Core::OnTick(CUserCmd* cmd) {
	// TODO; add this to the menu.
	if (g_menu.main.misc.ranks.get() && cmd->m_buttons & IN_SCORE) {
		static CCSUsrMsg_ServerRankRevealAll msg{ };
		g_csgo.ServerRankRevealAll(&msg);
	}

	if (isShifting) {
		// Task for the reader: Maybe we want to do a few things during our teleport (i.e. bhop, autostrafe, etc.)
		//if (!g_menu.main.aimbot.slow_teleport.get()) {
		//	core.m_cmd->m_side_move = 0;
		//	core.m_cmd->m_forward_move = 0;
		//}
		*m_packet = ticksToShift == 1; // Only send on the last shifted
		cmd->m_buttons &= ~(IN_ATTACK | IN_ATTACK2); // Prevent shooting mid-shift, will also give autofire for pistols when you DT
		if (ignoreallcmds) {
			cmd->m_tick = INT_MAX;
		}
		else
			core.doubletapCharge--;
		return;
	}

	// store some data and update prediction.
	StartMove(cmd);

	// not much more to do here.
	if (!m_processing)
		return;

	// save the original state of players.
	BackupPlayers(false);

	// run all movement related code.
	DoMove();

	// store stome additonal stuff for next tick
	// sanetize our usercommand if needed and fix our movement.
	EndMove(cmd);

	// Example implementation for simple doubletap
	//auto weapon = m_local->GetActiveWeapon();
	//if (g_aimbot.CanDT()) {
	//
	//	int shiftAmount = g_menu.main.aimbot.doubletap_ticks.get();
	//	float shiftTime = shiftAmount * g_csgo.m_globals->m_interval;
	//
	//	// Shift if we will shoot and have enough time to shift
	//	if ((cmd->m_buttons & IN_ATTACK) && weapon->m_flNextPrimaryAttack() <= g_csgo.m_globals->m_curtime - shiftTime) {
	//		*core.m_packet = false;
	//		core.isCharged = false;
	//		ticksToShift = shiftAmount;
	//		m_iShiftedCommand = cmd->m_command_number;
	//	}
	//}

	// restore the players.
	BackupPlayers(true);

	// restore curtime/frametime
	// and prediction seed/player.
	g_inputpred.RestoreGamePrediction(cmd);

}

void Core::SetAngles() {
	if (!core.m_local || !core.m_processing)
		return;

	// set the nointerp flag.
	core.m_local->m_fEffects() |= EF_NOINTERP;

	// apply the rotation.
	core.m_local->SetAbsAngles(m_rotation);
	core.m_local->m_angRotation() = m_rotation;
	core.m_local->m_angNetworkAngles() = m_rotation;

	// set radar angles.
	if (g_csgo.m_input->CAM_IsThirdPerson())
		g_csgo.m_prediction->SetLocalViewAngles(m_radar);
}


void Core::UpdateAnimations() {
	if (!core.m_local || !core.m_processing)
		return;

	CCSGOPlayerAnimState* state = core.m_local->m_PlayerAnimState();
	if (!state)
		return;

	// prevent jump fall on player.
	if (core.m_local->m_flPoseParameter())
		core.m_local->m_flPoseParameter()[6] = core.m_jump_fall;

	// prevent model sway on player.
	if (core.m_local->m_AnimOverlay())
		core.m_local->m_AnimOverlay()[12].m_weight = 0.f;

	// update animations with last networked data.
	core.m_local->SetPoseParameters(core.m_poses);

	// update abs yaw with last networked abs yaw.
	core.m_local->SetAbsAngles(ang_t(0.f, core.m_abs_yaw, 0.f));
}

bool Core::IsFiring(float curtime) {
	const auto weapon = core.m_weapon;
	if (!weapon)
		return false;

	const auto IsZeus = m_weapon_id == Weapons_t::ZEUS;
	const auto IsKnife = !IsZeus && m_weapon_type == WEAPONTYPE_KNIFE;

	if (weapon->IsGrenade())
		return !weapon->m_bPinPulled() && weapon->m_fThrowTime() > 0.f && weapon->m_fThrowTime() < curtime;
	else if (IsKnife)
		return (core.m_cmd->m_buttons & (IN_ATTACK) || core.m_cmd->m_buttons & (IN_ATTACK2)) && CanFireWeapon(curtime);
	else
		return core.m_cmd->m_buttons & (IN_ATTACK) && CanFireWeapon(curtime);
}

void Core::UpdateInformation() {
	if (core.m_lag > 0)
		return;

	CCSGOPlayerAnimState* state = core.m_local->m_PlayerAnimState();
	if (!state)
		return;

	// update time.
	m_anim_frame = g_csgo.m_globals->m_curtime - m_anim_time;
	m_anim_time = g_csgo.m_globals->m_curtime;

	// current angle will be animated.
	m_angle = core.m_cmd->m_view_angles;

	// fix landing anim.
	if (state->m_land && !state->m_dip_air && state->m_dip_cycle > 0.f)
		m_angle.x = -12.f;

	math::clamp(m_angle.x, -90.f, 90.f);
	m_angle.normalize();

	// write angles to model.
	g_csgo.m_prediction->SetLocalViewAngles(m_angle);

	// set lby to predicted value.
	core.m_local->m_flLowerBodyYawTarget() = m_body;

	// force to use correct abs origin and velocity ( no CalcAbsolutePosition and CalcAbsoluteVelocity calls )
	core.m_local->m_iEFlags() &= ~(EFL_DIRTY_ABSTRANSFORM | EFL_DIRTY_ABSVELOCITY);

	// CCSGOPlayerAnimState::Update, bypass already animated checks.
	if (state->m_frame >= g_csgo.m_globals->m_frame)
		state->m_frame -= 1;

	state->m_cur_feet_yaw = 0.f;

	// call original, bypass hook.
	if (g_hooks.m_UpdateClientSideAnimation)
		g_hooks.m_UpdateClientSideAnimation(core.m_local);

	// get last networked poses.
	core.m_local->GetPoseParameters(core.m_poses);

	// store updated abs yaw.
	core.m_abs_yaw = state->m_goal_feet_yaw;

	// pull the lower body direction towards the eye direction, but only when the player is moving
	if (state->m_ground) {
		// from csgo src sdk.
		const float CSGO_ANIM_LOWER_REALIGN_DELAY = 1.1f;

		// we are moving.
		if (state->m_speed > 0.1f || fabsf(state->m_fall_velocity) > 100.f) {
			core.m_body_pred = core.m_anim_time + (CSGO_ANIM_LOWER_REALIGN_DELAY * 0.2f);
			core.m_body = m_angle.y;
			firstBreak = true;
		}

		// we arent moving.
		else {
			// time for an update.
			if (core.m_anim_time > core.m_body_pred) {
				core.m_body_pred = core.m_anim_time + CSGO_ANIM_LOWER_REALIGN_DELAY;
				core.m_body = m_angle.y;
				firstBreak = false;
			}
		}
	}

	// save updated data.
	m_rotation = core.m_local->m_angAbsRotation();
	m_speed = state->m_speed;
	m_ground = state->m_ground;
}

void Core::print(const std::string text, ...) {
	va_list     list;
	int         size;
	std::string buf;

	if (text.empty())
		return;

	va_start(list, text);

	// count needed size.
	size = std::vsnprintf(0, 0, text.c_str(), list);

	// allocate.
	buf.resize(size);

	// print to buffer.
	std::vsnprintf(buf.data(), size + 1, text.c_str(), list);

	va_end(list);

	// print to console.
	g_csgo.m_cvar->ConsoleColorPrintf(colors::green, XOR("[nicotine.cc] "));
	g_csgo.m_cvar->ConsoleColorPrintf(colors::white, buf.c_str());

}

bool Core::CanFireWeapon(float curtime) {
	// the player cant fire.
	if (!m_player_fire)
		return false;

	if (m_weapon_type == WEAPONTYPE_GRENADE)
		return false;

	// if we have no bullets, we cant shoot.
	if (m_weapon_type != WEAPONTYPE_KNIFE && m_weapon->m_iClip1() < 1)
		return false;

	// do we have any burst shots to handle?
	if ((m_weapon_id == GLOCK || m_weapon_id == FAMAS) && m_weapon->m_iBurstShotsRemaining() > 0) {
		// new burst shot is coming out.
		if (g_csgo.m_globals->m_curtime >= m_weapon->m_fNextBurstShot())
			return true;
	}

	// r8 revolver.
	if (m_weapon_id == REVOLVER) {
		int act = m_weapon->m_Activity();

		// mouse1.
		if (!m_revolver_fire) {
			if ((act == 185 || act == 193) && m_revolver_cock == 0)
				return g_csgo.m_globals->m_curtime >= m_weapon->m_flNextPrimaryAttack();

			return false;
		}
	}

	// lol.
	if (curtime >= m_weapon->m_flNextPrimaryAttack())
		return true;

	// yeez we have a normal gun.
	if (g_csgo.m_globals->m_curtime >= m_weapon->m_flNextPrimaryAttack())
		return true;

	return false;
}

void Core::UpdateRevolverCock() {
	// default to false.
	m_revolver_fire = false;

	// reset properly.
	if (m_revolver_cock == -1)
		m_revolver_cock = 0;

	// we dont have a revolver.
	// we have no ammo.
	// player cant fire
	// we are waiting for we can shoot again.
	if (m_weapon_id != REVOLVER || m_weapon->m_iClip1() < 1 || !m_player_fire || g_csgo.m_globals->m_curtime < m_weapon->m_flNextPrimaryAttack()) {
		// reset.
		m_revolver_cock = 0;
		m_revolver_query = 0;
		return;
	}

	// calculate max number of cocked ticks.
	// round to 6th decimal place for custom tickrates..
	int shoot = (int)(0.25f / (std::round(g_csgo.m_globals->m_interval * 1000000.f) / 1000000.f));

	// amount of ticks that we have to query.
	m_revolver_query = shoot - 1;

	// we held all the ticks we needed to hold.
	if (m_revolver_query == m_revolver_cock) {
		// reset cocked ticks.
		m_revolver_cock = -1;

		// we are allowed to fire, yay.
		m_revolver_fire = true;
	}

	else {
		// we still have ticks to query.
		// apply inattack.
		if (g_menu.main.config.mode.get() == 0 && m_revolver_query > m_revolver_cock)
			m_cmd->m_buttons |= IN_ATTACK;

		// count cock ticks.
		// do this so we can also count 'legit' ticks
		// that didnt originate from the hack.
		if (m_cmd->m_buttons & IN_ATTACK)
			m_revolver_cock++;

		// inattack was not held, reset.
		else m_revolver_cock = 0;
	}

	// remove inattack2 if cocking.
	if (m_revolver_cock > 0)
		m_cmd->m_buttons &= ~IN_ATTACK2;
}

void Core::UpdateIncomingSequences() {
	if (!g_csgo.m_net)
		return;

	if (m_sequences.empty() || g_csgo.m_net->m_in_seq > m_sequences.front().m_seq) {
		// store new stuff.
		m_sequences.emplace_front(g_csgo.m_globals->m_realtime, g_csgo.m_net->m_in_rel_state, g_csgo.m_net->m_in_seq);
	}

	// do not save too many of these.
	while (m_sequences.size() > 128)
		m_sequences.pop_back();
}

void Core::MouseFix(CUserCmd* cmd) {
	/*
	  FULL CREDITS TO:
	  - chance ( for reversing it )
	  - polak ( for having this in aimware )
	  - llama ( for having this in onetap and confirming )
	*/

	// purpose is to fix mouse dx/dy - there is a noticeable difference once fixed

	static ang_t delta_viewangles{ };
	ang_t delta = cmd->m_view_angles - delta_viewangles;

	static ConVar* sensitivity = g_csgo.m_cvar->FindVar(HASH("sensitivity"));

	if (delta.x != 0.f) {
		static ConVar* m_pitch = g_csgo.m_cvar->FindVar(HASH("m_pitch"));

		int final_dy = static_cast<int>((delta.x / m_pitch->GetFloat()) / sensitivity->GetFloat());
		if (final_dy <= 32767) {
			if (final_dy >= -32768) {
				if (final_dy >= 1 || final_dy < 0) {
					if (final_dy <= -1 || final_dy > 0)
						final_dy = final_dy;
					else
						final_dy = -1;
				}
				else {
					final_dy = 1;
				}
			}
			else {
				final_dy = 32768;
			}
		}
		else {
			final_dy = 32767;
		}

		cmd->m_mousedy = static_cast<short>(final_dy);
	}

	if (delta.y != 0.f) {
		static ConVar* m_yaw = g_csgo.m_cvar->FindVar(HASH("m_yaw"));

		int final_dx = static_cast<int>((delta.y / m_yaw->GetFloat()) / sensitivity->GetFloat());
		if (final_dx <= 32767) {
			if (final_dx >= -32768) {
				if (final_dx >= 1 || final_dx < 0) {
					if (final_dx <= -1 || final_dx > 0)
						final_dx = final_dx;
					else
						final_dx = -1;
				}
				else {
					final_dx = 1;
				}
			}
			else {
				final_dx = 32768;
			}
		}
		else {
			final_dx = 32767;
		}

		cmd->m_mousedx = static_cast<short>(final_dx);
	}

	delta_viewangles = cmd->m_view_angles;
}

int Core::GetNextUpdate() const {
	auto current_tick = game::TIME_TO_TICKS(core.m_local->m_nTickBase() * g_csgo.m_globals->m_interval);
	auto update_tick = game::TIME_TO_TICKS(m_body_pred);

	return update_tick - current_tick;
}

void Core::PreMovementRunCommand(int m_iCommandNumber) {
	if (m_iCommandNumber == core.m_iShiftedCommand) {
		core.m_iShiftedTickbase = core.m_local->m_nTickBase();
	}
}

void Core::PostMovementRunCommand(int m_iCommandNumber) {
	if (m_iCommandNumber == core.m_iShiftedCommand) {
		core.m_local->m_nTickBase() = core.m_iShiftedTickbase;
	}
}

/*//killstreaks
int iKills = 0, posX, posY;
int iTotal = 2568, iCombo = 0, iYpos, iAlpha, iFly, iOut;	//xd

// Pos
void Core::SetupPosition()
{
	// store, do shit ot v2 does >_<
	iKills++, iCombo++, iTotal += 500, iYpos = 60, iAlpha = NULL, iFly = NULL, iOut = 255;

	// store all
	posX = NULL, posY = NULL;

	if (iKills >= 9)
		iKills = 1;	// reset last
}

void Core::Killstreaks()
{
	if (!g_csgo.m_engine->IsInGame() || !g_csgo.m_engine->IsConnected())
		return;

	g_csgo.m_engine->GetScreenSize(m_width, m_height);

	// super retarded shit, but let it stay only 2000 frames, not more :D
	if (posX < 2000)
		posX++;
	if (posY < 2000)
		posY--;

	// fx
	if (iFly < 255 && iOut == 255)
		iFly++;

	if (iFly > 250)
		iOut--;

	if (iOut < 5 && iFly > 0)
		iFly--;

	// push killstreaks and point text
	if (g_menu.main.misc.killstreak.get())
	{
		// type 1
		if (iKills == 1)
			render::big_point.string(m_width / 2 + posX, m_height / 2 - posY, { 255, 102, 102, 255 }, "FIRST BLOOD", render::ALIGN_CENTER), render::big_combo.string(m_width - 290, m_height / 1.65 - 35, { 255, 178, 102, iFly }, "+500 POINT", render::ALIGN_LEFT);
		else if (iKills == 2)
			render::big_point.string(m_width / 2 - posX, m_height / 2 + posY, { 178, 255, 102, 255 }, "DOUBLE KILL", render::ALIGN_CENTER), render::big_combo.string(m_width - 290, m_height / 1.65 - 35, { 255, 178, 102, iFly }, "+500 POINT", render::ALIGN_LEFT);
		else if (iKills == 3)
			render::big_point.string(m_width / 2 - posX, m_height / 2 - posY, { 102, 178, 255, 255 }, "TRIPLE KILL", render::ALIGN_CENTER), render::big_combo.string(m_width - 290, m_height / 1.65 - 35, { 255, 178, 102, iFly }, "+500 POINT", render::ALIGN_LEFT);
		else if (iKills == 4)
			render::big_point.string(m_width / 2 + posX, m_height / 2 + posY, { 255, 255, 102, 255 }, "ULTRA KILL", render::ALIGN_CENTER), render::big_combo.string(m_width - 290, m_height / 1.65 - 35, { 255, 178, 102, iFly }, "+500 POINT", render::ALIGN_LEFT);
		else if (iKills == 5)
			render::big_point.string(m_width / 2 - posX, m_height / 2 + posY, { 255, 178, 102, 255 }, "MONSTER KILL", render::ALIGN_CENTER), render::big_combo.string(m_width - 290, m_height / 1.65 - 35, { 255, 178, 102, iFly }, "+500 POINT", render::ALIGN_LEFT);
		else if (iKills == 6)
			render::big_point.string(m_width / 2 + posX, m_height / 2 - posY, { 102, 102, 255, 255 }, "UNSTOPPABLE", render::ALIGN_CENTER), render::big_combo.string(m_width - 290, m_height / 1.65 - 35, { 255, 178, 102, iFly }, "+500 POINT", render::ALIGN_LEFT);
		else if (iKills == 7)
			render::big_point.string(m_width / 2 + posX, m_height / 2 + posY, { 255, 51, 153, 255 }, "GODLIKE", render::ALIGN_CENTER), render::big_combo.string(m_width - 290, m_height / 1.65 - 35, { 255, 178, 102, iFly }, "+500 POINT", render::ALIGN_LEFT);
		else if (iKills == 8)
			render::big_point.string(m_width / 2 - posX, m_height / 2 + posY, { 255, 255, 51, 255 }, "LUDICROUS KILL", render::ALIGN_CENTER), render::big_combo.string(m_width - 290, m_height / 1.65 - 35, { 255, 178, 102, iFly }, "+500 POINT", render::ALIGN_LEFT);
	}
}*/
