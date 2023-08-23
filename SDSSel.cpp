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
vector<int> instersection(vector<int> &v1, vector<int> &v2)//
{
	vector<int> v3;
	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());
	set_intersection(v1.begin(),v1.end(),v2.begin(),v2.end(),back_inserter(v3));
	return v3;
}
int v,c,n;
vector<bool> del,apx;
vector<vector<pair<int,double>>> e;
vector<pair<int,int>> deg;
vector<int> dis;
void init()
{
	int x,y;
	int e1,e2,e3;
	double z;
	
	scanf("%d %d %d %d %d",&v,&c,&e1,&e2,&e3);
	n = v+c;
	e = vector<vector<pair<int,double>>> (n+1);
	deg = vector<pair<int,int>> (n+1);
	del = vector<bool> (n+1);
	apx = vector<bool> (n+1);

	REP(i,1,e1)
	{
		scanf("%d %d",&x,&y);
		e[x].PB( MP(y,0.0) );
		e[y].PB( MP(x,0.0) );
	}
	REP(i,1,e2)
	{
		scanf("%d %d %lf",&x,&y,&z);
		e[x].PB( MP(y,z) );
		e[y].PB( MP(x,z) );
	}
	REP(i,1,e3)
	{
		scanf("%d %d",&x,&y);
		e[x].PB( MP(y,0.0) );
		e[y].PB( MP(x,0.0) );
	}
}
void get_degree() //
{
	REP(i,1,n) deg[i] = MP(0,0);
	REP(i,1,n)if(!del[i]) for(auto j:e[i])if(!del[j.F])
	{
		if(j.F>v) deg[i].S++;
		else deg[i].F++;
	}
}
pair<vector<int>,vector<int>> get_h2(int cur)//
{
	queue<int> q;
	vector<int> dis2(n+1);
	dis2[cur]=1;

	//find social vertices
	for(auto i:e[cur]) if(i.F<=v && !dis2[i.F] && !del[i.F])
	{
		dis2[i.F] = dis2[cur]+1;
		q.push(i.F);
	}
	while(!q.empty())
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

	//
	vector<int> r1,r2;
	REP(i,1,v) if(dis2[i]>0 && dis2[i]<=H+1) r1.PB(i);
	REP(i,v+1,v+c) if(cnt[i]>=P && !del[i]) r2.PB(i);
	return MP(r1,r2);
}
vector<int> get_viewers(vector<int> &ch)//
{
	vector<int> re;
	vector<bool> viss(n+1);
	for(int i:ch) for(auto j:e[i]) if(j.F<=v && !del[j.F]) viss[j.F]=true;
	REP(i,1,v) if(viss[i]) re.PB(i);
	return re;
}
vector<int> get_channels(vector<int> &vi)//
{
	vector<int> re;
	vector<bool> viss(n+1);
	for(int i:vi) for(auto j:e[i]) if(j.F>v && !del[j.F]) viss[j.F]=true;
	REP(i,v+1,v+c) if(viss[i]) re.PB(i);
	return re;
}
int min_channel_diversity(vector<int> &x)//
{
	int re=-1;
	double mn = 1e9;

	vector<bool> visx(n+1);
	vector<double> sum(n+1);
	for(int i:x) visx[i]=true;
	for(int i:x) for(auto j:e[i]) if(!del[j.F] && visx[j.F] && j.F>v) sum[i] += j.S;

	for(int i:x) if(sum[i]<mn)
	{
		mn = sum[i];
		re = i;
	}
	return re;
}
int max_channel_diversity(vector<int> &x,vector<int> &y)//
{
	int re=-1;
	double mx = -1e9;

	vector<bool> visx(n+1);
	vector<double> sum(n+1);
	for(int i:x) visx[i]=true;
	for(int i:x) for(auto j:e[i]) if(!del[j.F] && visx[j.F] && j.F>v) sum[i] += j.S;

	for(int i:y) if(sum[i]>mx)
	{
		mx = sum[i];
		re = i;
	}
	return re;
}
double total_diversity(vector<int> &x) //
{
	double re=0;
	vector<bool> visx(n+1);
	for(int i:x) visx[i]=true;
	for(int i:x) for(auto j:e[i]) if(!del[j.F] && visx[j.F] && j.F>v) re += j.S;
	return re;
}
double vertex_total_diversity(int vv) //
{
	double re=0;
	int i=vv; for(auto j:e[i]) if(!del[j.F] && j.F>v) re += j.S;
	return re;
}
double get_objective(vector<int> &x)//
{
	if((int)x.size()==0) return 0.0;

	vector<bool> visx(n+1);
	vector<double> sum(n+1);
	for(int i:x) visx[i]=true;
	for(int i:x) for(auto j:e[i]) if(!del[j.F] && visx[j.F] && j.F>v) sum[i] += j.S;

	double re=0.0;
	REP(i,v+1,v+c)
	{
		re += sum[i];
	}
	return re / (double)x.size();
}
void shortest_path(int root)
{
	int INF = 100000000;
	int cur;
	queue<int> q;

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
	get_degree();
	REP(i,v+1,v+c) if(deg[i].F<P) del[i]=true;
	get_degree();
	REP(i,1,v) if(deg[i].S<=0) del[i]=true;

	int vb=-1, qb=-1;
	vector<int> h2v,h2c;
	vector<int> kapx, fapx;
	vector<int> kapximp, fapximp;
	vector<int> cfit;
	double objective;

	REP(Vs,1,v) if(!del[Vs])
	{
		//new cut added
		vector<int> vvtmp2; vvtmp2.PB(Vs);
		vector<int> cctmp2 = get_channels(vvtmp2);
		qb = max_channel_diversity(cctmp2, cctmp2);
		if( vertex_total_diversity(qb) <= 2*get_objective(kapx) )
		{
			continue;
		}

		tie(h2v,h2c) = get_h2(Vs);
		if( (int)h2v.size()<P ) continue;

		if(CUT2)
		{
			vector<int> ptmp; ptmp.PB(Vs);
			vector<int> vtmp = get_channels(ptmp);
			if((int)(instersection(h2c, vtmp).size())==0)
				continue;
		}
		if(CUT1 && vb!=-1)
		{
			vector<int> ptmp; ptmp.PB(vb);
			vector<int> vtmp = get_channels(ptmp);

			if( total_diversity(h2c) <= 2*get_objective(vtmp) )
				continue;
		}

		while(h2c.size())
		{	
			if( kapx.size()==0 || get_objective(h2c) > get_objective(kapx) )
			{
				vector<int> viewer = get_viewers(h2c);
				vector<int> tmpf = instersection(h2v, viewer);

				if((int)tmpf.size() >= P)
				{
					kapx = h2c;
					fapx = instersection(h2v, viewer);
					vb = v;
				}
			}
			qb = min_channel_diversity(h2c);
			h2c.erase(remove(h2c.begin(), h2c.end(), qb), h2c.end());
		}
	}

	//IMP
	sort(h2c.begin(), h2c.end());
	sort(kapx.begin(), kapx.end());

	if(!SKIP_IMP)
	{
		for(int i:h2c) if(lower_bound(kapx.begin(), kapx.end(), i) == kapx.end()) cfit.PB(i);
		kapximp = kapx;
		fapximp = fapx;
		objective = get_objective(kapx);

		//delete from cfit
		vector<int> tp;
		for(int i:cfit)
		{
			double s=0;
			for(auto j:e[i]) if(!del[j.F] && j.F>v && lower_bound(h2c.begin(), h2c.end(), j.F)!=h2c.end())
				s += j.S;
			if(s < objective)
				tp.PB(i);
		}
		for(int i:tp) cfit.erase(remove(cfit.begin(), cfit.end(), i), cfit.end());

		while(get_objective(kapximp)<=get_objective(kapx) && cfit.size())
		{
			qb = max_channel_diversity(kapximp, cfit);
			vector<int> tt2; tt2.PB(qb);
			vector<int> viewer = get_viewers( tt2 );

			//kapximp
			kapximp.PB(qb);
			sort(kapximp.begin(), kapximp.end());
			kapximp.resize( unique(kapximp.begin(), kapximp.end()) - kapximp.begin() );
		
			//fapximp
			for(int i:viewer) if(lower_bound(h2v.begin(), h2v.end(), i) != h2v.end()) fapximp.PB(i);
			sort(fapximp.begin(), fapximp.end());
			fapximp.resize( unique(fapximp.begin(), fapximp.end()) - fapximp.begin() );

			cfit.erase(remove(cfit.begin(), cfit.end(), qb), cfit.end());

			if(get_objective(kapximp)>get_objective(kapx))
			{
				kapx = kapximp;
				fapx = fapximp;
			}
		}
	}

	//POST
	vector<bool> delf(n+1); //delete from apx
	if(!SKIP_POST)
	{
		for(int i:fapx) if(!delf[i])
		{
			shortest_path(i);
			bool flag=false;
			for(int j:fapx) if(!delf[j]) if(dis[j]>H) { flag=true; break; }
			if(!flag) continue;

			//reduce channel
			vector<int> cnt(n+1);
			vector<int> newch;
			vector<bool> vnow(n+1);
			for(int j:fapx) if(!delf[j] && j!=i) vnow[j]=true;

			//count the viewers
			REP(j,v+1,v+c) for(auto k:e[j]) if(vnow[k.F]) cnt[j]++;
			for(int j:kapx) if(cnt[j]>=P) newch.PB(j);

			if(get_objective(newch) - get_objective(kapx) >= -1e-7)
			{
				delf[i] = true;
				kapx = newch;
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
	int farest = 0;
	if(!SKIP_DIAMETER)
	{
		for(int i:fapx)
		{
			shortest_path(i);
			for(int j:fapx) farest = max(farest, dis[j]);
		}
	}

	bool fes = true;
	REP(i,1,v) if(!del[i] && deg[i].S<1) fes = false;
	REP(i,v+1,v+c) 	if(!del[i] && deg[i].F<P) fes = false;

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

