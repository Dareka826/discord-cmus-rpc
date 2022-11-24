TESTS="colors utils discord_stuff cmus_stuff"

TESTS_SUCCEDEED=1
for TEST in ${TESTS}; do
    redo-ifchange "./build/${TEST}.test"

    printf "\033[33m[TEST]:\033[0m Running tests for %s\n" "${TEST}" >&2
    if "./build/${TEST}.test" >&2; then
        printf "\033[32m[TEST]:\033[0m %s passed.\n" "${TEST}" >&2
    else
        printf "\033[31m[TEST]: %s failed!\033[0m\n" "${TEST}" >&2
        TESTS_SUCCEDEED=0
    fi
done

# External
EXT_TESTS="src/snippets"

TESTS_SUCCEDEED=1
for TEST in ${EXT_TESTS}; do
    printf "\033[34m[TEST]:\033[0m Running external tests for %s\n" "${TEST}" >&2

    redo "./${TEST}/test" >&2 || {
        printf "\033[31m[TEST]: External test %s failed!\033[0m\n" "${TEST}" >&2
        TESTS_SUCCEDEED=0
    }
done

[ "${TESTS_SUCCEDEED}" = "1" ] \
    && exit 0 \
    || exit 1
