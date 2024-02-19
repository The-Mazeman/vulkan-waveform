#include "include.hpp"
#include "mainWindow.hpp"

START_SCOPE(mainWindow)

LRESULT CALLBACK windowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

void setAttribute(HWND window)
{
    SetWindowTheme(window, L"", L"");
    DragAcceptFiles(window, 1);
}
void create()
{
    State* state = {};
    allocateMemory(sizeof(State), (void**)&state);

    HWND mainWindowHandle = {};
    createWindowClass(L"mainWindowClass", windowCallback);
    createWindow(L"mainWindowClass", state, &mainWindowHandle);

    setAttribute(mainWindowHandle);
    placeWindow(mainWindowHandle, 0, 0, 1280, 720);

    HINSTANCE windowInstance = {};
    getModuleHandle(&windowInstance);

    void* vulkan = {};
    vulkan::create(mainWindowHandle, windowInstance, &vulkan);
    state->vulkan = vulkan;


    START_BLOCK
    uint entityIndex = {};
    vulkan::createEntity(vulkan, &entityIndex);

    uint positionIndex = {};
    vec2* position = {};
    vulkan::createPosition(vulkan, &positionIndex, &position);
    vulkan::bindPosition(vulkan, entityIndex, positionIndex);
    position->cells[0] = 0.0f;
    position->cells[1] = 0.0f;

    uint transformIndex = {};
    vec2* transform = {};
    vulkan::createTransform(vulkan, &transformIndex, &transform);
    transform->cells[0] = 400.0f;
    transform->cells[1] = 100.0f;
    vulkan::bindTransform(vulkan, entityIndex, transformIndex);

    uint colorIndex = {};
    vec4* color = {};
    vulkan::createColor(vulkan, &colorIndex, &color);
    color->cells[0] = 1.0f;
    color->cells[1] = 1.0f;
    color->cells[2] = 1.0f;
    color->cells[3] = 1.0f;
    vulkan::bindColor(vulkan, entityIndex, colorIndex);

    vulkan::drawEntity(vulkan, entityIndex);

    END_BLOCK

    vulkan::draw(vulkan);
}
LRESULT handleHitTesting(HWND window, LPARAM lParam)
{
	int cursorX = GET_X_LPARAM(lParam);
	int cursorY = GET_Y_LPARAM(lParam);

	int a, b, c, d;
	getWindowRectangle(window, &a, &b, &c, &d);

	if(cursorY < b + 16)
	{
		return HTCAPTION;
	}
	if(cursorX < a + 8)
	{
		return HTLEFT;
	}
	if(cursorX > c - 8)
	{
		return HTRIGHT;
	}
	if(cursorY > d - 8)
	{
		return HTBOTTOM;
	}
	return HTCLIENT;
}
void handleMouseMove(State* state, LPARAM lParam)
{
    void* vulkan = state->vulkan;

    vulkan::draw(vulkan);
}
void handleSizing(State* state, LPARAM lParam)
{
    RECT* window = (RECT*)lParam;
    int width = window->right - window->left;
    int height = window->bottom - window->top;

    void* vulkan = state->vulkan;
    vec2* transform = {};
    vulkan::getTransform(vulkan, 0, &transform);
    transform->cells[0] = (float)width;
    transform->cells[1] = (float)height;

    vulkan::drawEntity(vulkan, 0);
    vulkan::draw(vulkan);
}
void handleFileDrop(State* state, WPARAM wParam)
{
    uint characterCount = {};
    WCHAR filePath[256] = {};
    getFilePath(wParam, filePath, &characterCount, 0);

    void* file = {};
    uint64 fileSize = {};
    loadFile(filePath, &file, &fileSize);

    WavFile wavFile = {};
    wavFile::parse(file, &wavFile);
    //float* sampleChunk = wavFile.sampleChunk;
    uint64 frameCount = wavFile.frameCount;
    uint textureIndex = {};
    //vulkan::createWaveformTexture(vulkan, (float*)sampleChunk, frameCount, &textureIndex);
}
void handleMouseWheel(State* state, WPARAM wParam)
{
    short delta = GET_WHEEL_DELTA_WPARAM(wParam);
    void* vulkan = state->vulkan;
    vulkan::setWaveformWidth(vulkan, delta);
    vulkan::draw(vulkan);
}
void handleHorizontalScroll(State* state, WPARAM wParam)
{
    short delta = GET_WHEEL_DELTA_WPARAM(wParam);
    void* vulkan = state->vulkan;
    vulkan::setWaveformStartOffset(vulkan, delta);
    vulkan::draw(vulkan);
}
LRESULT CALLBACK windowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    State* state = (State*)GetProp(window, L"state");
    switch(message)
    {
        case WM_SIZING:
        {
            handleSizing(state, lParam);
            break;
        }
        case WM_MOUSEMOVE:
        {
            handleMouseMove(state, lParam);
            break;
        }
        case WM_MOUSEWHEEL:
        {
            handleMouseWheel(state, wParam);
            break;
        }
        case WM_MOUSEHWHEEL:
        {
            handleHorizontalScroll(state, wParam);
            break;
        }
        case WM_DROPFILES:
        {
            handleFileDrop(state, wParam);
            break;
        }
        case WM_NCCALCSIZE:
		{
			return 0;
		}
		case WM_NCACTIVATE:
		{
			lParam = -1;
            return 0;
		}
		case WM_NCPAINT:
		case WM_SETICON:
		case WM_SETTEXT:
		case WM_NCUAHDRAWCAPTION:
		case WM_NCUAHDRAWFRAME:
		{
			return 0;
		}
		case WM_NCHITTEST:
		{
			return handleHitTesting(window, lParam);
		}
    }
    return defaultWindowCallback(window, message, wParam, lParam);
}
END_SCOPE
