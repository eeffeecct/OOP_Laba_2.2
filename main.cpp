#include <windows.h>
#include <tchar.h> // for text
#include <vector>

struct Shape {
    enum Type { CIRCLE, RECTANGLE } type;
    RECT rect; // winapi struct, rectangle

    Shape(Type t, int left, int top, int right, int bottom) : type(t), rect{left, top, right, bottom} {}
};

std::vector<Shape> shapes;

// обработчик сообщений
// дескрипитор окна, код события, доп данные для события, доп данные
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: { // создание окна
        shapes.emplace_back(Shape::CIRCLE, 50, 50, 150, 150);
        shapes.emplace_back(Shape::RECTANGLE, 200, 50, 300, 150);
        break;
    }
    case WM_PAINT: { // перерисовка окна
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps); // процесс рисования

        for (std::vector<Shape>::iterator it = shapes.begin(); it != shapes.end(); it++) {
            if (it->type == Shape::CIRCLE) {
                Ellipse(hdc, it->rect.left, it->rect.top, it->rect.right, it->rect.bottom); // окр вписанная в прямоугольник
            } else if (it->type == Shape::RECTANGLE) {
                Rectangle(hdc, it->rect.left, it->rect.top, it->rect.right, it->rect.bottom);
            }
        }

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY: // закрытие окна
        PostQuitMessage(0);
        break;
    default: // стандартный обработчик windows
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// дескриптор экземпляра приложения, nullptr(не используется), аргументы ком строки, параметры окна
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {}; // структура описывающая окно
    wc.lpfnWndProc = WndProc; // указ функцию обработки сообщений
    wc.hInstance = hInstance; // передача дескриптора программы
    wc.lpszClassName = _T("Shapes");

    RegisterClass(&wc); // регистрация окна в системе

    HWND hWnd = CreateWindow(wc.lpszClassName, _T("Shapes"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) return 0;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg); // отправляет сообщения в wndProc()
    }

    return static_cast<int>(msg.wParam);
}
