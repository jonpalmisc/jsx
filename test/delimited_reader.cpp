#include <jsx/assert.h>
#include <jsx/delimited_reader.hpp>

int main(int argc, char **argv) {
	(void)argc;
	(void)argv;

        auto csv_reader = jsx::DelimitedReader("a,b\nc,d");

        auto field_1 = csv_reader.next_field();
        jsx_assert(field_1 == "a");

        auto field_2 = csv_reader.next_field();
        jsx_assert(field_2 == "b");

        auto field_3 = csv_reader.next_field();
        jsx_assert(field_3 == "c");

        auto field_4 = csv_reader.next_field();
        jsx_assert(field_4 == "d");

        csv_reader.reset();

        auto group_1 = csv_reader.next_group();
        jsx_assert(group_1.size() == 2);
        jsx_assert(group_1[0] == field_1);
        jsx_assert(group_1[1] == field_2);

        auto group_2 = csv_reader.next_group();
        jsx_assert(group_2.size() == 2);
        jsx_assert(group_2[0] == field_3);
        jsx_assert(group_2[1] == field_4);

        auto tsv_reader = jsx::DelimitedReader("a\tb\tc\td", '\t');
	jsx_assert(tsv_reader.next_field() == "a");
	jsx_assert(tsv_reader.next_field() == "b");
	jsx_assert(tsv_reader.next_field() == "c");
	jsx_assert(tsv_reader.next_field() == "d");

	return 0;
}
