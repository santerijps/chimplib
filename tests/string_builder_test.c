#include "../chimp/testing.h"
#include "../chimp/strings/String_Builder.h"

int test_create(void) {
    char buffer[3] = {0};
    String_Builder builder = string_builder_create(buffer, 3);
    assert_equal(builder.buffer, buffer);
    assert_equal(builder.capacity, 3);
    assert_equal(builder.length, 0);
    return 0;
}

int test_write_byte(void) {
    char buffer[3] = {0};
    String_Builder builder = string_builder_create(buffer, sizeof(buffer));
    assert_equal(string_builder_write_byte(&builder, 'A'), 0);
    assert_equal(string_builder_write_byte(&builder, 'B'), 0);
    assert_equal(string_builder_write_byte(&builder, 'C'), 1);
    assert_equal_string(builder.buffer, "AB");
    return 0;
}

int test_write_bytes(void) {
    char buffer[4] = {0};
    String_Builder builder = string_builder_create(buffer, sizeof(buffer));
    assert_equal(string_builder_write_bytes(&builder, "AB", 2), 0);
    assert_equal(string_builder_write_bytes(&builder, "C", 1), 0);
    assert_equal(string_builder_write_bytes(&builder, "DEF", 3), 1);
    return 0;
}

int test_write_string(void) {
    char buffer[4] = {0};
    String_Builder builder = string_builder_create(buffer, sizeof(buffer));
    assert_equal(string_builder_write_string(&builder, "AB"), 0);
    assert_equal(string_builder_write_string(&builder, "C"), 0);
    assert_equal(string_builder_write_string(&builder, "DEF"), 1);
    return 0;
}

int main(void) {
    int failures = (
        + test_create()
        + test_write_byte()
        + test_write_bytes()
        + test_write_string()
    );
    fprintf(
        stderr,
        __FILE__ " %sFailed tests: %d\n\033[0m",
        failures ? "\033[31m" : "\033[32m", failures
    );
    return 0;
}
