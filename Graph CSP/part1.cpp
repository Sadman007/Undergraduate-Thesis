#include <bits/stdtr1c++.h>
using namespace std;
#define pb push_back
#define for0(i,a) for(int i=0;i<a;i++)
#define for1(i,a) for(int i=1;i<=a;i++)
#define all(v) v.begin(),v.end()
#define MAX 1000
#define AUTH 1
#define PAP 2
#define VEN 3
#define pii pair<int,int>

vector<int>graph[MAX],typeList[MAX],qn_typeList[MAX],qn_graph[MAX];
vector<int>preli[MAX+5];
vector<int>candidate_list[MAX+5];
int type[MAX];
int qn_type[MAX];
int n_node,n_edge;

void add_edge(int u,int v);
void add_query_edge(int u,int v);
void add_type(int u,int v);
void add_query_type(int u,int v);
void take_input();
void gen_candidate_set(int nq,int mq);
void query_dfs(int u,int level,vector<pii>res,int qn);
bool searchNode(vector<pii>pp,int nd);
void print_res(vector<pii>res);

void add_edge(int u,int v)
{
    graph[u].pb(v);
    graph[v].pb(u);
}

void add_query_edge(int u,int v)
{
    qn_graph[u].pb(v);
    qn_graph[v].pb(u);
}

void add_type(int u,int v)
{
    typeList[u].pb(type[v]);
    typeList[v].pb(type[u]);
}

void add_query_type(int u,int v)
{
    qn_typeList[u].pb(qn_type[v]);
    qn_typeList[v].pb(qn_type[u]);
}

void take_input()
{
    printf("Enter the number of nodes in the main graph\n");
    scanf("%d",&n_node);
    printf("Enter the number of edges in the main graph\n");
    scanf("%d",&n_edge);

    printf("Enter the types of nodes serially\n");
    for1(i,n_node)
    {
        int tp;
        scanf("%d",&tp);
        type[i] = tp;
    }

    printf("Enter the connection between nodes\n");
    for1(i,n_edge)
    {
        int u,v;
        scanf("%d%d",&u,&v);
        add_edge(u,v);
        add_type(u,v);
    }
    /// sort the type list
    for(int i=1; i<=n_node; i++)
    {
        sort(all(typeList[i]));
        sort(all(graph[i]));
    }
}

void show_graph()
{
    for1(i,n_node)
    {
        printf("Node %d(%d):",i,type[i]);
        for0(j,(int)graph[i].size())
        {
            printf(" %d",graph[i][j]);
        }
        printf("\n");
    }
}

void show_query_type()
{
    for1(i,n_node)
    {
        if(qn_typeList[i].size()==0) continue;
        printf("Node %d(%d):",i,qn_type[i]);
        for0(j,(int)qn_typeList[i].size())
        {
            printf(" %d",qn_typeList[i][j]);
        }
        printf("\n");
    }

}

void show_type()
{
    for1(i,n_node)
    {
        if(typeList[i].size()==0) continue;
        printf("Node %d(%d):",i,type[i]);
        for0(j,(int)typeList[i].size())
        {
            printf(" %d",typeList[i][j]);
        }
        printf("\n");
    }
}

void gen_candidate_set(int nq,int mq)
{
    for(int i=1; i<=nq; i++)
    {
        for(int j=1; j<=n_node; j++)
        {
            if(type[j]==qn_type[i]) preli[i].push_back(j);
        }
    }

    /*printf("Preli:\n");
    for(int i=1; i<=nq; i++)
    {
        printf("%d : ",i);
        for(int j=0; j<preli[i].size(); j++)
        {
            printf("%d ",preli[i][j]);
        }
        printf("\n");
    }
    */
    for(int i=1; i<=nq; i++)
    {
        for(int j=0; j<preli[i].size(); j++)
        {
            int itm = preli[i][j];
            int len = 0;
            for(int k=0; k<qn_typeList[i].size(); k++)
            {
                int qn_spec_type = qn_typeList[i][k];
                int qn_type_cnt = count(all(qn_typeList[i]),qn_spec_type);
                /*if(i==2 && j==0){
                cout << "qn_spec_type : " << qn_spec_type << endl;
                cout << "qn_cnt : " << qn_type_cnt << endl;
                }*/
                /*if(binary_search(all(typeList[itm]),qn_typeList[i][k])) len++;
                else break;*/
                if(count(all(typeList[itm]),qn_spec_type)>=qn_type_cnt)
                {
                    len+=qn_type_cnt;
                    while(qn_spec_type==qn_typeList[i][k]) k++;
                    --k;
                }
                else break;
            }
            if(len==qn_typeList[i].size()) candidate_list[i].push_back(itm);
        }
    }
    cout << "printing candidate list:\n";
    for(int i=1; i<=nq; i++)
    {
        cout << i << ": ";
        for(int j=0; j<candidate_list[i].size(); j++)
        {
            printf("%d ",candidate_list[i][j]);
        }
        cout << endl;
    }
}

void query_dfs(int u,int level,vector<pii>res,int qn)
{
    if(res.size()==qn)
    {
        print_res(res);
        return;
    }

    for(int i=0; i<(int)candidate_list[level].size(); i++)
    {
        int v = candidate_list[level][i];
        bool pushed = 1;
        //if(!binary_search(all(graph[u]),v) && u!=1) continue;
        //if(binary_search(all(graph[])))

        if(res.size()!=0)
        {
            if(binary_search(all(graph[u]),v))
            {
                pushed = 1;
            }
            else
            {
                pushed = 0;
            }
        }
        if(searchNode(res,v)) pushed = 0;
        if(pushed)
        {
            res.push_back({u,v});
            //cout << v << endl;
            query_dfs(v,level+1,res,qn);
            res.pop_back();
        }
    }
    return;

}



void take_qn_graph()
{
    int nq,mq;
    printf("Enter the number of nodes in query graph\n");
    scanf("%d",&nq);
    printf("Enter the number of edges in query graph\n");
    scanf("%d",&mq);

    printf("Enter the types of nodes serially\n");
    for1(i,nq)
    {
        int tp;
        scanf("%d",&tp);
        qn_type[i] = tp;
    }

    printf("Enter the connection between nodes\n");
    for1(i,mq)
    {
        int u,v;
        scanf("%d%d",&u,&v);
        add_query_edge(u,v);
        add_query_type(u,v);
    }
    /// sort the type list
    for(int i=1; i<=nq; i++)
    {
        sort(all(qn_typeList[i]));
        sort(all(qn_graph[i]));
    }
    ///generate candidate set. candidate[i] contains candidates for node i
    gen_candidate_set(nq,mq);
    vector<pii>res;
    query_dfs(1,1,res,nq);
}

void print_res(vector<pii>res)
{
    for(int i=0; i<res.size(); i++)
    {
        pii tmp = res[i];
        printf("(%d,%d) ",tmp.first,tmp.second);
    }
    cout << endl;
    return;
}

bool searchNode(vector<pii>pp,int nd)
{
    for(int i=0; i<pp.size(); i++)
    {
        if(pp[i].second==nd) return 1;
    }
    return 0;
}

int main()
{
    freopen("inp.txt","r",stdin);
    take_input();
    //show_type();
    take_qn_graph();
    show_query_type();
    //show_graph();

    return 0;
}


/*
1: 2 3
2: 3 4 5
3: 4 5 6 7
*/
