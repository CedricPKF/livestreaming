#include<bits/stdc++.h>
#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
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
using namespace rapidjson;
typedef long long LL;
int V,C,P,DELETE_EDGE=0;
char F_BUSINESS[256], F_REVIEW[256], F_USER[256], F_TIP[256];
bool USE_EUCLID=false;
bool RANDOM_SAMPLE=false;
void exit(const char s[])
{
	puts(s);
	exit(0);
}
void readarg(int argc,char *argv[])
{
	V=-1;
	C=-1;
	P=-1;
	DELETE_EDGE=0;
	REP(i,0,argc-1)
	{
		if(!strcmp(argv[i], "-euclid")) USE_EUCLID=true;
		if(!strcmp(argv[i], "-random-sample")) RANDOM_SAMPLE=true;
		if(i<argc-1 && !strcmp(argv[i], "-v"))
		{
			V = atoi(argv[i+1]);
		}
		if(i<argc-1 && !strcmp(argv[i], "-c"))
		{
			C = atoi(argv[i+1]);
		}
		if(i<argc-1 && !strcmp(argv[i], "-p"))
		{
			P = atoi(argv[i+1]);
		}
		if(i<argc-1 && !strcmp(argv[i], "-delete-edge"))
		{
			DELETE_EDGE = atoi(argv[i+1]);
		}
		if(i<argc-1 && !strcmp(argv[i], "-business"))
		{
			strcpy(F_BUSINESS, argv[i+1]);
		}	
		if(i<argc-1 && !strcmp(argv[i], "-user"))
		{
			strcpy(F_USER, argv[i+1]);
		}
		if(i<argc-1 && !strcmp(argv[i], "-tip"))
		{
			strcpy(F_TIP, argv[i+1]);
		}		
		if(i<argc-1 && !strcmp(argv[i], "-review"))
		{
			strcpy(F_REVIEW, argv[i+1]);
		}		
	}
}

int cid=0, cid2=0;
unordered_map<string,int> businessid;
unordered_map<string,int> userid;

map<pair<int,int>,bool> edge_vis;
map<pair<int,int>,int> edge2_vis;

vector<vector<int>> e,e2;
vector<vector<double>> vec;
vector<bool> bfs_vis;
vector<int> cmap;

