#ifndef CQCC_H_
#define CQCC_H_

#define VERSION "1.3.0"
#define DATE "2026-09-22"
#define USAGE "usage: caquicocu [n]"

#define DEFAULT_N (5)

#include <regex>
#include <string>

enum class Verbe {
    null = -1,

    avoir,
    chanter,
    etre,
    faire,

    count,
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
    conditionnel_present,
    conditionnel_passe,
    /*
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

int get_regex(Verbe verb, Temps tense, Pronom pronoun, std::regex* re);
std::string get_answer(Verbe verb, Temps tense, Pronom pronoun);

std::string enum2str(Verbe v);
std::string enum2str(Temps t);
std::string enum2str(Pronom p);

#endif /* CQCC_H_ */
