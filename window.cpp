#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
using namespace std;

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define CLEAR "\033[2J\033[H"

string fg(int c) { return "\033[38;5;" + to_string(c) + "m"; }
string bg(int c) { return "\033[48;5;" + to_string(c) + "m"; }

struct ColorEntry
{
    string name;
    int ansi;
};
ColorEntry COLORS[] = {
    {"Red", 196}, {"Green", 46}, {"Blue", 21}, {"Yellow", 226}, {"Cyan", 51}, {"Magenta", 201}, {"White", 231}, {"Orange", 208}};
const int NUM_COLORS = 8;

string colorName(int ansi)
{
    for (auto &c : COLORS)
        if (c.ansi == ansi)
            return c.name;
    return "?";
}

struct Window
{
    string title;
    int color, width, height;
    Window *prev, *next;
    Window(string t, int c, int w, int h) : title(t), color(c), width(w), height(h), prev(nullptr), next(nullptr) {}
};

struct WindowManager
{
    Window *head;
    int count;
    WindowManager() : head(nullptr), count(0) {}

    void create(string title, int color, int width = 20, int height = 6)
    {
        Window *w = new Window(title, color, width, height);
        if (!head)
        {
            head = w;
        }
        else
        {
            Window *cur = head;
            while (cur->next)
                cur = cur->next;
            cur->next = w;
            w->prev = cur;
        }
        count++;
    }
    bool activate(int idx)
    {
        Window *w = getAt(idx);
        if (!w || w == head)
            return w != nullptr;
        if (w->prev)
            w->prev->next = w->next;
        if (w->next)
            w->next->prev = w->prev;
        w->prev = nullptr;
        w->next = head;
        if (head)
            head->prev = w;
        head = w;
        return true;
    }
    bool resize(int idx, int nw, int nh)
    {
        Window *w = getAt(idx);
        if (!w)
            return false;
        w->width = nw;
        w->height = nh;
        return true;
    }
    bool remove(int idx)
    {
        Window *w = getAt(idx);
        if (!w)
            return false;
        if (w->prev)
            w->prev->next = w->next;
        else
            head = w->next;
        if (w->next)
            w->next->prev = w->prev;
        delete w;
        count--;
        return true;
    }
    Window *getAt(int idx)
    {
        Window *cur = head;
        for (int i = 1; cur && i < idx; i++)
            cur = cur->next;
        return cur;
    }
    ~WindowManager()
    {
        Window *cur = head;
        while (cur)
        {
            Window *nx = cur->next;
            delete cur;
            cur = nx;
        }
    }
};

void drawWindow(Window *w, bool active, int slot)
{
    string border = active ? fg(226) : fg(244);
    int W = max((int)w->title.size() + 4, w->width);

    cout << border << "+";
    for (int i = 0; i < W; i++)
        cout << (active ? "=" : "-");
    cout << "+" << RESET << "\n";

    cout << border << "|" << RESET << bg(w->color) << fg(16) << BOLD;
    string label = " " + w->title + " ";
    int pad = W - (int)label.size(), lp = pad / 2, rp = pad - lp;
    for (int i = 0; i < lp; i++)
        cout << " ";
    cout << label;
    for (int i = 0; i < rp; i++)
        cout << " ";
    cout << RESET << border << "|" << RESET << "\n";

    cout << border << "+";
    for (int i = 0; i < W; i++)
        cout << (active ? "=" : "-");
    cout << "+" << RESET << "\n";

    for (int r = 0; r < w->height - 3; r++)
    {
        cout << border << "|" << RESET << bg(w->color);
        for (int i = 0; i < W; i++)
            cout << " ";
        cout << RESET << border << "|" << RESET << "\n";
    }

    cout << border << "+";
    for (int i = 0; i < W; i++)
        cout << (active ? "=" : "-");
    cout << "+" << RESET << "\n";

    cout << "  " << fg(244) << "#" << slot << " | " << colorName(w->color)
         << " | " << W << "x" << w->height
         << (active ? "  << ACTIVE" : "") << RESET << "\n\n";
}

void drawDesktop(WindowManager &wm)
{
    cout << CLEAR;
    cout << BOLD << fg(255)
         << "+==========================================+\n"
         << "|      WINDOW MANAGER  --  DSA Ass 2      |\n"
         << "+==========================================+\n"
         << RESET;
    if (!wm.head)
    {
        cout << "\n  " << fg(244) << "(no windows open)\n"
             << RESET;
        return;
    }
    cout << "\n";
    Window *cur = wm.head;
    int slot = 1;
    while (cur)
    {
        drawWindow(cur, slot == 1, slot);
        cur = cur->next;
        slot++;
    }
}