int getuserid(string x)
{
	if(userid[x]) return userid[x];

	userid[x] = ++cid;
	while((int)e.size() <= cid) e.PB(vector<int>());
	return userid[x];
}
int getbusinessid(string x)
{
	if(businessid[x]) return businessid[x];

	businessid[x] = ++cid2;
	while((int)e2.size() <= cid2) e2.PB(vector<int>());
	return businessid[x];
}
void addedge(string xx,string yy)
{
	if(xx.length()<=0 || yy.length()<=0) return;
	int x = getuserid(xx);
	int y = getuserid(yy);
	
	if( x!=y && !edge_vis[MP(x,y)] )
	{
		edge_vis[MP(x,y)] = edge_vis[MP(y,x)] = true;
		e[x].PB(y);
		e[y].PB(x);
	}
}
void addedge2(string xx,string yy)
{
	if(xx.length()<=0 || yy.length()<=0) return;

	if(cid2>=C && !businessid[xx]) return; //TODO: cut
	if(!userid[yy]) return;

	int x = cmap[getbusinessid(xx)];
	int y = userid[yy];

	edge2_vis[MP(x,y)]++;
	/*if( edge2_vis[MP(x,y)]==COUNT )
	{
		e2[x].PB(y); 
	}*/
}
void readuser()
{
	Document d;
	char buf[2097152];
	FILE *f = fopen(F_USER, "r");
	if(!f) exit("cannot read user file.");

	while(fgets(buf, 2097152, f))
	{
		if(buf[0]!='{') continue;
		d.Parse(buf);

		string cur = string(d["user_id"].GetString());
		const Value& frs = d["friends"];
		int sz = frs.Size();
		REP(i,0,sz-1) addedge(cur, string(frs[i].GetString()));

		Document(kObjectType).Swap(d);
	}
	fclose(f);
}
void bfs()
{
	int cur;
	vector<bool> inq(cid+1);
	bfs_vis = vector<bool> (cid+1);
	queue<int> q;
	int cnt = V;
	
	if(RAND_MAX==32767)cur = (rand()*32768+rand()) % cid + 1; //[1,cid]
	else cur = rand()%cid + 1;
	q.push(cur);
	inq[cur] = true;

	while(!q.empty() && cnt)
	{
		cur = q.front();
		q.pop();
		inq[cur] = false;

		cnt--;
		bfs_vis[cur] = true;
		for(int i:e[cur]) if(!bfs_vis[i] && !inq[i])
		{
			q.push(i);
			inq[i] = true;
		}
	}
}
void random_sample()
{
	bfs_vis = vector<bool> (cid+1);
	int noko = V;
	int cur;
	while(noko)
	{
		if(RAND_MAX==32767)cur = (rand()*32768+rand()) % cid + 1;
		else cur = rand()%cid + 1;
		if(bfs_vis[cur]) continue;

		bfs_vis[cur]=true;
		noko--;
	}
}
void readedges()
{
	vector<pair<string,string>> edges, bns;
	Document d;
	char buf[2097152];
	string u,b,p;
	int uid;

	FILE *f = fopen(F_BUSINESS, "r");
	if(!f) exit("cannot read business file.");

	while(fgets(buf, 2097152, f))
	{
		if(buf[0]!='{') continue;
		d.Parse(buf);
		b = string( d["business_id"].GetString());
		p = string(d["postal_code"].GetString());
		getbusinessid(b);
		bns.PB( MP(p,b) );
		Document(kObjectType).Swap(d);
	}
	fclose(f);

	sort(bns.begin(), bns.end());
	C = min(C, cid2);
	cmap = vector<int> (cid2+1);
	int g = cid2/C;
	REP(i,0,(int)bns.size()-1)
	{
		int id = getbusinessid(bns[i].S);
		cmap[id] = min(C, i/g+1);
	}
	cid2 = C;

	//begin read edges
	f = fopen(F_TIP, "r");
	if(!f) exit("cannot read tip file.");
	while(fgets(buf, 2097152, f))
	{
		if(buf[0]!='{') continue;
		d.Parse(buf);

		b = string( d["business_id"].GetString() );
		u = string( d["user_id"].GetString() );

		uid = userid[u];
		if(uid<=0 || uid>cid) continue;
		if( !bfs_vis[uid] ) continue;

		if( businessid[b] )
			addedge2(b, u);

		Document(kObjectType).Swap(d);
	}
	fclose(f);

	f = fopen(F_REVIEW, "r");
	if(!f) exit("cannot read review file.");
	while(fgets(buf, 2097152, f))
	{
		if(buf[0]!='{') continue;
		d.Parse(buf);

		b = string( d["business_id"].GetString() );
		u = string( d["user_id"].GetString() );
		uid = userid[u];
		if(uid<=0 || uid>cid) continue;
		if( !bfs_vis[uid] ) continue;

		if( businessid[b] )
			addedge2(b, u);

		Document(kObjectType).Swap(d);
	}
	fclose(f);
}
void add_edges()
{
	if(DELETE_EDGE==100) return;
	if(DELETE_EDGE==0)
	{
		for(auto i: edge2_vis) if(i.S>0)
			e2[i.F.F].PB(i.F.S);
	}

	vector<int> sor;
	for(auto i: edge2_vis) if(i.S>0) sor.PB(i.S);
	sort(sor.begin(), sor.end());
	if(sor.size()==0) return;

	int pos = (double)DELETE_EDGE/100.0 * sor.size();
	int ths = sor[pos];
	for(auto i: edge2_vis) if(i.S > ths) e2[i.F.F].PB(i.F.S);
}
void readbusiness()
{
	USE_EUCLID=true;
	vec = vector<vector<double>>(cid2+1);
	Document d;
	string b;
	int bid;
	char buf[2097152];
	FILE *f = fopen(F_BUSINESS, "r");
	if(!f) exit("cannot read business file.");

	while(fgets(buf, 2097152, f))
	{
		if(buf[0]!='{') continue;
		d.Parse(buf);

		b = string( d["business_id"].GetString() );
		bid = cmap[businessid[b]];
		if(bid<=0 || bid>cid2) continue;

		if(USE_EUCLID)
		{
			vec[bid].PB( d["latitude"].GetDouble() );
			vec[bid].PB( d["longitude"].GetDouble() );
		}

		Document(kObjectType).Swap(d);
	}
	fclose(f);

	//count the average
	REP(i,1,cid2) if(vec[i].size())
	{
		int len = vec[i].size() / 2;

		vec[i][0] /= len;
		vec[i][1] /= len;
		REP(j,1,len-1)
		{
			double x = vec[i].back()/len;
			vec[i].pop_back();
			vec[i][1] += x;

			x = vec[i].back()/len;
			vec[i].pop_back();
			vec[i][0] += x;
		}
		assert((int)vec[i].size()==2);
	}
}
void getvector()
{
	vector<string> lisan;
	vec = vector<vector<double>>(cid2+1);

	REP(i,1,cid2) vec[i] = vector<double>(cid);
	REP(i,1,cid2) for(int j:e2[i]) vec[i][j-1]=1.0;
}
double getdis(int x,int y)
{
	int sz = min(vec[x].size(), vec[y].size());

	double re=0.0;
	double xd=0, yd=0;
	REP(i,0,sz-1)
	{
		if(USE_EUCLID) re += (vec[x][i]-vec[y][i]) * (vec[x][i]-vec[y][i]);
		else
		{
			re += vec[x][i] * vec[y][i];
			if(vec[x][i]) xd += 1.0;
			if(vec[y][i]) yd += 1.0;
		}
	}

	
	if(USE_EUCLID) return sqrt(re);
	if(xd==0 || yd==0) return 1.0;
	return 1.0 - (re/sqrt(xd)/sqrt(yd));
}
void output()
{
	//labels
	map<int,int> newid;
	int cur=0;
	int n,m;
	REP(i,1,cid) if(bfs_vis[i])
	{
		if(!newid[i]) newid[i] = ++cur;
	}
	n = cur;
	m = cid2;

	//output
	vector<pair<int,int>> e1,e22;
	REP(i,1,cid) if(bfs_vis[i]) for(int j:e[i]) if(bfs_vis[j]) if(i<j)
		e1.PB( MP(newid[i], newid[j]) );

	REP(i,1,cid2) for(int j:e2[i]) if(bfs_vis[j])
		e22.PB( MP(i+n, newid[j]) );

	printf("%d %d %d %d %d\n",n,m,(int)e1.size(),m*(m-1)/2,(int)e22.size());
	for(auto i:e1) printf("%d %d\n",i.F,i.S);
	REP(i,1,cid2) REP(j,i+1,cid2) printf("%d %d %f\n",i+n,j+n,getdis(i,j));
	for(auto i:e22) printf("%d %d\n",i.F,i.S);
}
int main(int argc,char *argv[])
{
	srand(time(NULL));
	readarg(argc,argv);
	readuser();
	if(RANDOM_SAMPLE) random_sample();
	else bfs();
	readedges(); add_edges();
	readbusiness();
	//getvector();
	//
	if(P!=-1)
	{
		REP(i,1,cid2) if((int)e2[i].size()<P) e2[i].clear();
	}

	output();
	
	return 0;
}

