#pragma once

class Sequence {
public:
	float m_time;
	int   m_state;
	int   m_seq;

public:
	__forceinline Sequence() : m_time{}, m_state{}, m_seq{} {};
	__forceinline Sequence(float time, int state, int seq) : m_time{ time }, m_state{ state }, m_seq{ seq } {};
};

class NetPos {
public:
	float  m_time;
	vec3_t m_pos;

public:
	__forceinline NetPos() : m_time{}, m_pos{} {};
	__forceinline NetPos(float time, vec3_t pos) : m_time{ time }, m_pos{ pos } {};
};

class Core {
public:
	// hack thread.
	static ulong_t __stdcall init(void* arg);

	void StartMove(CUserCmd* cmd);
	void EndMove(CUserCmd* cmd);
	void BackupPlayers(bool restore);
	void DoMove();
	void Watermark();
	void DrawHUD();
	void UnlockHiddenConvars();
	void ClanTag();
	void Skybox();
	void UpdateInformation();
	void SetAngles();
	void UpdateAnimations();
	void KillFeed();

	void OnPaint();
	void OnMapload();
	void OnTick(CUserCmd* cmd);

	// debugprint function.
	void print(const std::string text, ...);

	// check if we are able to fire this tick.
	bool CanFireWeapon(float curtime);
	bool IsFiring(float curtime);
	void UpdateRevolverCock();
	void UpdateIncomingSequences();

	void MouseFix(CUserCmd* cmd);

	int GetNextUpdate() const;

	void PreMovementRunCommand(int m_iCommandNumber);
	void PostMovementRunCommand(int m_iCommandNumber);

	/*
	void SetupPosition()
	void Killstreaks();)*/

public:
	// local player variables.
	Player* m_local;
	// variables for double tap
	int				 m_tick_to_shift;
	int				 m_tick_to_recharge;
	int				 ticksToShift;
	int				 lastShiftedCmdNr;
	bool			 can_recharge;
	bool m_didFakeFlick;
	int				 doubletapCharge;
	bool			 isShifting;
	bool			 ignoreallcmds;
	bool             isCharged;
	int              m_iShiftedCommand;
	int              m_iShiftedTickbase;
	bool			 can_dt_shoot;
	bool			 m_charged;
	//Player*          resolve_data;
	bool	         m_processing;
	int	             m_flags;
	int              m_bhops;
	vec3_t	         m_shoot_pos;
	bool	         m_player_fire;
	bool	         m_shot;
	bool	         m_old_shot;
	float            m_abs_yaw;
	float            m_poses[24];
	float			 m_left_thickness[64], m_right_thickness[64], m_at_target_angle[64];

	// active weapon variables.
	Weapon* m_weapon;
	int         m_weapon_id;
	WeaponInfo* m_weapon_info;
	int         m_weapon_type;
	bool        m_weapon_fire;

	// revolver variables.
	int	 m_revolver_cock;
	int	 m_revolver_query;
	bool m_revolver_fire;

	// general game varaibles.
	bool     m_round_end;
	Stage_t	 m_stage;
	int	     m_max_lag;
	int      m_max_lag2;
	int      m_max_lag3;
	int      m_lag;
	int	     m_old_lag;
	bool* m_packet;
	bool m_pressing_move;
	bool* m_final_packet;
	bool	 m_old_packet;
	float	 m_lerp;
	float    m_latency;
	int      m_latency_ticks;
	int      m_server_tick;
	int      m_arrival_tick;
	int      m_width, m_height;

	bool             m_animate;
	bool             m_update;

	// usercommand variables.
	CUserCmd* m_cmd;
	int	      m_tick;
	int	      m_rate;
	int	      m_buttons;
	int       m_old_buttons;
	ang_t     m_view_angles;
	ang_t	  m_strafe_angles;
	vec3_t	  m_forward_dir;

	penetration::PenetrationOutput_t m_pen_data;

	std::deque< Sequence > m_sequences;
	std::deque< NetPos >   m_net_pos;

	// animation variables.
	ang_t  m_angle;
	ang_t  m_rotation;
	ang_t  m_radar;
	float  m_body;
	float  m_body_pred;
	bool firstBreak;
	float  m_speed;
	float  m_anim_time;
	float  m_anim_frame;
	bool   m_ground;
	bool   m_lagcomp;
	float  m_jump_fall;
	bool   m_trigger = true;

	// hack info.
	std::string user;
	std::string build;
	std::string name;
};

//killstreaks


extern Core core;