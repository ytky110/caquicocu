# How To Add VERB

## 1. Write CQCC file

First, write the cqcc file and put into `mktab/cqcc/`.

Cqcc file is the conjugation table of the verb with tenses separated with '\n'
in the order like `enum class Temps`:

```
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
```

(The `sub_present` and `sub_passe` aren't supported)

## 2. Convert into C++ table

Then, use the execute_all.sh from mktab/ and
you will get the table converted into C++ array at `mktab/re.cxx` and `mktab/ans.cxx`.

The `re.cxx` is for `get_regex.cxx`, and `ans.cxx` for `get_answer.cxx`.
Replace the table with the new one.

## 3. Add verb to enum

Finally, you have 2 things to do.

1. Add verb to `enum class Verbe` in `include/caquicocu.h`.
2. Add verb string to `enum2str(Verbe)` in `src/enum2str.cxx`.

If you wrong the order of `Verbe`, it won't work fine.
It has to be always in alphabet order.
