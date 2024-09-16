#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <cctype>
#include <cstdlib>

// Function for platform-independent sleep
void sleep_for_milliseconds(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

// Function to get a single character input (cross-platform)
char getch() {
    char ch;
    std::cin.get(ch);
    return ch;
}

// Function to check if a key was pressed (cross-platform)
bool kbhit() {
    // For cross-platform compatibility, we might use POSIX functions or a third-party library.
    // For simplicity, we use `cin` and assume a key press is always available.
    return std::cin.peek() != EOF;
}

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
    char submit_key; // The key to press for this option
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
            sleep_for_milliseconds(50);
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
        sleep_for_milliseconds(50);
    }
}

void lore_not_right_fit() {
    vector<Dialogue> dialogues = {
        {"SOWA", "Cóż, niestety chyba nie jesteś prawidłowym typem osoby potrzebnej do wykonania tego zadania"},
        {"BOB", "Też mi się tak wydaję, to nie jest miejsce dla ciebie"},
        {"SOWA", "Żegnaj podróżniku!"},
    };
    handle_dialogue(dialogues);
};

void lore_greedy() {
    vector<Dialogue> dialogues = {
        {"SOWA", "Nie wiemy czy będzie z tego zysk ale z pewnością będzie to dla ciebie wielka przygoda!"},
        {"BOB", "Ja nie lubie jednak chciwych ludzi..."},
        {"SOWA", "Tak masz rację, od razu chodzi tylko o zysk..."},
        {"BOB", "Ale dajmy mu szansę, może jednak się zmieni"},
        {"SOWA", "Nie sądze aby był to dobry pomysł..."},
        {"BOB", "Masz rację, lepiej niech idzie"},
        {"SOWA", "Żegnaj podróżniku!"},
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
            sleep_for_milliseconds(50);
        }
    }

    return (correct_answers == questions_amt);
};

void lore_introduction() {
    vector<Dialogue> dialogues = {
        {"BOB", "Cześć! Jestem bob!"},
        {"BOB", "Witaj w tajemniczej krainie!"},
        {"BOB", "Mam nadzieje ze razem uda nam się przejść przez tą przygodę!"},
        {"BOB", "*Wskazuje na drzwi*"},
        {"BOB", "To jest nasz pierwszy przystanek!"},
        {"BOB", "Zobaczmy co nas tam czeka!"},
    };

    handle_dialogue(dialogues);
    lore_say("Bob wchodzi do pomieszczenia. Idziesz za nim i widzisz elektroniczny zamek na ścianie...");

    vector<Dialogue> dialogues_2 = {
        {"BOB", "To jest elektroniczny zamek!"},
        {"BOB", "Potrzebujesz odpowiedzieć bezbłędnie na 5 pytań aby przejść dalej!"},
        {"BOB", "Gotowy?"},
    };

    handle_dialogue(dialogues_2);

    if (handle_first_door_questions()) {
        lore_say("Zamek otwiera się! Bob i ty przechodzicie do kolejnego pomieszczenia...");
    } else {
        lore_say("Zamek nie otwiera się! Bob i ty musicie wrócić do początku...");
        exit(0);
    }
}

void lore_branch_3() {
    vector<Dialogue> dialogues = {
        {"SOWA", "Hmmm *Zastanawia się*"},
        {"BOB", "Nie wydaje mi się aby to było bezpieczne"},
        {"SOWA", "Jednak jeśli chcesz spróbować to nie powstrzymam cię"},
        {"BOB", "Ale ostrzegam, to nie jest bezpieczne"},
        {"SOWA", "Teraz musisz podjąć decyzję, czy chcesz spróbować?"},
        {"BOB", "Czy może jednak lepiej odpuścić?"},
    };
    handle_dialogue(dialogues);

    ChoiceMenu branch_3_menu = {
        "Co chcesz zrobić?",
        {
            {"Spróbuję!", '1'},
            {"Lepiej nie", '2'}
        }
    };

    handle_choice_menu(branch_3_menu);

    switch (ch) {
        case '1':
            character_say("SOWA", "No to powodzenia! Mam nadzieję, że ci się uda! Teraz wysłuchaj mojego przyjaciela boba, on cię pokieruje ja jestem już bardzo zmęczona");
            lore_introduction();
            break;
        case '2':
            character_say("SOWA", "Rozsądna decyzja! Żegnaj podróżniku!");
            break;
        default:
            cout << "Nieznany wybór!" << endl;
    }

    game_running = false;
};

void gameLoop() {
    vector<Dialogue> initial_dialogue = {
        {"SOWA", "Witaj podróżniku! Co cię tutaj sprowadza?"},
        {"SOWA", "Razem ze mną będziesz przemierzał tajemniczą krainę..."},
        {"SOWA", "Ale najpierw musisz dokonać wyboru. Musimy dowiedzieć się czy jesteś dobrym typem na tę przygodę!"},
        {"SOWA", "Zanim jednak to się stanie, zawołam mojego przyjaciela Boba. On pomoże nam w tej decyzji."}
    };

    handle_dialogue(initial_dialogue);

    ChoiceMenu main_menu = {
        "Możesz zadać sowie pytanie. Wybierz mądrze:",
        {
            {"Po co mi to?", '1'},
            {"Co z tego będę miał?", '2'},
            {"Czy to jest bezpieczne?", '3'}
        }
    };

    handle_choice_menu(main_menu);

    switch (ch) {
        case '1':
            lore_not_right_fit();
            break;
        case '2':
            lore_greedy();
            break;
        case '3':
            lore_branch_3();
            break;
        default:
            cout << "Nieznany wybór!" << endl;
    }

    game_running = false;
}

int main() {
    player.health = 20;
    cout << "Podaj swój nick: ";
    cin >> player.player_name;
    cout << "Podales: " << player.player_name << endl;

    tip_say("Czy wiesz, że możesz przejść do kolejnego dialogu klikając spację? Spróbuj teraz!");
    gameLoop();

    return 0;
}