void printMenu()
{
    cout << fg(51) << BOLD
         << "+-----------------------------+\n"
         << "|         MAIN MENU           |\n"
         << "+-----------------------------+\n"
         << "|  1. Create window           |\n"
         << "|  2. Activate window         |\n"
         << "|  3. Resize window           |\n"
         << "|  4. Delete window           |\n"
         << "|  5. Refresh / Show desktop  |\n"
         << "|  0. Exit                    |\n"
         << "+-----------------------------+\n"
         << RESET;
    cout << "Choice: ";
}

void printColorMenu()
{
    cout << fg(226) << "Available colors:\n"
         << RESET;
    for (int i = 0; i < NUM_COLORS; i++)
        cout << "  " << (i + 1) << ". " << bg(COLORS[i].ansi) << fg(16) << " " << COLORS[i].name << " " << RESET << "\n";
    cout << "Pick color (1-" << NUM_COLORS << "): ";
}

int main()
{
    WindowManager wm;
    wm.create("Terminal", 21, 24, 7);
    wm.create("Explorer", 46, 22, 6);
    wm.create("Settings", 208, 20, 6);
    drawDesktop(wm);

    int choice;
    while (true)
    {
        cout << "\n";
        printMenu();
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n');

        if (choice == 0)
        {
            cout << RESET << "Goodbye.\n";
            break;
        }
        else if (choice == 1)
        {
            string title;
            cout << "Window title: ";
            getline(cin, title);
            if (title.empty())
                title = "Window " + to_string(wm.count + 1);
            printColorMenu();
            int ci;
            cin >> ci;
            cin.ignore(1000, '\n');
            if (ci < 1 || ci > NUM_COLORS)
                ci = 1;
            wm.create(title, COLORS[ci - 1].ansi);
            drawDesktop(wm);
            cout << fg(46) << "[OK] Window '" << title << "' created.\n"
                 << RESET;
        }
        else if (choice == 2)
        {
            if (!wm.head)
            {
                cout << fg(196) << "No windows.\n"
                     << RESET;
                continue;
            }
            cout << "Activate window # (1-" << wm.count << "): ";
            int idx;
            cin >> idx;
            cin.ignore(1000, '\n');
            if (wm.activate(idx))
            {
                drawDesktop(wm);
                cout << fg(46) << "[OK] Window #" << idx << " is now active.\n"
                     << RESET;
            }
            else
                cout << fg(196) << "Invalid index.\n"
                     << RESET;
        }
        else if (choice == 3)
        {
            if (!wm.head)
            {
                cout << fg(196) << "No windows.\n"
                     << RESET;
                continue;
            }
            cout << "Resize window # (1-" << wm.count << "): ";
            int idx;
            cin >> idx;
            cin.ignore(1000, '\n');
            int w, h;
            cout << "New width  (min 10): ";
            cin >> w;
            cin.ignore(1000, '\n');
            cout << "New height (min 4) : ";
            cin >> h;
            cin.ignore(1000, '\n');
            w = max(w, 10);
            h = max(h, 4);
            if (wm.resize(idx, w, h))
            {
                drawDesktop(wm);
                cout << fg(46) << "[OK] Resized to " << w << "x" << h << ".\n"
                     << RESET;
            }
            else
                cout << fg(196) << "Invalid index.\n"
                     << RESET;
        }
        else if (choice == 4)
        {
            if (!wm.head)
            {
                cout << fg(196) << "No windows.\n"
                     << RESET;
                continue;
            }
            cout << "Delete window # (1-" << wm.count << "): ";
            int idx;
            cin >> idx;
            cin.ignore(1000, '\n');
            if (wm.remove(idx))
            {
                drawDesktop(wm);
                cout << fg(46) << "[OK] Window #" << idx << " deleted.\n"
                     << RESET;
            }
            else
                cout << fg(196) << "Invalid index.\n"
                     << RESET;
        }
        else if (choice == 5)
        {
            drawDesktop(wm);
        }
        else
        {
            cout << fg(196) << "Invalid choice.\n"
                 << RESET;
        }
    }
    return 0;
}