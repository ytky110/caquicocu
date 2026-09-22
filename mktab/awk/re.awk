BEGIN {
    ended = 1
}

{
    if (ended) {
        print "{"
        ended = 0
    } else if ($0 == "") {
        print "},"
        ended = 1
        next
    }

    gsub(/é/, "(e'|é)")
    gsub(/à/, "(a`|à)")
    gsub(/è/, "(e`|è)")
    gsub(/â/, "(a^|â)")
    gsub(/ê/, "(e^|ê)")
    gsub(/î/, "(i^|î)")
    gsub(/ô/, "(o^|ô)")
    gsub(/û/, "(u^|û)")
    gsub(/ç/, "(c,|ç)")
    gsub(/ /, "\\\\s*")

    print "    RE(\"\\\\s*" $0 "\\\\s*\"),"
}

END {
    print "},"
}
