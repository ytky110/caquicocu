#include "caquicocu.h"

std::string enum2str(Verbe v)
{
    using V = Verbe;

    switch (v) {
    case V::aller:      return "aller";
    case V::avoir:      return "avoir";
    case V::chanter:    return "chanter";
    case V::dire:       return "dire";
    case V::etre:       return "être";
    case V::faire:      return "faire";
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
    case T::conditionnel_present:   return "conditionnel présent";
    case T::conditionnel_passe:     return "conditionnel passé";

    /*
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
