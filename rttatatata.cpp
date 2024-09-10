#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <vector>

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

void handle_dialogue(const vector<Dialogue>& dialogues) {

    for (const auto& dialogue : dialogues) {
        character_say(dialogue.character_name, dialogue.message);

        awaiting_input = true;
        while (awaiting_input) {
            if (_kbhit()) {
                ch = _getch();
                if (ch == ' ') {
                    awaiting_input = false;
                }
            }
            Sleep(50); 
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
        if (_kbhit()) {
            ch = _getch();
            for (const auto& choice : menu.choices) {
                if (ch == choice.submit_key) {
                    awaiting_input = false;
                    cout << "Wybrałeś: " << choice.option_description << endl;
                    return;
                }
            }
        }
        Sleep(50);
    }
}

void lore_not_right_fit() {
    vector<Dialogue> dialogues = {
        {"SOWA", "Cóż, niestety chyba nie jesteś prawidłowym typem osoby potrzebnej do wykonania tego zadania"},
        {"BOB", "Też mi sie tak wydaję, to nie jest miejsce dla ciebie"},
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
    lore_say("Bob wskazuje na drzwi, które prowadzą do pomiesczenia. Wchodzisz do środka i widzisz elektroniczny zamkek na ścianie...");


    vector<Dialogue> dialogues_2 = {
        {"BOB", "To jest elektroniczny zamek!"},
        {"BOB", "Potrzebujesz odpowiedzieć bezbłędnie na 5 pytań aby przejść dalej!"},
        {"BOB", "Gotowy?"},
    };

    handle_dialogue(dialogues_2);
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
        {"SOWA", "Ale najpierw musisz dokonać wyboru. Musimy dowiedzieć się czy jesteś dobrym typen na tą przygodę!"},
        {"SOWA", "Zanim jednak to sie stanie zawołam mojego przyjaciela boba. On pomoże nam w tej decyzji."}
    };

    handle_dialogue(initial_dialogue);

    ChoiceMenu main_menu = {
        "Możesz zadać sowie pytanie. Wybierz mądrze",
        {
            {"Po co mi to?", '1'},
            {"Co z tego będe miał?", '2'},
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
    tip_say("Czy wiesz, że możesz przejść do kolejnego dialogu klikając spację? Spróbuj teraz!");
    gameLoop();

    return 0;
}
