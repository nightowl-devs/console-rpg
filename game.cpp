#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h> 
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
#endif

using namespace std;

bool game_running = true;
bool awaiting_input = true;
string input_holder;
char ch;

void debug(const string& text) {
    cout << "{DEBUG LOGGER} " << text << endl;
}

void character_say(const string& character_name, const string& message) {
    cout << "[" << character_name << "] " << message << endl;
}

void tip_say(const string& message) {
    cout << "*Psst " << message << "*" << endl;
}

void lore_say(const string& message) {
    cout << "*" << message << "*" << endl;
}

class Player {
public:
    int health;
    string player_name;
};

Player player;

struct Dialogue {
    string character_name;
    string message;
};

struct Choice {
    string option_description;
    char submit_key; 
};

struct ChoiceMenu {
    string choice_title;
    vector<Choice> choices;
};

struct QuestionOption {
    string option_description;
    bool is_correct;
};

struct Question {
    string question;
    vector<QuestionOption> options;
};

struct Door {
    string door_name;
    vector<Question> questions;
    string door_description;
    string door_open_message;
    string door_fail_message;
};

#if defined(_WIN32) || defined(_WIN64)
bool kbhit() {
    return _kbhit();
}

char getch() {
    return _getch();
}

void sleep_for_ms(int ms) {
    Sleep(ms);
}

#else
bool kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }

    return false;
}

char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}

void sleep_for_ms(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}
#endif

void handle_dialogue(const vector<Dialogue>& dialogues) {
    for (const auto& dialogue : dialogues) {
        character_say(dialogue.character_name, dialogue.message);

        awaiting_input = true;
        while (awaiting_input) {
            if (kbhit()) {
                ch = getch();
                if (ch == ' ') {
                    awaiting_input = false;
                }
            }
            sleep_for_ms(50); 
        }
    }
}

void handle_choice_menu(const ChoiceMenu& menu) {
    cout << menu.choice_title << endl;
    
    for (const auto& choice : menu.choices) {
        cout << choice.submit_key << ") " << choice.option_description << endl;
    }

    awaiting_input = true;
    while (awaiting_input) {
        if (kbhit()) {
            ch = getch();
            for (const auto& choice : menu.choices) {
                if (ch == choice.submit_key) {
                    awaiting_input = false;
                    cout << "Wybrałeś: " << choice.option_description << endl;
                    return;
                }
            }
        }
        sleep_for_ms(50);
    }
}

void lore_not_right_fit() {
    vector<Dialogue> dialogues = {
        {"SOWA", "Cóż, niestety chyba nie jesteś prawidłowym typem osoby potrzebnej do wykonania tego zadania"},
        {"BOB", "Też mi się tak wydaje, to nie jest miejsce dla ciebie"},
        {"SOWA", "Żegnaj podróżniku!"}
    };
    handle_dialogue(dialogues);
}

void lore_greedy() {
    vector<Dialogue> dialogues = {
        {"SOWA", "Nie wiemy, czy będzie z tego zysk, ale z pewnością będzie to dla ciebie wielka przygoda!"},
        {"BOB", "Ja nie lubię jednak chciwych ludzi..."},
        {"SOWA", "Tak masz rację, od razu chodzi tylko o zysk..."},
        {"BOB", "Ale dajmy mu szansę, może jednak się zmieni"},
        {"SOWA", "Nie sądzę, aby był to dobry pomysł..."},
        {"BOB", "Masz rację, lepiej niech idzie"},
        {"SOWA", "Żegnaj podróżniku!"}
    };
    handle_dialogue(dialogues);
}

bool handle_first_door_questions() {
    vector<Question> questions = {
        {
            "Co to jest C++?",
            {
                {"Język programowania", true},
                {"Język obcy", false},
                {"Narzędzie graficzne", false},
                {"System operacyjny", false}
            }
        },
        {
            "Co to jest zmienna?",
            {
                {"Miejsce w pamięci do przechowywania danych", true},
                {"Program komputerowy", false},
                {"Rodzaj pętli w programowaniu", false},
                {"Funkcja w systemie operacyjnym", false}
            }
        },
        {
            "Co to jest funkcja?",
            {
                {"Blok kodu wykonujący określone zadanie", true},
                {"Rodzaj zmiennej", false},
                {"Pojedyncza linia kodu", false},
                {"System operacyjny", false}
            }
        },
        {
            "Co to jest pętla?",
            {
                {"Blok kodu, który wykonuje się wielokrotnie", true},
                {"Zmienna w programowaniu", false},
                {"Rodzaj tablicy", false},
                {"Metoda sortowania", false}
            }
        },
        {
            "Co to jest tablica?",
            {
                {"Struktura danych przechowująca wiele wartości tego samego typu", true},
                {"Rodzaj pętli", false},
                {"Funkcja matematyczna", false},
                {"Zmienna o stałej wartości", false}
            }
        }
    };

    int correct_answers = 0;
    int questions_amt = 0;

    for (const auto& question : questions) {
        cout << question.question << endl;
        questions_amt++;
        for (int i = 0; i < question.options.size(); i++) {
            cout << i + 1 << ") " << question.options[i].option_description << endl;
        }

        awaiting_input = true;
        while (awaiting_input) {
            if (kbhit()) {
                ch = getch();
                if (ch >= '1' && ch <= '4') {
                    int selected_option = ch - '0';
                    if (question.options[selected_option - 1].is_correct) {
                        cout << "Dobra odpowiedź!" << endl;
                        correct_answers++;
                    } else {
                        cout << "Zła odpowiedź!" << endl;
                    }
                    awaiting_input = false;
                }
            }
            sleep_for_ms(50);
        }
    }

    bool passed = false;
    if (correct_answers == questions_amt) {
        passed = true;
    }

    return passed;
}

void lore_introduction() {
    vector<Dialogue> dialogues = {
        {"BOB", "Cześć! Jestem Bob!"},
        {"BOB", "Witaj w tajemniczej krainie!"},
        {"BOB", "Mam nadzieję, że razem uda nam się przejść przez tę przygodę!"},
        {"BOB", "*Wskazuje na drzwi*"},
        {"BOB", "To jest nasz pierwszy przystanek!"},
        {"BOB", "Zobaczmy, co nas tam czeka!"}
    };

    handle_dialogue(dialogues);
    lore_say("Bob wchodzi do tajemniczych drzwi, prowadząc cię za sobą...");
}

void gameLoop() {
    lore_introduction();
    //initial choice menu to determine if the player is greedy not right or can proceeed
    vector<ChoiceMenu> initial_choice = { 
        {
            "Czy chcesz przejść przez drzwi?",
            {
                {"Tak", 't'},
                {"Nie", 'n'}
            }
        }
    }; 


    if (handle_first_door_questions()) {
        character_say("BOB", "Dobrze odpowiedziałeś na wszystkie pytania! Możemy przejść dalej!");
    } else {
        character_say("BOB", "Niestety, nie udało się odpowiedzieć na wszystkie pytania.");
        lore_not_right_fit();
    }
}

int main() {
    player.health = 20;
    cout << "Podaj swoj nick: ";
    cin >> player.player_name;
    cout << "Podales: " << player.player_name << endl;

    tip_say("Czy wiesz, że możesz przejść do kolejnego dialogu klikając spację? Spróbuj teraz!");
    gameLoop();

    return 0;
}
