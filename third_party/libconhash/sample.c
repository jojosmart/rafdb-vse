

#include <stdio.h>
#include <stdlib.h>

#include "conhash.h"

struct node_s g_nodes[64];
int main()
{
    int i;
    const struct node_s *node;
    char str[128];
    long hashes[20];

    /* init conhash instance */
    struct conhash_s *conhash = conhash_init(NULL);
    if(conhash)
    {
        /* set nodes */
        conhash_set_node(&g_nodes[0], "titanic", 5);
        conhash_set_node(&g_nodes[1], "terminator2018", 5);
        conhash_set_node(&g_nodes[2], "Xenomorph", 5);
        conhash_set_node(&g_nodes[3], "True Lies", 5);
        //conhash_set_node(&g_nodes[4], "avantar", 5);

        /* add nodes */
        conhash_add_node(conhash, &g_nodes[0]);
        conhash_add_node(conhash, &g_nodes[1]);
        conhash_add_node(conhash, &g_nodes[2]);
        conhash_add_node(conhash, &g_nodes[3]);
        //conhash_add_node(conhash, &g_nodes[4]);

        printf("virtual nodes number %d\n", conhash_get_vnodes_num(conhash));
        printf("the hashing results--------------------------------------:\n");

        /* try object */
        for(i = 0; i < 20; i++)
        {
            sprintf(str, "James.km%03d", i);
            node = conhash_lookup(conhash, str);
            if(node) printf("[%16s] is in node: [%16s]\n", str, node->iden);
        }
        conhash_get_vnodes(conhash, hashes, sizeof(hashes)/sizeof(hashes[0]));
        printf("hashes size is %d,size2 is %d,uint size is%d\n",sizeof(hashes),sizeof(hashes[0]),sizeof(u_int));
        for(i=0;i<20;i++) {
          printf("%u==>%d\n",hashes[i],i);
        }
        conhash_del_node(conhash, &g_nodes[2]);
        printf("remove node[%s], virtual nodes number %d\n", g_nodes[2].iden, conhash_get_vnodes_num(conhash));
        printf("the hashing results--------------------------------------:\n");
        for(i = 0; i < 20; i++)
        {
            sprintf(str, "James.km%03d", i);
            node = conhash_lookup(conhash, str);
            if(node) printf("[%16s] is in node: [%16s]\n", str, node->iden);
        }
    }
    conhash_fini(conhash);
    return 0;
}
