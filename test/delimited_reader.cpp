#include <jsx/assert.h>
#include <jsx/delimited_reader.hpp>

int main(int argc, char **argv) {
	(void)argc;
	(void)argv;

	auto csv_reader = jsx::DelimitedReader("a,b,c,d");
	jsx_assert(csv_reader.next_field() == "a");
	jsx_assert(csv_reader.next_field() == "b");
	jsx_assert(csv_reader.next_field() == "c");
	jsx_assert(csv_reader.next_field() == "d");

	auto tsv_reader = jsx::DelimitedReader("a\tb\tc\td", '\t');
	jsx_assert(tsv_reader.next_field() == "a");
	jsx_assert(tsv_reader.next_field() == "b");
	jsx_assert(tsv_reader.next_field() == "c");
	jsx_assert(tsv_reader.next_field() == "d");

	return 0;
}
