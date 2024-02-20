# C-interpreter

#### Note

This is a toy implement of C-interpreter.

#### Logs

-   2024.02.20: version 0.0.1

    Supported:

    -   `int` literal:

        ```
        123
        ```

    -   `int` variable definition (limited once a time):

        ```
        int a;
        int b = 0;
        ```

    -   Expressions of +, -, *, /, % and =:

        ```
        a = 1 + 2 * 3 / 5;
        ```

    -   Flow control statments of if, while, do-while and for (however, without bool-casting, the condition is not legal in principle):

        ```
        while(1) {
           if(a = 0) a ++;
           else a --;
        }
        ```

    -   The start non-terminal symbol `program` is parsed to `glo_stmts` or `glo_expr`.
        This means, a legal program must be a serial of statments or an expression.
        When it's an expression, the value will be returned and printed.
        However, the separator of programs is set to any unprintable character.

        ```
        >>> int a = 1;
        >>> int b = 2;
        >>> a = a + b;
        >>> ^A // Control + A or \x01
        >>> a
        >>> ^A // Control + A or \x01
        3
        >>>
        ```