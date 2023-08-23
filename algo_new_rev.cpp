#include<bits/stdc++.h>
#define REP(x,y,z) for(int x=y;x<=z;x++)
#define FORD(x,y,z) for(int x=y;x>=z;x--)
#define MSET(x,y) memset(x,y,sizeof(x))
#define FOR(x,y) for(__typeof(y.begin()) x=y.begin();x!=y.end();x++)
#define F first
#define S second
#define MP make_pair
#define PB push_back
#define SZ size()
#define M
void RI(){}
template<typename... T>
void RI( int& head, T&... tail ) {
    scanf("%d",&head);
    RI(tail...);
}
using namespace std;
typedef long long LL;
clock_t st,ed;
bool CUT1=false, CUT2=false;
bool SKIP_POST=false;
bool SKIP_DIAMETER=false;
bool SKIP_IMP = false;
int H,P;
void readarg(int argc,char *argv[])
{
	H = P = -1;
	REP(i,0,argc-1)
	{
		if(i<argc-1 && !strcmp(argv[i], "-h"))
		{
			H = atoi(argv[i+1]);
		}
		if(i<argc-1 && !strcmp(argv[i], "-p"))
		{
			P = atoi(argv[i+1]);
		}

		if(!strcmp(argv[i], "-prune1"))
		{
			CUT1 = true;
		}
		if(!strcmp(argv[i], "-prune2"))
		{
			CUT2 = true;
		}
		if(!strcmp(argv[i], "-skip-post"))
		{
			SKIP_POST = true;
		}
		if(!strcmp(argv[i], "-skip-diameter"))
		{
			SKIP_DIAMETER = true;
		}
		if(!strcmp(argv[i], "-skip-imp"))
		{
			SKIP_IMP = true;
		}
	}
}
/**
 * set intersection
 *
 * @param v1,v2: sorted set
 * @return: a sorted array
 */
vector<int> instersection(vector<int> &v1, vector<int> &v2)
{
	vector<int> v3;
	//sort(v1.begin(), v1.end());
	//sort(v2.begin(), v2.end());
	set_intersection(v1.begin(),v1.end(),v2.begin(),v2.end(),back_inserter(v3));
	return v3;
}
int v,c,n;
vector<bool> del,apx;
vector<vector<pair<int,double>>> e;
vector<vector<double>> ediv;
vector<pair<int,int>> deg;
vector<int> dis;
/**
 * initializa and read graph input
 */
void init()
{
	int x,y;
	int e1,e2,e3;
	double z;

	scanf("%d %d %d %d %d",&v,&c,&e1,&e2,&e3);
	n = v+c;
	e = vector<vector<pair<int,double>>> (n+1);
	ediv = vector<vector<double>> (c+1);
	REP(i,0,c) ediv[i] = vector<double>(c+1);
	deg = vector<pair<int,int>> (n+1);
	del = vector<bool> (n+1);
	apx = vector<bool> (n+1);

	REP(i,1,e1) //read social edges
	{
		scanf("%d %d",&x,&y);
		e[x].PB( MP(y,0.0) );
		e[y].PB( MP(x,0.0) );
	}
	REP(i,1,e2) //read diveristy edgess
	{
		scanf("%d %d %lf",&x,&y,&z);
		e[x].PB( MP(y,z) );
		e[y].PB( MP(x,z) );
		//ediv[x].PB( MP(y,z) );
		//ediv[y].PB( MP(x,z) );
		//printf("%d %d\n",x-v,y-v);
		ediv[x-v][y-v] = ediv[y-v][x-v] = z;
	}
	REP(i,1,e3) //read channel edge
	{
		scanf("%d %d",&x,&y);
		e[x].PB( MP(y,0.0) );
		e[y].PB( MP(x,0.0) );
	}
}
/**
 * get degree of all vertices(excepted deleted nodes)
 * deg[i].first: number of viewer connected to i
 * deg[i].second: number of channel conntected to i
 */
void get_degree()
{
	REP(i,1,n) deg[i] = MP(0,0);
	REP(i,1,n)if(!del[i]) for(auto j:e[i])if(!del[j.F]) //foreach edge i->j
	{
		if(j.F>v) deg[i].S++;
		else deg[i].F++;
	}
}
/**
 * return "cur" 's h-hop set
 *
 * @param cur: a vertex in viewers set
 * @return pair<vector<int>,vector<int>>: first vector are viewers in h-hop set,
 * second vector are channels in h-hop set(channels doesn't satisfy the constraint are deleted)
 */
