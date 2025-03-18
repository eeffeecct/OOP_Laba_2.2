#include <windows.h>
#include <tchar.h> // for text
#include <vector>

class Shape {
public:
    RECT rect;
    Shape(int left, int top, int right, int bottom) : rect{left, top, right, bottom} {}
    virtual void Draw(HDC hdc) const = 0;
    virtual ~Shape() {}
};

class RectangleShape : public Shape {
public:
    RectangleShape(int left, int top, int right, int bottom) : Shape(left, top, right, bottom) {}
    void Draw(HDC hdc) const override {
        Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    }
};

class EllipseShape : public Shape {
public:
    EllipseShape(int left, int top, int right, int bottom) : Shape(left, top, right, bottom) {}
    void Draw(HDC hdc) const override {
        Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
    }
};

std::vector<Shape*> shapes;

// обработчик сообщений
// дескриптор окна, код события, доп данные для события, доп данные
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: { // создание окна
        shapes.push_back(new EllipseShape(50, 50, 150, 150));
        shapes.push_back(new RectangleShape(200, 50, 300, 150));
        break;
    }
    case WM_PAINT: { // перерисовка окна
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        for (size_t i = 0; i < shapes.size(); ++i) {
            shapes[i]->Draw(hdc);
        }
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        for (size_t i = 0; i < shapes.size(); ++i) {
            delete shapes[i];
        }
        shapes.clear();
        PostQuitMessage(0);
        break;
    default:
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
