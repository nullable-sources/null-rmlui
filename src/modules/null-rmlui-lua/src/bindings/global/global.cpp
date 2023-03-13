#include <bindings/bindings.h>

namespace null::rml::modules::lua::global_bindings {
	namespace functions {
		std::function<Rml::Context* (int)> get_context() {
			return std::function<Rml::Context* (int)>{ [](int idx) { return Rml::GetContext(idx); } };
		}

		std::function<int()> get_max_contexts() {
			return std::function<int()>{ []() { return Rml::GetNumContexts(); } };
		}
	}

	void bind(sol::state_view& state_view) {
		#define add_key_identifier(name, lowercase) rmlui["e_key_identifier"][#lowercase] = Rml::Input::KI_##name
		
		sol::table rmlui{ state_view.create_named_table("rmlui") };

		// We can't make this into an enum.
		// The compiler can't handle everything under one call.
		rmlui.create_named("e_key_identifier");
		add_key_identifier(UNKNOWN, unknown);
		add_key_identifier(SPACE, space);
		add_key_identifier(0, 0);
		add_key_identifier(1, 1);
		add_key_identifier(2, 2);
		add_key_identifier(3, 3);
		add_key_identifier(4, 4);
		add_key_identifier(5, 5);
		add_key_identifier(6, 6);
		add_key_identifier(7, 7);
		add_key_identifier(8, 8);
		add_key_identifier(9, 9);
		add_key_identifier(A, a);
		add_key_identifier(B, b);
		add_key_identifier(C, c);
		add_key_identifier(D, d);
		add_key_identifier(E, e);
		add_key_identifier(F, f);
		add_key_identifier(G, g);
		add_key_identifier(H, h);
		add_key_identifier(I, i);
		add_key_identifier(J, j);
		add_key_identifier(K, k);
		add_key_identifier(L, l);
		add_key_identifier(M, m);
		add_key_identifier(N, n);
		add_key_identifier(O, o);
		add_key_identifier(P, p);
		add_key_identifier(Q, q);
		add_key_identifier(R, r);
		add_key_identifier(S, s);
		add_key_identifier(T, t);
		add_key_identifier(U, u);
		add_key_identifier(V, v);
		add_key_identifier(W, w);
		add_key_identifier(X, x);
		add_key_identifier(Y, y);
		add_key_identifier(Z, z);
		add_key_identifier(OEM_1, oem_1);
		add_key_identifier(OEM_PLUS, oem_plus);
		add_key_identifier(OEM_COMMA, oem_comma);
		add_key_identifier(OEM_MINUS, oem_minus);
		add_key_identifier(OEM_PERIOD, oem_period);
		add_key_identifier(OEM_2, oem_2);
		add_key_identifier(OEM_3, oem_3);
		add_key_identifier(OEM_4, oem_4);
		add_key_identifier(OEM_5, oem_5);
		add_key_identifier(OEM_6, oem_6);
		add_key_identifier(OEM_7, oem_7);
		add_key_identifier(OEM_8, oem_8);
		add_key_identifier(OEM_102, oem_102);
		add_key_identifier(NUMPAD0, numpad_0);
		add_key_identifier(NUMPAD1, numpad_1);
		add_key_identifier(NUMPAD2, numpad_2);
		add_key_identifier(NUMPAD3, numpad_3);
		add_key_identifier(NUMPAD4, numpad_4);
		add_key_identifier(NUMPAD5, numpad_5);
		add_key_identifier(NUMPAD6, numpad_6);
		add_key_identifier(NUMPAD7, numpad_7);
		add_key_identifier(NUMPAD8, numpad_8);
		add_key_identifier(NUMPAD9, numpad_9);
		add_key_identifier(NUMPADENTER, numpad_enter);
		add_key_identifier(MULTIPLY, multiply);
		add_key_identifier(ADD, add);
		add_key_identifier(SEPARATOR, separator);
		add_key_identifier(SUBTRACT, subtract);
		add_key_identifier(DECIMAL, decimal);
		add_key_identifier(DIVIDE, divide);
		add_key_identifier(OEM_NEC_EQUAL, oem_nec_equal);
		add_key_identifier(BACK, back);
		add_key_identifier(TAB, tab);
		add_key_identifier(CLEAR, clear);
		add_key_identifier(RETURN, return);
		add_key_identifier(PAUSE, pause);
		add_key_identifier(CAPITAL, capital);
		add_key_identifier(KANA, kana);
		add_key_identifier(HANGUL, hangul);
		add_key_identifier(JUNJA, junja);
		add_key_identifier(FINAL, final);
		add_key_identifier(HANJA, hanja);
		add_key_identifier(KANJI, kanju);
		add_key_identifier(ESCAPE, escape);
		add_key_identifier(CONVERT, convert);
		add_key_identifier(NONCONVERT, non_convert);
		add_key_identifier(ACCEPT, accept);
		add_key_identifier(MODECHANGE, mode_change);
		add_key_identifier(PRIOR, prior);
		add_key_identifier(NEXT, next);
		add_key_identifier(END, end);
		add_key_identifier(HOME, home);
		add_key_identifier(LEFT, left);
		add_key_identifier(UP, up);
		add_key_identifier(RIGHT, right);
		add_key_identifier(DOWN, down);
		add_key_identifier(SELECT, select);
		add_key_identifier(PRINT, print);
		add_key_identifier(EXECUTE, execute);
		add_key_identifier(SNAPSHOT, snapshot);
		add_key_identifier(INSERT, insert);
		add_key_identifier(DELETE, delete);
		add_key_identifier(HELP, help);
		add_key_identifier(LWIN, left_win);
		add_key_identifier(RWIN, right_win);
		add_key_identifier(APPS, apps);
		add_key_identifier(POWER, power);
		add_key_identifier(SLEEP, sleep);
		add_key_identifier(WAKE, wake);
		add_key_identifier(F1, f1);
		add_key_identifier(F2, f2);
		add_key_identifier(F3, f3);
		add_key_identifier(F4, f4);
		add_key_identifier(F5, f5);
		add_key_identifier(F6, f6);
		add_key_identifier(F7, f7);
		add_key_identifier(F8, f8);
		add_key_identifier(F9, f9);
		add_key_identifier(F10, f10);
		add_key_identifier(F11, f11);
		add_key_identifier(F12, f12);
		add_key_identifier(F13, f13);
		add_key_identifier(F14, f14);
		add_key_identifier(F15, f15);
		add_key_identifier(F16, f16);
		add_key_identifier(F17, f17);
		add_key_identifier(F18, f18);
		add_key_identifier(F19, f19);
		add_key_identifier(F20, f20);
		add_key_identifier(F21, f21);
		add_key_identifier(F22, f22);
		add_key_identifier(F23, f23);
		add_key_identifier(F24, f24);
		add_key_identifier(NUMLOCK, num_lock);
		add_key_identifier(SCROLL, scroll);
		add_key_identifier(OEM_FJ_JISHO, oem_fj_jisho);
		add_key_identifier(OEM_FJ_MASSHOU, oem_fj_masshou);
		add_key_identifier(OEM_FJ_TOUROKU, oem_fj_touroku);
		add_key_identifier(OEM_FJ_LOYA, oem_fj_loya);
		add_key_identifier(OEM_FJ_ROYA, oem_fj_roya);
		add_key_identifier(LSHIFT, left_shift);
		add_key_identifier(RSHIFT, right_shift);
		add_key_identifier(LCONTROL, left_control);
		add_key_identifier(RCONTROL, right_control);
		add_key_identifier(LMENU, left_menu);
		add_key_identifier(RMENU, right_menu);
		add_key_identifier(BROWSER_BACK, browser_back);
		add_key_identifier(BROWSER_FORWARD, browser_forward);
		add_key_identifier(BROWSER_REFRESH, browser_refresh);
		add_key_identifier(BROWSER_STOP, browser_stop);
		add_key_identifier(BROWSER_SEARCH, browser_search);
		add_key_identifier(BROWSER_FAVORITES, browser_favorites);
		add_key_identifier(BROWSER_HOME, browser_home);
		add_key_identifier(VOLUME_MUTE, volume_mute);
		add_key_identifier(VOLUME_DOWN, volume_down);
		add_key_identifier(VOLUME_UP, volume_up);
		add_key_identifier(MEDIA_NEXT_TRACK, media_next_track);
		add_key_identifier(MEDIA_PREV_TRACK, media_prev_track);
		add_key_identifier(MEDIA_STOP, media_stop);
		add_key_identifier(MEDIA_PLAY_PAUSE, media_play_pause);
		add_key_identifier(LAUNCH_MAIL, launch_mail);
		add_key_identifier(LAUNCH_MEDIA_SELECT, launch_media_select);
		add_key_identifier(LAUNCH_APP1, launch_app1);
		add_key_identifier(LAUNCH_APP2, launch_app2);
		add_key_identifier(OEM_AX, oem_ax);
		add_key_identifier(ICO_HELP, ico_help);
		add_key_identifier(ICO_00, ico_00);
		add_key_identifier(PROCESSKEY, process_key);
		add_key_identifier(ICO_CLEAR, ico_clear);
		add_key_identifier(ATTN, attn);
		add_key_identifier(CRSEL, crsel);
		add_key_identifier(EXSEL, exsel);
		add_key_identifier(EREOF, ereof);
		add_key_identifier(PLAY, play);
		add_key_identifier(ZOOM, zoom);
		add_key_identifier(PA1, pa1);
		add_key_identifier(OEM_CLEAR, oem_clear);

		rmlui.new_enum<Rml::Input::KeyModifier>("e_key_modifier", {
			{ "ctrl", Rml::Input::KM_CTRL },
			{ "shift", Rml::Input::KM_SHIFT },
			{ "alt", Rml::Input::KM_ALT },
			{ "meta", Rml::Input::KM_META },
			{ "capslock", Rml::Input::KM_CAPSLOCK },
			{ "numlock", Rml::Input::KM_NUMLOCK },
			{ "scroll_lock", Rml::Input::KM_SCROLLLOCK }
		});

		rmlui.new_enum<Rml::Style::FontWeight>("e_font_weight", {
			{ "auto", Rml::Style::FontWeight::Auto },
			{ "normal", Rml::Style::FontWeight::Normal },
			{ "bold", Rml::Style::FontWeight::Bold }
		});

		rmlui.new_enum<Rml::DefaultActionPhase>("e_default_action_phase", {
			{ "none", Rml::DefaultActionPhase::None },
			{ "target", Rml::DefaultActionPhase::Target },
			{ "target_and_bubble", Rml::DefaultActionPhase::TargetAndBubble }
		});
		
		rmlui.set("create_context", &Rml::CreateContext);
		rmlui.set("load_font_face", sol::overload(
			[](const std::string& file) { return Rml::LoadFontFace(file); },
			[](const std::string& file, bool fallback) { return Rml::LoadFontFace(file, fallback); },
			[](const std::string& file, bool fallback, Rml::Style::FontWeight weight) { return Rml::LoadFontFace(file, fallback, weight); }
		));
		rmlui.set("get_context", sol::resolve<Rml::Context* (const std::string&)>(&Rml::GetContext));
		rmlui.set("register_event_type", sol::overload(
			[](const std::string& type, bool interruptible, bool bubbles, Rml::DefaultActionPhase default_action_phase) { return Rml::RegisterEventType(type, interruptible, bubbles, default_action_phase); },
			[](const std::string& type, bool interruptible, bool bubbles) { return Rml::RegisterEventType(type, interruptible, bubbles, Rml::DefaultActionPhase::None); }
		));
		rmlui.set("contexts", sol::readonly_property(&get_indexed_table<Rml::Context, &functions::get_context, &functions::get_max_contexts>));
		rmlui.set("version", sol::readonly_property(&Rml::GetVersion));
	}
}