pair<vector<int>,vector<int>> get_h2(int cur)
{
	queue<int> q;
	vector<int> dis2(n+1);
	dis2[cur]=1;//distance from cur

	//find social vertices
	for(auto i:e[cur]) if(i.F<=v && !dis2[i.F] && !del[i.F])
	{
		dis2[i.F] = dis2[cur]+1;
		q.push(i.F);
	}
	while(!q.empty())//BFS queue
	{
		cur = q.front();
		q.pop();
		if(dis2[cur]>=H+1) continue;

		for(auto i:e[cur]) if(i.F<=v && !dis2[i.F] && !del[i.F])
		{
			dis2[i.F] = dis2[cur]+1;
			q.push(i.F);
		}
	}

	//channels
	vector<int> cnt(n+1);
	REP(i,v+1,v+c) for(auto j:e[i]) if(j.F<=v && dis2[j.F]) cnt[i]++;

	//return values and check channel constraint
	vector<int> r1,r2;
	REP(i,1,v) if(dis2[i]>0 && dis2[i]<=H+1) r1.PB(i);
	REP(i,v+1,v+c) if(cnt[i]>=P && !del[i]) r2.PB(i);
	return MP(r1,r2);
}
//given channels，return their preference users
vector<int> get_viewers(vector<int> &ch)//
{
	vector<int> re;
	vector<bool> viss(n+1);
	for(int i:ch) for(auto j:e[i]) if(j.F<=v && !del[j.F]) viss[j.F]=true;
	REP(i,1,v) if(viss[i]) re.PB(i);
	return re;
}
//given viewers，find their preference channels (doesn't delete channel < p preference)
vector<int> get_channels(vector<int> &vi)//
{
	vector<int> re;
	vector<bool> viss(n+1);
	for(int i:vi) for(auto j:e[i]) if(j.F>v && !del[j.F]) viss[j.F]=true;
	REP(i,v+1,v+c) if(viss[i]) re.PB(i);
	return re;
}
//given channels，find the one with max total diversity(and must be in set y)
int max_channel_diversity(vector<int> &x,vector<int> &y)//
{
	int re=-1;
	double mx = -1e9;

	vector<double> sum(c+1);
	int sz = (int)x.size();
	REP(i,0,sz-1)REP(j,i+1,sz-1) if(x[i]>v && x[j]>v)
	{
		sum[x[i]-v] += ediv[x[i]-v][x[j]-v];
		sum[x[j]-v] += ediv[x[i]-v][x[j]-v];
	}

	for(int i:y) if(sum[i-v]>mx)
	{
		mx = sum[i-v];
		re = i;
	}
	return re;
}
//given channels，find the total diversity of this subgraph
double total_diversity(vector<int> &x) //
{
	double re=0;
	for(int i:x) if(!del[i] && i>v) for(int j:x) if(!del[j] && j>v) re += ediv[i-v][j-v];
	return re;
}
//given one channel, find total diversity
double vertex_total_diversity(int vv) //
{
	double re=0;
	int i=vv;
	REP(j,v+1,v+c) if(!del[j]) re += ediv[i-v][j-v];
	return re;
}
//given channel set, find objective value
double get_objective(vector<int> &x)//
{
	if((int)x.size()==0) return 0.0;

	double re=0.0;
	for(int i:x) if(!del[i] && i>v) for(int j:x) if(!del[j] && j>v) re += ediv[i-v][j-v];
	return re / (double)x.size();
}
//find shortest path from node "root"(on viewers graph，nodes deleted are also considered)
void shortest_path(int root)
{
	int INF = 100000000;
	int cur;
	queue<int> q;

    //BFS
	dis = vector<int> (n+1, INF);
	{
		dis[root] = 0;
		q.push(root);
		while(!q.empty())
		{
			cur = q.front();
			q.pop();

			for(auto j:e[cur])if(j.F<=v && dis[j.F]==INF)
			{
				dis[j.F] = dis[cur]+1;
				q.push(j.F);
			}
		}
	}
}
void sdssel()
{
    //delete channel < p viewers
	get_degree();
	REP(i,v+1,v+c) if(deg[i].F<P) del[i]=true;

	//delete viewer with no preference edge
	get_degree();
	REP(i,1,v) if(deg[i].S<=0) del[i]=true;

	double kapxobj = 0.0;
	double kapximpobj = 0.0;
	int vb=-1, qb=-1;
	vector<int> h2v,h2c;
	vector<int> kapx, fapx;
	vector<int> kapximp, fapximp;
	vector<int> cfit;

	REP(Vs,1,v) if(!del[Vs]) //select v belong to I, I = I - {v}
	{
		//new cut added
		vector<int> vvtmp2; vvtmp2.PB(Vs);
		vector<int> cctmp2 = get_channels(vvtmp2);//C(v)
		//q_bar = argmax_{q belong to C(v)} (Delta C)
		qb = max_channel_diversity(cctmp2, cctmp2);
		if( vertex_total_diversity(qb) <= 2*kapxobj )
		{
			continue;
		}

        //get H_v^2(V) and H_v^2(C)
		tie(h2v,h2c) = get_h2(Vs);
		if( (int)h2v.size()<P ) continue;

        //prune 2
		if(CUT2)
		{
			vector<int> ptmp; ptmp.PB(Vs);
			vector<int> vtmp = get_channels(ptmp);
			if((int)(instersection(h2c, vtmp).size())==0)
				continue;
		}
		//prune 1
		if(CUT1 && vb!=-1)
		{
			vector<int> ptmp; ptmp.PB(vb);
			vector<int> vtmp = get_channels(ptmp);

			if( total_diversity(h2c) <= 2*get_objective(vtmp) )
				continue;
		}

		//calc diversity
		vector<double> h2cdiv = vector<double>(n+1);//h2c div = diversity of each node in H_v^2(C)
		for(int i:h2c) for(int j:h2c) if(i>v && j>v) h2cdiv[i] += ediv[i-v][j-v];

		double h2ctotdiv = total_diversity(h2c);
		double h2cobj = h2ctotdiv / (int)h2c.size();
		while(h2c.size())
		{
			if( kapx.size()==0 || h2cobj > kapxobj )
			{
				vector<int> viewer = get_viewers(h2c);
				vector<int> tmpf = instersection(h2v, viewer);

				if((int)tmpf.size() >= P)
				{
					kapx = h2c;
					kapxobj = h2cobj;
					fapx = instersection(h2v, viewer);
					vb = Vs;
				}
			}
			//q_bar = min_channel_diversity(h2c)
			qb = -1;
			for(int i:h2c) if(qb==-1 || h2cdiv[i]<h2cdiv[qb]) qb = i;

			//delete node q_bar
			////maintain diversity after q_bar deleted
			for(int i:h2c) h2cdiv[i] -= ediv[i-v][qb-v];

			//maintain h2c's total diversity
			for(int i:h2c) h2ctotdiv -= 2*ediv[i-v][qb-v];

			//delete q_bar from h2c
			h2c.erase(remove(h2c.begin(), h2c.end(), qb), h2c.end());

			//recalc h2c's objective value
			h2cobj = h2ctotdiv / (int)h2c.size();
		}
	}

	//IMP
	if(!SKIP_IMP)
	{
		for(int i:h2c) if(lower_bound(kapx.begin(), kapx.end(), i) == kapx.end()) cfit.PB(i);
		kapximp = kapx;
		kapximpobj = kapxobj;
		fapximp = fapx;

		//delete from cfit
		vector<int> tp;
		for(int i:cfit)
		{
			double s=0;
			for(auto j:e[i]) if(!del[j.F] && j.F>v && lower_bound(h2c.begin(), h2c.end(), j.F)!=h2c.end())
				s += j.S;
			if(s < kapxobj)
				tp.PB(i);
		}
		for(int i:tp) cfit.erase(remove(cfit.begin(), cfit.end(), i), cfit.end());

        //record diversity of every nodes in kapximp
        vector<double> kapximpdiv(n+1);
		for(int i:kapximp) for(int j:kapximp) if(i>v && j>v) kapximpdiv[i] += ediv[i-v][j-v];
		while(kapximpobj<=kapxobj && cfit.size())
		{
			//qb = max_channel_diversity(kapximp, cfit);
			qb = -1;
			for(int i:cfit) if(qb==-1 || kapximpdiv[i]>kapximpdiv[qb]) qb = i;
			vector<int> tt2; tt2.PB(qb);
			vector<int> viewer = get_viewers( tt2 );

			//if qb not in kapximp, insert it
			if( lower_bound(kapximp.begin(),kapximp.end(),qb) == kapximp.end() )
			{
				kapximp.PB(qb);
				sort(kapximp.begin(), kapximp.end());
				kapximp.resize( unique(kapximp.begin(), kapximp.end()) - kapximp.begin() );
				//recalc objective value
				kapximpobj = get_objective(kapximp);

				kapximpdiv = vector<double>(n+1);
				for(int i:kapximp) for(int j:kapximp) if(i>v && j>v) kapximpdiv[i] += ediv[i-v][j-v];
			}

			//fapximp
			for(int i:viewer) if(lower_bound(h2v.begin(), h2v.end(), i) != h2v.end()) fapximp.PB(i);
			sort(fapximp.begin(), fapximp.end());
			fapximp.resize( unique(fapximp.begin(), fapximp.end()) - fapximp.begin() );

			cfit.erase(remove(cfit.begin(), cfit.end(), qb), cfit.end());

			//if(get_objective(kapximp)>get_objective(kapx))
			if(kapximpobj > kapxobj)
			{
				kapx = kapximp;
				kapxobj = kapximpobj;
				fapx = fapximp;
			}
		}
	}

	//POST process
	vector<bool> delf(n+1); //delete from apx
	if(!SKIP_POST)
	{
		for(int i:fapx) if(!delf[i])
		{
		    //check shortest path
			shortest_path(i);
			bool flag=false;
			for(int j:fapx) if(!delf[j]) if(dis[j]>H) { flag=true; break; } //distance > H
			if(!flag) continue;

			//reduce channel
			vector<int> cnt(n+1);
			vector<int> newch;
			vector<bool> vnow(n+1);
			for(int j:fapx) if(!delf[j] && j!=i) vnow[j]=true;

			//count the viewers
			REP(j,v+1,v+c) for(auto k:e[j]) if(vnow[k.F]) cnt[j]++;
			for(int j:kapx) if(cnt[j]>=P) newch.PB(j);

			double newchobj = get_objective(newch);
			//if(get_objective(newch) - get_objective(kapx) >= -1e-7)
			if(newchobj - kapxobj >= -1e-7) //if objective value won't become smaller, delete the node
			{
				delf[i] = true;
				kapx = newch;
				kapxobj = newchobj;
			}
		}
	}

	vector<int> newf;
	for(int i:fapx) if(!delf[i]) newf.PB(i);
	fapx = newf;

	//degree
	fill(del.begin(), del.end(), true);
	for(int i:fapx) del[i] = false;
	for(int i:kapx) del[i] = false;
	get_degree();

	//DELETE viewer without preference
	REP(i,1,v) if(!del[i] && deg[i].S<1)
		delf[i] = true;
	newf.clear();
	for(int i:fapx) if(!delf[i]) newf.PB(i);
	fapx = newf;

	fill(del.begin(), del.end(), true);
	for(int i:fapx) del[i] = false;
	for(int i:kapx) del[i] = false;
	get_degree();

	ed = clock();//end timer

	//OUTPUT
	//calc farest pair
	int farest = 0;
	if(!SKIP_DIAMETER)
	{
		for(int i:fapx)
		{
			shortest_path(i);
			for(int j:fapx) farest = max(farest, dis[j]);
		}
	}

    //check is fesible solution(satisfy degree constraint)
	bool fes = true;
	REP(i,1,v) if(!del[i] && deg[i].S<1) fes = false;
	REP(i,v+1,v+c) 	if(!del[i] && deg[i].F<P) fes = false;

    //average viewer of each channel
	double avg = 0.0;
	REP(i,v+1,v+c) if(!del[i]) avg += deg[i].F;
	avg /= (double)kapx.size();

	if((int)kapx.size()==0)
	{
		fes = false;
		avg = 0.0;
	}
	printf("%f %d %d %f %d %f\n",
		(double)(ed-st)/CLOCKS_PER_SEC,
		(int)(fes && farest<=H),
		(int)fes,
		get_objective(kapx),
		farest,
		avg
	);
	printf("%d %d\n", (int)fapx.size(), (int)kapx.size());
	for(int i:fapx) printf("%d ",i); puts("");
	for(int i:kapx) printf("%d ",i); puts("");
}
int main(int argc,char *argv[])
{
	st = clock();
	readarg(argc,argv);
	init();
	sdssel();
	return 0;
}
