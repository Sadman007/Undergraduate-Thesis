#include <bits/stdtr1c++.h>
using namespace std;
#define pb push_back
#define for0(i,a) for(int i=0;i<a;i++)
#define for1(i,a) for(int i=1;i<=a;i++)
#define all(v) v.begin(),v.end()
#define MAX 1000
#define MAX_LABEL 3
#define MAX_QN_NODES 100
#define pii pair<int,int>
#define mp_s_vs map<string,vector<string> >
#define mp_i_vs map<int,vector<string> >
#define mp_s_vs_it mp_s_vs::iterator
#define mp_i_vs_it mp_i_vs::iterator

map<string,vector<string> >graph,graph_type,qn_typeList;
map<string,map<string,int> >type,qnType;
vector<int>qn_graph[MAX];
vector<string>qn_node_label(MAX);
map< int, vector<string> >candList,assList;

///total label info
map<string,vector<string> >labelInfo;

int n_node,n_edge;

void add_edge(string u,string v);
void qn_add_edge(string u,string v);
void add_type(string u,string v);
void qn_add_type(string u,string v);
void take_input();
void gen_candidate_set(int nq);
void show_adjacency_type();
void show_candidate_list();
void cand_cleaner_dfs();

void backtrack();
bool check_compatability(int domID,string nd,mp_i_vs &assList);
int get_best_id(mp_i_vs &candList,mp_i_vs &assList);
int num_of_soln = 0;

string toString(int num)
{
    stringstream ss;
    ss << num;
    string res;
    ss >> res;
    return res;
}

void add_edge(string u,string v)
{
    graph[u].pb(v);
    graph[v].pb(u);
}

void qn_add_edge(int u,int v)
{
    qn_graph[u].pb(v);
    qn_graph[v].pb(u);
}

void add_type(string u,string v)
{
    graph_type[u].pb(v);
    graph_type[v].pb(u);
}

void qn_add_type(string u,string v)
{
    qn_typeList[u].pb(v);
    qn_typeList[v].pb(u);
}

void show_graph(map<string,vector<string> >gg,string msg)
{
    cout << "**** " << msg << " ****\n";
    map<string, vector<string> >::iterator it = gg.begin();

    for(it; it!=gg.end(); it++)
    {
        cout << it->first << ": ";
        vector<string>vc = it->second;
        for(int i=0; i<(int)vc.size(); i++)
        {
            cout << vc[i] << " ";
        }
        printf("\n");
    }
}

void show_candidate_list(int nq,string msg)
{
    cout << "**** " << msg << " ****\n";
    for(int i=1; i<=nq; i++)
    {
        cout << qn_node_label[i] << ": ";
        for(int j=0; j<(int)candList[i].size(); j++)
        {
            cout << candList[i][j] << " ";
        }
        cout << endl;
    }
}

void qn_show_graph(int nq,string msg)
{
    cout << "**** " << msg << " ****\n";
    for(int i=1; i<=nq; i++)
    {
        cout << i << ": ";
        for(int j=0; j<(int)qn_graph[i].size(); j++)
        {
            cout << qn_graph[i][j] << " ";
        }
        printf("\n");
    }
}

void show_adjacency_type(map<string,map<string,int> >gg,string msg)
{
    cout << "**** " << msg << " ****\n";
    map<string, map<string,int> >::iterator it = gg.begin();
    for(it; it!=gg.end(); it++)
    {
        cout << it->first << ": ";
        map<string,int>vc = it->second;
        map<string,int>::iterator it2 = vc.begin();
        for(it2; it2!=vc.end(); it2++)
        {
            cout << "(" << it2->first << "," << it2->second << ") ";
        }
        printf("\n");
    }
}

void qn_show_type(int nq,string msg)
{
    cout << "**** " << msg << " ****\n";
    for(int i=1; i<=nq; i++)
    {
        cout << qn_node_label[i] << ": ";
        for(int j=0; j<(int)qn_graph[i].size(); j++)
        {
            cout << qn_node_label[qn_graph[i][j]] << " ";
        }
        printf("\n");
    }
}

void take_input(ifstream & inF)
{
    //printf("Enter the number of nodes in the main graph\n");
    inF >> n_node;
    //printf("Enter the number of edges in the main graph\n");
    inF >> n_edge;

    //printf("Enter the connection between nodes\n");
    for1(i,n_edge)
    {
        string u,v;
        inF >> u;
        inF >> v;
        add_edge(u,v);
        string uu,vv;
        uu = u[0];
        vv = v[0];
        add_type(uu,vv);
        labelInfo[uu].push_back(u);
        labelInfo[vv].push_back(v);
        type[u][vv]++;
        type[v][uu]++;
    }

    /// sort the type list
    for(map<string, vector<string> >::iterator it = graph.begin(); it!=graph.end(); it++)
    {
        sort(all(graph[it->first]));
    }
    ///sort the label list
    for(map<string, vector<string> >::iterator it = labelInfo.begin(); it!=labelInfo.end(); it++)
    {
        sort(all(labelInfo[it->first]));
        labelInfo[it->first].erase(unique(all(labelInfo[it->first])),labelInfo[it->first].end());
    }
    //show_adjacency_type(type,"main graph adjacency type");

}


