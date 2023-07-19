{
    struct s_job    *previous;
    char            cmd;
    char            file;
    int                fd[2];
    pid_t            pid;
    struct s_job    next;
}    t_job;

typedef enum e_type
{
    VOID,
    PIPE,
    STRING,
    REDIR_L,
    REDIR_R,
    APPEND,
    HEREDOC_L,
}    t_type;

// Single node of linked list contaning input tokenized as per quotes */
struct s_qnode {
    int                length;
    char            q_type;
    int                is_spaced;
    char            *str;
    struct s_qnode    next;
};

// Struct containig data related to input tokenized as per quotes */
typedef struct QuotesData {
    int                start;
    int                end;
    int                d_open;
    int                s_open;
    char            *raw_input;
    char            *delim;
    char            *after_delim;
    struct s_qnode    quotes_list;
}    t_qdata;

/ Struct for every node representing a word token in the linked list */
struct s_tnode {
    char            *str;
    int                len;
    char            q_type;
    t_type            type;
    int                is_spaced;
    struct s_tnode    *next;
    struct s_tnode    prev;
};

/ Highest level struct with data shared by whole program */
typedef struct CommonData
{
    char            envp;
    char            envp_export;
    int                exit_status;
    char            *cmd;
    int                syntax_error;
    char            **paths;
    int                found_here_doc;
    struct s_tnode    *tokens_list;
    t_pdata            *t_pdata;
    t_qdata            *t_qdata;
}    t_cdata;