#include <iostream>
#include <string>
#include <regex>

#include <cstdlib>
#include <cstring>

#define VERSION "1.1.0"
#define DATE "2026-05-02"
#define USAGE "usage: caquicocu [n]"

#define DEFAULT_N (5)

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

enum class Verbe {
    etre,
    avoir,
    chanter,

    count,
    null
};

enum class Temps {
    present,
    imparfait,
    futur_simple,
    passe_simple,

    passe_compose,
    plus_que_parfait,
    futur_anterieur,
    passe_anterieur,
    /*
    conditionnel_present,
    conditionnel_passe,
    sub_present,
    sub_passe,
    */

    count,
    null
};

enum class Pronom {
    je,
    tu,
    il,
    nous,
    vous,
    ils,

    count,
    null
};

int analyz_args(int argc, char** argv, int* n);
int analyz_opt(char* arg);

int ask_nquestions(int n, int* point, std::string* message);
int ask_question(int* success);

int get_regex(Verbe verb, Temps tense, Pronom pronoun, std::regex* re);
std::string get_answer(Verbe verb, Temps tense, Pronom pronoun);
std::string get_message(int point, int n);

Verbe gen_verbe(void);
Temps gen_temps(void);
Pronom gen_pronom(void);

std::string enum2str(Verbe v);
std::string enum2str(Temps t);
std::string enum2str(Pronom p);

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

