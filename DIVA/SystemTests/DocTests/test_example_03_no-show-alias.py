expected = """\
{InputFile} "example_03.o"
   {CompileUnit} "example_03.cpp"

{Source} "example_03.cpp"
4    {Function} "foo" -> "void"
         - No declaration
4      {Parameter} "c" -> "char"
6      {Variable} "i" -> "INTEGER"
"""


def test(diva):
    assert diva('--no-show-alias example_03.o') == expected
