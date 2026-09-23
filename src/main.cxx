#include "caquicocu.h"

#include <iostream>
#include <string>
#include <regex>

#include <cstdlib>
#include <cstring>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

int analyz_args(int argc, char** argv, int* n);
int analyz_opt(char* arg);

int ask_nquestions(int n, int* point, std::string* message);
int ask_question(int* success);

std::string get_message(int point, int n);

Verbe gen_verbe(void);
Temps gen_temps(void);
Pronom gen_pronom(void);

void help();
void guide();
void version();

int main(int argc, char** argv)
{
    int n, point, ret;
    std::string message;

    if (ret = analyz_args(argc, argv, &n))
        return ret;

    cout << "Asking " << n << " questions\n";

    guide();
    cout << endl;

    if (ret = ask_nquestions(n, &point, &message)) {
        cerr << message << endl;
        return ret;
    }

    cout << "result: " << point << "/" << n << "\n" << message << endl;

    return 0;
}

int analyz_args(int argc, char** argv, int* n)
{
    int tmp, i;
    char* end;

    if (argc < 2) {
        *n = DEFAULT_N;
        return 0;
    }


    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (analyz_opt(argv[i]))
                return 1;
        }

        tmp = (int) std::strtol(argv[1], &end, 10);

        if (argv[1] == end || *end != '\0') {
            cerr << "caquicocu: The first argument must be a number.\n"
                "Use --help for more information." << endl;
            return 2;
        }

        *n = tmp;
    }
    return 0;
}

int analyz_opt(char* arg)
{
    if (std::strcmp(arg, "--help") == 0) {
        help();
        std::exit(0);
    } else if (std::strcmp(arg, "--version") == 0) {
        version();
        std::exit(0);
    } else {
        cerr << "caquicocu: Unknown opt: " << arg << endl;
        return 1;
    }
}

int ask_nquestions(int n, int* point, std::string* message)
{
    int i, ret = 0, success = 0;
    double ratio;

    *point = 0;
    *message = "";

    std::srand(std::time(NULL));

    for (i = 0; i < n; i++) {
        ret = ask_question(&success);
        if (ret)
            return ret;
        *point += success;
        cout << endl;
    }

    *message = get_message(*point, n);

    return 0;
}

int ask_question(int* success)
{
    std::string in, ans;
    std::regex re;

    Verbe verb = Verbe::null;
    Temps tense = Temps::null;
    Pronom pronoun = Pronom::null;

    do {
        verb = gen_verbe();
        tense = gen_temps();
        pronoun = gen_pronom();
    } while (get_regex(verb, tense, pronoun, &re));

    cout << enum2str(tense)   << " "
         << enum2str(verb)    << " "
         << enum2str(pronoun) << ":" << endl;

    std::getline(cin, in);

    if (std::regex_match(in, re)) {
        cout << "Correct: "
                "\033[32m"
             << get_answer(verb, tense, pronoun)
             << "\033[0m" << endl;
        *success = 1;
    } else {
        cout << "Wrong: "
                "\033[31m"
             << get_answer(verb, tense, pronoun)
             << "\033[0m" << endl;
        *success = 0;
    }

    return 0;
}


std::string get_message(int point, int n)
{
    double ratio = (double)point / (double)n;

    if (point == n)
        return "Perfect! Good job!";
    else if (!point)
        return "You suck!";
    else if (ratio >= 0.8)
        return "Super!";
    else if (ratio >= 0.6)
        return "Not bad";
    else if (ratio >= 0.4)
        return "You can do better";
    else if (ratio >= 0.2)
        return "Did you practice ?";
    else
        return "Oh my gosh !";
}

Verbe gen_verbe(void)
{
    return (Verbe) (std::rand() % ((int)Verbe::count));
}

Temps gen_temps(void)
{
    return (Temps) (std::rand() % ((int)Temps::count));
}

Pronom gen_pronom(void)
{
    return (Pronom) (std::rand() % ((int)Pronom::count));
}

void help()
{
    cout << USAGE << "\n"
        "Give French conjugation exercises n times.\n";

    guide();
}

void guide()
{
    cout <<
        "Input the answer without the Pronoun in lowercase.\n"
        "You can input é as e', ç as c', ê as e^ and è as e` (and other)\n"
        "If the auxiliary is être, please input in masculine singular."
        << endl;
}

void version()
{
    cout << "caquicocu " << VERSION << "\n"
            "\nWritten by Yutaka Goy\n"
            "in C++, on " << DATE << endl;
}
