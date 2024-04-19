#include "minishell.h"

bool    syntax_error(t_token *tokens)
{
    // TOKEN_WORD,         // check unclosed quote
    // TOKEN_PIPE,         // '|' must not be the 1st token or followed by '|' '&' ')'
    // TOKEN_OR,           // '||' must not be the 1st token or followed by '|' '&' ')'
    // TOKEN_AND,          // '&&' must not be the 1st token or followed by '|' or '&'
    // TOKEN_PRT_L,        // '(' must not be single, the last token, or followed by ')' '|' '||' '&&'
    // TOKEN_PRT_R,        // ')' must not be single, or followed by word token or '('
    // TOKEN_REDIR_IN,     // '<' must be followed by word token
    // TOKEN_REDIR_HEREDOC,// '<<' idem
    // TOKEN_REDIR_OUT,    // '>' idem
    // TOKEN_REDIR_APPEND, // '>>' idem
    // TOKEN_UNACCEPTED,   // '&' ';' should be rejected
    int open_prt;

    open_prt = 0;
    if (error_first_token(tokens))
        return (1);
    while (tokens && tokens->type != TOKEN_UNACCEPTED)
    {
        update_parentheses(tokens->type, &open_prt);
        if ()
        tokens = tokens->next;
    }
}