#pragma once
namespace settings
{
	inline int width = GetSystemMetrics(SM_CXSCREEN);
	inline int height = GetSystemMetrics(SM_CYSCREEN);
	inline int screen_center_x = width / 2;
	inline int screen_center_y = height / 2;
	inline bool show_menu = true;
	inline int tab = 0;
	inline int rank;
	inline int get_rank;
	inline int framerate;

	namespace aimbot
	{
		inline bool enable = false;
		inline bool triggerbot = false;
		inline bool show_fov = false;
		inline float fov = 150;
		inline float smoothness = 5;
		float fovColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		inline bool prediction = false;
		inline bool headbone = false;
		inline bool chestbone = false;
		float projectileSpeed = 60;
		float scaledProjectileSpeed = 60000.0f;
	}

	namespace visuals
	{
		inline bool enable = false;
		inline bool box = false;
		inline bool fill_box = false;
		float boxColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float boxColor2[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float boxColor3[4] = { 1.0f, 1.0f, 1.0f, 50.0f };
		inline bool line = false;
		inline bool distance = false;
		inline bool fps = false;
		inline bool skeleton = false;
		inline int skeleton_thick = 4.0f;
	}

	namespace kmbox
	{
		inline bool kmboxb = false;
		inline bool kmboxnet = false;
		inline bool confirm = false;
		char kmbox_ip[24] = "";
		char kmbox_port[10] = "";
		char kmbox_uuid[32] = "";
		bool show_kmbox_details_popup = false;

	}
	namespace misc
	{
	inline bool zero_alpha = false;
	int fps_limit = 60;
	inline bool color = false;
	float blueColor[4] = { 0.28f, 0.56f, 1.00f, 1.00f };
	float textColor[4] = { 0.95f, 0.96f, 0.98f, 1.00f };
	float bgColor[4] = { 0.10f, 0.11f, 0.12f, 0.90f };
	}

	namespace debug
	{
		inline bool showDebugOptions = false;
		inline bool option1 = false;
		inline bool option2 = false;
		inline bool option3 = false;
		inline bool option4 = false;
	}
}