int get_regex(Verbe verb, Temps tense, Pronom pronoun, std::regex* re)
{
    using RE = std::regex;
    static const std::regex
        table[(const int) Verbe::count]
             [(const int) Temps::count]
             [(const int) Pronom::count] = {
        // etre
        {
            // present
            {
                RE("\\s*suis\\s*"),
                RE("\\s*es\\s*"),
                RE("\\s*est"),
                RE("\\s*sommes"),
                RE("\\s*(e\\^|ê)tes"),
                RE("\\s*sont")
            },
            // imparfait
            {
                RE("\\s*(e'|é)tais\\s*"),
                RE("\\s*(e'|é)tais\\s*"),
                RE("\\s*(e'|é)tait\\s*"),
                RE("\\s*(e'|é)tions\\s*"),
                RE("\\s*(e'|é)tiez\\s*"),
                RE("\\s*(e'|é)taient\\s*")
            },
            // futur_simple
            {
                RE("\\s*serai\\s*"),
                RE("\\s*seras\\s*"),
                RE("\\s*sera\\s*"),
                RE("\\s*serons\\s*"),
                RE("\\s*serez\\s*"),
                RE("\\s*seront\\s*")
            },
            // passe_simple
            {
                RE("\\s*fus\\s*"),
                RE("\\s*fus\\s*"),
                RE("\\s*fut\\s*"),
                RE("\\s*f(u^|û)mes\\s*"),
                RE("\\s*f(u^|û)tes\\s*"),
                RE("\\s*furent\\s*")
            },
            // passe_compose
            {
                RE("\\s*ai (e'|é)t(e'|é)\\s*"),
                RE("\\s*as (e'|é)t(e'|é)\\s*"),
                RE("\\s*a (e'|é)t(e'|é)\\s*"),
                RE("\\s*avons (e'|é)t(e'|é)\\s*"),
                RE("\\s*avez (e'|é)t(e'|é)\\s*"),
                RE("\\s*ont (e'|é)t(e'|é)\\s*")
            },
            // plus_que_parfait
            {
                RE("\\s*avais (e'|é)t(e'|é)\\s*"),
                RE("\\s*avais (e'|é)t(e'|é)\\s*"),
                RE("\\s*avait (e'|é)t(e'|é)\\s*"),
                RE("\\s*avions (e'|é)t(e'|é)\\s*"),
                RE("\\s*aviez (e'|é)t(e'|é)\\s*"),
                RE("\\s*avaient (e'|é)t(e'|é)\\s*")
            },
            // futur_anterieur
            {
                RE("\\s*aurai (e'|é)t(e'|é)\\s*"),
                RE("\\s*auras (e'|é)t(e'|é)\\s*"),
                RE("\\s*aura (e'|é)t(e'|é)\\s*"),
                RE("\\s*aurons (e'|é)t(e'|é)\\s*"),
                RE("\\s*aurez (e'|é)t(e'|é)\\s*"),
                RE("\\s*auront (e'|é)t(e'|é)\\s*")
            },
            // passe_anterieur
            {
                RE("\\s*eus (e'|é)t(e'|é)\\s*"),
                RE("\\s*eus (e'|é)t(e'|é)\\s*"),
                RE("\\s*eut (e'|é)t(e'|é)\\s*"),
                RE("\\s*e(u\\^|û)mes (e'|é)t(e'|é)\\s*"),
                RE("\\s*e(u\\^|û)tes (e'|é)t(e'|é)\\s*"),
                RE("\\s*eurent (e'|é)t(e'|é)\\s*")
            },
        },
        // avoir
        {
            // present
            {
                RE("\\s*ai\\s*"),
                RE("\\s*as\\s*"),
                RE("\\s*a\\s*"),
                RE("\\s*avons\\s*"),
                RE("\\s*avez\\s*"),
                RE("\\s*ont\\s*")
            },
            // imparfait
            {
                RE("\\s*avais\\s*"),
                RE("\\s*avais\\s*"),
                RE("\\s*avait\\s*"),
                RE("\\s*avions\\s*"),
                RE("\\s*aviez\\s*"),
                RE("\\s*avaient\\s*")
            },
            // futur_simple
            {
                RE("\\s*aurai\\s*"),
                RE("\\s*auras\\s*"),
                RE("\\s*aura\\s*"),
                RE("\\s*aurons\\s*"),
                RE("\\s*aurez\\s*"),
                RE("\\s*auront\\s*")
            },
            // passe_simple
            {
                RE("\\s*eus\\s*"),
                RE("\\s*eus\\s*"),
                RE("\\s*eut\\s*"),
                RE("\\s*e(u\\^|û)mes\\s*"),
                RE("\\s*e(u\\^|û)tes\\s*"),
                RE("\\s*eurent\\s*")
            },
            // passe_compose
            {
                RE("\\s*ai eu\\s*"),
                RE("\\s*as eu\\s*"),
                RE("\\s*a eu\\s*"),
                RE("\\s*avons eu\\s*"),
                RE("\\s*avez eu\\s*"),
                RE("\\s*ont eu\\s*")
            },
            // plus_que_parfait
            {
                RE("\\s*avais eu\\s*"),
                RE("\\s*avais eu\\s*"),
                RE("\\s*avait eu\\s*"),
                RE("\\s*avions eu\\s*"),
                RE("\\s*aviez eu\\s*"),
                RE("\\s*avaient eu\\s*")
            },
            // futur_anterieur
            {
                RE("\\s*aurai eu\\s*"),
                RE("\\s*auras eu\\s*"),
                RE("\\s*aura eu\\s*"),
                RE("\\s*aurons eu\\s*"),
                RE("\\s*aurez eu\\s*"),
                RE("\\s*auront eu\\s*")
            },
            // passe_anterieur
            {
                RE("\\s*eus eu\\s*"),
                RE("\\s*eus eu\\s*"),
                RE("\\s*eut eu\\s*"),
                RE("\\s*e(u\\^|û)mes eu\\s*"),
                RE("\\s*e(u\\^|û)tes eu\\s*"),
                RE("\\s*eurent eu\\s*")
            },
        },
        // chanter 
        {
            // present
            {
                RE("\\s*chante\\s*"),
                RE("\\s*chantes\\s*"),
                RE("\\s*chante\\s*"),
                RE("\\s*chantons\\s*"),
                RE("\\s*chantez\\s*"),
                RE("\\s*chantenet\\s*")
            },
            // imparfait
            {
                RE("\\s*chantais\\s*"),
                RE("\\s*chantais\\s*"),
                RE("\\s*chantait\\s*"),
                RE("\\s*chantions\\s*"),
                RE("\\s*chantiez\\s*"),
                RE("\\s*chantaient\\s*")
            },
            // futur_simple
            {
                RE("\\s*chanterai\\s*"),
                RE("\\s*chanteras\\s*"),
                RE("\\s*chantera\\s*"),
                RE("\\s*chanterons\\s*"),
                RE("\\s*chanterez\\s*"),
                RE("\\s*chanteront\\s*")
            },
            // passe_simple
            {
                RE("\\s*chantai\\s*"),
                RE("\\s*chantas\\s*"),
                RE("\\s*chanta\\s*"),
                RE("\\s*chant(a\\^|â)mes\\s*"),
                RE("\\s*chant(a\\^|â)tes\\s*"),
                RE("\\s*chant(e`|è)rent\\s*")
            },
            // passe_compose
            {
                RE("\\s*ai chant(e'|é)\\s*"),
                RE("\\s*as chant(e'|é)\\s*"),
                RE("\\s*a chant(e'|é)\\s*"),
                RE("\\s*avons chant(e'|é)\\s*"),
                RE("\\s*avez chant(e'|é)\\s*"),
                RE("\\s*ont chant(e'|é)\\s*")
            },
            // plus_que_parfait
            {
                RE("\\s*avais chant(e'|é)\\s*"),
                RE("\\s*avais chant(e'|é)\\s*"),
                RE("\\s*avait chant(e'|é)\\s*"),
                RE("\\s*avions chant(e'|é)\\s*"),
                RE("\\s*aviez chant(e'|é)\\s*"),
                RE("\\s*avaient chant(e'|é)\\s*")
            },
            // futur_anterieur
            {
                RE("\\s*aurai chant(e'|é)\\s*"),
                RE("\\s*auras chant(e'|é)\\s*"),
                RE("\\s*aura chant(e'|é)\\s*"),
                RE("\\s*aurons chant(e'|é)\\s*"),
                RE("\\s*aurez chant(e'|é)\\s*"),
                RE("\\s*auront chant(e'|é)\\s*")
            },
            // passe_anterieur
            {
                RE("\\s*eus chant(e'|é)\\s*"),
                RE("\\s*eus chant(e'|é)\\s*"),
                RE("\\s*eut chant(e'|é)\\s*"),
                RE("\\s*e(u\\^|û)mes chant(e'|é)\\s*"),
                RE("\\s*e(u\\^|û)tes chant(e'|é)\\s*"),
                RE("\\s*eurent chant(e'|é)\\s*")
            },
        },
    };

    if (verb == Verbe::null || verb == Verbe::count
    || tense == Temps::null || tense == Temps::count
    || pronoun == Pronom::null || pronoun == Pronom::count)
        return 1; 

    *re = table[(int)verb][(int)tense][(int)pronoun];
    return 0;
}

