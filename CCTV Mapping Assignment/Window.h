#pragma once
class Window
{
	SDL_Window * gWindow;
	SDL_GLContext gContext;
	bool gRenderQuad;
	bool Init(char* _windowName, int _windowX, int _windowY);
public:
	Window(char* _windowName, int _windowX, int _windowY);
	~Window();

	int windowWidth;
	int windowHeight;
	float xMultiplier;
	float yMultiplier;
	bool initiated;

	void Close();
	void DrawLine(float x1, float y1, float x2, float y2);
	void SwapWindow();
	void ClearWindow();
};

