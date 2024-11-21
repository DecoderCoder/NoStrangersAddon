#pragma once
#include "../ImGui/imgui.h"
#include "../../Vector/Vector.h"
//#include "Functions.h"
//#include "Geometry.h"
//#include "Resources.h"
#include "directxmath.h"
#include <Windows.h>
#include <ctime>
#include <vector>

namespace Render {
	//	inline Image_Manager Images_Manager = nullptr;

	inline int RenderWidth = 0;
	inline int RenderHeight = 0;

	void BeginOverlay();
	void EndOverlay();

	void BeginOverlayTab(std::string name);
	void EndOverlayTab();

	void Draw_Line(ImVec2 pos1, ImVec2 pos2, ImColor color, float thickness);
	void Draw_Line(float_t x1, float_t y1, float_t x2, float_t y2, ImColor color, float thickness);
	//	void Draw_Line3D(Vector3 pos1, Vector3 pos2, ImColor color, float thickness);

	void Draw_FilledRectangle(ImVec2 pos, ImVec2 size, ImColor color, ImColor borderColor = ImColor::ImColor(0, 0, 0, 0), float thickness = 1);
	void Draw_FilledRectangle(float_t x1, float_t y1, float_t x2, float_t y2, ImColor color, ImColor borderColor = ImColor::ImColor(0, 0, 0, 0), float thickness = 1);

	void Draw_Circle(ImVec2 vec1, float_t radius, ImColor color, float thickness = 1);
	void Draw_Circle(float_t x, float_t y, float_t radius, ImColor color, float thickness = 1);
	//	void Draw_Circle3DFilled(Vector3 center, float radius, ImColor color);
	//	void Draw_Circle3D(Vector3 center, float radius, ImColor color, float thickness);

	//	void Polygon(Geometry::Polygon poly, ImColor color, float tick);
	void Rect(Vector3 start, Vector3 end, float radius, float width, ImColor color);

	void Draw_Text(ImVec2 pos, std::string text, ImU32 color = 0xFFFFFFFF, int fontSize = 13);
	void Draw_Text(float_t x, float_t y, std::string text, ImU32 color = 0xFFFFFFFF, int fontSize = 13);
	void Draw_Text_Centered(ImVec2 pos, ImVec2 size, std::string text, ImU32 color = 0xFFFFFFFF, int fontSize = 13);
	void Draw_Text_Centered(float_t x, float_t y, float_t width, float_t height, std::string text, ImU32 color = 0xFFFFFFFF, int fontSize = 13);

	class TextRows {
	private:
		std::vector<std::string> strings;
	public:
		ImColor BackgroundColor = ImColor(0, 0, 0, 128);
		ImColor TextColor = ImColor(255, 255, 255, 255);
		ImColor BorderColor = ImColor(255, 255, 255, 128);
		float VPadding = 3;
		float HPadding = 10;
		bool Border = true;
		bool Background = true;
		int TextSize = 13;

		void AddRow(std::string row);
		std::vector<ImVec2> Render(ImVec2 pos, bool alignCenter = true);
	};
	//	void Draw_Image(ImVec2 pos, ImVec2 size, ID3D11ShaderResourceView* image);
	//	void Draw_Image(float_t x, float_t y, float_t width, float_t height, ID3D11ShaderResourceView* image);
}

namespace Color {
	inline ImColor White = ImColor(255, 255, 255);
	inline ImColor Black = ImColor(0, 0, 0);
	inline ImColor Red = ImColor(255, 0, 0);
	inline ImColor Green = ImColor(0, 255, 0);
	inline ImColor Blue = ImColor(0, 0, 255);
	inline ImColor Yellow = ImColor(255, 255, 0);
	inline ImColor LightBlue = ImColor(0, 255, 255);

	inline ImColor Rainbow(int slowing = 10) {
		return ImColor::HSV((float)(GetTickCount() / slowing % 255) / 255, 1.f, 1.f);
	}
}