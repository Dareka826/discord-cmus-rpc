TESTS="colors utils discord_stuff cmus_stuff"

TESTS_SUCCEDEED=1
for TEST in ${TESTS}; do
    redo-ifchange "./build/${TEST}.test"

    printf "[TEST]: Running tests for %s\n" "${TEST}" >&2
    "./build/${TEST}.test" >&2 || {
        printf "\033[31m[TEST]: %s failed!\033[0m\n" "${TEST}" >&2
        TESTS_SUCCEDEED=0
    }
done

# External
EXT_TESTS="src/snippets"

TESTS_SUCCEDEED=1
for TEST in ${EXT_TESTS}; do
    printf "[TEST]: Running external tests for %s\n" "${TEST}" >&2

    redo "./${TEST}/test" >&2 || {
        printf "\033[31m[TEST]: External test %s failed!\033[0m\n" "${TEST}" >&2
        TESTS_SUCCEDEED=0
    }
done

[ "${TESTS_SUCCEDEED}" = "1" ] \
    && exit 0 \
    || exit 1