void qn_take_graph(ifstream & inF)
{
    int nq,mq;
    //printf("Enter the number of nodes in query graph\n");
    inF >> nq;
    //printf("Enter the number of edges in query graph\n");
    inF >> mq;
    //enter the labels of the nodes

    for(int i=1; i<=nq; i++)
    {
        inF >> qn_node_label[i];
    }
    //printf("Enter the connection between nodes\n");

    for1(i,mq)
    {
        int u,v;
        inF >> u;
        inF >> v;
        qn_add_edge(u,v);

        qnType[toString(u)][qn_node_label[v]]++;
        qnType[toString(v)][qn_node_label[u]]++;

    }

    /// sort the type list
    for(int i=1; i<=nq; i++)
    {
        sort(all(qn_graph[i]));
    }
    //qn_show_graph(nq,"query adjacency list");
    //qn_show_type(nq,"query adjacency type");
    gen_candidate_set(nq);
    //show_adjacency_type(qnType,"query graph adjacency type");

}

bool hasFound(mp_i_vs &gg)
{
    for(mp_i_vs_it it = gg.begin(); it!=gg.end(); it++)
    {
        int sz = (it->second).size();
        if(!sz)
            return 0;
    }
    return 1;
}

bool isEmpty(mp_i_vs &candList)
{
    for(mp_i_vs_it it = candList.begin(); it!=candList.end(); it++)
    {
        int sz = (it->second).size();
        if(sz)
            return 0;
    }
    return 1;
}

void print_sol(mp_i_vs &soln)
{
    cout << "Solution!\n";
    for(mp_i_vs_it it=soln.begin(); it!=soln.end(); it++)
    {
        cout << it->first << ": ";
        for(int i=0; i<(int)(it->second).size(); i++)
        {
            cout << (it->second)[i] << " ";
        }
        cout << "\n";
    }
}

void filterDomain(mp_i_vs &assList,mp_i_vs &candList)
{
    for(mp_i_vs_it it=assList.begin(); it!=assList.end(); it++)
    {
        int id = it->first;
        vector<string>vc = it->second;
        for(int i=0; i<(int)vc.size(); i++)
        {
            string itm = vc[i];
            for(mp_i_vs_it it2=candList.begin(); it2!=candList.end(); it2++)
            {
                for(int j=0; j<(int)(it2->second).size(); j++)
                {
                    if((it2->second)[j]==itm)
                        (it2->second).erase((it2->second).begin()+j);
                }
            }
        }
    }
}

bool check_compatability(int domID,string nd,mp_i_vs &assList)
{
    for(int i=0; i<(int)qn_graph[domID].size(); i++)
    {
        int adj_nd = qn_graph[domID][i];
        if(assList[adj_nd].size()==0)
            continue;
        else
        {
            string nd2 = assList[adj_nd][0];
            if(!binary_search(all(graph[nd]),nd2))
                return 0;
        }
    }
    return 1;
}

int get_best_id(mp_i_vs &candList,mp_i_vs &assList)
{
    int MN = INT_MAX;
    int best_ID = -1;

    for(mp_i_vs_it it=candList.begin(); it!=candList.end(); it++)
    {
        int len = (it->second).size();
        if(len<MN && len && assList[(it->first)].size()==0)
        {
            MN = len;
            best_ID = it->first;
        }
    }
    return best_ID;
}

void backtrack()
{
    if(hasFound(assList))
    {
        num_of_soln++;
        print_sol(assList);
        return;
    }
    filterDomain(assList,candList);
    if(isEmpty(candList))
    {
        return;
    }

    int domID = get_best_id(candList,assList);

    if(domID==-1)
    {
        return;
    }

    //mp_i_vs pruned_domain;

    for(int i=0; i<candList[domID].size(); i++)
    {
        assList[domID].pb(candList[domID][i]);
        bool possible = check_compatability(domID,candList[domID][i],assList);
        if(!possible)
        {
            assList[domID].clear();
            continue;
        }
        else
        {
            string prsrv = candList[domID][i];
            candList[domID].erase(candList[domID].begin()+i);
            backtrack();
            candList[domID].pb(prsrv);
            sort(all(candList[domID]));
            assList[domID].clear();
        }
    }
}

void gen_candidate_set(int nq)
{
    ///stage 1
    for(int i=1; i<=nq; i++)
    {
        string ndType = qn_node_label[i];
        for(int j=0; j<(int)labelInfo[ndType].size(); j++)
        {
            string nd = labelInfo[ndType][j];
            string nd_0 = toString(i);
            /// jodi minimum labelwise connectivity assure kore,taile jabo
            if(qnType[nd_0]["a"]<=type[nd]["a"] &&
                    qnType[nd_0]["v"]<=type[nd]["v"] &&
                    qnType[nd_0]["p"]<=type[nd]["p"])
                candList[i].pb(nd);
        }
    }
    ///stage 2
    // it is left till now. Ei part e candidate list ta theke kichu node baad
    // deya hoy. Jeishob node actually query graph er ekta chach hishebe
    // used hoite parena

    ///stage 3
    /// ei backtrack() func ta soln khujbe :)
    for(int i=1; i<=nq; i++)
        assList[i];
    //show_candidate_list(nq,"candidate list");
    backtrack();
    if(num_of_soln==0) cout << "NO SOLUTION\n";

}


int main()
{
    ifstream inF,qn_inF;
    inF.open("test_graph2.txt");
    qn_inF.open("query7.txt");

    take_input(inF);
    qn_take_graph(qn_inF);

    //show_graph(graph,"graph list");
    //show_graph(labelInfo,"global label list");
    return 0;
}
