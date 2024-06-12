#include "main.h"

ImColor bg = ImColor(13.f, 13.f, 13.f, 200.f); // Main window color bg
ImColor cbg = ImColor(bg.Value.x, bg.Value.y, bg.Value.z, 105.f); // Childs color background

ImColor mainColor = ImColor(0.f, 163.f, 195.f, 255.f); // Used on buttons & items
ImColor secondColor = ImColor(25.f, 85.f, 255.f, 255.f); // Used on buttons when are clicked etc...

ImColor white = ImColor(255.f, 255.f, 255.f, 255.f); // General colors
ImColor gray = ImColor(117.f, 114.f, 114.f, 255.f);
ImColor lblack = ImColor(40.f, 40.f, 40.f, 50.f);

ImVec4 conv(ImColor color) {
	return ImVec4(
		color.Value.x / 255,
		color.Value.y / 255,
		color.Value.z / 255,
		color.Value.w / 255
	);
}

ImColor convToInt(ImColor color) {
	return ImColor(
		(int)color.Value.x,
		(int)color.Value.y,
		(int)color.Value.z,
		(int)color.Value.w
	);
}

bool custom::button(const char* name, float posX, float posY, float sizeX, float sizeY) {
	ImGui::PushStyleColor(ImGuiCol_Button, conv(mainColor));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, conv(secondColor));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, conv(mainColor));

	ImGui::SetCursorPos({ posX, posY });
	bool result = ImGui::Button(name, { sizeX, sizeY });
	ImGui::TreePop();

	ImGui::PopStyleColor(3);

	return result;
}

bool custom::input(const char* name, const char* hint, char buf[], size_t buf_size, float posX, float posY, float width, ImGuiInputTextFlags womp) {
	ImGui::SetNextItemWidth(width);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 15));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, conv(lblack));
	ImGui::PushStyleColor(ImGuiCol_TextDisabled, conv(gray));

	ImGui::SetCursorPos({ posX, posY });
	bool result = ImGui::InputTextEx(name, hint, buf, (int)buf_size, ImVec2(0, 0), womp, 0, 0);

	ImGui::PopStyleVar();
	ImGui::PopStyleColor(2);

	return result;
}

bool custom::imageButton(ID3D11ShaderResourceView* ptr, float posX, float posY, bool active) {

	ImVec4 bCo = { 0,0,0,0 };
	ImVec4 bCoH = conv(lblack);
	ImVec4 bCoA = conv(lblack);

	if (active)
	{
		bCo = bCoH = bCoA = bCoH;
	}

	ImGui::PushStyleColor(ImGuiCol_Button, bCo);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, bCoH);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, bCoA);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 8));

	ImGui::SetCursorPos({ posX, posY });

	bool result = ImGui::ImageButton(ptr, ImVec2(35, 35), ImVec2(0, 0), ImVec2(1, 1));
	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar(1);

	return result;
}

void custom::text(const char* text, float posX, float posY, ImFont* font, float fontSize, ImColor color) {
	ImGui::PushFont(font);
	ImGui::GetWindowDrawList()->AddText(font, fontSize, { posX, posY }, convToInt(color), text);
	ImGui::PopFont();
}

void custom::progressbar(float ptr, float posX, float posY, float sizeX, float sizeY) {
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, conv(mainColor));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, conv(ImColor(40.f, 40.f, 40.f, 200.f)));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

	ImGui::SetCursorPos({ posX, posY });
	ImGui::ProgressBar(ptr, { sizeX, sizeY }, "");
	ImGui::PopStyleVar();
}

void custom::style() {
	ImGui::PushStyleColor(ImGuiCol_WindowBg, conv(bg));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, conv(lblack));
	ImGuiStyle& style = ImGui::GetStyle();

	style.ChildBorderSize = 0.f;
	style.WindowBorderSize = 0.f;
	style.WindowRounding = 10.f;
	style.ChildRounding = 5.f;
	style.FrameBorderSize = 0.f;
	style.FrameRounding = 5.f;
}

bool custom::multicolorButton(const char* label, const char* text, float posX, float posY, float width, float height, ImColor c1, ImColor c2, ImColor c3, ImColor c4) {
	ImGui::SetCursorPos({posX, posY});

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImVec2 button_size(width, height);

	bool result = ImGui::InvisibleButton(label, button_size);

	ImVec2 p_min = ImGui::GetItemRectMin();
	ImVec2 p_max = ImGui::GetItemRectMax();

	draw_list->AddRectFilledMultiColor(p_min, p_max, c1, c2, c3, c4);
	draw_list->AddRect(ImVec2(p_min.x - 1, p_min.y - 1), ImVec2(p_max.x + 1, p_max.y + 1), convToInt(mainColor), 5.0f, 0, 2.0f);

	if (text) {
		ImVec2 text_size = ImGui::CalcTextSize(text);
		ImVec2 text_pos = ImVec2(
			p_min.x + (width - text_size.x) * 0.5f,
			p_min.y + (height - text_size.y) * 0.5f
		);
		draw_list->AddText(text_pos, ImColor(255, 255, 255, 255), text);
	}

	return result;
}