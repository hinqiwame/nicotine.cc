#include "includes.h"

void Form::draw() {
	// opacity should reach 1 in 500 milliseconds.
	constexpr float frequency = 1.f / 0.5f;

	// the increment / decrement per frame.
	float step = frequency * g_csgo.m_globals->m_frametime;

	// if open		-> increment
	// if closed	-> decrement
	m_open ? m_opacity += step : m_opacity -= step;

	// clamp the opacity.
	math::clamp(m_opacity, 0.f, 1.f);

	m_alpha = 0xff * m_opacity;
	if (!m_alpha)
		return;

	// get gui color.
	Color color = g_gui.m_color;
	color.a() = m_alpha;

	// background.
	render::rect_filled(m_x, m_y, m_width, m_height, { 12, 12, 12, 255 });
	Color gui_color = g_menu.main.config.menu_color.get();

	// border.
	render::rect(m_x, m_y, m_width, m_height, { 48, 48, 48, m_alpha });
	render::rect_filled(m_x, m_y + 38, m_width, 2, { 48, 48, 48, 150 });
	render::gradient1337(m_x + m_width - 251, m_y + 38, 250, 2, { 0, 0, 0, 0 }, { gui_color });

	render::gradient1337(m_x, m_y, 160, 3, { gui_color }, { 0, 0, 0, 0 });
	render::gradient(m_x, m_y, 3, 160, { gui_color }, { 0, 0, 0, 0 });

	render::gradient1337(m_x + m_width - 160, m_y + m_height - 3, 160, 3, { 0, 0, 0, 0 }, { gui_color });
	render::gradient(m_x + m_width - 3, m_y + m_height - 160, 3, 160, { 0, 0, 0, 0 }, { gui_color });

	// draw tabs if we have any.
	if (!m_tabs.empty()) {
		Rect tabs_area = GetTabsRect();

		for (size_t i{}; i < m_tabs.size(); ++i) {
			const auto& t = m_tabs[i];
			int font_width = render::menu_shade.size(t->m_title).m_width;

			// text
			render::menu_shade.string(tabs_area.x + (i * (tabs_area.w / m_tabs.size())) + 16, tabs_area.y - 7,
				Color{ 152, 152, 152, m_alpha }, t->m_title);

			// active tab indicator
			render::rect_filled_fade(tabs_area.x + (i * (tabs_area.w / m_tabs.size())) + 10, tabs_area.y + 7,
				font_width + 11, 2,
				t == m_active_tab ? Color{ 0, 0, 0, 0 } : Color{ 0, 0, 0, m_alpha }, 0, 150);

			render::rect_filled(tabs_area.x + (i * (tabs_area.w / m_tabs.size())) + 10, tabs_area.y + 7,
				font_width + 11, 2,
				t == m_active_tab ? color : Color{ 0, 0, 0, 0 });
		}

		// this tab has elements.
		if (!m_active_tab->m_elements.empty()) {
			// elements background and border.
			Rect el = GetElementsRect();
	
			render::rect(el.x + 5, el.y + 7, el.w / 2 - 20, el.h - 10, { 48, 48, 48, m_alpha });
			render::rect(el.x + 5 + el.h / 2 - 10, el.y + 7, el.w / 2 - 10, el.h - 10, { 48, 48, 48, m_alpha });

			//raindrop [2k17]
			render::gradient1337(el.x + 5, el.y + 7, 40, 2, { gui_color }, { 0, 0, 0, 0 });
			render::gradient(el.x + 5, el.y + 7, 2, 40, { gui_color }, { 0, 0, 0, 0 });

			render::gradient1337(el.x + 5 + el.h / 2 - 10, el.y + 7, 40, 2, { gui_color }, { 0, 0, 0, 0 });
			render::gradient(el.x + 5 + el.h / 2 - 10, el.y + 7, 2, 40, { gui_color }, { 0, 0, 0, 0 });

			render::gradient1337(el.x + 5 + el.h / 2 - 70, el.y + 5 + el.h - 10, 40, 2, { 0, 0, 0, 0 }, { gui_color });
			render::gradient(el.x + 5 + el.h / 2 - 32, el.y + 5 + el.h - 50, 2, 40, { 0, 0, 0, 0 }, { gui_color });

			render::gradient1337(el.x + 5 + el.h / 2 - 70 + el.h / 2, el.y + 5 + el.h - 10, 40, 2, { 0, 0, 0, 0 }, { gui_color });
			render::gradient(el.x + 5 + el.h / 2 - 32 + el.h / 2, el.y + 5 + el.h - 50, 2, 40, { 0, 0, 0, 0 }, { gui_color });

			// iterate elements to display.
			for (const auto& e : m_active_tab->m_elements) {

				// draw the active element last.
				if (!e || (m_active_element && e == m_active_element))
					continue;

				if (!e->m_show)
					continue;

				// this element we dont draw.
				if (!(e->m_flags & ElementFlags::DRAW))
					continue;

				e->draw();
			}

			// we still have to draw one last fucker.
			if (m_active_element && m_active_element->m_show)
				m_active_element->draw();
		}
	}
}