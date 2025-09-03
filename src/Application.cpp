#include "Application.h"

Application::Application()
	: m_window(sf::RenderWindow(sf::VideoMode(g_window_settings.width, g_window_settings.height), g_window_settings.name, sf::Style::Titlebar | sf::Style::Close)),
	m_gui(GUI(m_window)),
	m_is_mouse_left_pressed(false),
	m_piano_keys(m_piano.GetKeys()) {

	m_window.setKeyRepeatEnabled(false);
	m_piano.SetKeyPositions((float)g_window_settings.width, (float)g_window_settings.height);
}

void Application::HandleEvents(sf::Event& event) {

	m_gui.ProcessEvent(m_window, event);

	if (event.type == __noop) {
		m_window.close();
	}

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

		m_is_mouse_left_pressed = true;
	}
	else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {

		m_is_mouse_left_pressed = false;

		for (size_t i = 0; i < m_piano_keys.size(); i++) {

			PianoKey& key = m_piano_keys[i];

			if (key.HasBeenStruck())
				m_piano.ReleaseKey(key.GetMidiNote());
		}
	}
}

void Application::HandlePianoKeyStrikes() {

	if (m_is_mouse_left_pressed) {

		sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window));

		bool blackKeyHovered = false;

		// Check black keys first and flag if one has been hovered
		for (size_t i = 0; i < m_piano_keys.size(); ++i) {

			PianoKey& key = m_piano_keys[i];

			if (key.IsBlack()) {

				if (key.IsPointInsideBounds(mousePosition)) {

					if (!key.HasBeenStruck())
						m_piano.StrikeKey(key.GetMidiNote());

					blackKeyHovered = true;
				}
				else {

					if (key.HasBeenStruck())
						m_piano.ReleaseKey(key.GetMidiNote());
				}
			}
		}

		// Check white keys and make sure that a black key isn't hovered
		// This is done so that two keys can't be struck at the same time
		for (size_t i = 0; i < m_piano_keys.size(); ++i) {

			PianoKey& key = m_piano_keys[i];

			if (key.IsBlack())
				continue;

			if (key.IsPointInsideBounds(mousePosition) && !blackKeyHovered) {

				if (!key.HasBeenStruck())
					m_piano.StrikeKey(key.GetMidiNote());
			}
			else {

				if (key.HasBeenStruck())
					m_piano.ReleaseKey(key.GetMidiNote());
			}
		}
	}
}

void Application::Run() {

	sf::Clock clock;

	while (m_window.isOpen()) {

		sf::Event event;

		while (m_window.pollEvent(event)) {

			HandleEvents(event);
		}

		// Logic
		sf::Time sec = clock.restart();
		m_gui.Update(m_window, sec);

		m_piano.PlayComposition();

		HandlePianoKeyStrikes();

		// Rendering
		m_window.clear();

		m_piano.DrawKeys(m_window);

		HandleGUI();
		m_gui.Render(m_window);

		m_window.display();
	}

	m_gui.Close();
}

void Application::HandleGUI() {

	ImGui::Begin("Music", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	ImGui::SetWindowSize(ImVec2((float)g_window_settings.width / 2.f, (float)g_window_settings.height - m_piano_keys[0].GetHeight()));
	ImGui::SetWindowPos(ImVec2(0.f, 0.f));

	ImGui::SeparatorText("Files");
	if (ImGui::Button("Select midi")) {

		nfdfilteritem_t filters[1] = { {"Midi files", "mid"}};
		nfdresult_t result = m_nfd_handler.OpenDialog(filters, 1);

		if (result == NFD_OKAY) {

			m_piano.LoadMidiFile(m_nfd_handler.GetPath());
		}
	}

	ImGui::NewLine();

	ImGui::SeparatorText("Composition");
	if (ImGui::Button("Play")) {

		m_piano.StartComposition();
	}
	ImGui::SameLine();
	if (ImGui::Button(m_piano.IsCompositionPlaying() ? "Pause" : "Unpause")) {

		m_piano.PauseUnpauseComposition();
	}
	ImGui::SameLine();
	if (ImGui::Button("Restart")) {

		m_piano.RestartComposition();
	}

	ImGui::NewLine();

	const ImVec4 statusColor = m_piano.IsCompositionPlaying() ? ImVec4(0.f, 1.f, 0.f, 1.f) : ImVec4(1.f, 0.f, 0.f, 1.f);
	const char* statusText = m_piano.IsCompositionPlaying() ? "Playing" : "Stopped";
	ImGui::Text("Status:"); ImGui::SameLine(); ImGui::TextColored(statusColor, statusText);

	ImGui::End();

	ImGui::Begin("Information", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	ImGui::SetWindowSize(ImVec2((float)g_window_settings.width / 2.f, (float)g_window_settings.height - m_piano_keys[0].GetHeight()));
	ImGui::SetWindowPos(ImVec2((float)g_window_settings.width / 2.f, 0.f));

	ImGui::End();
}