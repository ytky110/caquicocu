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

    print "    \"" $0 "\","
}

END {
    print "},"
}
