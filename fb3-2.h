/*计算器声明部分*/
 
//词法分析器接口
extern int yylineno;
void yyerror(char *s, ...);

//符号表：每个符号可以有一个变量和一个用户自定义函数
struct symbol{
    char *name;
    double value;
    struct ast* func;   //函数体
    struct symlist* syms;   //虚拟参数列表
};

//固定大小的简单符号表
#define NHASH 9997
struct symbol symtab[NHASH];

struct symbol *lookup(char*);

struct symlist{
    struct symbol *sym;
    struct symlist *next;
};

struct symlist *newsymlist(struct symbol *sym, struct symlist *next);
void symlistfree(struct symlist *s1);

/*
节点类型
* + - * / |
0-7比较操作符，位编码：04等于，02小于，01大于
M单目负号
L表达式或语句列表
I IF语句
W WHILE语句
N 符号引用
= 赋值
S 符号列表
F 内置函数调用
C 用户函数调用
*/
enum bifs{              //内置函数
    B_sqrt = 1,
    B_exp,
    B_log,
    B_print
};

//ast节点，所有节点都有公共的初始nodetype
struct ast{
    int nodetype;
    struct ast *l;
    struct ast *r;
};

struct fncall{          //内置函数
    int nodetype;       //类型F
    struct ast *l;
    enum bifs functype;
};

struct ufncall{         //用户自定义函数
    int nodetype;       //类型C
    struct ast *l;
    struct symbol *s;
};

struct flow{            //控制流表达式
    int nodetype;       //类型I或W
    struct ast *cond;   //条件
    struct ast *t1;     //then分支或do语句
    struct ast *el;     //可选else分支
};

struct numval{          //常量
    int nodetype;       //类型K
    double number;
};

struct symref{          //符号引用
    int nodetype;       //类型N
    struct symbol *s;
};

struct symasgn{         //赋值
    int nodetype;
    struct symbol *s;   //类型=
    struct ast *v;      //值
};

struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);
struct ast *newfunc(int functype, struct ast *l);
struct ast *newcall(struct symbol *s, struct ast *l);
struct ast *newref(struct symbol *s);
struct ast *newasgn(struct symbol *s, struct ast *v);
struct ast *newnum(double d);
struct ast *newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *tr);

// 定义函数
void dodef(struct symbol *name, struct symlist *syms, struct ast *stmts);
// 计算ast
double eval(struct ast *);
// 删除和释放ast
void treefree(struct ast *);