std::string get_answer(Verbe verb, Temps tense, Pronom pronoun)
{
    static const std::string
        table[(const int) Verbe::count]
             [(const int) Temps::count]
             [(const int) Pronom::count] = {
        // etre
        {
            // present
            {
                "suis",
                "es",
                "est",
                "sommes",
                "êtes",
                "sont"
            },
            // imparfait
            {
                "étais",
                "étais",
                "était",
                "étions",
                "étiez",
                "étaient"
            },
            // futur_simple
            {
                "serai",
                "seras",
                "sera",
                "serons",
                "serez",
                "seront"
            },
            // passe_simple
            {
                "fus",
                "fus",
                "fut",
                "fûmes",
                "fûtes",
                "furent"
            },
            // passe_compose
            {
                "ai été",
                "as été",
                "a été",
                "avons été",
                "avez été",
                "ont été"
            },
            // plus_que_parfait
            {
                "avais été",
                "avais été",
                "avait été",
                "avions été",
                "aviez été",
                "avaient été"
            },
            // futur_anterieur
            {
                "aurai été",
                "auras été",
                "aura été",
                "aurons été",
                "aurez été",
                "auront été"
            },
            // passe_anterieur
            {
                "eus été",
                "eus été",
                "eut été",
                "eûmes été",
                "eûtes été",
                "eurent été"
            },
        },
        // avoir
        {
            // present
            {
                "ai",
                "as",
                "a",
                "avons",
                "avez",
                "ont"
            },
            // imparfait
            {
                "avais",
                "avais",
                "avait",
                "avions",
                "aviez",
                "avaient"
            },
            // futur_simple
            {
                "aurai",
                "auras",
                "aura",
                "aurons",
                "aurez",
                "auront"
            },
            // passe_simple
            {
                "eus",
                "eus",
                "eut",
                "eûmes",
                "eûtes",
                "eurent"
            },
            // passe_compose
            {
                "ai eu",
                "as eu",
                "a eu",
                "avons eu",
                "avez eu",
                "ont eu"
            },
            // plus_que_parfait
            {
                "avais eu",
                "avais eu",
                "avait eu",
                "avions eu",
                "aviez eu",
                "avaient eu"
            },
            // futur_anterieur
            {
                "aurai eu",
                "auras eu",
                "aura eu",
                "aurons eu",
                "aurez eu",
                "auront eu"
            },
            // passe_anterieur
            {
                "eus eu",
                "eus eu",
                "eut eu",
                "eûmes eu",
                "eûtes eu",
                "eurent eu"
            },
        },
        // chanter 
        {
            // present
            {
                "chante",
                "chantes",
                "chante",
                "chantons",
                "chantez",
                "chantenet"
            },
            // imparfait
            {
                "chantais",
                "chantais",
                "chantait",
                "chantions",
                "chantiez",
                "chantaient"
            },
            // futur_simple
            {
                "chanterai",
                "chanteras",
                "chantera",
                "chanterons",
                "chanterez",
                "chanteront"
            },
            // passe_simple
            {
                "chantai",
                "chantas",
                "chanta",
                "chantâmes",
                "chantâtes",
                "chantèrent"
            },
            // passe_compose
            {
                "ai chanté",
                "as chanté",
                "a chanté",
                "avons chanté",
                "avez chanté",
                "ont chanté"
            },
            // plus_que_parfait
            {
                "avais chanté",
                "avais chanté",
                "avait chanté",
                "avions chanté",
                "aviez chanté",
                "avaient chanté"
            },
            // futur_anterieur
            {
                "aurai chanté",
                "auras chanté",
                "aura chanté",
                "aurons chanté",
                "aurez chanté",
                "auront chanté"
            },
            // passe_anterieur
            {
                "eus chanté",
                "eus chanté",
                "eut chanté",
                "eûmes chanté",
                "eûtes chanté",
                "eurent chanté"
            },
        },
    };

    if (verb == Verbe::null || verb == Verbe::count
    || tense == Temps::null || tense == Temps::count
    || pronoun == Pronom::null || pronoun == Pronom::count)
        return std::string("");

    return table[(int)verb][(int)tense][(int)pronoun];
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

std::string enum2str(Verbe v)
{
    using V = Verbe;

    switch (v) {
    case V::etre:       return "être";
    case V::avoir:      return "avoir";
    case V::chanter:    return "chanter";
    default:            return "";
    }
}

std::string enum2str(Temps t)
{
    using T = Temps;

    switch (t) {
    case T::present:                return "présent";
    case T::imparfait:              return "imparfait";
    case T::futur_simple:           return "futur simple";
    case T::passe_simple:           return "passé simple";

    case T::passe_compose:          return "passé composé";
    case T::plus_que_parfait:       return "plus-que-parfait";
    case T::futur_anterieur:        return "futur antérieur";
    case T::passe_anterieur:        return "passé antérieur";
    /*

    case T::conditionnel_present:   return "conditionnel présent";
    case T::conditionnel_passe:     return "conditionnel passé";
    case T::sub_present:            return "subjonctif présent";
    case T::sub_passe:              return "subjonctif passé";
    */

    default:                        return "";
    }
}

std::string enum2str(Pronom p)
{
    using P = Pronom;

    switch (p) {
    case P::je:   return "je";
    case P::tu:   return "tu";
    case P::il:   return "il";
    case P::nous: return "nous";
    case P::vous: return "vous";
    case P::ils:  return "ils";
    default:      return "";
    }
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
        "Input the answer without the Pronoun.\n"
        "You can input é as e', ç as c', ê as e^ and è as e` (and other)"
        << endl;
}

void version()
{
    cout << "caquicocu " << VERSION << "\n"
            "\nWritten by Yutaka Goy\n"
            "in C++, on " << DATE << endl;
}
