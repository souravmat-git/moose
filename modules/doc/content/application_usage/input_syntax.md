
# Input File Syntax

MOOSE input file syntax basically works like this:

```
# comment
[section] # inline-comment
  field1 = 'quoted-string'
  field2 = "quoted-string"
  field3 = "multi-line"
           "string"
  field4 = unquoted_string # can't have whitespace

  field5 = 42 # integer
  field6 = 42.42 # floating point number
  field7 = true # boolean (false, on, off - case insensitive)

  field8 = 'item0 item1 item2' # array of items (strings or numbers)
  field9 = 'item00 item01 ;
            item10 item11 ;
            item20' # double indexed array (can even be jagged)

  [subsection]
    foo = 42
  [] # close subsection

  [another_subsection]
  []
[] # close section [../] is also allowed, but deprecated

[another_section]
[]
...
```

## Brace Expressions

Brace expressions allow you to assign computed/calculated values to fields in your input files.
Brace expressions have the following syntax:

```
${cmd [arg1] [arg2]...}
```

where arguments are whitespace delimited and are allowed to be brace expressions themselves.
Brace expressions can appear as field values either unquoted or quoted (i.e. inside a
string-value).  MOOSE currently has five built-in brace-expression commands:

- `${replace <var-name>}`: looks up the parameter `var-name` in the input file searching from the
  current scope looking at consecutive parent scopes until one exists, substituting its value
  for the replacement expression.

- `${raw <arg>...}`: concatenates all arguments' values with no intervening spaces.

- `${env <var-name>}`: evaluates to the named environment variable.

- `${fparse <arg>...}`: takes a
  [Function Parser math expression](http://warp.povusers.org/FunctionParser/fparser.html#literals)
  that can reference fields from the current input file and evaluates it with the function
  parser. Variables referencing input-file fields are looked up using the same procedure as with
  the `replace` command.

- `${units <arg> <unit> [-> <to_unit>]}`: takes an argument `arg` in physical units
  of `unit` and converts it to the unit `to_unit` using the [MooseUnit](/utils/Units.md)
  system. For example the expression `${units 1 J/mol -> eV/at}` would evaluate to the value
  `1.0364269656262e-05`. The `to_unit` argument is optional resulting in a no-op that serves
  only for documentation purposes (`${units 1 J/mol}` would evaluate to the value `1.0`). 

Examples:

```
foo1 = 42
foo2 = 43
[section1]
  num = 1
  bar = ${replace ${raw foo ${num}}}  # becomes 42
  bar2 = ${${raw foo ${num}}} # shortcut for above expression - becomes 42
[]
[section2]
  num = 2
  bar = ${${raw foo ${num}}} # becomes 43
[]

a = ${fparse
      ${section1/bar} + foo1 / foo2
     } # becomes 42+42/43 or 42.976744...
```

Here are some important details about how brace-expressions are evaluated:

- Brace expressions are evaluated in input-file order (i.e. lexical order) from top to bottom of
  the input file.  Brace expressions must *not* depend on field values using brace expressions
  that occur later in the input file - although they can depend on field values without brace
  expressions that occur later in the input file.

- Nested brace expressions are evaluated from the most deeply nested outward.

- A single field value is allowed to have multiple brace expressions if (and only if) the field
  value is presented as a quoted string - e.g. `foo = '${bla1} and ${bla2}'`. These are
  evaluated from left to right.

- If there are no arguments in the brace-expression beyond the "cmd" (e.g. `${foo}`), then the
  `replace` command is implied: e.g. `${foo}` means `${replace foo}`.

## Overridding input parameters from the command line.

See the [CommandLine.md] object for information on how input parameters can be
changed on the command line.
