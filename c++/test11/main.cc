#include <stdlib.h>     /* malloc, free, rand */
#include <assert.h>
#include <string.h>
#include <iostream>
using namespace std;

#define NOP_PER 1   /* 50% */
#define GET_NOP_COUNT(x) (((x)>>NOP_PER > 0) ? ((x)>>NOP_PER) : 1)

enum OpMode {iABC, iABx, iAsBx, iAx};
#define GET_OPCODE(x) ((x)>>6)          // head 2 bit
#define GETARG_sBx(x) ((x)&0x03)        // tail 2 bit
#define SETARG_sBx(i,v) ((i)=((i)&(~0x03))+((v)&0x03))
#define getOpMode(x) ((OpMode)(x))
#define lua_assert(x) assert(x)

typedef struct nop_node{
    int pos;
    char code;
    struct nop_node *next;
} nop_node;

char generate_nop_code() {
    return rand() % 0x100;
}

void cook(char **p_str, size_t *p_len) {
    int pos, newpos;
    size_t olen = *p_len;
    int nnop = GET_NOP_COUNT(olen);
    size_t nlen = olen + nnop;
    char *ostr = *p_str, *code;

    lua_assert(nnop>0);
    ostr = *p_str = (char*)realloc(*p_str, sizeof(char)*nlen);

    nop_node *ob_block = (nop_node *)malloc(sizeof(nop_node) * nlen);
    nop_node *raw_block = ob_block;
    nop_node *nop_block = ob_block + olen;

    // raw_block used to deal with iAsBx ( for fix ARG_sBx )
    // pc+=sBx => nop_node.next
    pos = olen;
    while(pos--) {
        code = ostr + pos;
        raw_block[pos].pos = pos;

        if (getOpMode(GET_OPCODE(*code)) == iAsBx) {
            newpos = pos + 1 + GETARG_sBx(*code); 
            lua_assert( pos >= 0 && pos < olen );
            raw_block[pos].next = &raw_block[newpos];
        } else {
            raw_block[pos].next = NULL;
        }
    }
    
    // nop_block : store random nop opcode
    // nop_head : order of nop opcode by it's random pos
    nop_node *nop_head = NULL, *p, *q;
    pos = nnop;
    while(pos--) {
        newpos = rand() % nlen;
        nop_block[pos].pos = newpos;
        nop_block[pos].code = generate_nop_code();
  
        // sort by pos in nop_head link list
        if (nop_head) {
            p = NULL;
            q = nop_head;
            while (q) {
                if (newpos > q->pos) {
                    nop_block[pos].next = q;
                    if (p)  // middle
                        p->next = &nop_block[pos];
                    else    // maximum pos/head
                        nop_head = &nop_block[pos];
                    break;
                }
                p = q;
                q = q->next;
            }
            if (q == NULL) {
                // minimum pos/tail
                nop_block[pos].next = NULL;
                p->next = &nop_block[pos];
            }
        } else {
            // init
            nop_block[pos].next = NULL;
            nop_head = &nop_block[pos];
        }
    }

    // fill the new code list
    p = nop_head;           // pos for nop_block
    pos = olen;             // pos for raw_block (f->code)
    newpos = nlen;

    while(newpos--){
        if (p && (p->pos >= newpos || pos == 0))  {
            (*p_str)[newpos] = p->code;
            p->pos = newpos;
            p = p->next;
        } else {
            pos--;
            (*p_str)[newpos] = ostr[pos];
            raw_block[pos].pos = newpos;
        }
    }
    lua_assert(p==NULL && pos == 0);

    // fix the ARG_sBx which means pc+=sBx
    pos = olen;
    while(pos--) {
        if (raw_block[pos].next) {
            newpos = raw_block[pos].pos;
            code = *p_str + newpos;
            SETARG_sBx(*code, raw_block[pos].next->pos - newpos - 1);
        }
    }
    
    *p_len = nlen;
    free(ob_block);
}

#define COOK_MEAT(str,len) cook(&str, &len)

void show(char *str, size_t len) {
    int i;
    for (i=0; i<len; i++) {
        if (i>0) {
            if (i%16==0) printf("\n");
            else if (i%8==0) printf("-");
            else if (i%4==0) printf(" ");
        }
        printf("%02x", ((str[i])&0xff));
    }
    printf("\n");

    lua_assert(*(str+len)==0);
    char *c;
    while((c=strstr(str, "\014"))) {
        /**
         * FF (NP form feed, new page) 
         * 换页键（ascii码0x0C，8进制\014）字符，会导致std流缓冲区失效
         * 也就是说，所有的换行操作都会失败：std::endl std::flush std::fflush std::cout<<"\n\r"
         * 只能用printf做换行
        */
        *c = 'P';

    }
    cout << len << ":" << str << endl;
}

int main(int argc, char *args[]) {
    if (argc <= 1) {
        cout << "usage: proc <raw_string_array>" << endl;
        return 0;
    }

    char *str_raw = args[1];
    size_t len = strlen(str_raw);
    char *str_meat = (char *)malloc(sizeof(char)*len);
    strncpy(str_meat, str_raw, len);

    show(str_meat, len);

    COOK_MEAT(str_meat, len);
    
    show(str_meat, len);

    return 0;
}